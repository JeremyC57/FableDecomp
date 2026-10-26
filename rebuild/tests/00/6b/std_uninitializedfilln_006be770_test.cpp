// behaviour test for 0x006be770 (std::__uninitialized_fill_n over pair<long, CRegisteredMusicEntry>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
class CWideString { public:
    void* m_pStringData;                        // +0x00
    CWideString(const CWideString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
class CMusicManager { public:
    class CRegisteredMusicEntry { public:              // 20 bytes
        CWideString SrcFileName;                // +0x00
        CWideString DstFileName;                // +0x04
        float       Gain;                       // +0x08
        float       Offset;                     // +0x0C
        bool        CacheToHD;                  // +0x10
    };
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CMusicEntryPair* __fastcall __uninitialized_fill_n(CMusicEntryPair*, unsigned int, const CMusicEntryPair&, const __false_type&);
}

int main()
{
    static double raw[64];
    CMusicEntryPair* p = (CMusicEntryPair*)raw;
    p[3].first = -1;
    static double xs[16];
    CMusicEntryPair& x = *(CMusicEntryPair*)xs;
    static char s1, s2;
    x.first = 5; x.second.SrcFileName.m_pStringData = &s1; x.second.DstFileName.m_pStringData = &s2;
    x.second.Gain = 0.5f; x.second.Offset = 2.0f; x.second.CacheToHD = true;
    __false_type tag;
    CMusicEntryPair* e = std::__uninitialized_fill_n(p, 3u, x, tag);
    int ok = e == p + 3 && g_stringCopies == 6 && p[0].first == 5 && p[2].second.SrcFileName.m_pStringData == &s1
          && p[1].second.DstFileName.m_pStringData == &s2 && p[2].second.Gain == 0.5f && p[0].second.Offset == 2.0f
          && p[1].second.CacheToHD && p[3].first == -1 && std::__uninitialized_fill_n(p, 0u, x, tag) == p;
    if (!ok) { printf("006be770_TEST FAIL\n"); return 1; }
    printf("006be770_TEST PASS\n");
    return 0;
}