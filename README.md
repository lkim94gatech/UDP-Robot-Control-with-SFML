# UDP Robot Control with SFML

This project implements a **robot movement simulator** using **UDP sockets** for communication and **SFML** for rendering a 2D graphical environment. A server renders the robot's movement based on commands sent from a client over a UDP connection.

---

## Objective

To develop a client-server application where:
1. The **server** listens for motion commands via UDP and displays the robot's movement on a 2D SFML window.
2. The **client** sends motion commands to the server, allowing user-controlled robot movement.

---

## Features

1. **Server (SFML)**:
   - Renders a robot in a 2D environment (800x600 window).
   - Listens for UDP packets to update the robot's position and speed.
   - Notifies the client when the server is closing.

2. **Client**:
   - Accepts user inputs to control the robot's movement (up, down, left, right) and speed (increase or decrease).
   - Sends commands to the server over a UDP connection.

3. **UDP Communication**:
   - Ensures lightweight and fast communication between the client and server.

---

## File Structure

### Source Files
- **udpServer.cpp**: Implements the server, handling robot rendering and communication.
- **udpClient.cpp**: Implements the client, allowing user control and sending commands to the server.

---

## How to Run

### Prerequisites
- **SFML Library**: Install SFML for graphical rendering and networking support.
- **C++ Compiler**: A compiler with C++11 or later support (e.g., GCC, Clang).

### Steps

1. **Clone the repository**:
   ```bash
   git clone <repository_url>
   cd udp-robot-simulator
   ```
2. Compile both Server and Client
3. Run the Server:
   ```bash
   ./server -p <port>
   ```
4. Run the Client:
   ```bash
   ./client
   ```
## Gameplay Instructions

### Server:
- Opens a graphical window (800x600) with a black background.
- Displays a green circular robot that moves based on the received commands.
- The server ends when:
  - The window is closed.
  - A "disconnect" command (`q`) is received from the client.

### Client:
- Prompts for the server's IP address and port.
- Accepts user inputs to control the robot:
  - `w`: Move up.
  - `s`: Move down.
  - `a`: Move left.
  - `d`: Move right.
  - `g`: Increase speed.
  - `h`: Decrease speed.
  - `q`: Quit and send a disconnect message to the server.
- Displays messages received from the server.

---


## Example Commands

### Running the Server:
```bash
./server -p 54000
```

### Running the Client:
```bash
./client
```

### Example Client Input:
```bash
Enter server IP address: 127.0.0.1
Enter server port: 54000
Control the robot with the following keys:
  w: Move up
  s: Move down
  a: Move left
  d: Move right
  g: Increase speed
  h: Decrease speed
  q: Quit
```
### Example Server Output:
```bash
Waiting on port 54000
Moving UP
Moving LEFT
Speed increased to: 4
Client disconnected
```

### Example Client Output:
```bash
Enter command: w
Sent command: w
Enter command: g
Sent command: g
Server: Speed increased to: 4
Enter command: q
Sent command: q
Server is shutting down. Exiting client.
```

## Testing and Debugging

### Key Features to Test

#### UDP Communication:
- Verify commands sent from the client are correctly received by the server.
- Ensure server-to-client responses are accurate.

#### Robot Movement:
- Test all directional commands (`w`, `a`, `s`, `d`).
- Confirm the robot moves within the window bounds.

#### Speed Adjustment:
- Validate speed increase (`g`) and decrease (`h`) commands.

#### Server Shutdown:
- Ensure the server notifies the client upon closing.
- Verify the client correctly handles the disconnect notification.

#### Error Handling:
- Test invalid inputs at the client.
- Validate server behavior when receiving malformed or unexpected packets.

