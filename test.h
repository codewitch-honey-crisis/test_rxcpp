#include <stdint.h>
typedef struct match_capture {
        int8_t capture[256];
        unsigned long long position;
        size_t length;
} match_capture_t;
typedef int32_t(*match_callback)(unsigned long long* out_advance,void* state);
match_capture_t match(int8_t* dfa, int8_t* blockEnd, unsigned long long* position, match_callback callback, void* callback_state) {
        match_capture_t result;
        result.position = 0;
        result.length = 0;
        unsigned long long adv = 0;
        int tlen;
        int8_t tto;
        int8_t prlen;
        int8_t pmin;
        int8_t pmax;
        uint32_t i, j;
        uint32_t ch;
        int8_t state = 0;
        int8_t acc = -1;
        int done;
        unsigned long long cursorPos = *position;
        ch = callback(&adv, callback_state);
        while (ch != -1) {
                result.length = 0;
                result.position = cursorPos;
                acc = -1;
                done = 0;
                while (!done) {
start_dfa:
                        done = 1;
                        acc = dfa[state++];
                        tlen = dfa[state++];
                        for (i = 0; i < tlen; ++i) {
                                tto = dfa[state++];
                                prlen = dfa[state++];
                                for (j = 0; j < prlen; ++j) {
                                        pmin = dfa[state++];
                                        pmax = dfa[state++];
                                        if (ch < pmin) {
                                                break;
                                        }
                                        if (ch <= pmax) {
                                                if (result.length < 256) {
                                                        result.capture[result.length++] = ch;
                                                }
                                                ch = callback(&adv, callback_state);
                                                ++cursorPos;
                                                state = tto;
                                                done = 0;
                                                goto start_dfa;
                                        }
                                }
                        }
                        if (acc != -1) {
                                if (blockEnd != NULL) {
                                        state = 0;
                                        while (ch != -1) {
                                                acc = -1;
                                                done = 0;
                                                while (!done) {
start_block_end:
                                                        done = 1;
                                                        acc = blockEnd[state++];
                                                        tlen = blockEnd[state++];
                                                        for (i = 0; i < tlen; ++i) {
                                                                tto = blockEnd[state++];
                                                                prlen = blockEnd[state++];
                                                                for (j = 0; j < prlen; ++j) {
                                                                        pmin = blockEnd[state++];
                                                                        pmax = blockEnd[state++];
                                                                        if (ch < pmin) {
                                                                                break;
                                                                        }
                                                                        if (ch <= pmax) {
                                                                                if (result.length < 256) {
                                                                                        result.capture[result.length++] = ch;
                                                                                }
                                                                                ch = callback(&adv, callback_state);
                                                                                ++cursorPos;
                                                                                state = tto;
                                                                                done = 0;
                                                                                goto start_block_end;

                                                                        }
                                                                }
                                                        }
                                                }
                                                if (acc != -1) {
                                                        return result;
                                                }
                                                else {
                                                        if (result.length < 256) {
                                                                result.capture[result.length++] = ch;
                                                        }
                                                        ch = callback(&adv, callback_state);
                                                        ++cursorPos;
                                                }
                                                state = 0;
                                        }
                                        state = 0;
                                        continue;
                                }
                                else {
                                        if (result.length > 0) {
                                                return result;
                                        }
                                }
                        }
                        ch = callback(&adv, callback_state);
                        ++cursorPos;
                        state = 0;
                }
        }
        result.length = 0;
        return result;
}