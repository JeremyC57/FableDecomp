// Behaviour test for 0x0052BCA0 std::fill(NThingManager::CEntityRuntimePersistence::CEntityScriptInfo*, ...): every entity-script record of the range becomes a copy of the value (script name and save data through their operator=, then size and valid flag)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
static int gArrayAssigns = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++gAssigns; return *this; }   // stand-in
};
template <class T> class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray& operator=(const CArray& other)            // stand-in
    {
        _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage;
        ++gArrayAssigns;
        return *this;
    }
};
namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {
    public:
        CCharString           Script;
        CArray<unsigned char> SaveData;
        long                  UncompressedSize;
        bool                  Valid;
    };
};
}
typedef NThingManager::CEntityRuntimePersistence::CEntityScriptInfo CEntityScriptInfo;
static unsigned char gBytes[8];
static CEntityScriptInfo gArr[4];
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    CEntityScriptInfo v;
    v.Script.PStringData = "script"; v.SaveData._M_start = gBytes; v.SaveData._M_finish = gBytes + 8; v.SaveData._M_end_of_storage = gBytes + 8;
    v.UncompressedSize = 4096; v.Valid = true;
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].Script.PStringData = 0; gArr[i].SaveData._M_start = 0; gArr[i].UncompressedSize = 0; gArr[i].Valid = false; }
    bool ok = true;
    std::fill(gArr, gArr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && gArr[i].Script.PStringData == v.Script.PStringData && gArr[i].SaveData._M_start == gBytes &&
             gArr[i].SaveData._M_finish == gBytes + 8 && gArr[i].UncompressedSize == 4096 && gArr[i].Valid;
    ok = ok && gArr[3].UncompressedSize == 0 && gAssigns == 3 && gArrayAssigns == 3;
    std::fill(gArr, gArr, v);
    ok = ok && gAssigns == 3 && gArrayAssigns == 3;
    if (ok) { printf("0052bca0_TEST PASS\n"); return 0; }
    printf("0052bca0_TEST FAIL\n");
    return 1;
}