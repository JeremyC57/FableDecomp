// Behaviour test for 0x008899C0 vector<CBlendedEnvironmentTheme>::vector(const vector& x): exactly x.size() 500-byte slots (none for an empty x), every element copy-constructed in order, finish = start + size.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_copies = 0;
struct Theme { float TimeOfDay; unsigned char rest[0x1F4 - 4]; Theme() { } Theme(const Theme& o) : TimeOfDay(o.TimeOfDay) { ++g_copies; } };
struct Vec {
    Theme* start; Theme* finish; Theme* eos;
    Vec() : start(0), finish(0), eos(0) { }
    Vec(const Vec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n ? (Theme*)std::malloc(n * sizeof(Theme)) : 0;
        finish = start; eos = start + n;
        Theme* cur = start;
        for (const Theme* p = x.start; p != x.finish; ++p, ++cur) new (cur) Theme(*p);
        finish = cur;
    }
};
static int fail() { std::printf("008899c0_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(Theme) != 0x1F4) return fail();
    Vec e; Vec ce(e);
    if (ce.start != 0 || ce.finish != 0 || ce.eos != 0 || g_copies != 0) return fail();
    Vec a; a.start = (Theme*)std::malloc(3 * sizeof(Theme)); a.finish = a.start + 3; a.eos = a.finish;
    for (int i = 0; i < 3; ++i) a.start[i].TimeOfDay = 6.0f * (float)i;
    Vec b(a);
    if (b.start == a.start || b.finish - b.start != 3 || b.eos != b.finish || g_copies != 3 || b.start[2].TimeOfDay != 12.0f) return fail();
    std::free(a.start); std::free(b.start);
    std::printf("008899c0_TEST PASS\n");
    return 0;
}