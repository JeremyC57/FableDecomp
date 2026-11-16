// CEnginePrimitiveManagerOcclusionBase::DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour) -- open a render-state capture block, set thirteen tracked render states (texture-stage colour/alpha ops and args from the globals 0x01396E74/0x01396EF4/0x01396E70, the texture factor = colour, +0x28B8 = 1, +0x291C/+0x2930 from 0x01396F78/0x01396F7C, +0x2840 = 0xF, +0x2868 = 0, +0x282C from 0x01396FA8), render the occlusion data's untextured test mesh with the box, and restore the capture block
// Leaf: DrawDebugVisibilityTestBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B90700, length 1488 bytes, `ret 8`.
// Callees: RenderUntexturedStaticMeshPrimitive @ 0x00B903D0 (on `this`: the mesh, the box; as the landed DrawVisibilityTestBox
//   0x00B906C0), CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (global 0x013CB508; the capture-block open is the inline
//   of the landed 0x00BE1E70 / 0x00B89A20). Render core 0x01436E18: the model of the landed CEnginePrimitiveRenderUtil::
//   SetupStencilMask 0x00BF1030 with the `__forceinline` SetState(state, value, core).
// Signature: Ego_r.pdb `DrawDebugVisibilityTestBox(const CMatrix3x4&, unsigned long)` (the landed caller RenderBoundingBox 0x00B91140).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h): the local CRenderManagerCoreView / SetState
//   model and the g_renderStateDefault_<addr> table entries are the shared inline CRenderStateManager setters (+0x29D0 TextureFactor
//   through the overlay opt-in SETTEXTUREFACTOR_INLINE; +0x2840 ColourWriteEnable(true) = 0xF through the repo OUTLINE_INLINE;
//   +0x282C PolyFillMode [POLY_FILL_WIRE] through D3DPolyFillModeTable 0x01396FA4). The capture open / RestoreCaptureBlock pair on
//   0x013CB508 is a CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h, CRenderStateCaptureBlock::StateManager; was
//   the local CCaptureStateManager / g_pRenderStateManager).
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETTEXTUREFACTOR_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
class CMatrix3x4 { public: float M[12]; };

struct COcclusionData {
    unsigned char _pad[0x94];
    void*         m_pTestMesh;                     // +0x94
};

struct CEnginePrimitiveManagerOcclusionBase {
    unsigned char   _pad[0x14];
    COcclusionData* m_pOcclusionData;              // +0x14
    void RenderUntexturedStaticMeshPrimitive(void* mesh, const CMatrix3x4& box);   // 0x00B903D0
    void DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour);
};

void CEnginePrimitiveManagerOcclusionBase::DrawDebugVisibilityTestBox(const CMatrix3x4& box, unsigned long colour)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
    NEngineInternal::GPRenderManager->SetTextureArgument1(0, TEXTURE_ARG_TFACTOR);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(0, TEXTURE_OP_SELECT_ARG1);
    NEngineInternal::GPRenderManager->SetTextureAlphaArgument1(0, TEXTURE_ARG_TFACTOR);
    NEngineInternal::GPRenderManager->SetTextureOperation(1, TEXTURE_OP_DISABLE);
    NEngineInternal::GPRenderManager->SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    NEngineInternal::GPRenderManager->SetTextureFactor(colour);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    NEngineInternal::GPRenderManager->SetColourWriteEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyFillMode(POLY_FILL_WIRE);
    RenderUntexturedStaticMeshPrimitive(m_pOcclusionData->m_pTestMesh, box);
}