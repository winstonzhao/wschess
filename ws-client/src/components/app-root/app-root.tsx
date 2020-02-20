import { Component, h, Element } from "@stencil/core";
import { HTMLStencilElement } from "@stencil/core/internal";
import { PLAYER_RADIUS, MOVE_SPEED } from "../../org/constants";
import { Player } from "../../org/players";
import { Server } from "../../org/server";

@Component({
  tag: "app-root",
  styleUrl: "app-root.css"
})
export class AppRoot {
  @Element() el!: HTMLStencilElement;
  canvas: HTMLCanvasElement;
  keys: { [key: string]: boolean } = {};

  server: Server;

  player: Player;
  otherPlayers: Player[] = [];

  componentWillLoad() {
    window.addEventListener("resize", () => {
      this.el.forceUpdate();
    });

    window.onkeyup = (e: KeyboardEvent) => (this.keys[e.key] = false);
    window.onkeydown = (e: KeyboardEvent) => (this.keys[e.key] = true);

    this.server = new Server(this);
  }

  componentDidLoad() {
    setInterval(() => this.tick(), 3);
  }

  tick() {
    this.updateGameState();
    this.draw();
  }

  updateGameState() {
    if (!this.player) {
      return;
    }

    let dx = 0,
      dy = 0;

    if (this.keys["ArrowUp"]) {
      dy -= window.innerHeight * MOVE_SPEED;
    }
    if (this.keys["ArrowDown"]) {
      dy += window.innerHeight * MOVE_SPEED;
    }
    if (this.keys["ArrowLeft"]) {
      dx -= window.innerHeight * MOVE_SPEED;
    }
    if (this.keys["ArrowRight"]) {
      dx += window.innerHeight * MOVE_SPEED;
    }

    this.player.x += dx;
    this.player.y += dy;

    if (this.player.x < PLAYER_RADIUS) {
      this.player.x = PLAYER_RADIUS;
    } else if (this.player.x > window.innerHeight - PLAYER_RADIUS) {
      this.player.x = window.innerHeight - PLAYER_RADIUS;
    }

    if (this.player.y < PLAYER_RADIUS) {
      this.player.y = PLAYER_RADIUS;
    } else if (this.player.y > window.innerHeight - PLAYER_RADIUS) {
      this.player.y = window.innerHeight - PLAYER_RADIUS;
    }

    if (dx || dy) {
      this.server.sendMove(this.player);
    }
  }

  draw() {
    const ctx = this.canvas.getContext("2d");
    ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    if (this.player) {
      this.player.render(ctx, true);
    }
    this.otherPlayers.forEach(p => p.render(ctx));
  }

  render() {
    return (
      <div>
        <main>
          <canvas
            id="myCanvas"
            ref={el => (this.canvas = el)}
            width={window.innerHeight}
            height={window.innerHeight}
          ></canvas>
        </main>
      </div>
    );
  }
}
