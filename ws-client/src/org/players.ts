import { PLAYER_RADIUS, CURRENT_PLAYER_COLOR } from "./constants";

export class Player {
  constructor(public id: string, public x = 0, public y = 0) {}

  render(ctx: CanvasRenderingContext2D) {
    ctx.beginPath();
    ctx.arc(this.x, this.y, PLAYER_RADIUS, 0, 2 * Math.PI);
    ctx.strokeStyle = CURRENT_PLAYER_COLOR;
    ctx.lineWidth = 7;
    ctx.stroke();
  }
}
