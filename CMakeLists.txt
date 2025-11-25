cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser VERSION 1.0.0 DESCRIPTION "Разбор потока length-prefixed Protobuf сообщений на C++")

cmake_minimum_required(VERSION 3.10)
project(ProtobufParser)

# Включаем тестирование
enable_testing()

# Найти Protobuf и GoogleTest
find_package(Protobuf REQUIRED)
find_package(GTest REQUIRED)

# Директория для сгенерированных .pb.h/.pb.cc
set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY ${PROTO_GEN_DIR})

# Сгенерировать protobuf файлы
set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf/message.proto")
set(PROTO_HEADER "${PROTO_GEN_DIR}/message.pb.h")
set(PROTO_SRC "${PROTO_GEN_DIR}/message.pb.cc")

add_custom_command(
    OUTPUT ${PROTO_HEADER} ${PROTO_SRC}
    COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
    ARGS --cpp_out=${PROTO_GEN_DIR}
         --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf
         ${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
    COMMENT "Generating protobuf files"
)

# Добавляем сгенерированные файлы в источник
add_library(protobuf_messages ${PROTO_SRC})
target_include_directories(protobuf_messages
    PUBLIC
        ${PROTO_GEN_DIR}
        ${Protobuf_INCLUDE_DIRS}
)
target_link_libraries(protobuf_messages ${Protobuf_LIBRARIES})

# Собираем основную библиотеку (пока только заголовки, но для порядка)
add_library(protobuf_parser INTERFACE)
target_sources(protobuf_parser INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf_parser/DelimitedMessagesStreamParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protobuf_parser/helpers.hpp
)
target_include_directories(protobuf_parser
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${PROTO_GEN_DIR}
)
target_link_libraries(protobuf_parser INTERFACE protobuf_messages)

# Подключаем тесты
add_subdirectory(test)