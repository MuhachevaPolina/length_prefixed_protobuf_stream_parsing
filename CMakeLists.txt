cmake_minimum_required(VERSION 3.10)
project(ProtobufParser)

# Находим Protobuf
find_package(Protobuf REQUIRED)

# Находим GTest
find_package(GTest REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDS message.proto)


# Основное приложение
# add_executable(parser_app ${PROTO_SRCS})
#target_include_directories(parser_app PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
#target_link_libraries(parser_app ${PROTOBUF_LIBRARIES})

# Тесты
add_executable(parser_tests test/test_parser.cpp ${PROTO_SRCS})
target_include_directories(parser_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(parser_tests 
    GTest::gtest 
    GTest::gtest_main 
    ${PROTOBUF_LIBRARIES}
)

# Добавляем тесты в CTest
include(GoogleTest)
gtest_discover_tests(parser_tests)
