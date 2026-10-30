// _global::__pop_heap_aux(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc*, CParticleUpdateCCompareParamDesc comp) -- SGI pop_heap front end for a heap of (unsigned long, particle update-parameter descriptor) pairs: move the back element out, put the root there and sift the old back element down
// Leaf: __pop_heap_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076AC70, length 115 bytes, `ret 8` (the unused T* and the empty comparator on
//   the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 16 bytes (`sar edx,4`) = { unsigned long ; CParticleUpdateParamDesc<unsigned long,unsigned long> }
//   (Ego_r.pdb: the descriptor is 12 bytes, vptr +0, Callback +4, Index +8). The descriptor is polymorphic -- its vtable
//   0x0125FEE4 is stored into the temporary and into the argument copy -- and derives from CBaseClass, whose out-of-line
//   destructor 0x0099A2E0 (`mov [ecx],0x01231710 ; ret`) destroys the temporary. The callee 0x0076A4E0 is the matching
//   __adjust_heap (16-byte stride, UNSIGNED compare of the first word). The row's `_Pop_heap_0<pair<long,EHeroAbility>*,...>`
//   label (an 8-byte element) is propagated.
// Callees: __adjust_heap @ 0x0076A4E0, CBaseClass::~CBaseClass @ 0x0099A2E0.
// Fixed (rename1, 2026-10-24): the callee 0x0076A4E0 was declared as a plain `__adjust_heap(PairULongParamDesc*, long, long,
//   PairULongParamDesc, CKeyPairCompareLess)` while that row itself defined a Dinkumware `_Adjust_heap` over another comparator;
//   both now name the one STLport template instance std::__adjust_heap<pair<unsigned long, CParticleUpdateParamDesc<ul,ul> >*,
//   int, pair<..>, CParticleUpdateParameters<ul,ul>::CParticleUpdateCCompareParamDesc<ul> > (the PDB comparator of this heap
//   family; the element pair is std::pair). Bytes unchanged.
// Shape: STLport __pop_heap_aux with __pop_heap inlined: the T(*(last - 1)) temporary is the 16-byte frame local (its
//   implicit copy constructor stores the vptr), the pair's implicit assignment writes the root into the back slot (key,
//   Callback, Index -- an implicit operator= never copies the vptr), the copy for __adjust_heap is built in the argument area
//   by STLport's explicit pair copy constructor, and the temporary's implicit destructor reduces to the base destructor call.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace std {
template <class T1, class T2> struct pair {             // STLport pair: explicit copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
}

namespace NParticleEngine {
template <class TOwner, class TValue> class CParticleUpdateParamCallback;

template <class TOwner, class TValue> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes
public:
    CParticleUpdateParamCallback<TOwner, TValue>* Callback;   // +0x04
    unsigned long                                 Index;      // +0x08
};

template <class TOwner, class TValue> class CParticleUpdateParameters {
public:
    template <class TKey> class CParticleUpdateCCompareParamDesc {   // empty comparator (by value)
    public:
        bool operator()(const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& a,
                        const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& b) const
        {
            return a.first < b.first;
        }
    };
};
}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > PairULongParamDesc;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> CParamDescCompare;

namespace std {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp);   // 0x0076A4E0
}

inline void __fastcall __pop_heap(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc* result,
                                  PairULongParamDesc value, CParamDescCompare comp, int*)
{
    *result = *first;
    std::__adjust_heap(first, 0, int(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc*, CParamDescCompare comp)
{
    __pop_heap(first, last - 1, last - 1, PairULongParamDesc(*(last - 1)), comp, (int*)0);
}
