#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
struct CEvent { unsigned long Index; float Time; };
static int g_smallCopies = 0;
static CEvent g_pool[16];
static int g_poolUsed = 0;
struct CSmallVectorEvents {
    CEvent* Start; int Size;
    CSmallVectorEvents() : Start(0), Size(0) { }
    CSmallVectorEvents(const CSmallVectorEvents& other) : Start(0), Size(0) { Copy(other); }
    void Copy(const CSmallVectorEvents& other)
    {
        ++g_smallCopies;
        Start = &g_pool[g_poolUsed]; g_poolUsed += other.Size; Size = other.Size;
        for (int k = 0; k < other.Size; ++k) Start[k] = other.Start[k];
    }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CSmallVectorEvents> SAnimEventsEntry;
static CEvent g_events[3][2];
typedef SAnimEventsEntry TElem;
static void Fill(TElem& e, int i)
{
    e.first = 77u * i; g_events[i - 1][0].Index = i; g_events[i - 1][0].Time = 0.5f * i; g_events[i - 1][1].Index = 10u * i; g_events[i - 1][1].Time = 1.0f; e.second.Start = g_events[i - 1]; e.second.Size = i == 3 ? 1 : 2;
}
static bool Check(const TElem& e, int i)
{
    return e.first == 77u * i && e.second.Size == (i == 3 ? 1 : 2) && e.second.Start != g_events[i - 1] && e.second.Start[0].Index == (unsigned long)i && e.second.Start[0].Time == 0.5f * i;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    if (g_smallCopies != 3) ok = false;
    if (!ok) { std::printf("006fbae0_TEST FAIL\n"); return 1; }
    std::printf("006fbae0_TEST PASS\n");
    return 0;
}