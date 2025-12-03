/*
 * helpers.h
 *
 *  Created on: 2 Feb 2023
 *      Author: sia
 */

#ifndef SRC_PROTOBUF_PARSER_HELPERS_H_
#define SRC_PROTOBUF_PARSER_HELPERS_H_

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/message.h>

#include <memory>
#include <list>
#include <vector>
#include <string>
#include <stdexcept> 

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
typedef std::vector<char> Data;
typedef std::shared_ptr<Data> PointerToData;
typedef std::shared_ptr<const Data> PointerToConstData;

template <typename Message> PointerToConstData serializeDelimited(const Message& msg) { 
  const size_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
  const size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

  const PointerToData& result = std::make_shared<Data>(headerSize + messageSize);
  google::protobuf::uint8* buffer = reinterpret_cast<google::protobuf::uint8*>(&*result->begin());

  google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
  msg.SerializeWithCachedSizesToArray(buffer + headerSize);

  return result;
}

/*!
 * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
 *
 * \tparam Message Тип сообщения, для работы с которым предназначена данная
 * функция.
 *
 * \param data Указатель на буфер данных.
 * \param size Размер буфера данных.
 * \param bytesConsumed Количество байт, которое потребовалось для расшифровки
 * сообщения в случае успеха.
 *
 * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то
 * он не пустой.
 */
template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed = 0)
{

  if (!bytesConsumed) 
  {
    throw std::runtime_error("nullptr was given to function (bytesConsumed)");
  }

  const uint8_t* buffer = static_cast<const uint8_t*>(data);
  google::protobuf::io::CodedInputStream codedStream(buffer, size);
  uint32_t messageSize = 0;
  
  if (!data || size == 0) 
  {
    return nullptr;
  }

  bool isRead = codedStream.ReadVarint32(&messageSize);
  if (!isRead) 
  {
    throw std::runtime_error("error while reading header");
  }

  size_t headerSize = codedStream.CurrentPosition();
  if (size < headerSize + messageSize) 
  {
    return nullptr;
  }

  auto message = std::make_shared<Message>();
  bool isParsed = message->ParseFromArray(buffer + headerSize, messageSize);
  if (!isParsed) 
  {
    throw std::runtime_error("error while parsing");
  }

  *bytesConsumed = headerSize + messageSize;

  return message;
}

#endif /* SRC_PROTOBUF_PARSER_HELPERS_H_ */
