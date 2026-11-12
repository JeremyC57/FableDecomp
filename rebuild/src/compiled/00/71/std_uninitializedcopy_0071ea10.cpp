// std::__uninitialized_copy(const CDefPointer<const CThingBaseDef>* first, const CDefPointer<const CThingBaseDef>* last, CDefPointer<const CThingBaseDef>* result, const std::__false_type&) -- STLport 4.6 _uninitialized.h: copy-construct each counted def pointer of [first, last) into raw storage at result (add-ref'ing the def), return the end
// Leaf: ??$__uninitialized_copy@PBV?$CDefPointer@$$CBVCThingBaseDef@@@@PAV1@@std@@YIPAV?$CDefPointer@$$CBVCThingBaseDef@@@@PBV1@0PAV1@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071EA10, 45 bytes, /Gr `__fastcall` (first ecx, last edx, result + tag on the stack), `ret 8`.
// Identity (agentFAM06): the only caller is 0x00721150 vector<CDefPointer<const CThingBaseDef> >::operator= (call 0x00721220, the
//   STLport 4.6 `__uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish, _M_finish, _IsPODType())`
//   arm, hence const first / last), itself called only by CTCHeroMorph::Copy 0x0071CD90 on Tattoos (Ego_r.pdb CTCHeroMorph +0x2C
//   vector<CDefPointer<const CThingBaseDef> >; that vector's members are in both PDB name sets). The catalog label
//   UninitCopy_CIVCountedPointer is a placeholder: the element is a CDefPointer, not a CIVCountedPointer. Byte-identical to
//   0x00A6AFD0 (CIVCountedPointer<CManagedTextureData>).
// Body: the STLport 4.6 template of the shared rebuild/include/stlport/fable_containers.h over the shared CDefPointer<T>
//   (its counted copy constructor `if (Object) Object->AddRef()`).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CDefPointer.h"
class CThingBaseDef;
template CDefPointer<const CThingBaseDef>* std::__uninitialized_copy<const CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>* >(const CDefPointer<const CThingBaseDef>*, const CDefPointer<const CThingBaseDef>*, CDefPointer<const CThingBaseDef>*, const std::__false_type&);
