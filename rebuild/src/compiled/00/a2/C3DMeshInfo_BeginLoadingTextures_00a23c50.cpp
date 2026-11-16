// C3DMeshInfo::BeginLoadingTextures(EBankLoadPriority priority) const -- nothing to do on the PC build: the empty `ret 4`.
// Leaf: ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z
// NF37 (2026-11-16): moved off the C00a23c50::M stand-in onto the shared engine/C3DMeshInfo.h (BEGINLOADINGTEXTURES opt-in, which names
// 0x00A23C50; Ego_r.pdb / FableWin.pdb ?BeginLoadingTextures@C3DMeshInfo@@QBEXW4EBankLoadPriority@@@Z, donor 0x00B5FEA0 between HasPhysicsMesh
// and AreTexturesLoaded; 0x00B9A6C6 calls it on the mesh info with priority 1).
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#include "engine/C3DMeshInfo.h"

void C3DMeshInfo::BeginLoadingTextures(EBankLoadPriority priority) const
{
}
