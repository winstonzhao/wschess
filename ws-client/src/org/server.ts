import { AppRoot } from "../components/app-root/app-root";
import { Player } from "./players";
import { StateUpdate } from "./types";

export class Server {
  ws: WebSocket;

  constructor(private root: AppRoot) {
    this.configure();
  }

  sendMove(player: Player) {
    this.ws.send(
      JSON.stringify({
        type: "movement",
        x: player.x,
        y: player.y
      })
    );
  }

  configure() {
    this.root;

    this.ws = new WebSocket("ws://localhost:8080/game");
    this.ws.onmessage = evt => {
      console.log(evt.data);
      const msg = JSON.parse(evt.data as string);
      switch (msg.type) {
        case "ident_confirm":
          this.root.player = new Player(msg.message);
          break;
        case "state_update":
          const update = msg as StateUpdate;
          this.root.otherPlayers = Object.keys(update.players)
            .filter(id => id !== this.root.player.id)
            .map(
              player =>
                new Player(
                  player,
                  update.players[player].pos.x,
                  update.players[player].pos.y
                )
            );
          break;
      }
    };

    this.ws.onopen = () => {
      this.ws.send(
        JSON.stringify({
          type: "ident",
          name: "james"
        })
      );
    };
  }
}
