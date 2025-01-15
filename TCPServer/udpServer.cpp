/**
 * Author: Leandro Alan Kim
 * Class: ECE 4122
 * Last Date Modified: 23 Nov 2024
 * Description: UDP cliend that sends comman to control a robot on a server
 */

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <map>

/**
 * Robot class for moveable robot
 */
class Robot : public sf::Drawable {
public:
    // default constructor
    Robot() : position(400.0f, 300.0f), direction(0.0f, 0.0f), speed(3.0f), windowSize(800.0f, 600.0f) {
        initializeShape();
        robotShape.setPosition(position);
    }

    /**
     * param constructor
     * @param startPos postion set to middle of the screen
     * @param windowSize window size
     */
    Robot(const sf::Vector2f& startPos, const sf::Vector2f& windowSize)
        : position(startPos), // start position
        direction(0.0f, 0.0f), // default direction
        speed(3.0f), // default speed
        windowSize(windowSize) {
        initializeShape();
        robotShape.setPosition(position);
    }

    /**
     * updates position of robot
     * @param dt delta time
     */
    void update(sf::Time dt) {
        position += direction * speed * dt.asSeconds(); //position based on direction, speed, and time

        // robot position bounded by window size
        position.x = std::max(0.0f, std::min(position.x, windowSize.x - robotShape.getRadius() * 2));
        position.y = std::max(0.0f, std::min(position.y, windowSize.y - robotShape.getRadius() * 2));

        robotShape.setPosition(position);
    }

    /**
     * cases for inputed commands from client
     * @param cmd commands for direction of robot
     */
    void setDirection(char cmd) {
        switch(cmd) {
            case 'w':
                direction = sf::Vector2f(0.0f, -1.0f); // up
                std::cout << "Moving UP" << std::endl;
                break;
            case 's':
                direction = sf::Vector2f(0.0f, 1.0f); // down
                std::cout << "Moving DOWN" << std::endl;
                break;
            case 'a':
                direction = sf::Vector2f(-1.0f, 0.0f); // left
                std::cout << "Moving LEFT" << std::endl;
                break;
            case 'd':
                direction = sf::Vector2f(1.0f, 0.0f); // right
                std::cout << "Moving RIGHT" << std::endl;
                break;
        }
    }

    /**
     * speed of the robot
     * @param increase increase spped if true
     */
    void changeSpeed(bool increase) {
        if (increase) {
            speed += 1.0f; // increase
            std::cout << "Speed increased to: " << speed << std::endl;
        } else {
            if (speed > 1.0f) {
                speed -= 1.0f; // decrease
                std::cout << "Speed decreased to: " << speed << std::endl;
            }
        }
    }

private:
    sf::Vector2f position; // position
    sf::Vector2f direction; // direction
    float speed; // speed
    sf::Vector2f windowSize; // window size
    sf::CircleShape robotShape; // circle

    /**
     * set robot as circle
     */
    void initializeShape() {
        robotShape.setRadius(10.0f); // size of circle (3x3) was a little hard to show on video
        robotShape.setFillColor(sf::Color::Green); // color of circle
        robotShape.setOrigin(robotShape.getRadius(), robotShape.getRadius()); // centering
    }

    /**
     * draw method
     * @param target target to draw to 
     * @param states current render states
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(robotShape, states);
    }
};

/**
 * sets up and runs the UDP server
 * @param argc number of command line arg
 * @param argv arr of command line arg
 * @return 0 on successful execution, else -1
 */
int main(int argc, char* argv[]) {
    // check command line arg
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " -p <port>" << std::endl;
        return -1;
    }

    // set up udp
    uint16_t port = static_cast<uint16_t>(std::stoi(argv[2]));
    sf::UdpSocket socket;
    socket.setBlocking(false);

    // bind socket to port
    if (socket.bind(port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        return -1;
    }

    std::cout << "Waiting on port " << port << std::endl;

    // sfml
    sf::RenderWindow window(sf::VideoMode(800, 600), "Robot Server");
    std::map<std::pair<uint32_t, uint16_t>, Robot> robots; // map to store robots
    sf::Clock clock; // clock

    bool windowClosing = false; // flag for window closing

    // server loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowClosing = true;
            }
        }

        // server shutdown
        if (windowClosing) {
            std::string closeMsg = "Server closing";
            // notify shutdown
            for (const auto& [clientId, robot] : robots) {
                auto [clientIP, clientPort] = clientId;
                sf::IpAddress clientAddress(clientIP);
                
                // send shutdown messages
                for (int i = 0; i < 3; i++) {
                    socket.send(closeMsg.c_str(), closeMsg.length(), clientAddress, clientPort);
                    sf::sleep(sf::milliseconds(10));
                }
            }
            
            sf::sleep(sf::milliseconds(100));
            window.close();
            break;
        }

        // receive and proess commands
        char buffer[1];
        std::size_t received;
        sf::IpAddress sender;
        uint16_t senderPort;
        
        if (socket.receive(buffer, sizeof(buffer), received, sender, senderPort) == sf::Socket::Done) {
            char command = buffer[0];
            auto clientId = std::make_pair(sender.toInteger(), senderPort);

            // commands
            switch (command) {
                case 'q': // quit
                    std::cout << "Client disconnected" << std::endl;
                    robots.erase(clientId);
                    break;
                // direction command
                case 'w':
                case 'a':
                case 's':
                case 'd':
                    // update robot
                    if (robots.find(clientId) == robots.end()) {
                        robots[clientId] = Robot(sf::Vector2f(400.0f, 300.0f), sf::Vector2f(800.0f, 600.0f));
                    }
                    robots[clientId].setDirection(command);
                    break;
                // speed command
                case 'g': // increase
                case 'h': // decrease
                    if (robots.find(clientId) != robots.end()) {
                        robots[clientId].changeSpeed(command == 'g');
                    }
                    break;

                default:
                    std::cout << "Wrong command" << std::endl;
                    break;
            }
        }

        // update and render
        sf::Time deltaTime = clock.restart();
        window.clear(sf::Color::Black); // clear window
        
        // update robot
        for (auto& [id, robot] : robots) {
            robot.update(deltaTime);
            window.draw(robot);
        }
        
        window.display();
    }

    return 0;
}