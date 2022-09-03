#include <stdint.h>
#include <stdio.h>
#include "test.h"
int8_t ident_dfa[] = {
    -1, 1, 10, 3, 65, 90, 95, 95, 97, 122,
    1, 1, 10, 4, 48, 57, 65, 90, 95, 95,
    97, 122};
typedef struct cb_state {
    char* sz;
} cb_state_t;

int32_t callback(unsigned long long* out_advance, void* state) {
    cb_state_t* ps = (cb_state_t*)state;
    int32_t cp;
    if (!*ps->sz) {
        *out_advance = 0;
        return -1;
    }
    uint8_t byte = (uint8_t)*ps->sz;
    if ((byte & 128) == 0) {
        cp = ((uint32_t) *ps->sz & ~128);
		*out_advance = 1;
	}

	if ((byte & 224) == 192) {
        cp=((uint32_t) ps->sz[0] & ~224) << 6 |
		((uint32_t) ps->sz[1] & ~192);
		*out_advance = 2;
	}

	if ((byte & 240) == 224) {
		cp=((uint32_t) ps->sz[0] & ~240) << 12 |
        ((uint32_t) ps->sz[1] & ~192) << 6 |
        ((uint32_t) ps->sz[2] & ~192);
        *out_advance = 3;
	}

	if ((byte & 248) == 240) {
		cp=((uint32_t) ps->sz[0] & ~248) << 18 |
				((uint32_t) ps->sz[1] & ~192) << 12 |
				((uint32_t) ps->sz[2] & ~192) << 6 |
				((uint32_t) ps->sz[3] & ~192);
        *out_advance = 4;
	}
    ps->sz+=*out_advance;
	
    return cp;
}

int main(int argc, char** argv) {
    char* test = "1234 foobar 5678 abc123 /";
    unsigned long long pos = 0;
    cb_state_t st;
    st.sz = test;
    while (1) {
        match_capture_t c = match(ident_dfa, NULL, &pos, callback, &st);
        if (0 == c.length) {
            return 0;
        }
        c.capture[c.length] = '\0';
        printf("capture: %s\n", c.capture);
    }
    return 0;
}