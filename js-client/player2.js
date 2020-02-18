const WebSocket = require("ws");

let ws = new WebSocket("ws://localhost:8080/game");

ws.on('message', (evt) => {
  console.log(evt);
  const msg = JSON.parse(evt);
  
});


ws.on('open', () => {
  ws.send(JSON.stringify({
    type: 'ident',
    name: 'james'
  }));
});
