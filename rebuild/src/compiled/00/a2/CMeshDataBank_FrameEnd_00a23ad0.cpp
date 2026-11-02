// CMeshDataBank::FrameEnd() -- end the frame of the mesh, animation and physics-mesh resource banks (CResourceBank::FrameEnd on each)
// Leaf: ?FrameEnd@CMeshDataBank@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23AD0, 37 bytes, thiscall, plain tail `jmp CResourceBank::FrameEnd`.
// Identity (proven): the only caller is CEngineResourceManager::FrameEnd 0x00B3B870, which calls this row on each non-null
//   entry of the three-pointer array at +0x10 -- Ego_r.pdb CEngineResourceManager::MeshBanks (CMeshDataBank*[3], +0x10);
//   Ego_r.pdb / FableWin.pdb both carry ?FrameEnd@CMeshDataBank@@QAEXXZ (non-virtual). The three `this` offsets are the
//   CMeshDataBank members MeshResourceBank +0x170, AnimationResourceBank +0x268 and PhysicsMeshResourceBank +0x2e4
//   (engine/CMeshDataBank.h); MeshStatsResourceBank (+0x1ec) is not ended. The catalog label `_Dest_val<...CSymbolInfo>` is
//   propagated; the landed caller's stand-in `CResourcePool_00a23ad0::Release` is this function (NAME_FIXES.tsv).
// Callees: CResourceBank::FrameEnd 0x009FC430 (?FrameEnd@CResourceBank@@QAEXXZ, Ego_r + FableWin), three times.
// Layout: engine/CMeshDataBank.h with the chunk 712 retail opt-in FABLE_RETAIL_CMESHDATABANK (the four banks typed
//   CResourceBank from engine/CResourceBank.h, sizeof and offsets unchanged).
// Shape: three member calls in Ego_r declaration order; the last becomes the tail jump.
#define FABLE_RETAIL_CMESHDATABANK
#include "engine/CMeshDataBank.h"

void CMeshDataBank::FrameEnd()
{
    MeshResourceBank.FrameEnd();
    AnimationResourceBank.FrameEnd();
    PhysicsMeshResourceBank.FrameEnd();
}