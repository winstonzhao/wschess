export interface StateUpdate {
  type: "state_update";
  players: { [id: string]: PlayerUpdate };
}

export interface PlayerUpdate {
  pos: { x: number; y: number };
}
