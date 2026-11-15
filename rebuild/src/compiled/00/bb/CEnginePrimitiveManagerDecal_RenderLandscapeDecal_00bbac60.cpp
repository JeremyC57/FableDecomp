// CEnginePrimitiveManagerDecal::RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal) -- draw a landscape decal: fetch its frame from graphic bank 0, open a render-state capture block with shaders on, set the blend state, the colour constant and the clamp-to-border sampler states, build a unit-grid mesh over the decal's footprint (alternating diagonals), light it and draw it as indexed triangles
// Leaf: ?RenderLandscapeDecal@CEnginePrimitiveManagerDecal@@IAEXAAVCEngineInternalPrimitiveDecal@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBAC60, 3743 bytes (`ret 4`); no flags. Identity: Ego_r.pdb / FableWin.pdb
// Retail-asm: 0x00BBB000..0x00BBB020 / 0x00BBB03A..0x00BBB05A `fld [v] ; fsub [ebp-1Ch] (0.5f) ; fistp dword ptr [ebp-14h] ; fild ; fadd [ebp-20h] (1.0f) ; fstp ; cmp ; jne ; inc dword ptr` --
//   the engine's inline-assembly floor helper (engine/GFFloatToLongFloor.h) expanded for min_x / min_y, and 0x00BBB080 / 0x00BBB0BA the
//   matching ceil helper (engine/GFFloatToLongCeil.h, `fld [v] ; fadd [ebp-1Ch] ; fistp dword ptr ; fild ; fsub [ebp-20h] ; fstp ; cmp ; jne ; dec dword ptr`) for max_x / max_y; VC7.1 emits no bare dword fistp from C++.
//   ?RenderLandscapeDecal@CEnginePrimitiveManagerDecal@@IAEXAAVCEngineInternalPrimitiveDecal@@@Z (protected; FableWin 0x02EB0A60, the decal TU).
// Inferred: three dead release asserts (FABLE_RETAIL_ASSERT, fable_assert.h): FableWin's /Od body is 861 instructions v 597 for this text
//   = +264 = ~3 asserts of ~87; with 0 or 1 the index-store SIB base/index order of the two LSR'd index loops (0x00BBB213.., 0x00BBB40B..)
//   flips (a symbol-record threshold: a no-byte ruler at the end of the body closes it too); 2..6 match, 3 = the FableWin count. FITTED positions.
// EVIDENCED: SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode (FableWin.pdb names, /Od sizes SAME; landed sibling 0x00BB8EF0 text);
//   the constant built in both arms (retail repeats the byte -> float conversion in the premultiplied arm 0x00BBB394 and the plain one
//   0x00BBAF5B, and the SetPixelShaderConstant tail is shared by a jump 0x00BBB400); SetTextureBorderColour expanded (0x00BBB611; overlay
//   opt-in FABLE_RETAIL_CRENDERSTATEMANAGER_BORDERCOLOUR, no retail out-of-line copy).
// Local class models: CEngineInternalPrimitiveDecal / CEnginePrimitiveManagerDecal have no shared header (the landed decal rows
//   0x00BBA290 / 0x00BBA340 model them locally too).
// Callees: CBankFile::EntryExists 0x009CC190, CGraphicInfo::GetFrame 0x009FF8D0 (?GetFrame@CGraphicInfo@@QBE_NAAVCGraphicFrame@@KKK@Z),
//   CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC, vector<CTVertexNormTex1>(n) 0x00BB9FD0 (same TU),
//   vector<unsigned short>(n) 0x00A8D750, __ftol2 0x00BFEA70, CEngineLightingManager::GetLightsInSphereFast 0x00B46B90 /
//   SetupShaderPointLights 0x00B44F20 / UpdateGlobalLightingStates 0x00B46C80, CShaderRenderManager::ApplyPixelShader 0x00988140 /
//   ApplyVertexShader 0x00988020 / ResetWorldTransform 0x00988290 and the PrepareForRender(9) helpers, CRenderManagerCore::
//   DrawAsIndexedTris 0x00A0AF40, free 0x00BFEA14, the scoped shader enable (0x00987FE0 / 0x00988110 / 0x009880E0 / 0x00988190) and
//   CRenderStateManager::RestoreCaptureBlock 0x00A05840.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_LIGHTSINSPHERE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS_USER
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_BORDERCOLOUR
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#include "fable_compiler.h"
#include "fable_assert.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicFrame.h"
#include "engine/CArray.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTVertexNormTex1.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongCeil.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C

// The engine/CEnginePrimitiveRenderUtil.h opt-in declares these FableWin.pdb members and leaves the bodies to the including row (landed
// sibling 0x00BB8EF0 carries the same text).
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

class CEngineInternalPrimitiveDecal : public CEngineInternalPrimitiveBase { public:   // Ego_r.pdb, 0x94
    bool                      LandscapeOnlyDecal;                        // +0x48
    long                      DecalIndex;                                // +0x4c
    long                      DecalFrame;                                // +0x50
    C3DVector                 WorldPos;                                  // +0x54
    C2DVector                 Size;                                      // +0x60
    CRGBColour                Colour;                                    // +0x68
    CRightHandedSet           RHSet;                                     // +0x6c
    EEnginePrimitiveBlendMode BlendMode;                                 // +0x84
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0x88
    bool                      Lit;                                       // +0x8c
    float                     StencilCubeHeight;                         // +0x90
};

