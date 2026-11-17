// CMeshInfoOwner_00b9ce10::IsLoadedAndValid() -- long-valued "both checks pass" predicate on `this`.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B9CE10, length 34 bytes.
// Callees (both on `this`, no arguments, bool results): C3DMeshInfo::IsLoaded @ 0x00A23E40 (walks the mesh
// list at +0xc) and the trivial `mov al,1` query @ 0x00A23C60 (manifest "CThingFilter_IsUnconscious::
// operator()" is a propagated label for a constant-true body). The manifest module label
// "CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction" is propagated from an unrelated AI body; the
// class is named descriptively after what the callees look at.
// Shape: `if (a() && b()) return 1; return 0;` with a LONG result (`mov eax,1` / `xor eax,eax`, not `al`).
// Name (round 7, NF53): 0x00A23E40 is ?IsLoaded@C3DMeshInfo@@QBE_NXZ and 0x00A23C60 ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ (Ego_r.pdb donor
//   order: BeginLoadingTextures 0x00B5FEA0 / AreTexturesLoaded 0x00B5FEB0 = retail 0x00A23C50 / 0x00A23C60, the latter `mov al,1`); both run
//   on `this` unchanged, so the owner is the mesh info itself, viewed through engine/C3DMeshInfo.h (_ISLOADED, overlay _ARETEXTURESLOADED).
//   Was local IsLoaded / IsValid declarations on the owner stand-in. The owner's own name stays descriptive (no PDB match found).
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_ARETEXTURESLOADED
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"

struct CMeshInfoOwner_00b9ce10 {
    const C3DMeshInfo& MeshInfo() const { return *reinterpret_cast<const C3DMeshInfo*>(this); }
    long IsLoadedAndValid();
};

long CMeshInfoOwner_00b9ce10::IsLoadedAndValid()
{
    if (MeshInfo().IsLoaded() && MeshInfo().AreTexturesLoaded())
        return 1;
    return 0;
}
