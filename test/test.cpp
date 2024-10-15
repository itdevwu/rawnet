#include "net.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace rawnet::protocol;

void client_A(boost::asio::io_context &io_context, const std::string &server_address, unsigned short server_port)
{
    NetClient client(io_context, server_address, server_port);

    // Create a POST-like message
    Message msg(1, "Hello from Client A!");

    // Send the message
    std::cout << "Client A: Sending message to Client B...\n";
    client.send_message(msg);

    // Wait and receive the response
    Message response = client.receive_message();
    std::cout << "Client A: Received response from Client B: " << response.data << "\n";
}

void client_B(boost::asio::io_context &io_context, unsigned short listen_port)
{
    boost::asio::ip::udp::endpoint listen_endpoint(boost::asio::ip::udp::v4(), listen_port);
    NetClient client(io_context, "127.0.0.1", listen_port);

    // Wait for a message from Client A
    std::cout << "Client B: Waiting for a message from Client A...\n";
    Message msg = client.receive_message();
    std::cout << "Client B: Received message: " << msg.data << "\n";

    // Create a GET-like response message
    Message response(0, "Hello from Client B!");

    // Send the response back to Client A
    std::cout << "Client B: Sending response back to Client A...\n";
    client.send_message(response);
}

int main()
{
    // Create a Boost ASIO io_context
    boost::asio::io_context io_context;

    // Launch Client B (receiver) in a separate thread
    std::thread clientB_thread([&io_context]() {
        client_B(io_context, 9001); // Client B listens on port 9001
    });

    // Allow Client B some time to start
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Run Client A (sender)
    client_A(io_context, "127.0.0.1", 9001); // Client A sends to Client B on port 9001

    // Wait for Client B to finish
    clientB_thread.join();

    return 0;
}
