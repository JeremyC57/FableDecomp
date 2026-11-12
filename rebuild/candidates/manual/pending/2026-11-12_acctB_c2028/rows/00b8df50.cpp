// CEngineOcclusionManager::FrameEnd(const CEngineComponentParams& params) -- draw this frame's flare occlusion tests: inside a render-state capture block, with vertex shaders on (the flare-test shader), pixel shaders off, the diffuse-only state block and depth / colour writes off, each flare test whose sphere the engine camera can see gets an expected pixel count (pi * the projected radii, from the render-target size and the camera's view scales) and is drawn as a triangle fan of CircleSegments vertices placed by the camera's inverse view rotation scaled by the radius at the flare position; a test the camera clips gets a count of 1; afterwards depth / colour writes and both shader enables are restored and the camera's D3D transforms refreshed
// Leaf: ?FrameEnd@CEngineOcclusionManager@@UAEXABVCEngineComponentParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8DF50, 1626 bytes (ret 4 at 0x00B8E5A7; the reservation's 1841 ran over int3 padding and the
//   TU's unreferenced inline COMDATs 0x00B8E5B0..0x00B8E680 -- vector<COccluderFlareEntry>::size / empty / begin / end and four tiny
//   helpers, no caller: SPLITS.tsv), thiscall, no EH frame, /O2.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?FrameEnd@CEngineOcclusionManager@@UAEXABVCEngineComponentParams@@@Z (virtual; Ego_r donor
//   order FrameStart 0x00A35440, SelectFlareTestVertexBuffer 0x00A35510, FrameEnd 0x00A35820 = retail 0x00B8DBE0 (landed FrameStart),
//   0x00B8DCD0, 0x00B8DF50); the body walks FlareTests (+0x24024, 28-byte COccluderFlareEntry: the /28 size, Radius +0x0C,
//   CircleSegments +0x10 passed to SelectFlareTestVertexBuffer, MaxPixelCount +0x14 written) and applies VertexShaderFlareTest +0x24060.
// Callees (every relocation checked against retail, tools/rel.py): CShaderRenderManager::EnableVertexShaders 0x00987FE0 /
//   DisableVertexShaders 0x009880E0 / EnablePixelShaders 0x00988110 / DisablePixelShaders 0x00988190 / ApplyVertexShader(const
//   CVertexShader&, bool) 0x00988020 / SetWorldTransform(const CMatrix3x4&) 0x009881F0 and, through the inline PrepareForRender(1),
//   SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760,
//   UpdateFogColour 0x009897C0, UpdateLightGlobals 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50; GFGetSystemManager
//   0x009A4EC0; CDisplayManager::GetRenderTargetDimensions 0x009BEDC0; SelectFlareTestVertexBuffer(unsigned long) 0x00B8DCD0 (private);
//   CRenderManagerCore::DrawCurrentBuffersAsTriFan(long, long) 0x00A0AE40; CEngineCamera::RefreshD3DTransforms 0x00B2FC50;
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840 (the scoped capture block's destructor); the diffuse-only state block's
//   virtual Apply (slot 0, CEngineStateBlocks +0x28).
// Globals: CRenderStateCaptureBlock::StateManager 0x013CB508, NEngineInternal::GPShaderRenderManager 0x01436E14 / GPRenderManager
//   0x01436E18 and g_pEngineCamera 0x01436EA0 (the spellings of the definer CEngine::Initialise 0x00B26360 / NAME_FIXES_QUEUE c722; no PDB data symbols), g_pEngineStateBlocks 0x01436E90 (the
//   spelling of the landed CEngine::Initialise 0x00B26360), DELTA_NOTIONAL_ZERO 0x0129BA3C; 0.5f / pi are __real pool constants.
// Inline (no retail out-of-line copy; shared headers): CRenderStateScopedCaptureBlock (StartCaptureBlock / RestoreCaptureBlock),
//   CShaderRenderManager::SetVertexShadersEnable / SetPixelShadersEnable / PrepareForRender, CRenderStateManager::SetZBufferWriteEnable /
//   SetColourWriteEnable (bookmark + update list), CEngineStateBlocks::ApplyDiffuseOnlyStateBlock, CEngineCamera::ClipSphere(const
//   C3DVector&, float) (as the landed CEngine::ClipSphere 0x00B24150 expands it) / PeekInverseNonHomogenousViewTransform /
//   TransformWorldToView / PeekNonHomogenousViewToViewScaleX / Y, CMatrix3x4::SetTranslateOnly / PreScale / TransformPoint,
//   CSystemManager::GetDisplayManager, CArray size / empty / operator[].
// Retail-asm: 0x00B8E353 `fld dword ptr [esp+18h]` + 0x00B8E357 `fistp dword ptr [esp+24h]` (the pixel count stored to a frame slot,
//   reloaded and stored with a bare dword fistp) -- the engine's inline-asm float-to-long, the shared engine/GFFloatToLongNear.h helper
//   (byte-matching path + FABLE_PORTABLE twin); this row carries no asm of its own. C++ attempts at that site before the helper
//   (probe/fragW.cpp, VC7.1 RTM /O2): (long)x, (int)x, (unsigned long)x, implicit float -> unsigned long, a named float then (long),
//   (long)(x + 0.5f), (short)x, through a double -- all call __ftol2; the same eight with /QIfist emit `fistp qword ptr [esp]`;
//   no VC7.1 intrinsic emits an x87 fistp (_mm_cvtss_si32 is the SSE cvtss2si). No C++ form gives the bare dword fistp.
// Shape: the capture block constructed first, then `if (FlareTests.empty()) return;` (the exit jumps to the block's destructor);
//   the loop is an unsigned index over size() (recomputed each pass, the index and its *28 offset kept in frame slots); a visible
//   flare's world transform is the camera's inverse non-homogenous view transform with a copied position as translation and the
//   radius as pre-scale: the copy placed after the matrix copy and SetTranslateOnly(const C3DVector&) forwarding to the float overload
//   (Z read first) give retail's `fld y ; fld x ; rep movsd ; fstp E41 ; fstp E42` with E43 one dword copy and no frame word for the
//   copy; TransformPoint's rows accumulated Z-term first keep retail's E33, E13, E23 order after those reads (hdr/README.md).
//   radiusX is computed before radiusY (retail evaluates the Y product first); the clipped arm stores MaxPixelCount = 1.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUS
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINESTATEBLOCKS
#define FABLE_RETAIL_CENGINESTATEBLOCKS_APPLY_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASTRIFAN
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY_XYZ
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_COPY_SUMS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#include "engine/CMatrix3x4.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineStateBlocks.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/C2DExtentsI.h"
#include "engine/GFFloatToLongNear.h"

namespace NEngineInternal {
extern CShaderRenderManager* GPShaderRenderManager;                     // 0x01436E14
extern CRenderManager*       GPRenderManager;                           // 0x01436E18
}
extern CEngineCamera* g_pEngineCamera;                                   // 0x01436EA0
extern CEngineStateBlocks* g_pEngineStateBlocks;                        // 0x01436E90

void CEngineOcclusionManager::FrameEnd(const CEngineComponentParams& params)
{
    CRenderStateScopedCaptureBlock captureBlock;
    if (FlareTests.empty())
        return;
    bool vertexShadersEnabled;
    NEngineInternal::GPShaderRenderManager->SetVertexShadersEnable(true, &vertexShadersEnabled);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderFlareTest, false);
    bool pixelShadersEnabled;
    NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(false, &pixelShadersEnabled);
    g_pEngineStateBlocks->ApplyDiffuseOnlyStateBlock();
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetColourWriteEnable(false);
    for (unsigned long i = 0; i < FlareTests.size(); ++i) {
        COccluderFlareEntry& entry = FlareTests[i];
        if (g_pEngineCamera->ClipSphere(entry.Position, entry.Radius)) {
            CMatrix3x4 transform = g_pEngineCamera->PeekInverseNonHomogenousViewTransform();
            C3DVector position = entry.Position;
            transform.SetTranslateOnly(position);
            transform.PreScale(entry.Radius);
            NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
            C3DVector viewPos;
            g_pEngineCamera->TransformWorldToView(entry.Position, viewPos);
            if (viewPos.Z > DELTA_NOTIONAL_ZERO) {
                float scale = entry.Radius / viewPos.Z;
                C2DExtentsI dims = GFGetSystemManager()->GetDisplayManager()->GetRenderTargetDimensions();
                float radiusX = (float)dims.X * g_pEngineCamera->PeekNonHomogenousViewToViewScaleX() * scale * 0.5f;
                float radiusY = (float)dims.Y * g_pEngineCamera->PeekNonHomogenousViewToViewScaleY() * scale * 0.5f;
                entry.MaxPixelCount = GFFloatToLongNear(radiusX * radiusY * 3.1415927f);
            }
            SelectFlareTestVertexBuffer(entry.CircleSegments);
            NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
            NEngineInternal::GPRenderManager->DrawCurrentBuffersAsTriFan(0, entry.CircleSegments);
        } else {
            entry.MaxPixelCount = 1;
        }
    }
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
    NEngineInternal::GPRenderManager->SetColourWriteEnable(true);
    NEngineInternal::GPShaderRenderManager->SetVertexShadersEnable(vertexShadersEnabled, 0);
    NEngineInternal::GPShaderRenderManager->SetPixelShadersEnable(pixelShadersEnabled, 0);
    g_pEngineCamera->RefreshD3DTransforms();
}
