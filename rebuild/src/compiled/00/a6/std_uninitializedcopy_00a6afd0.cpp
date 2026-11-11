// std::__uninitialized_copy(CIVCountedPointer<CManagedTextureData>* first, CIVCountedPointer<CManagedTextureData>* last, CIVCountedPointer<CManagedTextureData>* result, const std::__false_type&) -- STLport 4.6 _uninitialized.h: copy-construct each counted texture-data pointer of [first, last) into raw storage at result (add-ref'ing the pointee), return the end
// Leaf: ??$__uninitialized_copy@PAV?$CIVCountedPointer@VCManagedTextureData@@@@PAV1@@std@@YIPAV?$CIVCountedPointer@VCManagedTextureData@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6AFD0, 45 bytes, /Gr `__fastcall` (first ecx, last edx, result + tag on the stack), `ret 8`.
// Identity (agentFAM06): callers 0x00A6BA60 vector<CIVCountedPointer<CManagedTextureData> >::_M_insert_overflow (call 0x00A6BB18)
//   and 0x00A6BCD0 ::_M_fill_insert (calls 0x00A6BD2E / 0x00A6BD8B), both landed and bound to this exact symbol in
//   rebuild/oracles/reloc_bindings.tsv; Ego_r.pdb and FableWin.pdb both hold vector<CIVCountedPointer<CManagedTextureData> >
//   (reserve / resize / push_back), the owner CManagedTexture::Data. The catalog label UninitCopy_CIVCountedPointer is a placeholder.
// Body: the STLport 4.6 template of the shared rebuild/include/stlport/fable_containers.h over the shared CIVCountedPointer<T>
//   (FABLE_RETAIL_CIVCOUNTEDPOINTER: the counted copy constructor `if (Object) Object->AddRef()`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"
class CManagedTextureData;
template CIVCountedPointer<CManagedTextureData>* std::__uninitialized_copy<CIVCountedPointer<CManagedTextureData>*, CIVCountedPointer<CManagedTextureData>* >(CIVCountedPointer<CManagedTextureData>*, CIVCountedPointer<CManagedTextureData>*, CIVCountedPointer<CManagedTextureData>*, const std::__false_type&);
