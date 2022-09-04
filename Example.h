#ifndef Example_H_HEADER
#define Example_H_HEADER
#include <stdint.h>
#ifdef ARDUINO
#ifndef ESP32
#include <avr/progmem.h>
#else
#include <progmem.h>
#endif
#endif
typedef struct Example_capture {
	int32_t capture[256];
	unsigned long long position;
	size_t length;
} Example_capture_t;
typedef int32_t(*Example_callback)(unsigned long long* out_advance,void* state);
Example_capture_t match_JsonStringLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_VerbatimStringLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_StringLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_CharacterLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_IntegerLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_FloatLiteral(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_Whitespace(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_Identifier(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_CIdentifier(unsigned long long* position,Example_callback callback, void* callback_state);
Example_capture_t match_CommentBlock(unsigned long long* position,Example_callback callback, void* callback_state);
#endif