class CEnginePrimitiveManagerDecal : public CEnginePrimitiveManager {
protected:
    void RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal);
    unsigned char _pad0c[0x54 - 0xc];                                    // +0x0c MeshDecalVertexBuffer, MeshDecalIndexBuffer
    unsigned long PointLightShaderIndex[6];                              // +0x54
    CVertexShader LandscapeDecalVertexShader[3];                         // +0x6c
    CVertexShader MeshDecalVertexShader[3];                              // +0x84
    CVertexShader MeshDecalTwoPassStencilVertexShader;                   // +0x9c
    CVertexShader MeshDecalOnePassStencilVertexShader;                   // +0xa4
    CVertexShader MeshDecalStencilClearVertexShader;                     // +0xac
    CPixelShader  LandscapeDecalPixelShader;                             // +0xb4
};

void CEnginePrimitiveManagerDecal::RenderLandscapeDecal(CEngineInternalPrimitiveDecal& decal)
{
    if (!NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->EntryExists(decal.DecalIndex))
        return;
    const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(decal.DecalIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, decal.DecalFrame, 0, 0))
        return;

    CRenderStateScopedCaptureBlock capture;
    CScopedShadersEnable shaders(true, true);
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
    SetupBlendOpState(decal.BlendOp);
    bool premultiply = SetupAlphaBlendingStatesFromBlendMode(decal.BlendMode);
    if (premultiply) {
        CRGBFloatColour colour(decal.Colour);
        colour.R *= colour.A;
        colour.G *= colour.A;
        colour.B *= colour.A;
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour));
    } else {
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(CRGBFloatColour(decal.Colour)));
    }

    long min_x = GFFloatToLongFloor(decal.WorldPos.X - decal.Size.X * 0.5f);
    long min_y = GFFloatToLongFloor(decal.WorldPos.Y - decal.Size.Y * 0.5f);
    long max_x = GFFloatToLongCeil(decal.WorldPos.X + decal.Size.X * 0.5f);
    long max_y = GFFloatToLongCeil(decal.WorldPos.Y + decal.Size.Y * 0.5f);
    long width = max_x - min_x;
    long height = max_y - min_y;
    long vertex_count = (height + 1) * (width + 1);
    CArray<CTVertexNormTex1> vertices(vertex_count);
    long vertex = 0;
    for (long y = min_y; y <= max_y; ++y) {
        for (long x = min_x; x <= max_x; ++x) {
            vertices[vertex].X = (float)x;
            vertices[vertex].Y = (float)y;
            vertices[vertex].Z = decal.WorldPos.Z;
            vertices[vertex].NormalX = decal.RHSet.GetUp().X;
            vertices[vertex].NormalY = decal.RHSet.GetUp().Y;
            vertices[vertex].NormalZ = decal.RHSet.GetUp().Z;
            vertices[vertex].U1 = (vertices[vertex].X - decal.WorldPos.X) / decal.Size.X + 0.5f;
            vertices[vertex].V1 = (vertices[vertex].Y - decal.WorldPos.Y) / decal.Size.Y + 0.5f;
            ++vertex;
        }
    }
    FABLE_RETAIL_ASSERT(vertex == vertex_count);
    long triangle_count = height * width * 2;
    CArray<unsigned short> indices(triangle_count * 3);
    long index = 0;
    long corner = 0;
    for (long triangle = 0; triangle < triangle_count; triangle += 2) {
        if ((long)(vertices[corner].Y + vertices[corner].X) & 1) {
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(width + corner + 2);
        } else {
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 2);
            indices[index++] = (unsigned short)(corner + 1);
            indices[index++] = (unsigned short)corner;
            indices[index++] = (unsigned short)(width + corner + 1);
            indices[index++] = (unsigned short)(width + corner + 2);
        }
        ++corner;
        if ((corner + 1) % (width + 1) == 0)
            ++corner;
    }

    FABLE_RETAIL_ASSERT(index == triangle_count * 3);
    long light_count;
    if (decal.Lit) {
        CEnginePrimitiveThemeChannelDesc channels;
        channels = CEnginePrimitiveThemeChannelDesc();
        CEngineLocalLightArray* lights = g_pEngineLightingManager->GetLightsInSphereFast(
            C3DBoundingSphere(decal.WorldPos, decal.Size.GetMagnitude()), 5, channels);
        FABLE_RETAIL_ASSERT(lights);

        light_count = lights->LightCount;
        g_pEngineLightingManager->SetupShaderPointLights(*lights);
    } else {
        light_count = 0;
        g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
    }
    g_pEngineLightingManager->UpdateGlobalLightingStates();
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_BORDER);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_BORDER);
    NEngineInternal::GPRenderManager->SetTextureBorderColour(0, 0);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferEnable(true);
    NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaRef(1);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(LandscapeDecalPixelShader);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(LandscapeDecalVertexShader[PointLightShaderIndex[light_count]], false);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(9);
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(vertices.GetAsCArray(), sizeof(CTVertexNormTex1), vertex_count,
                                                        indices.GetAsCArray(), triangle_count);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}