// Behaviour test for 00cb7210: std::vector<pair<CCharString,CScriptInfo> >::push_back -- copy at the finish when there is room, else the overflow path
#include <cstdio>
#include <cstring>
static int g_overflows = 0;
struct Rec { char name[8]; int a, b; char value[8]; int c; bool d; };
struct CVec {
    Rec* start; Rec* finish; Rec* eos; Rec store[4];
    void overflow(const Rec& x) { ++g_overflows; (void)x; }
    void push_back(const Rec& x) { if (finish != eos) { *finish = x; ++finish; } else overflow(x); }
};
int main()
{
    CVec v; v.start = v.store; v.finish = v.store; v.eos = v.store + 2;
    Rec r; std::strcpy(r.name, "n"); r.a = 1; r.b = 2; std::strcpy(r.value, "v"); r.c = 3; r.d = true;
    v.push_back(r); r.a = 5; v.push_back(r); v.push_back(r);
    if (v.finish - v.start != 2 || g_overflows != 1 || v.store[0].a != 1 || v.store[1].a != 5 || !v.store[1].d || std::strcmp(v.store[1].value, "v") != 0)
    { std::printf("00cb7210_TEST FAIL\n"); return 1; }
    std::printf("00cb7210_TEST PASS\n");
    return 0;
}