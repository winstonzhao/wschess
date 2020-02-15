#include "server.hpp"

using namespace std;

namespace WsChess
{
void Server::Start()
{
  Configure();
  AddIdentificationEndpoint();
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
  endpoint.on_close = [](shared_ptr<WsServer::Connection> connection, int status, const string & /*reason*/) {
    cout << "Server: Closed connection " << connection.get() << " with status code " << status << endl;
  };

  // Can modify handshake response headers here if needed
  endpoint.on_handshake = [](shared_ptr<WsServer::Connection> /*connection*/, SimpleWeb::CaseInsensitiveMultimap & /*response_header*/) {
    return SimpleWeb::StatusCode::information_switching_protocols; // Upgrade to websocket
  };

  // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
  endpoint.on_error = [](shared_ptr<WsServer::Connection> connection, const SimpleWeb::error_code &ec) {
    cout << "Server: Error in connection " << connection.get() << ". "
         << "Error: " << ec << ", error message: " << ec.message() << endl;
  };

  return endpoint;
}

WsEndpoint &Server::AddIdentificationEndpoint()
{
  auto &endpoint = AddGenericEndpoint("echo");

  endpoint.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> in_message) {
    auto out_message = in_message->string();

    cout << "Server: Message received: \"" << out_message << "\" from " << connection.get() << endl;

    cout << "Server: Sending message \"" << out_message << "\" to " << connection.get() << endl;

    // connection->send is an asynchronous function
    connection->send(out_message, [](const SimpleWeb::error_code &ec) {
      if (ec)
      {
        cout << "Server: Error sending message. " <<
            // See http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/reference.html, Error Codes for error code meanings
            "Error: " << ec << ", error message: " << ec.message() << endl;
      }
    });
  };
}

} // namespace WsChess