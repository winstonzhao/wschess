const WebSocket = require("ws");

let ws = new WebSocket("ws://localhost:8080/game");

ws.on('message', (evt) => {
  console.log(evt);
  const msg = JSON.parse(evt);
  switch(msg.type) {
    case "ident_confirm":
      ws.send(JSON.stringify({type: "join_room", message: "62Op2OR2XO85ba7iOtS2hA=="}))
      break;
  }
});


ws.on('open', () => {
  ws.send(JSON.stringify({
    type: 'ident',
    name: 'james'
  }));
});
