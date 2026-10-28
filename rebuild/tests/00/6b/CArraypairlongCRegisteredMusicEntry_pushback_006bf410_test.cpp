// Behaviour test for 0x006BF410 CArray<pair<long, CRegisteredMusicEntry>>::push_back(const value_type&).
#include <cstdio>
#include <cstring>
#include <new>
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

static int g_overflowCalls = 0, g_lastFill = 0, g_lastAtEnd = 0;
struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    double storage[24 * sizeof(Pair) / sizeof(double) + 1];
    Vec() { start = finish = (Pair*)storage; eos = start + 2; }
    void insert_overflow(Pair* pos, const Pair& x, unsigned fill, bool atend) {
        ++g_overflowCalls; g_lastFill = (int)fill; g_lastAtEnd = atend ? 1 : 0;
        if (pos != finish) return;
        eos = start + 24;                         // the model grows in place (capacity 2 -> 24)
        new (finish) Pair(x); ++finish;
    }
    void push_back(const Pair& x) {
        if (finish != eos) { if (finish) new (finish) Pair(x); ++finish; }
        else insert_overflow(finish, x, 1, true);
    }
};
int main() {
    int ok = 1;
    {
        Vec v;
        Pair a(3), b(4), c(5);
        v.push_back(a);
        ok &= (v.finish - v.start) == 1 && g_overflowCalls == 0 && v.start[0].first == 3 && Consistent(v.start[0]);
        ok &= std::strcmp(v.start[0].second.SrcFileName.text, "s") == 0 && std::strcmp(v.start[0].second.DstFileName.text, "d") == 0;
        v.push_back(b);
        ok &= (v.finish - v.start) == 2 && g_overflowCalls == 0 && v.start[1].first == 4 && Consistent(v.start[1]);
        v.push_back(c);                           // full: the overflow path with fill 1, atend true
        ok &= g_overflowCalls == 1 && g_lastFill == 1 && g_lastAtEnd == 1 && (v.finish - v.start) == 3 && v.start[2].first == 5;
        v.push_back(a);
        ok &= g_overflowCalls == 1 && (v.finish - v.start) == 4 && v.start[3].first == 3 && Consistent(v.start[3]);
        ok &= g_live == 2 * 3 + 2 * 4;            // three locals + four constructed copies, two strings each
        for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    }
    ok &= g_live == 0;
    if (!ok) { std::printf("006bf410_TEST FAIL\n"); return 1; }
    std::printf("006bf410_TEST PASS\n");
    return 0;
}