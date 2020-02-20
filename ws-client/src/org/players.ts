import {
  PLAYER_RADIUS,
  CURRENT_PLAYER_COLOR,
  OTHER_PLAYERS_COLOR
} from "./constants";

export class Player {
  constructor(public id: string, public x = 0, public y = 0) {}

  render(ctx: CanvasRenderingContext2D, curPlayer = false) {
    ctx.beginPath();
    ctx.arc(this.x, this.y, PLAYER_RADIUS, 0, 2 * Math.PI);
    ctx.strokeStyle = curPlayer ? CURRENT_PLAYER_COLOR : OTHER_PLAYERS_COLOR;
    ctx.lineWidth = 7;
    ctx.stroke();
  }
}
