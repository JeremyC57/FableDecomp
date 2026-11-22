// The reconstructed source body is compiled below against deterministic graphics interfaces.
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
#include "engine/CEngineRenderEnums.h"
#include "engine/EEnginePrimitiveType.h"
#include "engine/EGraphicBankNo.h"
#include "engine/RenderStateEnums.h"

struct CTexture {};
struct CVertexShader { int id; };
struct CPixelShader { int id; };
struct CEngineInternalPrimitiveBase {};
struct CEngineInternalPrimitiveSpriteTrail : CEngineInternalPrimitiveBase {
    unsigned long GraphicIndex, GraphicFrame;
    unsigned short Flags;
    EEnginePrimitiveBlendOp BlendOp;
    EEnginePrimitiveBlendMode BlendMode;
};
struct CEngineSubPrimitiveBase {
    CEngineInternalPrimitiveBase* parent;
    CEngineInternalPrimitiveBase* GetParentBase() { return parent; }
};

struct Trace {
    bool frameOK, displacement, premultiply, captured, textureBound;
    unsigned long lightCount, shaderCount;
    unsigned int frameRequested, captureEnd, layout, layoutEnded, blendWrites, sourceWrites, destWrites;
    unsigned int lightSetups, lightingUpdate, lightingRestore, vertexShader, pixelShader, groupShader, submitted;
    unsigned int selfIllumination, cullNone, cullCCW, prepares;
    BLEND_OP op;
    ALPHA_BLEND_MODE source, dest;
    float uv[4];
    Trace() : frameOK(true), displacement(false), premultiply(false), captured(false), textureBound(false),
        lightCount(2), shaderCount(4), frameRequested(0), captureEnd(0), layout(0), layoutEnded(0),
        blendWrites(0), sourceWrites(0), destWrites(0), lightSetups(0), lightingUpdate(0), lightingRestore(0),
        vertexShader(0), pixelShader(0), groupShader(0), submitted(0), selfIllumination(0), cullNone(0), cullCCW(0), prepares(0),
        op(BLEND_OP_ADD), source(ALPHA_BLEND_ZERO), dest(ALPHA_BLEND_ZERO) {}
};
static Trace trace;
struct CGraphicFrame {
    CTexture texture;
    CTexture* GetTexture() const { return const_cast<CTexture*>(&texture); }
    float GetFloatTLX() const { return 8192.0f / 32767.0f; }
    float GetFloatTLY() const { return 16384.0f / 32767.0f; }
};
struct CGraphicInfo {
    bool GetFrame(CGraphicFrame&, unsigned long frame, unsigned long mipmap, unsigned long flags) const {
        trace.frameRequested = frame;
        assert(mipmap == 0 && flags == 0);
        return trace.frameOK;
    }
    float GetFrameWidthFactor() const { return 16.0f / 64.0f; }
    float GetFrameHeightFactor() const { return 12.0f / 32.0f; }
};
struct CGraphicDataBank {
    CGraphicInfo graphic;
    const CGraphicInfo* PeekGraphic(unsigned long index) const {
        assert(index == 5);
        return &graphic;
    }
};
struct CEngineResourceManager {
    CGraphicDataBank bank;
    const CGraphicDataBank* PeekGraphicBank(EGraphicBankNo bank_no) const {
        assert(bank_no == MAIN_GRAPHIC_BANK_NO);
        return &bank;
    }
};
struct CRenderStateScopedCaptureBlock {
    CRenderStateScopedCaptureBlock() { assert(!trace.captured); trace.captured = true; }
    ~CRenderStateScopedCaptureBlock() { assert(trace.layoutEnded == 1); trace.captured = false; ++trace.captureEnd; }
};
struct CScopedVertexShaderConstantLayout {
    explicit CScopedVertexShaderConstantLayout(unsigned layout) { assert(trace.captured); trace.layout = layout; }
    ~CScopedVertexShaderConstantLayout() { ++trace.layoutEnded; }
};
struct CEnginePrimitiveManager {};
struct CEnginePrimitiveManagerParticleGroup : CEnginePrimitiveManager {
    long SetUpPointLights() { ++trace.lightSetups; return trace.lightCount; }
    void SetupPixelShader(unsigned long index, const CPixelShader& stipple, const CPixelShader& normal) {
        assert(index == 0 && stipple.id == 2 && normal.id == 1);
        ++trace.groupShader;
    }
};
struct CEnginePrimitiveTypeInfo {
    CEnginePrimitiveManager* PrimitiveManagers[47];
    CEnginePrimitiveManager* GetPrimitiveManager(long index) const {
        assert(index == ENGINE_PRIMITIVE_PARTICLE_GROUP);
        return PrimitiveManagers[index];
    }
};
struct CEngineScreenEffectDisplacementRenderer {
    bool PeekRenderEnabled() const { return trace.displacement; }
};
struct CEngineLightingManager {
    void SetSelfIlluminationAmbient(unsigned long percentage) { trace.selfIllumination = percentage; }
    void UpdateGlobalLightingStates() { ++trace.lightingUpdate; }
    void RestoreDefaultPrimitiveParameters() { assert(trace.cullCCW == 1); ++trace.lightingRestore; }
};
struct CRenderManager {
    void SetBlendOp(BLEND_OP op) { assert(trace.captured); ++trace.blendWrites; trace.op = op; }
    void SetSourceAlphaBlendMode(ALPHA_BLEND_MODE mode) { ++trace.sourceWrites; trace.source = mode; }
    void SetDestAlphaBlendMode(ALPHA_BLEND_MODE mode) { ++trace.destWrites; trace.dest = mode; }
    void AttachTextureToStage(long index, CTexture* texture) { assert(index == 0 && texture); trace.textureBound = true; }
    void SetPolyCullMode(POLY_CULL_MODE mode) {
        if (mode == POLY_CULL_NONE) ++trace.cullNone;
        else { assert(mode == POLY_CULL_CCW && trace.submitted == 1); ++trace.cullCCW; }
    }
};
struct CShaderRenderManager {
    void ApplyVertexShader(const CVertexShader& shader, bool enable) { assert(!enable); trace.vertexShader = shader.id; }
    void ApplyPixelShader(const CPixelShader& shader) { trace.pixelShader = shader.id; }
    void SetUserVertexShaderConstant(unsigned long index,float a,float b,float c,float d) {
        assert(index == 1 && trace.textureBound && trace.cullNone == 1);
        trace.uv[0] = a; trace.uv[1] = b; trace.uv[2] = c; trace.uv[3] = d;
    }
    void PrepareForRender(unsigned long mask) { assert(mask == 0x11); ++trace.prepares; }
};
struct CEnginePrimitiveManagerSpriteTrails {
    void RenderSpriteTrail(const CEngineInternalPrimitiveSpriteTrail& primitive,bool premultiply) {
        assert(primitive.GraphicIndex == 5 && trace.prepares == 1);
        trace.premultiply = premultiply;
        ++trace.submitted;
    }
};
namespace NEngineInternal {
    static CEnginePrimitiveTypeInfo primitiveTypes;
    static CEngineLightingManager lighting;
    static CEngineScreenEffectDisplacementRenderer displacement;
    static CRenderManager render;
    static CShaderRenderManager shaders;
    static CEngineResourceManager resources;
    CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo = &primitiveTypes;
    CEngineLightingManager* GPLightingManager = &lighting;
    CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer = &displacement;
    CRenderManager* GPRenderManager = &render;
    CShaderRenderManager* GPShaderRenderManager = &shaders;
    CEngineResourceManager* GPResourceManager = &resources;
}
struct CEnginePrimitiveRenderUtil {
    static void __fastcall ApplyVertexShader(const CVertexShader&);
    static void __fastcall ApplyPixelShader(const CPixelShader&);
    void SetupBlendOpState(EEnginePrimitiveBlendOp);
    bool SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode);
};
struct CEnginePrimitiveManagerParticleSpriteTrailGroup : CEnginePrimitiveRenderUtil {
    std::vector<CVertexShader> VertexShadersSpriteTrailPointLights;
    CVertexShader VertexShaderDisplacement;
    CPixelShader PixelShader, PixelShaderStipple, PixelShaderDisplacement;
    CEnginePrimitiveManagerSpriteTrails* PTrailManager;
    void ApplySpriteTrailPointLightVertexShader(unsigned long);
    void ApplyDisplacementVertexShader();
    void RenderSingleSpriteTrail(CEngineSubPrimitiveBase&,ERenderPassType);
};

