// Behaviour test for 0x007AF980 std::fill(CIntelligentPointer<CThingAICreature>*, ...): every intelligent pointer of the range is pointed at the value's thing (SetPItem(other.GetPItem()) per element)
#include <stdio.h>
#if !defined(_MSC_VER)
#define __fastcall
#define __cdecl
#define __int64 long long
#endif
static int gAssigns = 0;
static int gGets = 0;
class CThingAICreature;
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
    CIntelligentPointer<CThingAICreature> arr[4];
    int i;
    for (i = 0; i < 4; ++i) arr[i].PItem = 0;
    CIntelligentPointer<CThingAICreature> v; v.PItem = &gTarget;
    bool ok = true;
    std::fill(arr, arr + 3, v);
    for (i = 0; i < 3; ++i)
        ok = ok && arr[i].PItem == &gTarget;
    ok = ok && arr[3].PItem == 0 && gAssigns == 3 && gGets == 3;
    std::fill(arr, arr, v);
    ok = ok && gAssigns == 3 && gGets == 3;
    if (ok) { printf("007af980_TEST PASS\n"); return 0; }
    printf("007af980_TEST FAIL\n");
    return 1;
}