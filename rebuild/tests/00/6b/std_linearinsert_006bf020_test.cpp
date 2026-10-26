// Behaviour test for 0x006BF020 std::__linear_insert(pair<long, CRegisteredMusicEntry>*, .., val, comp).
#include <cstdio>
#include <cstring>
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

static Pair* CopyBackward(Pair* first, Pair* last, Pair* result)
{
    for (long n = (long)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
template <class RandomAccessIter, class T, class Compare>
void UnguardedLinearInsert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
template <class RandomAccessIter, class T, class Compare>
void LinearInsert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        CopyBackward(first, last, last + 1);
        *first = val;
    }
    else
        UnguardedLinearInsert(last, val, comp);
}

int main()
{
    int ok = 1;
    {
        Pair v[5] = { Pair(2), Pair(5), Pair(9), Pair(-1), Pair(6) };
        int live = g_live;
        LinearInsert(v, v + 3, Pair(v[3]), KeyLess());  // new minimum
        if (v[0].first != -1 || v[1].first != 2 || v[2].first != 5 || v[3].first != 9) ok = 0;
        LinearInsert(v, v + 4, Pair(v[4]), KeyLess());  // middle
        int expect[5] = { -1, 2, 5, 6, 9 };
        for (int i = 0; i < 5; ++i) if (v[i].first != expect[i] || !Consistent(v[i])) ok = 0;
        if (g_live != live) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006bf020_TEST FAIL\n"); return 1; }
    std::printf("006bf020_TEST PASS\n");
    return 0;
}