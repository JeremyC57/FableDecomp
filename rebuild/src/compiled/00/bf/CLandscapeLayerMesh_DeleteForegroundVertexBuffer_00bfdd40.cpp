// CLandscapeLayerMesh::DeleteForegroundVertexBuffer() -- hand the layer mesh's foreground vertex buffer (+0x38) to the landscape memory manager's disposal queue (GPLandscapeRenderer -> +0x6B0, DeleteVertexBuffer 0x00BDA0D0) and clear it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFDD40, length 48 bytes.
// Leaf: ?DeleteForegroundVertexBuffer@CLandscapeLayerMesh@@QAEXXZ
// Shape: the buffer pushed before the manager is loaded, `ret`.
// Identity (NF272, 2026-11-22; NF269 NAME_FIXES): ?DeleteForegroundVertexBuffer@CLandscapeLayerMesh@@QAEXXZ, Ego_r.pdb 00a49360 + FableWin.pdb 02ddde80;
//   Ego_r order DeleteForegroundVertexBuffer 00a49360 -> OnPostRelocation 00a49390 = retail 0x00BFDD40 -> 0x00BFDD70 (landed
//   CLandscapeLayerMesh::OnPostRelocation); the body frees and clears CLandscapeLayerMesh::ForegroundVertexBuffer +0x38 (Ego_r layout,
//   engine/_quarantine/CLandscapeLayerMesh.h) through DeleteVertexBuffer; its landed caller CEngineLandscapePatch::Clear 0x00BF38F0 (E8 0x00BF3903)
//   calls it on each mesh of the patch's LayerMeshList (+0x08), linked through Next (+0x3C). Was the address-suffixed stand-in
//   ?ClearBlock@CTextureOwner_00bfdd40@@QAEXXZ (the caller bound a third spelling, ?ClearBlock@CTextureOwner@@QAEXXZ).
// Layout: CLandscapeLayerMesh's only header is quarantined (engine/_quarantine/, header-less for the local-model rule, EXTRA_RULES
//   2026-11-20); modelled locally in the shape of the landed siblings 0x00BFDE20 / 0x00BFDD70 (IndexBuffer +0x34, ForegroundVertexBuffer
//   +0x38, Ego_r names), on the shared retail CMovableResource base (RTTI .?AVCLandscapeLayerMesh@@ : CMovableResource, vtable 0x012A835C).
// Callee (NF269): 0x00BDA0D0 is ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z (Ego_r.pdb 009c2640 /
//   FableWin.pdb 02cb7450), the retail branch of engine/CEngineLandscapeMemoryManager.h with the landed DeleteVertexBuffer opt-in.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CMovableResource.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"        // MemoryManager +0x6b0 (Ego_r.pdb CEngineLandscapeRenderer)

class CIndexBuffer;
class CVertexBufferWin32;
class CLandscapeLayerMesh : public CMovableResource { public:    // vtable 0x012A835C
    unsigned char       _pad04[0x30];
    CIndexBuffer*       IndexBuffer;               // +0x34
    CVertexBufferWin32* ForegroundVertexBuffer;    // +0x38
    void DeleteForegroundVertexBuffer();
};

void CLandscapeLayerMesh::DeleteForegroundVertexBuffer()
{
    if (ForegroundVertexBuffer) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteVertexBuffer(ForegroundVertexBuffer);
        ForegroundVertexBuffer = 0;
    }
}
