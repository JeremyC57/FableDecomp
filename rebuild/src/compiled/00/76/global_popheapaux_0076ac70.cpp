// _global::__pop_heap_aux(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc*, CKeyPairCompareLess comp) -- SGI pop_heap front end for a heap of (unsigned long, particle update-parameter descriptor) pairs: move the back element out, put the root there and sift the old back element down
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
// Shape: STLport __pop_heap_aux with __pop_heap inlined: the T(*(last - 1)) temporary is the 16-byte frame local (its
//   implicit copy constructor stores the vptr), the pair's implicit assignment writes the root into the back slot (key,
//   Callback, Index -- an implicit operator= never copies the vptr), the copy for __adjust_heap is built in the argument area
//   by STLport's explicit pair copy constructor, and the temporary's implicit destructor reduces to the base destructor call.
class CBaseClass {
public:
    virtual ~CBaseClass();                              // 0x0099A2E0
};

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

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, CParticleUpdateParamDesc<ul,ul> >, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairULongParamDesc* first, long holeIndex, long len, PairULongParamDesc value,
                              CKeyPairCompareLess comp);   // 0x0076A4E0

inline void __fastcall __pop_heap(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc* result,
                                  PairULongParamDesc value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

void __fastcall __pop_heap_aux(PairULongParamDesc* first, PairULongParamDesc* last, PairULongParamDesc*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, PairULongParamDesc(*(last - 1)), comp, (long*)0);
}