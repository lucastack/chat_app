# Simple Chat Application

This chat application consists of a server and a client that can communicate over a network. Multiple clients can connect to the server, and messages from one client are broadcast to all other clients. 

Disclaimer: i did this for learning purposes (C and threading), also, for fun.

## Features
- Multi-client support using multithreading (using POSIX threads).
- Clients are recognized by a name they set upon connection.
- Real-time broadcasting of messages to all connected clients.

## Compilation

To compile both the server and the client use `make`

## Usage

Start the server with `./server`, it will start the server and wait for clients to connect.

Start a client with `./client`. Upon starting, the client will prompt you to enter a name. This name will be used to identify your messages to other clients. After setting the name, you can start sending messages to the server.

## TODO

 - Enhance code style (could be cleaner lol)
 - Allow private messages between clients
 - Display an indicator when another user is typing a message
 - Allow users to send and receive files.
 - Implement a database to store chat histories.
