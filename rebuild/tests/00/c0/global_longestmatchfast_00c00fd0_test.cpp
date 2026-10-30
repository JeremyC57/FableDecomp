// Behaviour test for zlib deflate.c longest_match_fast (0x00C00FD0): single-candidate match length, clamped to lookahead.
#include <cstdio>
typedef unsigned char Bytef; typedef unsigned int uInt; typedef unsigned IPos;
#define MAX_MATCH 258
#define MIN_MATCH 3
struct deflate_state { Bytef* window; uInt strstart; uInt match_start; uInt lookahead; };
static uInt longest_match_fast(deflate_state* s, IPos cur_match)
{
    Bytef* scan = s->window + s->strstart; Bytef* match; int len;
    Bytef* strend = s->window + s->strstart + MAX_MATCH;
    match = s->window + cur_match;
    if (match[0] != scan[0] || match[1] != scan[1]) return MIN_MATCH - 1;
    scan += 2, match += 2;
    do {
    } while (*++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match &&
             *++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match && scan < strend);
    len = MAX_MATCH - (int)(strend - scan);
    if (len < MIN_MATCH) return MIN_MATCH - 1;
    s->match_start = cur_match;
    return len <= (int)s->lookahead ? len : s->lookahead;
}
int main()
{
    static Bytef w[1024];
    const char* t = "abcdefXYabcdefZZ";
    for (int i = 0; t[i]; i++) w[i] = (Bytef)t[i];
    deflate_state s = { w, 8, 0, 100 };
    uInt r = longest_match_fast(&s, 0);
    if (r != 6 || s.match_start != 0) { std::printf("00c00fd0_TEST FAIL\n"); return 1; }
    s.match_start = 77; if (longest_match_fast(&s, 1) != 2 || s.match_start != 77) { std::printf("00c00fd0_TEST FAIL\n"); return 1; }
    std::printf("00c00fd0_TEST PASS\n"); return 0;
}