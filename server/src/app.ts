import cors from 'cors'
import express, { Application } from 'express'
import http from 'http'
import WebSocket from 'ws'

// Server Initialization
const app: Application = express()
const server = http.createServer(app)

// Configuration
const WEBSOCKET_PORT = 3000
const API_SERVER_PORT = 3001
const API_PATH = '/api/v1/'
const corsOptions = {
    origin: 'localhost:3000',
    credentials: true,
    optionSuccessStatus: 200
}

// Request handlers
app.use(cors(corsOptions))

// WebSocket config
const websocketServer = new WebSocket.Server({ server })

websocketServer.on('request', (socket) => {
    console.log(socket)
})

websocketServer.on('headers', (socket) => {
    console.log(socket)
})

//Listen for WebSocket connections
websocketServer.on('connection', (socket) => {
    // Log a message when a new client connects
    console.log('client connected.')
    // Listen for incoming WebSocket messages
    socket.on('message', (data) => {

        console.log(data)

        // Broadcast the message to all connected clients
        websocketServer.clients.forEach(function each(client) {
            if (client !== socket && client.readyState === WebSocket.OPEN) {
                console.log('message', data.toString())
                client.send(data.toString())
            }
        })
    })

    // Listen for WebSocket connection close events
    socket.on('close', () => {
        // Log a message when a client disconnects
        console.log('Client disconnected')
    })
})

// Start server
server.listen(WEBSOCKET_PORT, () => {
    console.log(`Websocket server started on port ${WEBSOCKET_PORT}`)
})

app.listen(API_SERVER_PORT, () => {
    console.log(`Server server started on port ${API_SERVER_PORT}`)
})