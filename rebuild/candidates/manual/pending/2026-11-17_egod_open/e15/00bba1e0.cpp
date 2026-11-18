// CEngineInternalPrimitiveParticleGroup::`scalar deleting destructor'(unsigned int flags) -- the compiler-generated destructor expanded (FreePrimitiveIDs, PrimitiveIDToIterator, Primitives torn down, then the root CIVCountedPointeeBase vtable store) and the flag-tested operator delete
// Grade: DIFFER(30v82) -- VC7.1 calls the implicit ??1 (0x00BBA140 body, which itself compiles to the landed shape except list<long>) instead of expanding it into the ??_G; retail expands it
// Leaf: ??_GCEngineInternalPrimitiveParticleGroup@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBA1E0, 82 bytes, thiscall, `ret 4`, returns this.
// Identity: slot 0 of RTTI vtable 0x012A59D4 = CEngineInternalPrimitiveParticleGroup (rebuild/manifest/rtti-vtables.tsv); FableWin.pdb
//   ??_GCEngineInternalPrimitiveParticleGroup@@UAEPAXI@Z (0x02EA4D00); Ego_d.pdb: the class's destructor is `compiler-generated`
//   (LF_ONEMETHOD attrs), its bases' (CEngineInternalPrimitiveBase, CEnginePersistentPrimitiveBase) are user-declared virtual and
//   inline (no Ego_r ??1). Catalog label "CGuiControlTreePane_OnFileLoadComplete" is propagated and wrong.
// Callees: ~_List_base<long> 0x0042AB73 (FreePrimitiveIDs), free 0x00BFEA14, _List_base<CEnginePrimitiveHandle>::clear 0x00834F90,
//   operator delete 0x00BFE9BC; vtable 0x012354D0 (CIVCountedPointeeBase).
// The ctor definition (FableWin.pdb protected ??0..@@IAE@W4EEnginePrimitiveType@@K@Z, inline in retail) is only here to emit the
//   vtable and so the ??_G; it is not a retail out-of-line function.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE_INLINE_DTOR
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_INLINE_DTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_CTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CEngineInternalPrimitiveParticleGroup.h"

CEngineInternalPrimitiveParticleGroup::CEngineInternalPrimitiveParticleGroup(EEnginePrimitiveType type, unsigned long flags)
    : CEngineInternalPrimitiveBase(type, flags)
{
}
