// Copyright (c) 2024 itdevwu.

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <chrono>
#include <cstddef>
#include <string>
#include <utility>

#include <boost/asio.hpp>

namespace rawnet::protocol
{

class Message
{
  public:
    // 0: GET, 1: POST
    unsigned short method;

    unsigned int hash;

    // UNIX timestamp
    unsigned int timestamp;

    // payload size
    size_t size;

    // payload
    std::string data;

    // djb2 hash function
    unsigned int hash_payload();

    // hash entire message
    unsigned int hash_message();

    // is timeout
    bool is_timeout(unsigned int timeout = 1000);

    // constructor
    Message(unsigned short method, std::string data, unsigned int timestamp = 0) : method(method), data(std::move(data))
    {
        if (timestamp == 0)
        {
            timestamp =
                std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch())
                    .count();
        }
        this->timestamp = timestamp;

        this->size = this->data.size();
        this->hash = this->hash_payload();
    }
    // destructor
    ~Message() = default;
};

} // namespace rawnet::protocol

#endif // MESSAGE_HPP
