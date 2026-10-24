// Behaviour test for 0x006BED50 std::__unguarded_partition(pair<long, CRegisteredMusicEntry>*, .., pivot, comp).
#include <cstdio>
#include <cstring>
static int g_live = 0;
struct CWideString {
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

template <class T> static void IterSwap(T* a, T* b) { T tmp = *a; *a = *b; *b = tmp; }

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter UnguardedPartition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        IterSwap(first, last);
        ++first;
    }
}

int main()
{
    int ok = 1;
    {
        Pair v[7] = { Pair(12), Pair(-3), Pair(8), Pair(5), Pair(0), Pair(9), Pair(2) };
        int live = g_live;
        Pair* cut = UnguardedPartition(v, v + 7, Pair(5), KeyLess());
        if (g_live != live) ok = 0;
        if (cut <= v || cut >= v + 7) ok = 0;
        for (Pair* p = v; ok && p < cut; ++p) if (p->first > 5 || !Consistent(*p)) ok = 0;
        for (Pair* p = cut; ok && p < v + 7; ++p) if (p->first < 5 || !Consistent(*p)) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006bed50_TEST FAIL\n"); return 1; }
    std::printf("006bed50_TEST PASS\n");
    return 0;
}