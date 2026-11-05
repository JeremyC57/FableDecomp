// std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>(CTextureMorphMap* __first, CTextureMorphMap* __last, CTextureMorphMap* __result, const __false_type&) -- STLport copy-construct the texture-morph maps [first, last) into raw storage at result
// Leaf: ??$__uninitialized_copy@PAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@@std@@YIPAV?$CVectorMap@KVCTextureMorph@CRandomAppearanceMorph@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@@PAV1@00ABU__false_type@0@@Z
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086D530, 52 bytes, __fastcall (first ecx, last edx; result and the tag reference on the
//   stack, `ret 8`); CTextureMorphMap = CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
//   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > (16 bytes).
// Identity: the callers are the landed vector<CTextureMorphMap>::_M_insert_overflow 0x0086E1C0 (head and tail copies,
//   `__uninitialized_copy(this->_M_start, __position, ...)` -- non-const pointers) and the landed _M_fill_insert 0x0086E4B0
//   (the `__old_finish - __n` tail move); both are instantiated over the per-body-part texture-morph maps (Ego_r.pdb
//   CRandomAppearanceMorph::BodyPartTextureMorphs, FableWin.pdb ?resize@?$vector@V?$CVectorMap@KVCTextureMorph@... of
//   the landed 0x0086EA80). The callee 0x00673E10 is the map base's vector<pair<unsigned long, CTextureMorph> > copy
//   constructor (bound). Already the bound spelling of reloc_bindings.tsv (from 0x0086E1C0); the second binding
//   ?UninitCopy@@YIPAUElement_0086e4b0@@... (from 0x0086E4B0) is a stand-in (NAME_FIXES). The twin 0x00674CE0 is the
//   const-source instantiation __uninitialized_copy<const CTextureMorphMap*, CTextureMorphMap*> (bound PBV; its callers are
//   _M_allocate_and_copy 0x00674EB0 and operator= 0x006753F0, which copy from a const vector), so the two bodies are two
//   COMDATs. The manifest "_Uninit_copy<CFeatDef*,...>" is propagated (CFeatDef is 100 bytes).
// Shape: STLport 4.6 stl/_uninitialized.h verbatim (no EH: _STLP_TRY/_STLP_UNWIND are empty without /EHsc); the implicit
//   CVectorMap copy constructor = the out-of-line vector copy, nothing for the empty comparator, the Dirty byte (+0x0D);
//   `_Construct` is placement new with its null test.

#include "engine/CVectorMap.h"



class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                           // Ego_r.pdb, POD
    public:
        unsigned long NewTexture;
        unsigned long GroupID;
    };
};

typedef CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph,
                   CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> > CTextureMorphMap;

template CTextureMorphMap* std::__uninitialized_copy<CTextureMorphMap*, CTextureMorphMap*>(
    CTextureMorphMap*, CTextureMorphMap*, CTextureMorphMap*, const std::__false_type&);