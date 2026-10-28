// Behaviour test for 0x006BEE80 std::__make_heap(pair<long, CRegisteredMusicEntry>*, .., comp, T*, int*).
#include <cstdio>
#include <cstring>
static int g_live = 0;
class CWideString { public:
    char text[16];
    CWideString(const char* t) { std::strcpy(text, t); ++g_live; }
    CWideString(const CWideString& o) { std::strcpy(text, o.text); ++g_live; }
    const CWideString& operator=(const CWideString& o) { std::strcpy(text, o.text); return *this; }
    ~CWideString() { --g_live; }
};
struct CRegisteredMusicEntry {
    CWideString SrcFileName;
    CWideString DstFileName;
    float Gain;
    float Offset;
    bool CacheToHD;
    CRegisteredMusicEntry(const char* s, const char* d, float g, float o, bool c) : SrcFileName(s), DstFileName(d), Gain(g), Offset(o), CacheToHD(c) { }
    CRegisteredMusicEntry(const CRegisteredMusicEntry& o)
        : SrcFileName(o.SrcFileName), DstFileName(o.DstFileName), Gain(o.Gain), Offset(o.Offset), CacheToHD(o.CacheToHD) { }
};
struct Pair {
    int first;
    CRegisteredMusicEntry second;
    Pair(int k) : first(k), second("s", "d", (float)k, 0.0f, (k & 1) != 0) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};
struct KeyLess { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };
static int Consistent(const Pair& p) { return p.second.Gain == (float)p.first && p.second.CacheToHD == ((p.first & 1) != 0); }

template <class C> static void PushHeap(Pair* first, int hole, int top, Pair value, C comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
template <class C> static void AdjustHeap(Pair* first, int hole, int len, Pair value, C comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) --second;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, value, comp);
}
template <class C> static void MakeHeap(Pair* first, Pair* last, C comp, Pair*, int*)
{
    if (last - first < 2)
        return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        AdjustHeap(first, parent, len, Pair(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}

int main()
{
    int ok = 1;
    {
        Pair v[8] = { Pair(4), Pair(-7), Pair(15), Pair(3), Pair(9), Pair(1), Pair(22), Pair(6) };
        int live = g_live;
        MakeHeap(v, v + 8, KeyLess(), (Pair*)0, (int*)0);
        if (g_live != live) ok = 0;
        for (int i = 1; i < 8; ++i) if (v[(i - 1) / 2].first < v[i].first || !Consistent(v[i])) ok = 0;   // max-heap
        if (v[0].first != 22) ok = 0;
        Pair one[1] = { Pair(5) };
        MakeHeap(one, one + 1, KeyLess(), (Pair*)0, (int*)0);
        if (one[0].first != 5) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006bee80_TEST FAIL\n"); return 1; }
    std::printf("006bee80_TEST PASS\n");
    return 0;
}