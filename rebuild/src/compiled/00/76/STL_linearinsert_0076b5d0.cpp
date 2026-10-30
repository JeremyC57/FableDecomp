// std::__linear_insert(pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* first, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> >* last, pair<unsigned long, CParticleUpdateParamDesc<unsigned long, unsigned long> > val, CParticleUpdateCCompareParamDesc<unsigned long> comp) -- insert val into the sorted prefix [first, last): shift the whole prefix when val is the new minimum, else an unguarded linear insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B5D0, 115 bytes (+int3 pad to 128), `ret 14h`.
// Caller: the landed __insertion_sort 0x0076BB70 (0x0076BBAA).
// Callees: the landed __copy_backward 0x00769EA0 (first, last, last + 1, random_access_iterator_tag&, null distance),
//   __unguarded_linear_insert 0x00769D00, CBaseClass::~CBaseClass 0x0099A2E0 (the by-value val, once per branch).
// Element: the landed siblings' 16-byte pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<ul,ul> > (0x00769EA0
//   __copy_backward, 0x00769E20 __push_heap, 0x0076A4E0 __adjust_heap, 0x0076BB70 __insertion_sort): the descriptor is
//   polymorphic (vtable 0x0125FEE4 at +4 of the pair), so the implicit operator= skips that word, the implicit copy
//   constructor stores the vtable immediate, and every by-value or local copy is destroyed through the
//   compiler-generated destructor, which only calls CBaseClass::~CBaseClass 0x0099A2E0. Key compares are UNSIGNED.
// Shape: STLport `__linear_insert(first, last, val, comp)`, /Gr (__fastcall: first in ecx, last in edx). copy_backward
//   resolves through the inline pointer dispatch to `__copy_backward(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)`, the empty tag homed in the dead comparator slot (`lea eax,[esp+1Ch]`); `*first = val` copies
//   key, Callback and Index. The else branch rebuilds `val` in the argument area for the unguarded insert.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {

template <class T1, class T2> class CParticleUpdateParamCallback;

template <class T1, class T2> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes (Ego_r.pdb)
public:
    CParticleUpdateParamCallback<T1, T2>* Callback;         // +0x04
    unsigned long                         Index;            // +0x08
};

}

namespace std {

template <class _T1, class _T2>
class pair {
public:
    _T1 first;                                              // +0x00 sort key
    _T2 second;                                             // +0x04 (its vptr is never assigned by operator=)
    pair(const pair& o) : first(o.first), second(o.second) { }
};

}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > SKeyedDesc;   // 16 bytes

namespace NParticleEngine {

template <class K> struct CParticleUpdateCCompareParamDesc {
    bool operator()(const SKeyedDesc& a, const SKeyedDesc& b) const { return a.first < b.first; }
};

}

typedef NParticleEngine::CParticleUpdateCCompareParamDesc<unsigned long> SKeyedDescLess;

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00769EA0

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00769D00

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

template void __fastcall __linear_insert<SKeyedDesc*, SKeyedDesc, SKeyedDescLess>(SKeyedDesc*, SKeyedDesc*, SKeyedDesc, SKeyedDescLess);

}