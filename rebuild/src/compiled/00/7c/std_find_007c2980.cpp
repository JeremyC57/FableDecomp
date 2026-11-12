// std::__find(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, const CIntelligentPointer<CThing>& val, const random_access_iterator_tag&) -- STLport random-access find: four-way unrolled scan of a shot's hit-things list for the entry naming the same thing as val
// Leaf: ??$__find@PAV?$CIntelligentPointer@$$CBVCThing@@@@V?$CIntelligentPointer@VCThing@@@@@std@@YIPAV?$CIntelligentPointer@$$CBVCThing@@@@PAV1@0ABV?$CIntelligentPointer@VCThing@@@@ABUrandom_access_iterator_tag@0@@Z
// Naming (agentNF31, 2026-11-12): the value type respelled CIntelligentPointer<const CThing> -> CIntelligentPointer<CThing>: the caller
//   0x007BF510 (OnCollision) builds the value with the CIntelligentPointer<CThing> vtable 0x01238C6C (ctor 0x004A96F0; the <const CThing>
//   vtable is 0x01237F1C, stored at 0x007C0FC8 for the element type) and that landed caller binds this symbol (RELOCATION_MATCH).
//   Descriptive-by-library (STLport __find; the PDBs are Dinkumware builds). The engine operator== takes the two pointer types.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C2980, 249 bytes, /Gr `__fastcall` (first ecx, last edx), `ret 8`
//   (the value reference + the tag reference).
// Identity: the callers are CTCShotDefinitionBase code (0x007BF55F OnCollision, 0x007C021D) and the
//   `find(first, last, val)` front end 0x007C2A90; all pass HitThingsList (+0x14 / +0x18, PDB:
//   vector<CIntelligentPointer<const CThing> >, 8-byte elements) and a CIntelligentPointer temporary. The catalog's
//   "StdVector_Find" is descriptive.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50.
// Shape: SGI/STLport `__find` verbatim (`__trip_count = (__last - __first) >> 2`, the unrolled loop, then the
//   fall-through switch on `__last - __first`); the value is itself an intelligent pointer, so the engine
//   operator== resolves the element first and the value second (`mov ecx,esi` then `mov ecx,ebx`). Same body as the
//   landed CThing* instantiation 0x006E66E0 apart from the value type.
class CBaseObject;
class CThing;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

template <class T, class U>
inline bool operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<U>& b)
{
    return a.GetPItem() == b.GetPItem();
}

namespace std {
struct random_access_iterator_tag { };

template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;

    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;

        if (*__first == __val) return __first;
        ++__first;
    }

    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
    case 0:
    default:
        return __last;
    }
}

template CIntelligentPointer<const CThing>* __fastcall __find(CIntelligentPointer<const CThing>*,
                                                              CIntelligentPointer<const CThing>*,
                                                              const CIntelligentPointer<CThing>&,
                                                              const random_access_iterator_tag&);
}