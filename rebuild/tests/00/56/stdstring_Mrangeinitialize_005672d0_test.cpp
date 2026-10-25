// Behaviour test for 0x005672D0 basic_string::_M_range_initialize(first, last, forward tag): copy [first, last) and terminate.
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Str {
    char* start; char* finish; char* eos; int throws;
    void range_init(const char* f, const char* l)
    {
        int n = l - f;
        unsigned int blk = n + 1;
        if (blk <= 0xFFFFFFFFu && blk > 0) { start = (char*)std::malloc(blk); finish = start; eos = start + blk; }
        else { ++throws; return; }
        finish = (l == f) ? start : (char*)std::memmove(start, f, l - f) + (l - f);
        *finish = 0;
    }
};
int main()
{
    const char* text = "Lionhead";
    Str s; s.throws = 0;
    s.range_init(text, text + 4);
    if (std::strcmp(s.start, "Lion") != 0 || s.finish != s.start + 4 || s.eos != s.start + 5 || s.throws) { std::printf("005672d0_TEST FAIL\n"); return 1; }
    Str e; e.throws = 0;
    e.range_init(text, text);
    if (e.start[0] != 0 || e.finish != e.start || e.eos != e.start + 1) { std::printf("005672d0_TEST FAIL\n"); return 1; }
    std::free(s.start); std::free(e.start);
    std::printf("005672d0_TEST PASS\n");
    return 0;
}