// std::__uninitialized_fill_n(CTextureMorphMap* __first, unsigned int __n, const CTextureMorphMap& __x, const __false_type&) -- STLport construct n copies of one texture-morph map into raw storage
// Leaf: ??$__uninitialized_fill_n@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D090, 48 bytes (ends at the next function 0x0086D0C0), __fastcall (first ecx, n edx;
//   the value and the tag reference on the stack, `ret 8`). CTextureMorphMap = CVectorMap<unsigned long,
//   CRandomAppearanceMorph::CTextureMorph, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (16 bytes).
// Identity (proven, two sources): (1) the only caller is the landed vector<CTextureMorphMap>::_M_insert_overflow 0x0086E1C0
//   (call at 0x0086E246: the `__fill_len != 1` arm, `__uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type())`);
//   the bound spelling of reloc_bindings.tsv from that caller is exactly the symbol this source emits; (2) STLport 4.6
//   stl/_uninitialized.h: the four-argument `__false_type` body, `ret 8` = value + tag on the stack, and the element copy is the
//   implicit CVectorMap copy (the out-of-line base vector<pair<unsigned long, CTextureMorph> > copy constructor 0x00673E10,
//   nothing for the empty comparator +0x0C, the Dirty byte +0x0D) -- the same element as the landed __uninitialized_copy
//   0x0086D530. Byte near-twin 0x0086D570 is the three-argument front end uninitialized_fill_n (ret 4, its own caller
//   _M_fill_insert 0x0086E4B0): two symbols. The catalog label "_Fill_n<unsigned long*,...>" is propagated and wrong.
// Callee: vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >::vector(const vector&) 0x00673E10.
// Shape: STLport 4.6 text verbatim (no EH: _STLP_TRY / _STLP_UNWIND are empty without /EHsc); `__n > 0` on the unsigned size
//   is the `test edx,edx ; jbe` entry and `dec edi ; jne` loop; `_Construct` is placement new with its null test.
//   /Gr as the landed siblings of this TU (0x0086D530, 0x0086E1C0, 0x0086E4B0).

#include "engine/CVectorMap.h"



class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                           // Ego_r.pdb, POD
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> CTextureMorphMap;

template CTextureMorphMap* std::__uninitialized_fill_n<CTextureMorphMap*, unsigned int, CTextureMorphMap>(
    CTextureMorphMap*, unsigned int, const CTextureMorphMap&, const std::__false_type&);