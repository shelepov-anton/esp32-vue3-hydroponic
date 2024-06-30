const WEBSOCKET_PORT = 3000

const socket = new WebSocket(`ws://192.168.0.5:${WEBSOCKET_PORT}`)

export default socket