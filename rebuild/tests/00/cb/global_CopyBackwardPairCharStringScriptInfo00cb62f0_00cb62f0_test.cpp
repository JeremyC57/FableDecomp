// Behaviour test for 0x00CB62F0 CopyBackward_PairCharStringScriptInfo_00cb62f0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo { public:                         // Ego_r.pdb, 0x14 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                   // +0x08
    long             ID;                     // +0x0C
    bool             MasterScript;           // +0x10
};

struct PairCharStringScriptInfo {            // std::pair<CCharString, CScriptInfo>
    CCharString first;                       // +0x00
    CScriptInfo second;                      // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairCharStringScriptInfo* __fastcall CopyBackward_PairCharStringScriptInfo_00cb62f0(PairCharStringScriptInfo* first, PairCharStringScriptInfo* last, PairCharStringScriptInfo* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_strAssign = 0;
const CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }

typedef PairCharStringScriptInfo T;
#define KEY(E) (E).second.ID
#define INIT(E) (E).first.m_pData = &(E); (E).second.Name.m_pData = 0; (E).second.AllocFunc = 0; (E).second.MasterScriptDataAlloc = 0; (E).second.MasterScript = true
#define CALL(f, l, r) CopyBackward_PairCharStringScriptInfo_00cb62f0(f, l, r, tag, 0)

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
    CHECK(g_strAssign == 6 && a[3].first.m_pData == &a[2] && a[3].second.MasterScript);
    if (g_fail) { std::printf("00cb62f0_TEST FAIL\n"); return 1; }
    std::printf("00cb62f0_TEST PASS\n");
    return 0;
}