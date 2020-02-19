import { PLAYER_RADIUS, CURRENT_PLAYER_COLOR } from "./constants";

export class Player {
  x: number = 0;
  y: number = 0;

  constructor() {}

  render(ctx: CanvasRenderingContext2D) {
    ctx.beginPath();
    ctx.arc(this.x, this.y, PLAYER_RADIUS, 0, 2 * Math.PI);
    ctx.strokeStyle = CURRENT_PLAYER_COLOR;
    ctx.lineWidth = 7;
    ctx.stroke();
  }
}