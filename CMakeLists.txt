cmake_minimum_required(VERSION 3.15.0)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser VERSION 1.0.0 DESCRIPTION "Разбор потока length-prefixed Protobuf сообщений на C++")

find_package(Protobuf REQUIRED)

find_package(GTest REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDS src/protobuf/message.proto)

add_executable(parser_tests test/Test_DelimitedMessagesStreamParser.cpp test/Test_Helpers.cpp ${PROTO_SRCS})
target_include_directories(parser_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(parser_tests 
    GTest::gtest 
    GTest::gtest_main 
    ${PROTOBUF_LIBRARIES}
)

include(GoogleTest)
gtest_discover_tests(parser_tests)

