// Behaviour test for 0x006BE870 std::iter_swap(pair<long, CRegisteredMusicEntry>*, pair<long, CRegisteredMusicEntry>*).
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
    Pair(int k, const CRegisteredMusicEntry& e) : first(k), second(e) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};

template <class ForwardIter1, class ForwardIter2, class T>
inline void IterSwapAux(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}
template <class ForwardIter1, class ForwardIter2>
void IterSwap(ForwardIter1 a, ForwardIter2 b) { IterSwapAux(a, b, (Pair*)0); }

int main()
{
    int ok = 1;
    {
        Pair a(3, CRegisteredMusicEntry("a.wma", "a.ogg", 0.5f, 1.0f, true));
        Pair b(9, CRegisteredMusicEntry("b.wma", "b.ogg", 0.75f, 2.0f, false));
        int live = g_live;
        IterSwap(&a, &b);
        if (g_live != live) ok = 0;                     // the temporary's strings were destroyed
        if (a.first != 9 || std::strcmp(a.second.SrcFileName.text, "b.wma") || std::strcmp(a.second.DstFileName.text, "b.ogg") ||
            a.second.Gain != 0.75f || a.second.Offset != 2.0f || a.second.CacheToHD != false) ok = 0;
        if (b.first != 3 || std::strcmp(b.second.SrcFileName.text, "a.wma") || std::strcmp(b.second.DstFileName.text, "a.ogg") ||
            b.second.Gain != 0.5f || b.second.Offset != 1.0f || b.second.CacheToHD != true) ok = 0;
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("006be870_TEST FAIL\n"); return 1; }
    std::printf("006be870_TEST PASS\n");
    return 0;
}