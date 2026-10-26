// Behaviour test for 0x006BEAA0 CopyBackward_PairLongMusicEntry_006beaa0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CWideString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    CWideString& operator=(const CWideString& other);     // 0x0099B7D0
};

struct CRegisteredMusicEntry {               // CMusicManager::CRegisteredMusicEntry, 0x14 bytes
    CWideString SrcFileName;                 // +0x00
    CWideString DstFileName;                 // +0x04
    float       Gain;                        // +0x08
    float       Offset;                      // +0x0C
    bool        CacheToHD;                   // +0x10
};

struct PairLongMusicEntry {                  // std::pair<long, CMusicManager::CRegisteredMusicEntry>
    long                  first;             // +0x00
    CRegisteredMusicEntry second;            // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairLongMusicEntry* __fastcall CopyBackward_PairLongMusicEntry_006beaa0(PairLongMusicEntry* first, PairLongMusicEntry* last, PairLongMusicEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_strAssign = 0;
CWideString& CWideString::operator=(const CWideString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }

typedef PairLongMusicEntry T;
#define KEY(E) (E).first
#define INIT(E) (E).second.SrcFileName.m_pData = &(E); (E).second.DstFileName.m_pData = 0; (E).second.Gain = 0.5f; (E).second.CacheToHD = true
#define CALL(f, l, r) CopyBackward_PairLongMusicEntry_006beaa0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(g_strAssign == 6 && a[3].second.SrcFileName.m_pData == &a[2] && a[3].second.Gain == 0.5f);
    if (g_fail) { std::printf("006beaa0_TEST FAIL\n"); return 1; }
    std::printf("006beaa0_TEST PASS\n");
    return 0;
}