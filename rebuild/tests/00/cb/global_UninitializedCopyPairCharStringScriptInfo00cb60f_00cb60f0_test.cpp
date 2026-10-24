// Behaviour test for 0x00CB60F0 UninitializedCopy_PairCharStringScriptInfo_00cb60f0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
#include <new>

struct CCharString {                         // one-word handle
    void* m_pData;                           // +0x00
    CCharString(const CCharString& other);   // 0x0099EC30
};

struct CScriptBase;
struct CScriptDataBase;
struct CGameScriptInterfaceBase;

struct CScriptInfo {                         // Ego_r.pdb, 0x14 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                   // +0x08
    long             ID;                     // +0x0C
    bool             MasterScript;           // +0x10
};

struct PairCharStringScriptInfo {            // std::pair<CCharString, CScriptInfo>
    CCharString first;                       // +0x00
    CScriptInfo second;                      // +0x04
    PairCharStringScriptInfo(const PairCharStringScriptInfo& p) : first(p.first), second(p.second) { }
};

struct __false_type { __false_type() { } };

inline void _Construct(PairCharStringScriptInfo* p, const PairCharStringScriptInfo& value)
{
    new (p) PairCharStringScriptInfo(value);
}

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringScriptInfo* __fastcall UninitializedCopy_PairCharStringScriptInfo_00cb60f0(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result, const __false_type&)
{
    PairCharStringScriptInfo* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}

static int g_strCopy = 0;
CCharString::CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopy; }

typedef PairCharStringScriptInfo T;
#define KEY(E) (E).
#define INIT(E) (void)0
#define CALL(f, l, r) UninitializedCopy_PairCharStringScriptInfo_00cb60f0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    static double srcRaw[(sizeof(PairCharStringScriptInfo) * 3) / sizeof(double) + 1];
    static double dstRaw[(sizeof(PairCharStringScriptInfo) * 4) / sizeof(double) + 1];
    PairCharStringScriptInfo* s = (PairCharStringScriptInfo*)srcRaw;
    PairCharStringScriptInfo* d = (PairCharStringScriptInfo*)dstRaw;
    for (i = 0; i < 3; ++i) { s[i].first.m_pData = &s[i]; s[i].second.Name.m_pData = &d[i]; s[i].second.ID = 50 + i; s[i].second.MasterScript = (i == 1); s[i].second.AllocFunc = 0; s[i].second.MasterScriptDataAlloc = 0; }
    __false_type ft;
    PairCharStringScriptInfo* r = UninitializedCopy_PairCharStringScriptInfo_00cb60f0(s, s + 3, d, ft);
    CHECK(r == d + 3 && g_strCopy == 6);
    CHECK(d[0].first.m_pData == &s[0] && d[2].second.Name.m_pData == &d[2] && d[1].second.ID == 51 && d[1].second.MasterScript && !d[2].second.MasterScript);
    r = UninitializedCopy_PairCharStringScriptInfo_00cb60f0(s, s, d + 1, ft);
    CHECK(r == d + 1 && g_strCopy == 6);
    if (g_fail) { std::printf("00cb60f0_TEST FAIL\n"); return 1; }
    std::printf("00cb60f0_TEST PASS\n");
    return 0;
}