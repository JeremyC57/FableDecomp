// Behaviour test for 0x00A37BE0 _STL::fill(CClassTypeInfo*, ...): every class-factory type record of the range becomes a copy of the value (type list and name through their operator=, the counts, flags and allocator copied)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
static int gListAssigns = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++gAssigns; return *this; }   // stand-in
};
template <class T> class CList {
public:
    void* _M_node;
    CList& operator=(const CList& other) { _M_node = other._M_node; ++gListAssigns; return *this; }   // stand-in
};
class CClass { public: int Id; };
static CClass gMade = { 9 };
static CClass* __cdecl MakeClass() { return &gMade; }
class CClassTypeInfo {
public:
    CList<CClass*> TypeList;
    unsigned long  NoAllocated;
    bool           Used;
    bool           Local;
    CClass*        (__cdecl* AllocFunc)();
    CCharString    Name;
};
static int gNodeDummy;
static CClassTypeInfo gArr[4];
namespace _STL {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
int main()
{
    CClassTypeInfo v;
    v.TypeList._M_node = &gNodeDummy; v.NoAllocated = 5; v.Used = true; v.Local = true; v.AllocFunc = MakeClass; v.Name.PStringData = "CThing";
    int i;
    for (i = 0; i < 4; ++i) { gArr[i].TypeList._M_node = 0; gArr[i].NoAllocated = 0; gArr[i].Used = false; gArr[i].Local = false; gArr[i].AllocFunc = 0; gArr[i].Name.PStringData = 0; }
    bool ok = true;
    _STL::fill(gArr + 1, gArr + 4, v);
    for (i = 1; i < 4; ++i)
        ok = ok && gArr[i].TypeList._M_node == (void*)&gNodeDummy && gArr[i].NoAllocated == 5 && gArr[i].Used && gArr[i].Local &&
             gArr[i].AllocFunc == MakeClass && gArr[i].AllocFunc()->Id == 9 && gArr[i].Name.PStringData == v.Name.PStringData;
    ok = ok && gArr[0].NoAllocated == 0 && gArr[0].AllocFunc == 0 && gAssigns == 3 && gListAssigns == 3;
    _STL::fill(gArr, gArr, v);
    ok = ok && gAssigns == 3 && gListAssigns == 3;
    if (ok) { printf("00a37be0_TEST PASS\n"); return 0; }
    printf("00a37be0_TEST FAIL\n");
    return 1;
}