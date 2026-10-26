// Behaviour test for 0x006BE930 std::__unguarded_linear_insert(pair<long, CRegisteredMusicEntry>*, pair, CKeyPairCompareLess).
#include <cstdio>
#include <cstring>
#include <new>
static int g_live = 0;
class CWideString { public:
    char text[16];
    CWideString(const char* t) { std::strcpy(text, t); ++g_live; }
    CWideString(const CWideString& o) { std::strcpy(text, o.text); ++g_live; }
    CWideString& operator=(const CWideString& o) { std::strcpy(text, o.text); return *this; }
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

template <class RandomAccessIter, class T, class Compare>
void UnguardedLinearInsert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

int main()
{
    int ok = 1;
    {
        Pair v[5] = { Pair(-4), Pair(1), Pair(6), Pair(10), Pair(3) };
        int live = g_live;
        UnguardedLinearInsert(v + 4, v[4], KeyLess());
        if (g_live != live) ok = 0;
        int expect[5] = { -4, 1, 3, 6, 10 };
        for (int i = 0; i < 5; ++i) if (v[i].first != expect[i] || !Consistent(v[i])) ok = 0;
        UnguardedLinearInsert(v + 4, v[4], KeyLess());   // already in place
        if (v[4].first != 10 || v[3].first != 6) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006be930_TEST FAIL\n"); return 1; }
    std::printf("006be930_TEST PASS\n");
    return 0;
}