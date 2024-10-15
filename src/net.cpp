// Copyright (c) 2024 itdevwu.

#include "net.hpp"
#include <cstddef>
#include <cstring>

namespace rawnet::protocol
{

NetClient::NetClient(boost::asio::io_context &io_context, const std::string &server_address, unsigned short port)
    : socket_(io_context)
{
    boost::asio::ip::udp::resolver resolver(io_context);
    server_endpoint_ = *resolver.resolve(boost::asio::ip::udp::v4(), server_address, std::to_string(port)).begin();
    socket_.open(boost::asio::ip::udp::v4());
}

void NetClient::send_message(const Message &message)
{
    // Serialize the message into a byte buffer
    std::vector<unsigned char> buffer = serialize(message);

    // Send the buffer over UDP
    socket_.send_to(boost::asio::buffer(buffer), server_endpoint_);
}

Message NetClient::receive_message(size_t buffer_size)
{
    std::vector<unsigned char> recv_buffer(buffer_size);
    size_t len = socket_.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint_);

    // Resize buffer to actual length
    recv_buffer.resize(len);

    // Deserialize the buffer into a Message object
    return deserialize(recv_buffer);
}

std::vector<unsigned char> NetClient::serialize(const Message &message)
{
    std::vector<unsigned char> buffer;

    // Reserve enough space in the buffer for all fields
    buffer.reserve(18 + message.size);

    // Serialize each field into the buffer
    unsigned short method_net = htons(message.method);
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char *>(&method_net),
                  reinterpret_cast<unsigned char *>(&method_net) + sizeof(method_net));

    unsigned int hash_net = htonl(message.hash);
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char *>(&hash_net),
                  reinterpret_cast<unsigned char *>(&hash_net) + sizeof(hash_net));

    unsigned int timestamp_net = htonl(message.timestamp);
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char *>(&timestamp_net),
                  reinterpret_cast<unsigned char *>(&timestamp_net) + sizeof(timestamp_net));

    size_t size_net = htobe64(message.size); // Big endian for size_t
    buffer.insert(buffer.end(), reinterpret_cast<unsigned char *>(&size_net),
                  reinterpret_cast<unsigned char *>(&size_net) + sizeof(size_net));

    // Append the payload (data)
    buffer.insert(buffer.end(), message.data.begin(), message.data.end());

    return buffer;
}

Message NetClient::deserialize(const std::vector<unsigned char> &buffer)
{
    // Deserialize each field from the buffer
    size_t offset = 0;

    unsigned short method_net;
    std::memcpy(&method_net, &buffer[offset], sizeof(method_net));
    method_net = ntohs(method_net);
    offset += sizeof(method_net);

    unsigned int hash_net;
    std::memcpy(&hash_net, &buffer[offset], sizeof(hash_net));
    hash_net = ntohl(hash_net);
    offset += sizeof(hash_net);

    unsigned int timestamp_net;
    std::memcpy(&timestamp_net, &buffer[offset], sizeof(timestamp_net));
    timestamp_net = ntohl(timestamp_net);
    offset += sizeof(timestamp_net);

    size_t size_net;
    std::memcpy(&size_net, &buffer[offset], sizeof(size_net));
    size_net = be64toh(size_net);
    offset += sizeof(size_net);

    std::string data(buffer.begin() + offset, buffer.begin() + offset + size_net);

    // Construct the Message object
    return Message(method_net, data, timestamp_net);
}

} // namespace rawnet::protocol
