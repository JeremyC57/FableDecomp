// C3DMeshInfo::AreTexturesLoaded() const -- the PC build keeps every mesh texture resident: always true
// Leaf: ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23C60, 3 bytes (`mov al,1 ; ret`).
// Name (round 7, NF53): Ego_r.pdb ?AreTexturesLoaded@C3DMeshInfo@@QBE_NXZ (donor 0x00B5FEB0, right after BeginLoadingTextures 0x00B5FEA0 =
//   retail 0x00A23C50, the empty `ret 4`); every retail caller (0x00B9CE1E, 0x00B9EADC, 0x00BD2102) calls it on a mesh info. The catalog
//   label C3DMeshInfo::IsBuilt is in neither PDB; the row was the stand-in free function fn_operator. engine/C3DMeshInfo.h
//   (FABLE_RETAIL_CMESHDATABANKENTRY + the overlay opt-in FABLE_RETAIL_C3DMESHINFO_ARETEXTURESLOADED).
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_ARETEXTURESLOADED
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"

bool C3DMeshInfo::AreTexturesLoaded() const
{
    return true;
}
