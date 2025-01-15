/**
 * Author: Leandro Alan Kim
 * Class: ECE 4122
 * Last Date Modified: 23 Nov 2024
 * Description: UDP cliend that sends comman to control a robot on a server
 */

#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

int main() {
    // recieve server details
    std::string serverAddress;
    unsigned short serverPort;
    std::cout << "Enter server IP address: ";
    std::cin >> serverAddress;
    std::cout << "Enter server port: ";
    std::cin >> serverPort;

    // udp socket
    sf::UdpSocket socket;
    std::atomic<bool> running(true);

    // server messages
    std::thread listenerThread([&]() {
        while (running) {
            char buffer[128];
            std::size_t received;
            sf::IpAddress sender;
            unsigned short senderPort;
            if (socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Done) {
                std::string message(buffer, received);
                std::cout << "Server: " << message << std::endl;
                if (message == "Server closing") {
                    std::cout << "Server is shutting down. Exiting client." << std::endl;
                    running = false;
                }
            }
        }
    });

    //  prompting input controls
    std::cout << "Control the robot with the following keys:\n"
              << "  w: Move up\n"
              << "  s: Move down\n"
              << "  a: Move left\n"
              << "  d: Move right\n"
              << "  g: Increase speed\n"
              << "  h: Decrease speed\n"
              << "  q: Quit\n";

    // main loop
    while (running) {
        std::cout << "Enter command: ";
        char command;
        std::cin >> command;

        // validating input
        if (command != 'w' && command != 'a' && command != 's' && command != 'd' &&
            command != 'g' && command != 'h' && command != 'q') {
            std::cout << "Invalid command. Try again." << std::endl;
            continue;
        }

        // sending command
        if (socket.send(&command, 1, serverAddress, serverPort) != sf::Socket::Done) {
            std::cerr << "Error sending command to server!" << std::endl;
        } else {
            std::cout << "Sent command: " << command << std::endl;
        }

        // quit command
        if (command == 'q') {
            running = false;
        }
    }

    // thread to finish
    listenerThread.join();
    return 0;
}