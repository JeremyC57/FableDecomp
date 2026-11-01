// Test for _tr_flush_block (0x00C057D0, zlib 1.2.1 trees.c): with level > 0 an unknown s->data_type is classified first
// (binary when bin_freq > ascii_freq/4 over the literal tree), the block lengths are (opt_len+3+7)>>3 and
// (static_len+3+7)>>3 with the smaller kept; level 0 uses stored_len+5. The block is stored when stored_len+4 fits and a
// buffer exists, else sent with the static trees when they are no longer, else with the dynamic trees; eof winds up.
#include <cstdio>
enum { Z_BINARY = 0, Z_ASCII = 1, Z_UNKNOWN = 2 };
enum { STORED = 0, STATIC = 1, DYNAMIC = 2 };
struct State { int level; unsigned char data_type; unsigned short freq[256]; unsigned long opt_len, static_len; int windup; int resets; };
static void set_data_type(State* s)
{
    int n = 0; unsigned ascii_freq = 0, bin_freq = 0;
    while (n < 7)   bin_freq += s->freq[n++];
    while (n < 128) ascii_freq += s->freq[n++];
    while (n < 256) bin_freq += s->freq[n++];
    s->data_type = (unsigned char)(bin_freq > (ascii_freq >> 2) ? Z_BINARY : Z_ASCII);
}
static int tr_flush_block(State* s, const char* buf, unsigned long stored_len, int eof)
{
    unsigned long opt_lenb, static_lenb; int kind;
    if (s->level > 0) {
        if (s->data_type == Z_UNKNOWN) set_data_type(s);
        opt_lenb = (s->opt_len + 3 + 7) >> 3;
        static_lenb = (s->static_len + 3 + 7) >> 3;
        if (static_lenb <= opt_lenb) opt_lenb = static_lenb;
    } else {
        opt_lenb = static_lenb = stored_len + 5;
    }
    if (stored_len + 4 <= opt_lenb && buf != 0) kind = STORED;
    else if (static_lenb == opt_lenb) kind = STATIC;
    else kind = DYNAMIC;
    ++s->resets;
    if (eof) ++s->windup;
    return kind;
}
static State Make(int level) { State s; s.level = level; s.data_type = Z_UNKNOWN; for (int i = 0; i < 256; ++i) s.freq[i] = 0; s.opt_len = s.static_len = 0; s.windup = s.resets = 0; return s; }
int main()
{
    char buf[4]; State s = Make(6);
    s.freq['a'] = 100; s.freq[200] = 10; s.opt_len = 800; s.static_len = 900;
    if (tr_flush_block(&s, buf, 200, 0) != DYNAMIC || s.data_type != Z_ASCII || s.windup != 0) { std::printf("00c057d0_TEST FAIL\n"); return 1; }
    s = Make(6); s.freq[0] = 30; s.freq['a'] = 100; s.opt_len = 800; s.static_len = 700;
    if (tr_flush_block(&s, buf, 200, 1) != STATIC || s.data_type != Z_BINARY || s.windup != 1) { std::printf("00c057d0_TEST FAIL\n"); return 1; }
    s = Make(6); s.data_type = Z_ASCII; s.freq[0] = 1000; s.opt_len = 8000; s.static_len = 9000;
    if (tr_flush_block(&s, buf, 100, 0) != STORED || s.data_type != Z_ASCII) { std::printf("00c057d0_TEST FAIL\n"); return 1; }
    if (tr_flush_block(&s, 0, 100, 0) != DYNAMIC) { std::printf("00c057d0_TEST FAIL\n"); return 1; }
    s = Make(0);
    if (tr_flush_block(&s, buf, 50, 1) != STORED || s.data_type != Z_UNKNOWN || tr_flush_block(&s, 0, 50, 0) != STATIC || s.resets != 2) { std::printf("00c057d0_TEST FAIL\n"); return 1; }
    std::printf("00c057d0_TEST PASS\n");
    return 0;
}