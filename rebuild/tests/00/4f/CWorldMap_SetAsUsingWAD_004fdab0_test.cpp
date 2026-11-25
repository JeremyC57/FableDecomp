// Behaviour test for 0x004FDAB0 CWorldMap::SetAsUsingWAD(const CWideString&) (NF415: linked): links the row object (the function is
// declared here, not defined) and defines its callees under the row's spellings. Checks the bank switch (empty name: dropped; a name:
// a new CWADBank opened with flags 0x102, dropped again when the open fails) and that both empty-name tests read retail's pooled
// wide L"" (0x0122D70C): wcscmp gets that literal twice, char_traits compare gets it with length 0.
#include <cstdio>


extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b);   // the row's callee (0x00BFE94C), defined here
namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60, defined here
};
}

class CWideStringData { public:
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    void Reset(T* data);                                // 0x0050F2B0, defined here for T = CWADBank
};
class CBankFileAsync {
public:
    CBankFileAsync();                                   // 0x009D5F80, defined here
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual bool Open(const CWideString& name, unsigned long flags);
    unsigned char _body[0x164 - 4];
};
class CWADBank : public CBankFileAsync {
public:
    virtual bool Open(const CWideString& name, unsigned long flags);
};
class CWorldMap {
public:
    unsigned char             _pad00[0xa0];
    CCountedPointer<CWADBank> LevelWADBank;             // +0xa0
    bool                      UseLevelWADBank;          // +0xa8
    void SetAsUsingWAD(const CWideString& wadName);     // the row (linked)
};

static int g_banks, g_deleted, g_wcscmpCalls, g_compareCalls, g_badText;
static bool g_openResult; static unsigned long g_openFlags; static const void* g_openName;
static bool IsWideEmpty(const unsigned short* s) { return s != 0 && s[0] == 0; }

extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b)
{
    ++g_wcscmpCalls;
    if (!IsWideEmpty(a) || !IsWideEmpty(b) || a != b) ++g_badText;   // retail passes the one pooled L"" for both
    while (*a && *a == *b) { ++a; ++b; }
    return (int)*a - (int)*b;
}
template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(const unsigned short* l, const unsigned short* r, unsigned int n)
{
    ++g_compareCalls;
    if (!IsWideEmpty(r) || n != 0) ++g_badText;
    for (unsigned int i = 0; i < n; ++i) if (l[i] != r[i]) return l[i] < r[i] ? -1 : 1;
    return 0;
}
static void __fastcall DeleteData(void* data) { ++g_deleted; delete (CWADBank*)data; }
template <> void CCountedPointer<CWADBank>::Reset(CWADBank* data)
{
    if (Info && --Info->RefCount == 0) { Info->DeleteFunc(Info->Data); ::operator delete(Info); }
    Data = data; Info = (CCPPointerInfo*)::operator new(sizeof(CCPPointerInfo));
    Info->RefCount = 1; Info->DeleteFunc = DeleteData; Info->Data = data;
}
CBankFileAsync::CBankFileAsync() { ++g_banks; }
void CBankFileAsync::v00() {} void CBankFileAsync::v01() {} void CBankFileAsync::v02() {}
bool CBankFileAsync::Open(const CWideString&, unsigned long) { return false; }
bool CWADBank::Open(const CWideString& name, unsigned long flags) { g_openName = &name; g_openFlags = flags; return g_openResult; }

static int Fail(int n) { std::printf("004fdab0_TEST FAIL %d\n", n); return 1; }
int main()
{
    static CWorldMap map; map.LevelWADBank.Data = 0; map.LevelWADBank.Info = 0; map.UseLevelWADBank = true;
    CWideString none; none.PStringData = 0;
    map.SetAsUsingWAD(none);                               /* unassigned name: wcscmp(L"", L"") == 0 -> dropped */
    if (map.UseLevelWADBank != false || g_banks != 0 || g_wcscmpCalls != 1 || g_compareCalls != 0) return Fail(1);
    unsigned short text[] = { 'a', '.', 'w', 'a', 'd', 0 };
    CWideStringData data; data._M_start = text; data._M_finish = text + 5; data._M_end_of_storage = text + 5; data.NoRefs = 1;
    CWideString name; name.PStringData = &data;
    g_openResult = true;
    map.SetAsUsingWAD(name);
    if (map.UseLevelWADBank != true || g_banks != 1 || map.LevelWADBank.Data == 0 || g_openName != &name || g_openFlags != 0x102) return Fail(2);
    if (g_wcscmpCalls != 1 || g_compareCalls != 0) return Fail(3);   /* a non-empty name reaches neither test */
    g_openResult = false;
    map.SetAsUsingWAD(name);                               /* replaces the bank (old one released), open fails: dropped */
    if (map.UseLevelWADBank != false || g_banks != 2 || g_deleted != 2 || map.LevelWADBank.Data != 0 || map.LevelWADBank.Info != 0) return Fail(4);
    g_openResult = true; map.SetAsUsingWAD(name);
    CWideStringData empty; empty._M_start = empty._M_finish = empty._M_end_of_storage = text; empty.NoRefs = 1;
    CWideString blank; blank.PStringData = &empty;
    map.SetAsUsingWAD(blank);                              /* assigned but empty: compare(start, L"", 0) == 0 -> dropped */
    if (map.UseLevelWADBank != false || g_deleted != 3 || map.LevelWADBank.Data != 0 || g_compareCalls != 1) return Fail(5);
    if (g_badText != 0) return Fail(6);                    /* every empty-name test read the wide L"" */
    std::printf("004fdab0_TEST PASS\n");
    return 0;
}
