// std::sort_heap(PairULongParamDesc* first, PairULongParamDesc* last, CParticleUpdateCCompareParamDesc comp) -- SGI sort_heap over (unsigned long, particle update-parameter descriptor) pairs: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076B650, length 171 bytes (+ int3 padding), `ret 4` (first/last in ecx/edx,
//   the empty comparator on the stack -- the SGI algorithm TUs are /Gr).
// Identity: the element is 16 bytes (`and eax,-10h ; cmp eax,10h`) = { unsigned long ; CParticleUpdateParamDesc<unsigned
//   long,unsigned long> } (Ego_r.pdb: descriptor 12 bytes, vptr +0, Callback +4, Index +8; vtable 0x0125FEE4), the element
//   model of the landed __pop_heap_aux 0x0076AC70 and __adjust_heap 0x0076A4E0 of the same instantiation. The catalog label
//   `_Sort_heap<..., CParticleUpdateParameters<ul,ul>::CParticleUpdateCCompareParamDesc<ul> >` names the comparator; the
//   function is the STLport sort_heap.
// Callees: __adjust_heap @ 0x0076A4E0 (landed; ecx first, edx hole, then len, the 16-byte value and the comparator, `ret 18h`),
//   CBaseClass::~CBaseClass @ 0x0099A2E0 (destroys the T(*(last - 1)) temporary).
// Shape: `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap, __pop_heap_aux and __pop_heap all inline: the
//   temporary is the 16-byte frame local (vptr immediate stored by the implicit copy ctor), the root goes to the back slot by
//   the implicit pair assignment (three dwords, no vptr), the __adjust_heap value is built in the argument area by STLport's
//   explicit pair copy constructor; `last - 1 - first` is kept as `-12 - first + esi` with esi = &(last - 1)->Index.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
template <class TOwner, class TValue> class CParticleUpdateParamCallback;

template <class TOwner, class TValue> class CParticleUpdateParamDesc : public CBaseClass {   // vtable 0x0125FEE4, 12 bytes
public:
    CParticleUpdateParamCallback<TOwner, TValue>* Callback;   // +0x04
    unsigned long                                 Index;      // +0x08
};
}

typedef NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> CULongParamDesc;

struct PairULongParamDesc {                             // std::pair<unsigned long, CParticleUpdateParamDesc<ul,ul> >, 16 bytes
    unsigned long   first;                              // +0x00
    CULongParamDesc second;                             // +0x04
    PairULongParamDesc(const PairULongParamDesc& other) : first(other.first), second(other.second) { }
};

struct CParticleUpdateCCompareParamDesc {               // CParticleUpdateParameters<ul,ul>::CParticleUpdateCCompareParamDesc<ul>, empty
    CParticleUpdateCCompareParamDesc() { }
};

void __fastcall __adjust_heap(PairULongParamDesc* first, long holeIndex, long len, PairULongParamDesc value,
                              CParticleUpdateCCompareParamDesc comp);   // 0x0076A4E0

inline void __fastcall __pop_heap(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc* result,
                                  PairULongParamDesc value, CParticleUpdateCCompareParamDesc comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

inline void __fastcall __pop_heap_aux(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc*,
                                      CParticleUpdateCCompareParamDesc comp)
{
    __pop_heap(first, last - 1, last - 1, PairULongParamDesc(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(PairULongParamDesc* first, PairULongParamDesc* last, CParticleUpdateCCompareParamDesc comp)
{
    __pop_heap_aux(first, last, (PairULongParamDesc*)0, comp);
}

void __fastcall sort_heap(PairULongParamDesc* first, PairULongParamDesc* last, CParticleUpdateCCompareParamDesc comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}