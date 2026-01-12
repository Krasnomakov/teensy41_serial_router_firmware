#pragma once

#include <Arduino.h>

struct MessageParts {
	String addresses[4];
	size_t addressCount = 0;
	String type;
	String command;
	String parameters;
};

void trimToken(String& token);
String normalizeIdentifier(const String& text);
bool parseMessage(const String& raw, MessageParts& out);
String buildMessage(const MessageParts& parts);
