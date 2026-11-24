// CWaterPatchMesh::~CWaterPatchMesh() -- free the vertex height data (+0x14), hand the vertex buffer (+0x10) to the landscape memory manager's disposal queue (DeleteVertexBuffer 0x00BDA0D0), clearing each, then the CMovableResource base
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEAAA0, length 71 bytes, thiscall.
// Leaf: ??1CWaterPatchMesh@@UAE@XZ
// Identity (NF272, 2026-11-22; NF269 NAME_FIXES): the virtual destructor ??1CWaterPatchMesh@@UAE@XZ (Ego_r.pdb 00aa27a0 + FableWin.pdb 02e67a50): the
//   body stores the CWaterPatchMesh vtable 0x012A8134 first (rebuild/manifest/rtti-vtables.tsv; table ??_7CWaterPatchMesh@@6B@), frees
//   VertexHeightData +0x14 (operator delete 0x00BFE9BC) and VertexBuffer +0x10 (DeleteVertexBuffer), then stores the CMovableResource vtable
//   0x0129B860 (the base's inline destructor); ??_GCWaterPatchMesh 0x00BEB4D0 (vtable slot 0) is this body inlined plus the class delete.
//   Catalog said CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures; the July row defined ??1CEngineScreenEffectOutlineGlow@@QAE@XZ on
//   candidates/outline_glow_compiled.h (cast vtable addresses, stand-in members Blur2Texture / field_10). Now `this` is the shared retail class.
// Shape: the vertex-buffer half is CWaterPatchMesh::ReleaseBuffers 0x00BEAA00's body written out (that row's own TU defines it; whether the
//   original called it inline is not decidable from the bytes, which are the same).
// TU: this TU also emits ??_7CWaterPatchMesh@@6B@ / ??_GCWaterPatchMesh@@UAEPAXI@Z (pick-any; the class delete from the
//   FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE opt-in) and the CMovableResource vtable / ??_G; 0x00BEB4D0's row is this TU folded (nf251 pattern).
// Callees: 0x00BDA0D0 ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z (Ego_r.pdb 009c2640 / FableWin.pdb
//   02cb7450), 0x00BD9B90 ?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z (the class delete, in ??_G only), the global through
//   NEngineInternal::GPLandscapeRenderer (0x01436EA8) -> MemoryManager +0x6B0.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#define FABLE_RETAIL_CWATERPATCHMESH
#define FABLE_RETAIL_CWATERPATCHMESH_OPERATORDELETE     // ??3CWaterPatchMesh@@SIXPAX@Z (FableWin), inline: FreeVirtual 0x00BD9B90
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)
#include "engine/CWaterPatchMesh.h"

CWaterPatchMesh::~CWaterPatchMesh()
{
    if (VertexHeightData) {
        delete VertexHeightData;
        VertexHeightData = 0;
    }
    if (VertexBuffer) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(VertexBuffer);
        VertexBuffer = 0;
    }
}
