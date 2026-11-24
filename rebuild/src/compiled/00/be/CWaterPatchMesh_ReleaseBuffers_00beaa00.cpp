// CWaterPatchMesh::ReleaseBuffers() -- hand the patch's vertex buffer (+0x10) to the landscape memory manager's disposal queue (GPLandscapeRenderer -> +0x6B0, DeleteVertexBuffer 0x00BDA0D0) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEAA00, length 48 bytes.
// Leaf: ?ReleaseBuffers@CWaterPatchMesh@@QAEXXZ
// Shape: the buffer pushed before the manager is loaded, `ret`.
// Identity (NF272, 2026-11-22; NF269 NAME_FIXES): ?ReleaseBuffers@CWaterPatchMesh@@QAEXXZ, Ego_r.pdb 00aa2710 + FableWin.pdb 02e67fe0; Ego_r order
//   ReleaseBuffers 00aa2710 -> OnPostRelocation 00aa2740 = retail 0x00BEAA00 -> 0x00BEAA30; the body frees and clears CWaterPatchMesh::VertexBuffer +0x10
//   (engine/CWaterPatchMesh.h, Ego_r layout) through DeleteVertexBuffer; its only landed caller 0x00BF4220 (E8 0x00BF43C3 / 0x00BF445D) calls it on
//   CEngineLandscapePatch::WaterPatch +0x14. Was the address-suffixed stand-in ?ClearBlock@CTextureOwner_00beaa00@@QAEXXZ on a row-local
//   `struct CTextureOwner_00beaa00` (pad + one member); now `this` is the shared retail class (engine/CWaterPatchMesh.h, which already declares it).
// Callee (NF269): 0x00BDA0D0 is ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z (Ego_r.pdb 009c2640 /
//   FableWin.pdb 02cb7450), the retail branch of engine/CEngineLandscapeMemoryManager.h with the landed DeleteVertexBuffer opt-in.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#define FABLE_RETAIL_CWATERPATCHMESH
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
#include "engine/CWaterPatchMesh.h"

void CWaterPatchMesh::ReleaseBuffers()
{
    if (VertexBuffer) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(VertexBuffer);
        VertexBuffer = 0;
    }
}
