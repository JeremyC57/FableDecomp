// behaviour test for 0x0054C160 NUISystem::CSlider::AddAction: with room the counted pointer is copied into the next slot
// (count bumped only when the pointer has a count record) and finish advances; when full the growth path takes the value
#include <cstdio>
struct Info { unsigned int RefCount; };
struct CP { void* Data; Info* PInfo; };
struct Vec { CP* start; CP* finish; CP* eos; };
static int g_overflow = 0;
static CP g_big[8];
static void overflow(Vec& v, const CP& x)
{
    ++g_overflow;
    int n = (int)(v.finish - v.start);
    for (int i = 0; i < n; ++i) g_big[i] = v.start[i];
    g_big[n] = x;
    if (x.PInfo) ++x.PInfo->RefCount;
    v.start = g_big; v.finish = g_big + n + 1; v.eos = g_big + 8;
}
static void push_back(Vec& v, const CP& x)
{
    if (v.finish != v.eos) {
        if (v.finish) { v.finish->Data = x.Data; v.finish->PInfo = x.PInfo; if (x.PInfo) ++x.PInfo->RefCount; }
        ++v.finish;
    } else
        overflow(v, x);
}
int main()
{
    int fail = 0;
    static CP small[2];
    Vec v; v.start = small; v.finish = small; v.eos = small + 2;
    Info info; info.RefCount = 1;
    int obj = 7;
    CP a; a.Data = &obj; a.PInfo = &info;
    CP z; z.Data = 0; z.PInfo = 0;
    push_back(v, a);
    if (v.finish != small + 1 || small[0].Data != &obj || small[0].PInfo != &info || info.RefCount != 2) fail |= 1;
    push_back(v, z);
    if (v.finish != small + 2 || small[1].PInfo != 0 || info.RefCount != 2 || g_overflow != 0) fail |= 2;
    push_back(v, a);
    if (g_overflow != 1 || v.start != g_big || v.finish != g_big + 3 || g_big[2].PInfo != &info || info.RefCount != 3) fail |= 4;
    if (g_big[0].Data != &obj || g_big[1].Data != 0) fail |= 8;
    if (fail) { std::printf("0054c160_TEST FAIL %d\n", fail); return 1; }
    std::printf("0054c160_TEST PASS\n");
    return 0;
}