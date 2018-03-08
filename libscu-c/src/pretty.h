#ifndef _PRETTY_H_
#define _PRETTY_H_

#include <stdio.h>

static inline void __attribute__((used))
_scu_prettyprint_integer_value(char *buf, size_t size, unsigned long long value, size_t value_size)
{
	unsigned long long mask = value_size < 8 ? (1ULL << (value_size * 8)) - 1 : ~0ULL;

	if (value & (1LLU << ((value_size * 8) - 1))) {
		snprintf(buf, size, "%llu (0x%llx == %lld)", value & mask, value & mask, value | ~mask);
	} else {
		snprintf(buf, size, "%llu (0x%llx)", value & mask, value & mask);
	}
}

static inline void __attribute__((used))
_scu_prettyprint_pointer_value(char *buf, size_t size, const void *ptr)
{
	if (ptr) {
		snprintf(buf, size, "%p", ptr);
	} else {
		snprintf(buf, size, "NULL");
	}
}

static inline void __attribute__((used))
_scu_prettyprint_bytes_value(char *buf, size_t size, const void *value, size_t value_size)
{
	const unsigned char *value_as_char = (const unsigned char *)value;
	size_t o = 0;

	#define BYTES_PER_LINE 16
	size_t lines = ((value_size + (BYTES_PER_LINE - 1)) / BYTES_PER_LINE);

	for (size_t i = 0; i < lines; i++) {
		bool last_line = (i == (lines - 1));

		for (size_t j = 0; (i * BYTES_PER_LINE + j < value_size) && (j < BYTES_PER_LINE) && (o < (size - 3)); j++) {
			snprintf(buf + o, size, "%02x ", value_as_char[i * BYTES_PER_LINE + j]);
			o += 3;
		}

		if (i && last_line) {
			for (size_t j = 0; j < (BYTES_PER_LINE - (value_size - i * BYTES_PER_LINE)) * 3; j++) {
				buf[o++] = ' ';
			}
		}
		buf[o++] = ' ';

		for (size_t j = 0; (i * BYTES_PER_LINE + j < value_size) && (j < BYTES_PER_LINE) && (o < (size - 1)); j++) {
			unsigned char c = value_as_char[i * BYTES_PER_LINE + j];
			if (c < 32 || c >= 127) {
				buf[o++] = '.';
			} else {
				buf[o++] = c;
			}
		}

		if (last_line) {
			buf[o] = 0;
		} else {
			buf[o++] = '\n';
		}
	}
	#undef BYTES_PER_LINE
}

#endif
