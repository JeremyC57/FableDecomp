// CEngineClothRenderer::Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth, const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh, CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix, ERenderPassType pass, bool flag) -- render a 2D (flat) cloth primitive: pass-dependent alpha / z render state, the cloth vertex and pixel shaders, the material colour constant, then the polygons
// Grade: DIFFER(2456v3538, md=641) -- skeleton: the pass-dependent alpha / z state prefix and the RenderPolygons call only; the material colour constant (SetUserVertexShaderConstant 0x00989A60), shader selection, ApplyPixelShader, the GetDiffuseTexture / Get2DAlphaMaskTexture calls, the lighting update and the D3D device stage cleanup are missing
// Next idea: decode the shader selection (sub-mesh flags 0x200 / 0x10 choose +0x44 / +0x40 / +0x04 of the shader set) and the D3D cleanup loop (device +0x3CF0 slots 100 / 104 / 65, texture-stage count +0x3CB8), then model them on the landed CEngineScreenEffectColourFilter rows
// Leaf: ?Render2DClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                 // 0x01436E18

class CParticleSimulation;
class CParticleProgram;
class C3DPrimitive2;
class CEngineInternalPrimitiveMeshBase;
class CEngineLocalLightArray;
enum ERenderPassType { RENDER_PASS_0, RENDER_PASS_1 };

class CEngineSubPrimitiveMeshBase {
public:
    void*           __vftable;                                                         // +0x00
    unsigned long   Flags;                                                             // +0x04
    unsigned char   Alpha;                                                             // +0x08
    unsigned char*  Material;                                                          // +0x0c
};

class C3DClothPrimitive {
public:
    void*             __vftable;                                                       // +0x00
    CParticleProgram* Program;                                                         // +0x04
};

class CEngineClothRenderer {
public:
    void RenderPolygons(const CParticleSimulation& simulation, const CParticleProgram& program, const CMatrix3x4& matrix,
                        const CEngineLocalLightArray& lights, const CTexture** textures, unsigned long texture_count,
                        bool a, bool b, unsigned long c, bool d, bool e, unsigned long f, ERenderPassType pass,
                        CEngineInternalPrimitiveMeshBase& mesh);                       // 0x00BB2540
    void Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth,
                                const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh,
                                CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix, ERenderPassType pass,
                                bool flag);
};

extern const CEngineLocalLightArray* g_pClothLightArray;

void CEngineClothRenderer::Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth,
                                                  const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh,
                                                  CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix,
                                                  ERenderPassType pass, bool flag)
{
    CRenderStateScopedCaptureBlock rsblock;
    const CParticleProgram* program = cloth.Program;
    unsigned long flags = sub_mesh.Flags;
    unsigned char alpha = sub_mesh.Alpha;
    unsigned char alpha_ref = sub_mesh.Material[0x28];
    if (pass == RENDER_PASS_1) {
        NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        NEngineInternal::GPRenderManager->SetAlphaRef(alpha_ref);
        NEngineInternal::GPRenderManager->SetColourWriteEnable(false, false, false, false);
    } else {
        if (flags & 2) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaRef((alpha_ref * alpha) >> 8);
        } else {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaRef(0);
        }
        if (flags & 1) {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
            NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
        } else {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
        }
    }
    CScopedShadersEnable enabled(true, true);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
    NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    const CTexture* textures[2] = { 0, 0 };
    RenderPolygons(simulation, *program, matrix, *g_pClothLightArray, textures, 2, true, true, 9, true, true,
                   flag, pass, mesh);
}
