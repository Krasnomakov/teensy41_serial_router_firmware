#include "ProtocolParser.h"
#include <ctype.h>

void trimToken(String& token) {
	token.trim();
}

String normalizeIdentifier(const String& text) {
	String normalized;
	normalized.reserve(text.length());
	for (size_t i = 0; i < static_cast<size_t>(text.length()); ++i) {
		char c = text.charAt(static_cast<unsigned int>(i));
		if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
			continue;
		}
		normalized += static_cast<char>(toupper(static_cast<unsigned char>(c)));
	}
	return normalized;
}

bool parseMessage(const String& raw, MessageParts& out) {
	if (!raw.startsWith("!!") || !raw.endsWith("##")) {
		return false;
	}

	MessageParts result;
	String core = raw.substring(2, raw.length() - 2);
	core.trim();
	String header = core;
	String parameters;

	const int braceStart = core.indexOf('{');
	if (braceStart >= 0) {
		const int braceEnd = core.lastIndexOf('}');
		if (braceEnd < braceStart) {
			return false;
		}
		header = core.substring(0, braceStart);
		parameters = core.substring(braceStart + 1, braceEnd);
		const size_t coreLength = static_cast<size_t>(core.length());
		const size_t closingOffset = static_cast<size_t>(braceEnd + 1);
		if (closingOffset != coreLength) {
			return false; // Trailing characters after closing brace
		}
	}

	String tokens[8];
	int tokenCount = 0;
	int start = 0;
	const size_t headerLength = static_cast<size_t>(header.length());
	for (size_t i = 0; i < headerLength; ++i) {
		if (header.charAt(static_cast<unsigned int>(i)) == ':') {
			tokens[tokenCount++] = header.substring(start, static_cast<int>(i));
			start = static_cast<int>(i) + 1;
			if (tokenCount >= 8) {
				return false;
			}
		}
	}
    tokens[tokenCount++] = header.substring(start);

	if (tokenCount < 2) {
		return false;
	}

	for (int i = 0; i < tokenCount; ++i) {
		trimToken(tokens[i]);
	}

	result.type = tokens[tokenCount - 2];
	result.command = tokens[tokenCount - 1];
	result.parameters = parameters;

	const int addrCount = tokenCount - 2;
	if (addrCount > 0) {
		if (addrCount > 4) {
			return false;
		}
		for (int i = 0; i < addrCount; ++i) {
			result.addresses[i] = tokens[i];
		}
		result.addressCount = static_cast<size_t>(addrCount);
	}

	out = result;
	return true;
}

String buildMessage(const MessageParts& parts) {
	String message = "!!";
	for (size_t i = 0; i < parts.addressCount; ++i) {
		message += parts.addresses[i];
		message += ':';
	}
	message += parts.type;
	message += ':';
	message += parts.command;
	if (parts.parameters.length() > 0) {
		message += '{';
		message += parts.parameters;
		message += '}';
	}
	message += "##";
	return message;
}
