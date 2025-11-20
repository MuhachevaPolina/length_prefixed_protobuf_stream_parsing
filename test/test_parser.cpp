#include <gtest/gtest.h>

#include "../parser.h"
#include "../serialize.h"

#include "../build/message.pb.h"

TEST(ParseTest, OneMessageTest) {
    TestTask::Messages::WrapperMessage msg;
    msg.mutable_fast_response()->set_current_date_time("19851019T050107.333");
    auto serialized = serializeDelimited(msg);

    size_t bytesUsed;
    auto parsed = parseDelimited<TestTask::Messages::WrapperMessage>(serialized->data(), serialized->size(), &bytesUsed);

    ASSERT_NE(parsed, nullptr);
    EXPECT_EQ(bytesUsed, serialized->size());
    EXPECT_EQ(parsed->fast_response().current_date_time(), "19851019T050107.333");
}

TEST(ParseTest, SeveralMessagesTest) {
    TestTask::Messages::WrapperMessage msg1;
    msg1.mutable_fast_response()->set_current_date_time("19851019T050107.333");
    auto serialized1 = serializeDelimited(msg1);

    TestTask::Messages::WrapperMessage msg2;
    msg2.mutable_slow_response()->set_connected_client_count(40);
    auto serialized2 = serializeDelimited(msg2);

    TestTask::Messages::WrapperMessage msg3;
    msg3.mutable_request_for_slow_response()->set_time_in_seconds_to_sleep(60);
    auto serialized3 = serializeDelimited(msg3);

    std::vector<char> buffer;
    buffer.insert(buffer.end(), serialized1->begin(), serialized1->end());
    buffer.insert(buffer.end(), serialized2->begin(), serialized2->end());
    buffer.insert(buffer.end(), serialized3->begin(), serialized3->end());

    size_t bytesUsed;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;
    auto messages = parser.parse(std::string(buffer.begin(), buffer.end()));
    
    ASSERT_EQ(messages.size(), 3);
    
    auto iterator = messages.begin();
    EXPECT_TRUE((*iterator)->has_fast_response());
    EXPECT_TRUE((*(++iterator))->has_slow_response());
    EXPECT_TRUE((*(++iterator))->has_request_for_slow_response());
}

TEST(ParseTest, PartialSendMessagesTest) {
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    TestTask::Messages::WrapperMessage msg1;
    msg1.mutable_fast_response()->set_current_date_time("19851019T050107.333");
    auto serialized1 = serializeDelimited(msg1);

    TestTask::Messages::WrapperMessage msg2;
    msg2.mutable_slow_response()->set_connected_client_count(40);
    auto serialized2 = serializeDelimited(msg2);

    std::vector<char> buffer;
    buffer.insert(buffer.end(), serialized1->begin(), serialized1->end());
    buffer.insert(buffer.end(), serialized2->begin(), serialized2->end());

    auto messages1 = parser.parse(std::string(buffer.begin(), buffer.begin() + 5));
    EXPECT_TRUE(messages1.empty());

    auto messages2 = parser.parse(std::string(buffer.begin() + 5, buffer.end()));
    ASSERT_EQ(messages2.size(), 2);
    
    auto iterator = messages2.begin();
    EXPECT_TRUE((*iterator)->has_fast_response());
    EXPECT_TRUE((*(++iterator))->has_slow_response());

}

TEST(ParseTest, EmptyMessage) {
    TestTask::Messages::WrapperMessage msg1;
}