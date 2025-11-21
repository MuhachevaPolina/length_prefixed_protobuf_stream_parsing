cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser VERSION 1.0.0 DESCRIPTION "Разбор потока length-prefixed Protobuf сообщений на C++")

cmake_minimum_required(VERSION 3.10)
project(ProtobufParser)

# Настройка C++ стандарта
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск зависимостей
find_package(Protobuf REQUIRED)
find_package(GTest REQUIRED)

# Включение директорий для заголовочных файлов
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/src/protobuf
)

# Генерация protobuf файлов
protobuf_generate_cpp(PROTO_SRCS PROTO_HDS
    src/protobuf/message.proto
)

# Создание статической библиотеки protobuf_parser
add_library(protobuf_parser STATIC
    ${PROTO_SRCS}
)

# Подключение protobuf к библиотеке
target_link_libraries(protobuf_parser
    ${PROTOBUF_LIBRARIES}
)

# Включение директорий для библиотеки
target_include_directories(protobuf_parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}/src/protobuf
)

# Тесты для DelimitedMessagesStreamParser
add_executable(test_delimited_messages_parser
    test/Test_DelimitedMessagesStreamParser.cpp
    ${PROTO_SRCS}
)

# Тесты для Helpers
add_executable(test_helpers
    test/Test_Helpers.cpp
    ${PROTO_SRCS}
)

# Подключение библиотек к тестам
target_link_libraries(test_delimited_messages_parser
    protobuf_parser
    GTest::gtest
    GTest::gtest_main
    ${PROTOBUF_LIBRARIES}
)

target_link_libraries(test_helpers
    protobuf_parser
    GTest::gtest
    GTest::gtest_main
    ${PROTOBUF_LIBRARIES}
)

# Добавление тестов в CTest
include(GoogleTest)
gtest_discover_tests(test_delimited_messages_parser)
gtest_discover_tests(test_helpers)

# Установка выходных файлов protobuf в правильную директорию
set_target_properties(protobuf_parser PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Копирование заголовочных файлов в билд директорию для include
file(COPY src/protobuf_parser DESTINATION ${CMAKE_BINARY_DIR}/include)
