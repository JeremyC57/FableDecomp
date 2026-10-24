// Behaviour test for 0x007B4F20 back_insert_iterator<vector<CCrimeWaitingForBribe> >::operator=: in-place append or growth.
#include <cstdio>
struct Rec { int a, b; };
static int g_grow = 0;
struct Vec {
    Rec* start; Rec* finish; Rec* eos;
    Rec big[8];
    void grow(Rec* pos, const Rec& x) { ++g_grow; *pos = x; finish = pos + 1; eos = big + 8; }
    void push_back(const Rec& x) { if (finish != eos) { *finish = x; ++finish; } else grow(finish, x); }
};
struct Inserter { Vec* c; Inserter& operator=(const Rec& v) { c->push_back(v); return *this; } };
int main()
{
    Vec v; v.start = v.big; v.finish = v.big; v.eos = v.big + 1;
    Inserter it; it.c = &v;
    Rec r1 = { 1, 2 }, r2 = { 3, 4 };
    Inserter& self = (it = r1);
    if (&self != &it || v.finish != v.big + 1 || v.big[0].a != 1 || g_grow != 0) { std::printf("007b4f20_TEST FAIL\n"); return 1; }
    it = r2;   // full: grows
    if (g_grow != 1 || v.finish != v.big + 2 || v.big[1].b != 4) { std::printf("007b4f20_TEST FAIL\n"); return 1; }
    std::printf("007b4f20_TEST PASS\n");
    return 0;
}