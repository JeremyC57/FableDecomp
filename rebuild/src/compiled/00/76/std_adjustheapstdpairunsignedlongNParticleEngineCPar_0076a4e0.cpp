// std::__adjust_heap<std::pair<unsigned long,NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long> >*>(first,
// int hole, int len, pair value, CParticleUpdateParameters<ul,ul>::CParticleUpdateCCompareParamDesc<ul>) -- STLport heap
// sift-down for 16-byte (unsigned long key, particle update parameter descriptor) pairs, then __push_heap @ 0x00769E20;
// destroys the by-value pair
// Flags: /Gr
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076a4e0, 169 bytes (+ int3 padding).
// Renamed (rename1, 2026-10-24) from _global::_Adjust_heap @ 00/76/global_Adjustheap_0076a4e0.cpp: STLport spelling and
//   template form, the PDB comparator (the name sets have the instantiation family `...@V?$CParticleUpdateCCompareParamDesc@K@
//   ?$CParticleUpdateParameters@KK@NParticleEngine@@` over `pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<
//   unsigned long, unsigned long> >`), CBaseClass for the descriptor's base (RTTI of vtable 0x01231710, landed dtor
//   0x0099A2E0). The landed caller __pop_heap_aux 0x0076AC70 declared this callee in the STLport dialect while this row
//   used the Dinkumware one (FINDINGS chunk 363); both now name the same symbol.
// Callees: 0x00769E20 __push_heap (same instantiation), 0x0099A2E0 CBaseClass::~CBaseClass (stores the CBaseClass
// vtable 0x01231710).  first in ecx, hole in edx (link-time __fastcall); len, the 16-byte pair
// by value and the empty comparator follow (`ret 18h`); VC destroys the by-value pair in the callee
// (`lea ecx,[esp+1Ch] ; call ~CBaseClass` on value.second after the push call).
// Element: the vtable stored at +4 of the argument copy, 0x0125FEE4, is
// NParticleEngine::CParticleUpdateParamDesc<unsigned long,unsigned long>'s (RTTI); Ego_r.pdb gives it 0x0C bytes = vptr +0,
// Callback +4, Index +8, so the element is pair<unsigned long, CParticleUpdateParamDesc<..> > (16 bytes, `shl esi,4`).
// Shape: the element assignment is the IMPLICIT operator= (key, Callback, Index: three dword moves,
// the vptr at +4 skipped); the argument copy is STLport's pair copy constructor (key, then the descriptor's implicit copy:
// vtable immediate, two members); the descriptor's compiler-generated destructor stores no vtable and just calls the
// out-of-line CBaseClass destructor.  The key test is an UNSIGNED `jae`.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace std {
template <class T1, class T2> struct pair {
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
    template <class TKey> class CParticleUpdateCCompareParamDesc {
    public:
        bool operator()(const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& a,
                        const std::pair<TKey, CParticleUpdateParamDesc<TOwner, TValue> >& b) const
        {
            return a.first < b.first;
        }
    };
};
}

typedef std::pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> > SKeyedDesc;
typedef NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>::CParticleUpdateCCompareParamDesc<unsigned long> SCompare;

namespace std {
template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __push_heap(RandomAccessIter first, Distance hole, Distance top, T value, Compare comp);   // 0x00769E20

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance hole, Distance len, T value, Compare comp)
{
    Distance top = hole;
    Distance idx = 2 * hole + 2;
    for (; idx < len; idx = 2 * idx + 2) {
        if (comp(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == len) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    __push_heap(first, hole, top, value, comp);
}
template void __fastcall __adjust_heap(SKeyedDesc* first, int hole, int len, SKeyedDesc value, SCompare comp);
}
