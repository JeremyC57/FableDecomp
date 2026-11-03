// CShaderRenderManager::UpdateObjectSpaceLighting(unsigned long flags) -- transform and upload object-space lights and camera constants.
// Leaf: ?UpdateObjectSpaceLighting@CShaderRenderManager@@AAEXK@Z
// Grade: RELOCATION_MATCH(2188v2188) -- native bytes agree after relocation masking.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"
#pragma intrinsic(sqrt)
namespace NVectorMath {void GFNormalise3DVectors(const C4DVector*,unsigned long,C4DVector*,unsigned long,unsigned long);}
// The CShaderRenderManager.cpp body of ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z (out of line at 0x0098B930), visible in this
// TU so VC7.1 expands it here as retail does (0x00989CB0 calls IDirect3DDevice9::SetVertexShaderConstantF, vtable +0x178, in place).
inline void CShaderRenderManager::SetVSConstants(long start,const void* data,unsigned long count)
{
    if(count!=0) D3DDevice->SetVertexShaderConstantF(start,(const float*)data,count);
}
void CShaderRenderManager::UpdateObjectSpaceLighting(unsigned long flags)
{
    CMatrix4x4 inverse_world;
    inverse_world.SetInverseOfUniformScaleRotationTranslation(WorldMatrix);
    C3DVector unit(WorldMatrix.E11,WorldMatrix.E21,WorldMatrix.E31);
    float scale=(float)sqrt(unit.X*unit.X+(unit.Y*unit.Y+unit.Z*unit.Z));
    long update_begin=-1;
    long update_end=0;
    long update_lights=0;
    for(unsigned long c_light=0;c_light<LightCount;++c_light) {
        CLightInfo& light_info=Lights[c_light];
        if(light_info.UpdateFlag || light_info.IsEnabled) {
            light_info.UpdateFlag=false;
            unsigned long dest_pos=CurrentVSConstantLayout->LightSize*c_light;
            C4DVector* dest_light=&LightBuffer[dest_pos];
            if(update_begin==-1) update_begin=dest_pos;
            update_end=dest_pos+CurrentVSConstantLayout->LightSize;
            update_lights=c_light+1;
            if(light_info.IsEnabled) {
                const CShaderGenericLight& src_light=light_info.Light;
                unsigned long no_params=src_light.Size;
                for(unsigned long c_param=0;c_param<no_params;++c_param) {
                    const C4DVector& src_param=src_light.Parameters[c_param];
                    C4DVector& dest_param=dest_light[c_param];
                    switch(src_light.Types[c_param]) {
                    case CShaderGenericLight::PARAMTYPE_COLOUR:
                        dest_param=src_param;break;
                    case CShaderGenericLight::PARAMTYPE_DIRECTION:
                        inverse_world.TransformTransposedAssumeW(src_param,dest_param,0.0f);
                        NVectorMath::GFNormalise3DVectors(&dest_param,0,&dest_param,0,1);break;
                    case CShaderGenericLight::PARAMTYPE_POSITION:
                        inverse_world.TransformTransposedAssumeW(src_param,dest_param,1.0f);break;
                    case CShaderGenericLight::PARAMTYPE_ATTENUATION: {
                        float attbase=src_param.W;
                        float att2=scale*src_param.Z;
                        float att1=scale*src_param.Y;
                        att1*=scale;
                        float att0=src_param.X;
                        dest_param.Initialise(att0,att1,att2,attbase);break;
                    }
                    }
                }
            } else {
                for(unsigned long c_param=0;c_param<CurrentVSConstantLayout->LightSize;++c_param)
                    dest_light[c_param]=C4DVector(0.0f,0.0f,0.0f,1.0f);
            }
        } else if(update_begin!=-1) {
            unsigned long dest_reg=CurrentVSConstantLayout->LightArray.Offset+update_begin;
            unsigned long reg_count=update_end-update_begin;
            SetVSConstants(dest_reg,&LightBuffer[update_begin],reg_count);
            update_begin=-1;
        }
    }
    if(update_begin!=-1) {
        unsigned long dest_reg=CurrentVSConstantLayout->LightArray.Offset+update_begin;
        unsigned long reg_count=update_end-update_begin;
        SetVSConstants(dest_reg,&LightBuffer[update_begin],reg_count);
    }
    if(update_lights>1) {
        unsigned long dest_reg=CurrentVSConstantLayout->LightAttenuations.Offset;
        unsigned long attenuation_count=(update_lights+2)/4;
        unsigned long reg_count=attenuation_count*2;
        C4DVector localised_attenuation[4];
        float att2_scale=1.0f/(scale*scale);
        for(unsigned long c_att=0;c_att<attenuation_count;++c_att) {
            localised_attenuation[c_att*2]=LightAttenuations[c_att*2]*att2_scale;
            localised_attenuation[c_att*2+1]=LightAttenuations[c_att*2+1];
        }
        SetVSConstants(dest_reg,localised_attenuation,reg_count);
    }
    if((flags&2)!=0 && CurrentVSConstantLayout->CameraPos.Count!=0) {
        C4DVector object_space_pos;
        inverse_world.TransformTransposedAssumeW(C4DVector(CameraPos,0.0f),object_space_pos,1.0f);
        object_space_pos.W=scale;
        SetVSConstants(CurrentVSConstantLayout->CameraPos.Offset,&object_space_pos,1);
        UpdateFlags|=8;
    }
    UpdateFlags|=0x200;
    UpdateFlags&=~0x100;
}