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
    size_t curPosition = 0;
    m_buffer.insert(m_buffer.end(), data.begin(), data.end());
    std::shared_ptr<MessageType> message;
    while (curPosition < m_buffer.size()) 
    {
      //try 
      //{
      size_t tmpBytesConsumed = 0;
        message = parseDelimited<MessageType>(m_buffer.data() + curPosition, m_buffer.size() - curPosition, 
                                              &tmpBytesConsumed);
        if (!message || tmpBytesConsumed == 0) 
        {
          break;
        }
      //}
      //catch (const std::runtime_error &e) 
      //{
      //  break;
      //}

      result.push_back(message);
      curPosition += tmpBytesConsumed;
    }
    
    if (curPosition != 0) {
      m_buffer.erase(m_buffer.begin(), m_buffer.begin() + curPosition);
    }
    return result;
  }
 private:
  std::vector<char> m_buffer;
};

#endif /* SRC_PROTOBUF_PARSER_DELIMITEDMESSAGESSTREAMPARSER_HPP_ */
