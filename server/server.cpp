#include "server.hpp"

using namespace std;

namespace WsChess
{

void Server::Start()
{
  Configure();
  AddGameEndpoint();
  Listen();
}

void Server::Configure()
{
  mWsServer.config.port = 8080;
}

void Server::Listen()
{
  // Start server and receive assigned port when server is listening for requests
  promise<unsigned short> server_port;
  thread server_thread([&server_port, this]() {
    // Start server
    this->mWsServer.start([&server_port](unsigned short port) {
      server_port.set_value(port);
    });
  });

  cout << "Server listening on port " << server_port.get_future().get() << endl
       << endl;

  server_thread.join();
}

WsEndpoint &Server::AddGenericEndpoint(const std::string name)
{
  auto &endpoint = mWsServer.endpoint["^/" + name + "/?$"];

  endpoint.on_open = [](shared_ptr<WsServer::Connection> connection) {
    cout << "Server: Opened connection " << connection.get() << endl;
  };

  // See RFC 6455 7.4.1. for status codes
  endpoint.on_close = [this](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
    mPlayers.erase(GetConnectionId(*connection));
    cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
  };

  // Can modify handshake response headers here if needed
  endpoint.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
    return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
  };

  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  endpoint.on_error = [this](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
    mPlayers.erase(GetConnectionId(*connection));
    cout << "Server: Error in connection " << connection.get() << ". "
         << "Error: " << ec << ", error message: " << ec.message() << endl;
  };

  return endpoint;
}

WsEndpoint &Server::AddGameEndpoint()
{
  auto &endpoint = AddGenericEndpoint("game");

  endpoint.on_message = [this](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
    auto payload = in_message->string();

    auto id = GetConnectionId(*connection);

    // All Connections Require an ID
    if (id.empty())
    {
      cout << "No id found for connection " << connection.get() << endl;
      json error = Error("No id found for connection", Errors::NO_ID_FOUND);
      connection->send(error.dump(), HandleError);
      return;
    }

    //  Check type of the message.
    auto payloadJson = json::parse(payload);
    auto type = payloadJson["type"].get<std::string>();

    if (type == Messages::Incoming::IDENTIFICATION)
    {
      auto incoming = payloadJson.get<WsChess::Identification>();

      mPlayers.insert({id, Player(id, incoming.name, connection.get())});
      LogPlayers();

      json confirm = GenericMessage(Messages::Outgoing::IDENT_CONFIRM, id);
      connection->send(confirm.dump(), HandleError);
      return;
    }

    auto playerIdPair = mPlayers.find(id);

    if (playerIdPair == mPlayers.end())
    {
      json error = Error("Player not found", Errors::NO_PLAYER_FOUND);
      connection->send(error.dump(), HandleError);
    }

    auto &player = playerIdPair->second;

    if (type == Messages::Incoming::MOVEMENT)
    {
      auto incoming = payloadJson.get<WsChess::Pos>();
      player.SetPos(incoming);
      NotifyPlayersOfChanges();
      return;
    }

    cout
        << "Server: Message received: \"" << payload << "\" from " << connection.get() << endl;

    cout << "Server: Sending message \"" << payload << "\" to " << connection.get() << endl;

    // connection->send is an asynchronous function
    connection->send(payload, HandleError);
  };
}

void WsChess::Server::LogPlayers()
{
  cout << "Logging all players" << endl;
  for (auto &pair : mPlayers)
  {
    cout << "\t" << pair.first << " " << pair.second.GetName() << endl;
  }
}

void WsChess::Server::NotifyPlayersOfChanges()
{
  for (auto &playerPair : mPlayers)
  {
    auto &player = playerPair.second;
    json stateUpdate = StateUpdate(Messages::Outgoing::STATE_UPDATE, &mPlayers);
    player.GetConnection()->send(stateUpdate.dump(), HandleError);
  }
}

std::string GetConnectionId(const WsServer::Connection &conn)
{
  auto pair = conn.header.find("Sec-WebSocket-Key");

  if (pair == conn.header.end())
  {
    return "";
  }

  return pair->second;
}

void HandleError(const SimpleWeb::error_code &ec)
{
  if (ec)
  {
    cout << "Server: Error sending message. " <<
        // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
        "Error: " << ec << ", error message: " << ec.message() << endl;
  }
}

} // namespace WsChess