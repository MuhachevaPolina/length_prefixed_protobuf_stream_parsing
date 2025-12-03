/*
 * DelimitedMessagesStreamParser.hpp
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */

#ifndef SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_
#define SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_

#include "helpers.hpp"


template <typename MessageType>
class DelimitedMessagesStreamParser
{
 public:
  typedef std::shared_ptr<const MessageType> PointerToConstValue;

  std::list<PointerToConstValue> parse(const std::string& data)
  {
    std::list<PointerToConstValue> result;
    size_t bytesConsumed = 0;
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    while (true) 
    {
      auto message = parseDelimited<MessageType>(m_buffer.data(), m_buffer.size(), &bytesConsumed);
      if (!message) 
      {
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

#endif /* SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_ */
