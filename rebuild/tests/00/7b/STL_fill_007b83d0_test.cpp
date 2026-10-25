// Behaviour test for 0x007B83D0 _STL::fill(CIntelligentPointer<CThingObject>*, ...): every intelligent pointer of the range is pointed at the value's object (SetPItem(other.GetPItem()) per element)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
static int gGets = 0;
class CThingObject;
class CBaseObject { public: int Id; };
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { ++gGets; return PItem; }           // stand-in
    void SetPItem(CBaseObject* item) { PItem = item; ++gAssigns; }     // stand-in
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
static CBaseObject gTarget = { 3 };
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
    CIntelligentPointer<CThingObject> arr[4];
    int i;
    for (i = 0; i < 4; ++i) arr[i].PItem = 0;
    CIntelligentPointer<CThingObject> v; v.PItem = &gTarget;
    bool ok = true;
    _STL::fill(arr, arr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && arr[i].PItem == &gTarget;
    ok = ok && arr[3].PItem == 0 && gAssigns == 3 && gGets == 3;
    _STL::fill(arr, arr, v);
    ok = ok && gAssigns == 3 && gGets == 3;
    if (ok) { printf("007b83d0_TEST PASS\n"); return 0; }
    printf("007b83d0_TEST FAIL\n");
    return 1;
}