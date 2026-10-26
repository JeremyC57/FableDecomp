// behaviour test for 0x00cb6160 (std::__uninitialized_fill_n over pair<CCharString, CScriptInfo>)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
#include <new>
class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString(const CCharString& other) : m_pStringData(other.m_pStringData) { ++g_stringCopies; }
};
class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;
class CScriptInfo { public:                            // 20 bytes
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                      // +0x08
    long             ID;                        // +0x0C
    bool             MasterScript;              // +0x10
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<CCharString, CScriptInfo> CScriptInfoPair;

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
template CScriptInfoPair* __fastcall __uninitialized_fill_n(CScriptInfoPair*, unsigned int, const CScriptInfoPair&, const __false_type&);
}

int main()
{
    static double raw[64];
    CScriptInfoPair* p = (CScriptInfoPair*)raw;
    p[2].second.ID = -1;
    static double xs[16];
    CScriptInfoPair& x = *(CScriptInfoPair*)xs;
    static char s1, s2;
    x.first.m_pStringData = &s1; x.second.AllocFunc = 0; x.second.MasterScriptDataAlloc = 0;
    x.second.Name.m_pStringData = &s2; x.second.ID = 77; x.second.MasterScript = true;
    __false_type tag;
    CScriptInfoPair* e = std::__uninitialized_fill_n(p, 2u, x, tag);
    int ok = e == p + 2 && g_stringCopies == 4 && p[0].first.m_pStringData == &s1 && p[1].second.Name.m_pStringData == &s2
          && p[1].second.ID == 77 && p[0].second.MasterScript && p[2].second.ID == -1
          && std::__uninitialized_fill_n(p, 0u, x, tag) == p && g_stringCopies == 4;
    if (!ok) { printf("00cb6160_TEST FAIL\n"); return 1; }
    printf("00cb6160_TEST PASS\n");
    return 0;
}