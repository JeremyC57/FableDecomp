// CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine& line) -- draw one line as a camera-facing textured quad: clip it to the near plane in view space, offset the ends by half the width across the projected direction, set the blend state and colour constant, draw two triangles
// Spellings (17th author, wfE5): the screen projection through the PDB operators `C2DVector screen_start = C2DVector(start) / start.Z;` -- EVIDENCED
// (FableWin ??0C2DVector@@QAE@ABVC3DVector@@@Z = existing opt-in FABLE_RETAIL_C2DVECTOR_FROM3D, ??KC2DVector@@QBE?BV0@M@Z = the existing reciprocal operator/;
// retail `fld 1.0 ; fdiv [z]` then two fmul 0x00BC8D05); with the open-coded `start.X * inv_start` the forward Y rows (0x00BC8B1A / 0x00BC8BAC), the near
// compare (0x00BC8C03) and the corner adds (0x00BC8DE2 / 0x00BC8E12) take the other x87 operand order. The blend state through the 3-argument overload
// SetupAlphaBlendingStatesFromBlendMode(mode, alpha, colour_scale&) -- EVIDENCED (FableWin.pdb + FableWin.exe signatures ?SetupAlphaBlendingStatesFromBlendMode@CEnginePrimitiveRenderUtil@@
// QAEXW4EEnginePrimitiveBlendMode@@KAAK@Z, FableWin 0x02E97220; body = the matched twin 0x00BAE2D0 text, opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_2DSPRITES):
// the bool overload + `if (!..) brightness = 0xff; else brightness = alpha;` put the const-0 ebp piece on the textured compare (`cmp edi,ebp` v retail
// `test edi,edi` 0x00BC9315) and moved the dest-blend store 0x00BC92F4. The vertices written per vertex (`SetPosition` then `SetU1V1`) -- EVIDENCED by the
// retail store order (vertex 3's Position.Y store 0x00BC9493 after vertex 2's UVs).
// Leaf: ?RenderLine@CEnginePrimitiveManagerLines@@QAEXABVCEngineInternalPrimitiveLine@@@Z
// Retail: 0x00BC8A60, 3416 bytes (`ret 4`).
// Helpers (14th author): TransformWorldToNonHomogenousView / TransformNonHomogenousViewToWorld (FableWin COMDATs 0x02EC0730 / 0x02EC0700, EVIDENCED: /Od = FableWin);
// CEnginePrimitiveRenderUtil::SetupDiffuseShading (FableWin COMDAT 0x02EC0760; call EVIDENCED by placement; body (15th author) in the spelling of the matched
// twin Setup2DDiffuseShading (0x00BAE2D0: `float scale = colour_scale / 255.0f;`, `alpha / 255.0f`, FableWin static getters GetPixelShaderDiffuse /
// GetPixelShaderDiffuseOnly through the existing CLOTHSHADERS opt-in): byte-neutral; /Od 42i v FableWin 49i (float code: FableWin sizes unreliable);
// ApplyPixelShader / ApplyVertexShader through the RenderUtil statics (landed bodies of 0x00BBF4A0; ApplyPixelShader one EVIDENCED by bytes, 25 -> 20).
// Spellings (16th author): the texture lookup as `if (GetFrame(frame, 0, 0)) { texture = ..; texture_count = 1; } else return;` -- FITTED (chosen by
// grading; the matched siblings 0x00BCC800 / 0x00B9AC30 write `if (!GetFrame) return;` where nothing follows); it reproduces the retail tail (path 1 `push ebp ; push ebp ; call ; jmp` 0x00BC9733 kept apart from path 2 0x00BC976C; `if (!GetFrame) return;` cross-jumps them);
// FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX (the matched twin 0x00BAE2D0's opt-in; FableWin SetPixelShaderConstant 28i/79b exact): byte-neutral.
// Local class models: CEngineInternalPrimitiveLine and CEnginePrimitiveManagerLines have no shared header (the landed siblings
// RenderPrimitive 0x00BC9800 / CreateInternalPrimitive 0x00BC7E50 model them locally too). Ego_r.pdb layouts.
#include "fable_compiler.h"
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_2DSPRITES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPDIFFUSESHADING
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUSVIEW_TRANSFORMS
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CVERTEXBUFFERBASE_ISBUSY_SIZEOF
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_DYNAMICVB
#define FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DYNAMICVB
#define FABLE_RETAIL_CSHADERRENDERMANAGER_DRAWNONINDEXED_TEXTURES
#define FABLE_RETAIL_CENGINETREEFILER_INLINES
#define FABLE_RETAIL_CENGINETREEFILER_FINDNODE_INDEX
#define FABLE_RETAIL_CENGINEVERTEXBUFFERPOOL_INLINES
#include "engine/C3DVector.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CArray.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicInfo.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CEngineVertexBufferPool.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/C2DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCamera.h"
#include "engine/CPixelShader.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                      // 0x01436EA0

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic, unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}



FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode, unsigned long alpha,
                                                                                         unsigned long& colour_scale)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        colour_scale = alpha;
        break;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        colour_scale = alpha;
        break;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        colour_scale = 255;
        break;
    }
}

inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuse; }
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
FABLE_FORCEINLINE void CEnginePrimitiveRenderUtil::SetupDiffuseShading(bool textured, unsigned long colour_scale, unsigned long alpha)
{
    float scale = colour_scale / 255.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(scale, scale, scale, alpha / 255.0f));
    if (textured)
        ApplyPixelShader(GetPixelShaderDiffuse());
    else
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
}

class CEngineInternalPrimitiveLine : public CEngineInternalPrimitiveBase {   // Ego_r.pdb, 0x7c
public:
    C3DVector                 Start;                                     // +0x48
    C3DVector                 End;                                       // +0x54
    CRGBColour                Colour;                                    // +0x60
    float                     Width;                                     // +0x64
    unsigned long             BankIndex;                                 // +0x68
    unsigned long             Frame;                                     // +0x6c
    EEnginePrimitiveBlendMode BlendMode;                                 // +0x70
    EEnginePrimitiveBlendOp   BlendOp;                                   // +0x74
    bool                      UseZBuffer;                                // +0x78
};

class CEnginePrimitiveManagerLines : public CEnginePrimitiveManager {
public:
    CVertexShader VertexShader;                                          // +0x0c
    void RenderLine(const CEngineInternalPrimitiveLine& line);
};

void CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine& line)
{
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    CScopedShadersEnable shaders(true, true);
    C3DVector start = line.Start;
    C3DVector end = line.End;
    NEngineInternal::GPCamera->TransformWorldToNonHomogenousView(start);
    NEngineInternal::GPCamera->TransformWorldToNonHomogenousView(end);
    float near_plane = NEngineInternal::GPCamera->PeekCameraState().NearZPlane;
    if (start.Z < near_plane) {
        if (end.Z < near_plane)
            return;
        start += (end - start) * ((near_plane - start.Z) / (end.Z - start.Z));
    } else if (end.Z < near_plane) {
        end += (start - end) * ((near_plane - end.Z) / (start.Z - end.Z));
    }
    C2DVector screen_start = C2DVector(start) / start.Z;
    C2DVector screen_end = C2DVector(end) / end.Z;
    C2DVector dir(screen_end.X - screen_start.X, screen_end.Y - screen_start.Y);
    if (!dir.IsNonZero())
        return;
    dir.Normalise();
    dir *= line.Width * 0.5f;
    C2DVector perp(dir.Y, -dir.X);
    C3DVector offset(perp.X, perp.Y, 0.0f);
    C3DVector corner0 = start - offset;
    C3DVector corner1 = start + offset;
    C3DVector corner2 = end - offset;
    C3DVector corner3 = end + offset;
    NEngineInternal::GPCamera->TransformNonHomogenousViewToWorld(corner0);
    NEngineInternal::GPCamera->TransformNonHomogenousViewToWorld(corner1);
    NEngineInternal::GPCamera->TransformNonHomogenousViewToWorld(corner2);
    NEngineInternal::GPCamera->TransformNonHomogenousViewToWorld(corner3);

    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    unsigned long alpha = line.Colour.A;
    unsigned long colour_scale;
    SetupAlphaBlendingStatesFromBlendMode(line.BlendMode, alpha, colour_scale);
    SetupDiffuseShading(line.BankIndex != 0, colour_scale, line.Colour.A);
    CTVertexColTex1 vertices[4];
    for (long i = 0; i < 4; ++i)
        vertices[i].SetDiffuseColour(line.Colour.GetAsInt());
    vertices[0].SetPosition(corner0);
    vertices[0].SetU1V1(0.0f, 0.0f);
    vertices[1].SetPosition(corner1);
    vertices[1].SetU1V1(1.0f, 0.0f);
    vertices[2].SetPosition(corner2);
    vertices[2].SetU1V1(0.0f, 1.0f);
    vertices[3].SetPosition(corner3);
    vertices[3].SetU1V1(1.0f, 1.0f);
    ApplyVertexShader(VertexShader);
    NEngineInternal::GPShaderRenderManager->ResetWorldTransform();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    const CTexture* texture;
    unsigned long texture_count = 0;
    if (line.BankIndex != 0) {
        CGraphicFrame frame;
        if (NEngineInternal::GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphicInfoFrame(line.BankIndex, line.Frame)->GetFrame(frame, 0, 0)) {
            texture = frame.GetTexture();
            texture_count = 1;
        } else
            return;
    }
    static unsigned short indices[6] = { 0, 1, 3, 0, 3, 2 };
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(&texture, texture_count, vertices, sizeof(CTVertexColTex1), 4, indices, 2);
}
