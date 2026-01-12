#include "ProtocolParser.h"
#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void setUp(void) { ArduinoFakeReset(); }

void tearDown(void) {}

void test_parse_valid_packet(void) {
  // Arrange
  String input = "!!MASTER:REQUEST:MAKE_STAR{color=red}##";
  MessageParts parts;

  // Act
  bool success = parseMessage(input, parts);

  // Assert
  TEST_ASSERT_TRUE(success);
  TEST_ASSERT_EQUAL_STRING("REQUEST", parts.type.c_str());
  TEST_ASSERT_EQUAL_STRING("MAKE_STAR", parts.command.c_str());
  TEST_ASSERT_EQUAL_STRING("color=red", parts.parameters.c_str());
  TEST_ASSERT_EQUAL(1, parts.addressCount);
  TEST_ASSERT_EQUAL_STRING("MASTER", parts.addresses[0].c_str());
}

void test_parse_invalid_packet_no_markers(void) {
  // Arrange
  String input = "MASTER:REQUEST:MAKE_STAR";
  MessageParts parts;

  // Act
  bool success = parseMessage(input, parts);

  // Assert
  TEST_ASSERT_FALSE(success);
}

void test_build_message(void) {
  // Arrange
  MessageParts parts;
  parts.addressCount = 2;
  parts.addresses[0] = "ARM1";
  parts.addresses[1] = "MASTER";
  parts.type = "CONFIRM";
  parts.command = "STATUS";
  parts.parameters = "ok";

  // Act
  String output = buildMessage(parts);

  // Assert
  TEST_ASSERT_EQUAL_STRING("!!ARM1:MASTER:CONFIRM:STATUS{ok}##",
                           output.c_str());
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_parse_valid_packet);
  RUN_TEST(test_parse_invalid_packet_no_markers);
  RUN_TEST(test_build_message);
  UNITY_END();
  return 0;
}
