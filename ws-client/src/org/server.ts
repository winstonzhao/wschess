import WebSocket from "ws";
import { AppRoot } from "../components/app-root/app-root";

export class Server {
  constructor(private root: AppRoot) {}

  configureNet() {
    const ws = new WebSocket("ws://localhost:8080/game");
    ws.on("message", evt => {
      console.log(evt);
      const msg = JSON.parse(evt as string);
      switch (msg.type) {
        case "state_update":
      }
    });

    ws.on("open", () => {
      ws.send(
        JSON.stringify({
          type: "ident",
          name: "james"
        })
      );
    });
  }
}
