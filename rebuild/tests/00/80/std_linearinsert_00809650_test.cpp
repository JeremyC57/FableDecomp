// Behaviour test for 0x00809650 std::__linear_insert<CIntelligentPointer<CThing>*, CIntelligentPointer<CThing>, CDoNotUseBinaryPredicate>: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
class CThing;
class CBaseObject;

long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CRandomSeed {
    unsigned long Seed;
    unsigned long Random()
    {
        Seed = Seed * 0x24a1 + 0x24df;
        Seed = GFROR13(Seed);
        return Seed;
    }
};

class CWorld { public:
    unsigned char _pad[0x1613c];
    CRandomSeed RandomSeed;                     // +0x1613c
};

extern CWorld* g_pWorld;                        // 0x013B86A0

class CBaseIntelligentPointer {
public:
    void* PData;
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& p) { SetPItem(p.GetPItem()); return *this; }
};
typedef CIntelligentPointer<CThing> ThingPointer;

namespace {
struct CDoNotUseBinaryPredicate {
    char m_tag;
    bool operator()(const ThingPointer&, const ThingPointer&) const
    {
        unsigned long bit = g_pWorld->RandomSeed.Random() & 1;
        if (bit != 0)
            return false;
        return true;
    }
};
}

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // user ctor: no tag-byte store (see Shape)
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00655884
template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00806910

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template void __fastcall __linear_insert(ThingPointer*, ThingPointer*, ThingPointer, CDoNotUseBinaryPredicate);
}

long __fastcall GFROR13(unsigned long v) { return (long)((v >> 13) | (v << 19)); }
static CWorld g_world;
CWorld* g_pWorld = &g_world;
static int g_dtors = 0, g_unguarded = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) { }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtors; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }
namespace tstd {
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare)
{
    ++g_unguarded;
    *__last = __val;                                    // marks where the value went
}
}
struct Slot : CBaseIntelligentPointer { Slot() { } };   // same layout as ThingPointer, default-constructible
static unsigned long Expect(unsigned long seed) { unsigned long s = seed * 0x24a1 + 0x24df; return (unsigned long)GFROR13(s); }
int main()
{
    Slot a[4];
    for (int i = 0; i < 4; ++i) a[i].PData = (void*)(long)(100 + i);
    CDoNotUseBinaryPredicate comp; comp.m_tag = 0;
    int ok = 1, sawShift = 0, sawUnguarded = 0;
    for (unsigned long seed = 1; seed < 40 && !(sawShift && sawUnguarded); ++seed) {
        for (int i = 0; i < 4; ++i) a[i].PData = (void*)(long)(100 + i);
        g_world.RandomSeed.Seed = seed;
        unsigned long next = Expect(seed);
        int before = g_unguarded, dtorsBefore = g_dtors;
        Slot v; v.PData = (void*)(long)999;
        tstd::__linear_insert((ThingPointer*)a, (ThingPointer*)a + 2, *(ThingPointer*)&v, comp);
        ok &= g_world.RandomSeed.Seed == next;
        if ((next & 1) == 0) {                          // the coin says "val first": shift [0, 2) up one, val at 0
            sawShift = 1;
            ok &= g_unguarded == before && (long)a[0].PData == 999 && (long)a[1].PData == 100 && (long)a[2].PData == 101 && (long)a[3].PData == 103;
        } else {
            sawUnguarded = 1;
            ok &= g_unguarded == before + 1 && (long)a[2].PData == 999 && (long)a[0].PData == 100;
        }
        ok &= g_dtors > dtorsBefore;                    // the by-value pointer dies in the callee
    }
    ok &= sawShift && sawUnguarded;
    if (ok) printf("00809650_TEST PASS\n");
    return ok ? 0 : 1;
}