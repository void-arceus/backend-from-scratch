# High-Performance HTTP Server & Reverse Proxy

## Overview

A high-performance HTTP/1.1 server and reverse proxy built from scratch in C++ using raw TCP sockets. The project implements HTTP request parsing, routing, persistent connections, multithreaded request handling, and a thread pool for concurrent connections. It also includes a reverse proxy with Round-Robin load balancing across multiple backend servers.

## Features

- HTTP/1.1 server built using raw TCP sockets
- HTTP request parsing and routing
- Persistent HTTP connections (Keep-Alive)
- Multithreaded connection handling
- Thread pool with a thread-safe task queue
- Reverse proxy for forwarding client requests to backend servers
- Round-Robin load balancing across multiple upstream servers
- Support for multiple backend server instances
- Performance benchmarking using wrk

## Architecture

                         Client
                           │
                           ▼
                  ┌─────────────────┐
                  │  Reverse Proxy  │
                  │     :8080       │
                  └────────┬────────┘
                           │
                    Round-Robin
                    Load Balancing
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
         Server 1      Server 2      Server 3
          :3000         :3001         :3002

## How It Works

The server accepts TCP connections and processes incoming HTTP requests through a multithreaded architecture.

- The server listens for incoming TCP connections.
- Accepted connections are placed into a thread-safe task queue.
- Worker threads from the thread pool process connections concurrently.
- HTTP requests are parsed to extract the request method, URI, headers, and body when applicable.
- Requests are either handled by the server's routing logic or forwarded through the reverse proxy.
- The proxy selects an upstream server using Round-Robin load balancing.
- The request is forwarded to the selected backend server.
- The backend response is received and relayed back to the client.
- Persistent connections allow multiple HTTP requests to use the same TCP connection.

## Building & Running

### Prerequisites

- C++ compiler with C++11 or later support
- Linux-based operating system
- `wrk` (optional, for performance benchmarking)

### Build

Clone the repository and navigate to the web server directory:

```bash
git clone https://github.com/void-arceus/backend-from-scratch.git
cd backend-from-scratch/web_server
```

Compile the main HTTP server and reverse proxy using:

```bash
g++ main.cpp server.cpp parser.cpp router.cpp proxy.cpp ThreadPool.cpp
```

Start the backend server instances on their respective ports:

```bash
g++ server_one_main.cpp server1.cpp ../parser.cpp ../router.cpp ../server.cpp ../ThreadPool.cpp
```

Start Server 2 and Server 3 using the same command, replacing `server_one_main.cpp` and `server1.cpp` with their respective files

Start the main HTTP server and reverse proxy:

You can send requests through the proxy using:

```bash
curl http://localhost:8080
```

The proxy forwards incoming requests to the configured backend servers and distributes traffic using Round-Robin load balancing.

## Benchmark

The server can be benchmarked using `wrk`:

```bash
wrk -t8 -c2000 -d10s http://localhost:8080
```

This runs the benchmark using 8 threads, 2,000 concurrent connections, for 10 seconds against the reverse proxy.

## Project Structure

```text
web_server/
├── main.cpp
├── server.cpp
├── server.hpp
├── parser.cpp
├── parser.hpp
├── router.cpp
├── router.hpp
├── proxy.cpp
├── proxy.hpp
├── ThreadPool.cpp
├── ThreadPool.hpp
└── servers/
    ├── server1.cpp
    ├── server2.cpp
    └── server3.cpp
```

### Core Components

- **Server** — Handles TCP socket creation, connection acceptance, and HTTP server operations.
- **Parser** — Parses incoming HTTP requests and extracts request information.
- **Router** — Handles request routing.
- **ThreadPool** — Manages worker threads and the thread-safe task queue for concurrent connection handling.
- **Proxy** — Handles request forwarding and Round-Robin load balancing between backend servers.
- **Servers** — Contains the individual backend server implementations used by the reverse proxy.

## What I Learned

Building this project gave me practical experience with low-level networking and backend system design. Through the project, I learned how to:

- Work with TCP sockets and understand the client-server connection lifecycle.
- Parse and handle HTTP/1.1 requests over raw TCP connections.
- Build concurrent systems using threads, thread pools, and thread-safe queues.
- Implement persistent HTTP connections using Keep-Alive.
- Design a reverse proxy that forwards requests between clients and backend servers.
- Implement Round-Robin load balancing across multiple upstream servers.
- Benchmark network services under high concurrency using `wrk`.
- Investigate system-level limitations such as file descriptor limits during load testing.
- Understand how networking, concurrency, and operating system resources interact under high load.
