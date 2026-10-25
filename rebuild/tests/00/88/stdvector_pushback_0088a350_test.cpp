// Behaviour test for 0x0088A350 vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::push_back: in-place copy while there is room, overflow path otherwise.
#include <cstdio>
#include <cstring>
#include <new>
struct CFeatDef { unsigned char Data[0x1F4]; };
static int g_overflows = 0;
struct Vec {
    CFeatDef* start; CFeatDef* finish; CFeatDef* eos;
    void overflow(CFeatDef* pos, const CFeatDef& x)
    {
        ++g_overflows;
        unsigned int n = finish - start, cap = n ? 2 * n : 1;
        CFeatDef* s = (CFeatDef*)::operator new(cap * sizeof(CFeatDef));
        for (unsigned int i = 0; i < n; ++i) new (s + i) CFeatDef(start[i]);
        new (s + n) CFeatDef(x);
        ::operator delete(start);
        start = s; finish = s + n + 1; eos = s + cap; (void)pos;
    }
    void push_back(const CFeatDef& x)
    {
        if (finish != eos) { new (finish) CFeatDef(x); ++finish; }
        else overflow(finish, x);
    }
};
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CFeatDef d; std::memset(d.Data, 0, sizeof(d.Data));
    for (int i = 0; i < 5; ++i) { d.Data[0] = (unsigned char)i; d.Data[0x1F3] = (unsigned char)(i + 9); v.push_back(d); }
    if (v.finish - v.start != 5 || g_overflows != 4 || v.start[3].Data[0] != 3 || v.start[4].Data[0x1F3] != 13 || sizeof(CFeatDef) != 0x1F4) {
        std::printf("0088a350_TEST FAIL\n"); return 1;
    }
    ::operator delete(v.start);
    std::printf("0088a350_TEST PASS\n");
    return 0;
}