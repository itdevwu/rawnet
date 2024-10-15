// Copyright (c) 2024 itdevwu.

#ifndef NET_HPP
#define NET_HPP

#include "message.hpp"
#include <boost/asio.hpp>
#include <vector>

namespace rawnet::protocol
{

class NetClient
{
  public:
    NetClient(boost::asio::io_context &io_context, const std::string &server_address, unsigned short port);

    // send a Message
    void send_message(const Message &message);

    // receive a Message
    Message receive_message(size_t buffer_size = 8192);

  private:
    std::vector<unsigned char> serialize(const Message &message);

    Message deserialize(const std::vector<unsigned char> &buffer);

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint server_endpoint_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
};

} // namespace rawnet::protocol

#endif // NET_HPP