inline void CEnginePrimitiveManagerParticleSpriteTrailGroup::ApplySpriteTrailPointLightVertexShader(unsigned long lights)
{
    if (lights > VertexShadersSpriteTrailPointLights.size() - 1)
        lights = VertexShadersSpriteTrailPointLights.size() - 1;
    ApplyVertexShader(VertexShadersSpriteTrailPointLights[lights]);
}
inline void CEnginePrimitiveManagerParticleSpriteTrailGroup::ApplyDisplacementVertexShader()
{
    ApplyVertexShader(VertexShaderDisplacement);
}
inline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
inline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
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
inline bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
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

void CEnginePrimitiveManagerParticleSpriteTrailGroup::RenderSingleSpriteTrail(CEngineSubPrimitiveBase& subprim, ERenderPassType render_pass)
{
    CEngineInternalPrimitiveSpriteTrail* primitive =
        static_cast<CEngineInternalPrimitiveSpriteTrail*>(subprim.GetParentBase());
    const CGraphicInfo* graphic_info = NEngineInternal::GPResourceManager->PeekGraphicBank(MAIN_GRAPHIC_BANK_NO)
        ->PeekGraphic(primitive->GraphicIndex);
    CGraphicFrame graphic;
    if (!graphic_info->GetFrame(graphic, primitive->GraphicFrame, 0, 0))
        return;
    CRenderStateScopedCaptureBlock state_capture;
    CScopedVertexShaderConstantLayout layout(2);
    CEnginePrimitiveManagerParticleGroup* particle_group_prim_manager = static_cast<CEnginePrimitiveManagerParticleGroup*>(
        NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_PARTICLE_GROUP));
    bool colour_factor_is_alpha;
    if (render_pass == RENDER_PASS_DISPLACEMENT_BUFFER && NEngineInternal::GPScreenEffectDisplacementRenderer->PeekRenderEnabled()) {
        colour_factor_is_alpha = false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        ApplyDisplacementVertexShader();
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
    } else if (render_pass == RENDER_PASS_DISPLACEMENT_STENCIL) {
        colour_factor_is_alpha = false;
        ApplyDisplacementVertexShader();
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(primitive->BlendOp);
        colour_factor_is_alpha = SetupAlphaBlendingStatesFromBlendMode(primitive->BlendMode);
        long point_light_count = 0;
        if (primitive->Flags & 0x80)
            point_light_count = particle_group_prim_manager->SetUpPointLights();
        else
            NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(100);
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
        ApplySpriteTrailPointLightVertexShader(point_light_count);
        particle_group_prim_manager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, graphic.GetTexture());
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    const float tex_offset_u = graphic.GetFloatTLX();
    const float tex_offset_v = graphic.GetFloatTLY();
    const float tex_mult_u = graphic_info->GetFrameWidthFactor();
    const float tex_mult_v = graphic_info->GetFrameHeightFactor();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, tex_offset_u, tex_offset_v, tex_mult_u, tex_mult_v);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
    PTrailManager->RenderSpriteTrail(*primitive, colour_factor_is_alpha);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
}
static void CheckSubmission()
{
    assert(trace.frameRequested == 3 && trace.layout == 2 && trace.layoutEnded == 1 && trace.captureEnd == 1);
    assert(!trace.captured && trace.textureBound && trace.prepares == 1 && trace.submitted == 1);
    assert(trace.cullNone == 1 && trace.cullCCW == 1 && trace.lightingRestore == 1);
    assert(trace.uv[0] == 8192.0f / 32767.0f && trace.uv[1] == 16384.0f / 32767.0f);
    assert(trace.uv[2] == 0.25f && trace.uv[3] == 0.375f);
}

