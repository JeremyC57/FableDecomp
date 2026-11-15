// 00BC3F3D assigned key -- CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive, whole entry00BC3F30.
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleLineGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: /d2inlT512
// Inline-budget: /d2inlT512 -- SetupBlendOpState stays called below T512 (T64 2449, T128/T256 2642 bytes); T512 expands it; SetupAlphaBlendingStatesFromBlendMode stays called at every threshold up to T4096, so its header body is FABLE_FORCEINLINE (see the note at its definition; INLINE_BUDGET_PROBES.json).
// Retail: Fable.exe TLC00BC3F30..00BC4C4D,3357codebytes,virtualslot5,thiscall ret8. Assigned3344 bytes omit13prologue bytes.
// Render textured line groups through shader constants in batches of ten lines.
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_RENDERUTIL_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_RGBFLOAT_NATIVE_BYTE_DIVISION
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_TEXTURE_BINDING
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_PASS22_PARTICLE_LINE_RENDER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFO_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_FRAMEFACTORS
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_NATIVE_FLOAT_TL_DIVISION_PASS22
#define FABLE_RETAIL_CGRAPHICFRAME_FLOATTL
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINECOMPONENT
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/_quarantine/CEngineInternalPrimitiveParticleLineGroup.h"
#include "engine/_quarantine/CEngineInternalPrimitiveParticleLine.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicFrame.h"
#include "engine/CGraphicInfo.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
namespace NEngineInternal {
extern CEngineResourceManager* GPResourceManager;
extern CRenderManager* GPRenderManager;
extern CShaderRenderManager* GPShaderRenderManager;
extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo;
extern CEngineCamera* GPCamera;
extern CEngineLightingManager* GPLightingManager;
extern CEngineScreenEffectDisplacementRenderer* GPScreenEffectDisplacementRenderer;
}
using namespace NEngineInternal;
// Original TU-local S_LDATA32 int enable, Ego_r module934/segment3+22A3C; native0139B864.
static int enable = 1;
inline void CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& vshader)
{
    GPShaderRenderManager->ApplyVertexShader(vshader,false);
}
inline void CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& pshader)
{
    GPShaderRenderManager->ApplyPixelShader(pshader);
}
inline CEngineCamera& CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *GPCamera; }
inline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp blend_op)
{
    switch(blend_op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUB: GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);break;
    case ENGINE_PRIMITIVE_BLEND_OP_REVSUB:GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);break;
    default:GPRenderManager->SetBlendOp(BLEND_OP_ADD);break;
    }
}
// Inline-budget: genuine helper remains called after CPP variants, T64..4096 and scoped MS depth255; retail expands all states.
FABLE_FORCEINLINE bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode blend_mode)
{
    switch(blend_mode) {
    case ENGINE_PRIMITIVE_BLEND_ADDITIVE:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_ADDSMOOTH:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}
inline void CEnginePrimitiveManagerParticleLineGroup::ApplyLinePointLightVertexShader(unsigned long point_lights)
{
    if(point_lights>VertexShadersLinePointLights.size()-1)point_lights=VertexShadersLinePointLights.size()-1;
    CVertexShader& vshader=VertexShadersLinePointLights[point_lights];
    ApplyVertexShader(vshader);
}
inline void CEnginePrimitiveManagerParticleLineGroup::ApplyDisplacementVertexShader()
{
    ApplyVertexShader(VertexShaderDisplacement);
}
void CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& subprim,ERenderPassType render_pass)
{
    if(!enable)return;
    CEngineInternalPrimitiveParticleLineGroup& primitive=static_cast<CEngineInternalPrimitiveParticleLineGroup&>(*subprim.GetParentBase());
    const CGraphicInfo* graphic_info=GPResourceManager->PeekGraphicBank((EGraphicBankNo)0)->PeekGraphic(primitive.BankIndex);
    CGraphicFrame graphic;
    if(!graphic_info->GetFrame(graphic,0,0,0))return;
    GPRenderManager->AttachTextureToStage(0,graphic.GetTexture());
    GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    CEnginePrimitiveManagerParticleGroup* particle_group_prim_manager=static_cast<CEnginePrimitiveManagerParticleGroup*>(GPPrimitiveTypeInfo->GetPrimitiveManager(ENGINE_PRIMITIVE_PARTICLE_GROUP));
    bool colour_factor_is_alpha;
    if(render_pass==RENDER_PASS_DISPLACEMENT_BUFFER&&GPScreenEffectDisplacementRenderer->PeekRenderEnabled()) {
        colour_factor_is_alpha=false;
        SetupBlendOpState(ENGINE_PRIMITIVE_BLEND_OP_ADD);
        SetupAlphaBlendingStatesFromBlendMode(ENGINE_PRIMITIVE_BLEND_ALPHA);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else if(render_pass==RENDER_PASS_DISPLACEMENT_STENCIL) {
        colour_factor_is_alpha=false;
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(primitive.BlendOp);
        colour_factor_is_alpha=SetupAlphaBlendingStatesFromBlendMode(primitive.BlendMode);
        long point_light_count=0;
        if(primitive.Flags&0x80)point_light_count=particle_group_prim_manager->SetUpPointLights();
        else GPLightingManager->SetSelfIlluminationAmbient(100);
        GPLightingManager->UpdateGlobalLightingStates();
        ApplyLinePointLightVertexShader(point_light_count);
        particle_group_prim_manager->SetupPixelShader(0,PixelShaderStipple,PixelShader);
    }
    GPRenderManager->SelectVertexBuffer(0,&VertexBuffer);
    GPRenderManager->SelectIndexBuffer(&IndexBuffer,0);
    const CMatrix3x4& nh_view=GetCurrentCamera().PeekNonHomogenousViewTransform();
    CEngineParticleConstantBlock& constant_block0=ConstantBlock[0];
    constant_block0.X=nh_view.E11;constant_block0.Y=nh_view.E21;constant_block0.Z=nh_view.E31;constant_block0.W=nh_view.E41;
    CEngineParticleConstantBlock& constant_block1=ConstantBlock[1];
    constant_block1.X=nh_view.E12;constant_block1.Y=nh_view.E22;constant_block1.Z=nh_view.E32;constant_block1.W=nh_view.E42;
    CEngineParticleConstantBlock& constant_block2=ConstantBlock[2];
    constant_block2.X=nh_view.E13;constant_block2.Y=nh_view.E23;constant_block2.Z=nh_view.E33;constant_block2.W=nh_view.E43;
    const CMatrix3x4& inv_nh_view=GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    CEngineParticleConstantBlock& constant_block3=ConstantBlock[3];
    constant_block3.X=inv_nh_view.E11;constant_block3.Y=inv_nh_view.E21;constant_block3.Z=inv_nh_view.E31;constant_block3.W=inv_nh_view.E41;
    CEngineParticleConstantBlock& constant_block4=ConstantBlock[4];
    constant_block4.X=inv_nh_view.E12;constant_block4.Y=inv_nh_view.E22;constant_block4.Z=inv_nh_view.E32;constant_block4.W=inv_nh_view.E42;
    CEngineParticleConstantBlock& constant_block5=ConstantBlock[5];
    constant_block5.X=inv_nh_view.E13;constant_block5.Y=inv_nh_view.E23;constant_block5.Z=inv_nh_view.E33;constant_block5.W=inv_nh_view.E43;
    CEngineParticleConstantBlock& constant_block6=ConstantBlock[6];
    constant_block6.X=graphic.GetFloatTLX();constant_block6.Y=graphic.GetFloatTLY();
    constant_block6.Z=graphic_info->GetFrameWidthFactor();constant_block6.W=graphic_info->GetFrameHeightFactor();
    GPShaderRenderManager->SetUserVertexShaderConstants(1,7,&ConstantBlock[0].X);
    long current_user_constant_index=0;
    for(std::list<CEnginePrimitiveHandle>::iterator it=primitive.Primitives.begin();it!=primitive.Primitives.end();++it) {
        CEngineInternalPrimitiveParticleLine& child_prim=static_cast<CEngineInternalPrimitiveParticleLine&>(it->GetData());
        {
        CRGBFloatColour prim_colour(child_prim.Colour);
        prim_colour.A*=2.0f;
        if(colour_factor_is_alpha){float colour_factor=prim_colour.A;prim_colour.R*=colour_factor;prim_colour.G*=colour_factor;prim_colour.B*=colour_factor;}
        CEngineParticleConstantBlock& block0=ConstantBlock[current_user_constant_index++];
        C3DVector& start=child_prim.Start;
        block0.X=start.X;block0.Y=start.Y;block0.Z=start.Z;block0.W=child_prim.Width;
        CEngineParticleConstantBlock& block1=ConstantBlock[current_user_constant_index++];
        C3DVector& end=child_prim.End;
        block1.X=end.X;block1.Y=end.Y;block1.Z=end.Z;block1.W=1.0f;
        CEngineParticleConstantBlock& block2=ConstantBlock[current_user_constant_index++];
        block2.X=prim_colour.R;block2.Y=prim_colour.G;block2.Z=prim_colour.B;block2.W=prim_colour.A;
        if(current_user_constant_index==30){GPShaderRenderManager->SetUserVertexShaderConstants(8,30,&ConstantBlock[0].X);GPShaderRenderManager->PrepareForRender(0x11);GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,40,0,20);current_user_constant_index=0;}
        }
    }
    if(current_user_constant_index){long num_lines=current_user_constant_index/3UL;GPShaderRenderManager->SetUserVertexShaderConstants(8,num_lines*3,&ConstantBlock[0].X);GPShaderRenderManager->PrepareForRender(0x11);GPRenderManager->DrawCurrentBuffersAsIndexedTris(0,num_lines*4,0,num_lines*2);}
    GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    GPLightingManager->RestoreDefaultPrimitiveParameters();
}