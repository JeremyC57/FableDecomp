// Behaviour test for zlib 1.2.1 deflate.c lm_init (0x00C00DD0, static).
#include <stdio.h>
#include <string.h>
typedef unsigned int uInt; typedef unsigned short ush; typedef unsigned long ulg;
struct deflate_state { uInt w_size; ulg window_size; ush* head; uInt ins_h; uInt hash_size; long block_start; uInt match_length;
    int match_available; uInt strstart; uInt lookahead; uInt prev_length; uInt max_chain_length; uInt max_lazy_match; int level;
    uInt good_match; int nice_match; };
struct config { ush good_length, max_lazy, nice_length, max_chain; };
static const config configuration_table[10] = { {0,0,0,0},{4,4,8,4},{4,5,16,8},{4,6,32,32},{4,4,16,16},{8,16,32,32},{8,16,128,128},{8,32,128,256},{32,128,258,1024},{32,258,258,4096} };
static void lm_init(deflate_state* s)
{
    s->window_size = (ulg)2L * s->w_size;
    s->head[s->hash_size - 1] = 0;
    memset(s->head, 0, (unsigned)(s->hash_size - 1) * sizeof(*s->head));
    s->max_lazy_match = configuration_table[s->level].max_lazy;
    s->good_match = configuration_table[s->level].good_length;
    s->nice_match = configuration_table[s->level].nice_length;
    s->max_chain_length = configuration_table[s->level].max_chain;
    s->strstart = 0; s->block_start = 0L; s->lookahead = 0;
    s->match_length = s->prev_length = 3 - 1;
    s->match_available = 0; s->ins_h = 0;
}
int main()
{
    static ush head[16]; int i; for (i = 0; i < 16; i++) head[i] = 0x55;
    deflate_state s; memset(&s, 0x77, sizeof(s)); s.w_size = 0x8000; s.head = head; s.hash_size = 16; s.level = 9;
    lm_init(&s);
    for (i = 0; i < 16; i++) if (head[i] != 0) { printf("00c00dd0_TEST FAIL\n"); return 1; }
    if (s.window_size != 0x10000 || s.max_lazy_match != 258 || s.good_match != 32 || s.nice_match != 258 || s.max_chain_length != 4096
        || s.strstart != 0 || s.block_start != 0 || s.lookahead != 0 || s.match_length != 2 || s.prev_length != 2 || s.match_available != 0 || s.ins_h != 0)
    { printf("00c00dd0_TEST FAIL\n"); return 1; }
    s.level = 1; lm_init(&s);
    if (s.max_lazy_match != 4 || s.nice_match != 8 || s.max_chain_length != 4) { printf("00c00dd0_TEST FAIL\n"); return 1; }
    printf("00c00dd0_TEST PASS\n"); return 0;
}