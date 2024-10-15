// Copyright (c) 2024 itdevwu.

#include "message.hpp"

unsigned int rawnet::protocol::Message::hash_payload()
{
    unsigned int hash = 0;
    for (const auto &c : this->data)
    {
        hash = (hash << 5) + c;
    }
    return hash;
}

unsigned int rawnet::protocol::Message::hash_message()
{
    unsigned int hash = this->hash_payload();
    hash = (hash << 5) + this->method;
    hash = (hash << 5) + this->timestamp;
    return hash;
}

bool rawnet::protocol::Message::is_timeout(unsigned int timeout)
{
    unsigned int now =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return now - this->timestamp > timeout;
}