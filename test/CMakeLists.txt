add_executable(test_protobuf_parser
    Test_DelimitedMessagesStreamParser.cpp
    Test_Helpers.cpp
)

target_link_libraries(test_protobuf_parser
    protobuf_parser
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(test_protobuf_parser
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        ${CMAKE_BINARY_DIR}
)

add_test(NAME ProtobufParserTests COMMAND test_protobuf_parser)