int main()
{
    CEnginePrimitiveManagerParticleGroup particles;
    NEngineInternal::primitiveTypes.PrimitiveManagers[ENGINE_PRIMITIVE_PARTICLE_GROUP] = &particles;
    CEnginePrimitiveManagerSpriteTrails trails;
    CEnginePrimitiveManagerParticleSpriteTrailGroup group;
    group.PTrailManager = &trails;
    for (int n = 0; n < 4; ++n) { CVertexShader shader = {10+n}; group.VertexShadersSpriteTrailPointLights.push_back(shader); }
    group.VertexShaderDisplacement.id = 20;
    group.PixelShader.id = 1;
    group.PixelShaderStipple.id = 2;
    group.PixelShaderDisplacement.id = 3;
    CEngineInternalPrimitiveSpriteTrail primitive;
    primitive.GraphicIndex = 5;
    primitive.GraphicFrame = 3;
    primitive.Flags = 0x80;
    primitive.BlendOp = ENGINE_PRIMITIVE_BLEND_OP_REVSUB;
    primitive.BlendMode = ENGINE_PRIMITIVE_BLEND_ADDITIVE;
    CEngineSubPrimitiveBase sub = {&primitive};

    trace.frameOK = false;
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_COMBINED);
    assert(trace.frameRequested == 3 && trace.captureEnd == 0 && trace.submitted == 0);

    trace = Trace();
    trace.displacement = true;
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_DISPLACEMENT_BUFFER);
    CheckSubmission();
    assert(!trace.premultiply && trace.vertexShader == 20 && trace.pixelShader == 3);
    assert(trace.blendWrites == 1 && trace.op == BLEND_OP_ADD && trace.source == ALPHA_BLEND_SRC_ALPHA);
    assert(trace.dest == ALPHA_BLEND_INV_SRC_ALPHA && trace.lightSetups == 0 && trace.groupShader == 0);

    trace = Trace();
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_DISPLACEMENT_STENCIL);
    CheckSubmission();
    assert(!trace.premultiply && trace.vertexShader == 20 && trace.pixelShader == 3);
    assert(trace.blendWrites == 0 && trace.sourceWrites == 0 && trace.destWrites == 0);

    trace = Trace();
    trace.lightCount = 8;
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_DISPLACEMENT_BUFFER);
    CheckSubmission();
    assert(trace.premultiply && trace.op == BLEND_OP_REVSUBTRACT && trace.source == ALPHA_BLEND_ONE && trace.dest == ALPHA_BLEND_ONE);
    assert(trace.vertexShader == 13 && trace.pixelShader == 0 && trace.lightSetups == 1 && trace.groupShader == 1 && trace.lightingUpdate == 1);

    trace = Trace();
    primitive.Flags = 0;
    primitive.BlendOp = ENGINE_PRIMITIVE_BLEND_OP_SUB;
    primitive.BlendMode = ENGINE_PRIMITIVE_BLEND_ADDSMOOTH;
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_COMBINED);
    CheckSubmission();
    assert(trace.premultiply && trace.op == BLEND_OP_SUBTRACT && trace.dest == ALPHA_BLEND_INV_SRC_COLOR);
    assert(trace.vertexShader == 10 && trace.lightSetups == 0 && trace.selfIllumination == 100);

    trace = Trace();
    primitive.Flags = 0x80;
    primitive.BlendOp = ENGINE_PRIMITIVE_BLEND_OP_ADD;
    primitive.BlendMode = ENGINE_PRIMITIVE_BLEND_ALPHA;
    group.RenderSingleSpriteTrail(sub, RENDER_PASS_POSTWATER_EFFECT);
    CheckSubmission();
    assert(!trace.premultiply && trace.op == BLEND_OP_ADD && trace.source == ALPHA_BLEND_SRC_ALPHA);
    assert(trace.dest == ALPHA_BLEND_INV_SRC_ALPHA && trace.vertexShader == 12);
    std::puts("00bbe090_TEST PASS");
}