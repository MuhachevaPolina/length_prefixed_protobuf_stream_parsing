#pragma once
#include <memory>
#include <list>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/message.h>

template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed) {
    const uint8_t* buffer = static_cast<const uint8_t*>(data);
    google::protobuf::io::CodedInputStream codedStream(buffer, size);
    uint32_t messageSize = 0;

    bool isRead = codedStream.ReadVarint32(&messageSize);
    if (!isRead) {
        return nullptr;
    }

    size_t headerSize = codedStream.CurrentPosition();
    if (size < headerSize + messageSize) {
        return nullptr;
    }

    auto message = std::make_shared<Message>();
    bool isParsed = message->ParseFromArray(buffer + headerSize, messageSize);
    if (!isParsed) {
        return nullptr;
    }

    *bytesConsumed = headerSize + messageSize;

    return message;
}

template<typename MessageType>
class DelimitedMessagesStreamParser
      {
public:
typedef std::shared_ptr<const MessageType> PointerToConstValue;
 
        std::list<PointerToConstValue> parse(const std::string& data) {
            std::list<PointerToConstValue> result;
            size_t bytesConsumed = 0;
            m_buffer.insert(m_buffer.end(), data.begin(), data.end());
            while (true) {
                auto message = parseDelimited<MessageType>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
                if (!message) {
                    break;
                }

                result.push_back(message);
                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesConsumed);

            }
            return result;
        }
 
 
private:
        std::vector<char> m_buffer;
      };