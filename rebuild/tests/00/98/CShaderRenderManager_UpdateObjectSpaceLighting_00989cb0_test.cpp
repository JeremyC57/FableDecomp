#include "fable_compiler.h"
#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstring>
#include <vector>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class C3DVector {public:float X,Y,Z;C3DVector(){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}};
class C4DVector {public:float X,Y,Z,W;C4DVector(){} C4DVector(float x,float y,float z,float w):X(x),Y(y),Z(z),W(w){}
 C4DVector(const C3DVector&v,float w):X(v.X),Y(v.Y),Z(v.Z),W(w){}
 void Initialise(float x,float y,float z,float w){X=x;Y=y;Z=z;W=w;}
 const C4DVector operator*(float sc)const{return C4DVector(X*sc,Y*sc,Z*sc,W*sc);}
};
class CMatrix4x4 {public:float E11,E12,E13,E14,E21,E22,E23,E24,E31,E32,E33,E34,E41,E42,E43,E44;
 float SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4&);
    FABLE_FORCEINLINE void TransformTransposedAssumeW(const C4DVector& src,C4DVector& dest,float w) const {
        C4DVector vect=src;
        dest.X=E11*vect.X+E12*vect.Y+E13*vect.Z+E14*w;
        dest.Y=E21*vect.X+E22*vect.Y+E23*vect.Z+E24*w;
        dest.Z=E31*vect.X+E32*vect.Y+E33*vect.Z+E34*w;
        dest.W=w;
    }
};
float CMatrix4x4::SetInverseOfUniformScaleRotationTranslation(const CMatrix4x4& matrix)
{
    float mult=1.0f/(matrix.E11*matrix.E11+(matrix.E21*matrix.E21+matrix.E31*matrix.E31));
    E11=mult*matrix.E11;E12=mult*matrix.E21;E13=mult*matrix.E31;
    E21=mult*matrix.E12;E22=mult*matrix.E22;E23=mult*matrix.E32;
    E31=mult*matrix.E13;E32=mult*matrix.E23;E33=mult*matrix.E33;
    E14=-(matrix.E34*E13+matrix.E14*E11+matrix.E24*E12);
    E24=-(matrix.E24*E22+matrix.E34*E23+E21*matrix.E14);
    E34=-(matrix.E24*E32+matrix.E34*E33+matrix.E14*E31);
    E41=0.0f;E42=0.0f;E43=0.0f;E44=1.0f;
    return mult;
}

class CShaderGenericLight {public:enum EParameterType{PARAMTYPE_COLOUR,PARAMTYPE_DIRECTION,PARAMTYPE_POSITION,PARAMTYPE_ATTENUATION}; C4DVector Parameters[2];EParameterType Types[2];float Attenuation2,AttenuationBase;unsigned long Size;};
class CShaderConstantRange { public:long Offset;unsigned long Count;};
class CVertexShaderConstantLayout { public:unsigned long LightSize;CShaderConstantRange LightArray,LightAttenuations,CameraPos;};
struct Upload {long first;std::vector<C4DVector> data;};
struct IDirect3DDevice9 {
 std::vector<Upload> uploads;
 long SetVertexShaderConstantF(unsigned int first,const float*data,unsigned int count){
  Upload u;u.first=first;u.data.resize(count);if(count)std::memcpy(&u.data[0],data,count*sizeof(C4DVector));uploads.push_back(u);return 0;
 }
};
namespace NVectorMath {
void GFNormalise3DVectors(const C4DVector*in,unsigned long si,C4DVector*out,unsigned long so,unsigned long count){
 for(unsigned long i=0;i<count;++i){float n=(float)std::sqrt(in->X*in->X+in->Y*in->Y+in->Z*in->Z);out->X=in->X/n;out->Y=in->Y/n;out->Z=in->Z/n;in=(const C4DVector*)((const char*)in+si);out=(C4DVector*)((char*)out+so);}
}
}
class CShaderRenderManager {public:
 class CLightInfo{ public:CShaderGenericLight Light;bool IsEnabled,UpdateFlag;};
 CMatrix4x4 WorldMatrix;C3DVector CameraPos;const CVertexShaderConstantLayout*CurrentVSConstantLayout;CLightInfo*Lights;C4DVector*LightBuffer;unsigned long LightCount;std::vector<C4DVector>LightAttenuations;IDirect3DDevice9*D3DDevice;unsigned long UpdateFlags;
 void SetVSConstants(long first,const void*data,unsigned long count){if(count)D3DDevice->SetVertexShaderConstantF(first,(const float*)data,count);}
 void UpdateObjectSpaceLighting(unsigned long flags);
};
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

static bool Equal(const C4DVector&a,const C4DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z&&a.W==b.W;}
static C4DVector ObjectCoordinates(C4DVector v,float scale,int turn,bool point){
 if(point){v.X-=2.0f;v.Y+=1.0f;v.Z-=0.5f;}
 float x=v.X,y=v.Y;
 if(turn){v.X=y/scale;v.Y=-x/scale;}else{v.X=x/scale;v.Y=y/scale;}
 v.Z/=scale;v.W=point?1.0f:0.0f;return v;
}
static void Add(std::vector<Upload>&out,long first,const C4DVector*data,unsigned long count){if(count){Upload u;u.first=first;u.data.assign(data,data+count);out.push_back(u);}}
int main(){unsigned long cases=0;
 for(unsigned long count=0;count<=8;++count)for(unsigned int pattern=0;pattern<32;++pattern)for(int turn=0;turn<2;++turn)for(int scale_case=0;scale_case<3;++scale_case)for(unsigned long flags=0;flags<4;++flags){
  float scale=scale_case==0?0.5f:(scale_case==1?1.0f:2.0f);
  CShaderRenderManager m;IDirect3DDevice9 dev;CVertexShaderConstantLayout layout;
  layout.LightSize=(pattern%3);layout.LightArray.Offset=20;layout.LightArray.Count=16;layout.LightAttenuations.Offset=80;layout.LightAttenuations.Count=4;layout.CameraPos.Offset=100;layout.CameraPos.Count=(pattern&2)?1:0;
  m.CurrentVSConstantLayout=&layout;m.D3DDevice=&dev;m.LightCount=count;m.UpdateFlags=(pattern*17)|0x500;
  CShaderRenderManager::CLightInfo lights[8];C4DVector buffer[16],expected[16];m.Lights=lights;m.LightBuffer=buffer;
  m.CameraPos=C3DVector(4.0f,1.0f,2.5f);std::memset(&m.WorldMatrix,0,sizeof(m.WorldMatrix));
  if(turn){m.WorldMatrix.E12=-scale;m.WorldMatrix.E21=scale;}else{m.WorldMatrix.E11=scale;m.WorldMatrix.E22=scale;}
  m.WorldMatrix.E33=scale;m.WorldMatrix.E44=1.0f;m.WorldMatrix.E14=2.0f;m.WorldMatrix.E24=-1.0f;m.WorldMatrix.E34=0.5f;
  CMatrix4x4 saved_matrix=m.WorldMatrix;
  for(int i=0;i<16;++i)buffer[i]=expected[i]=C4DVector(-8.0f,-4.0f,-2.0f,-1.0f);
  bool active[8],old_update[8],old_enabled[8];
  for(unsigned long i=0;i<8;++i){
   lights[i].UpdateFlag=((pattern+i*3)&4)!=0;lights[i].IsEnabled=((pattern+i*5)&8)!=0;
   old_update[i]=lights[i].UpdateFlag;old_enabled[i]=lights[i].IsEnabled;active[i]=old_update[i]||old_enabled[i];
   lights[i].Light.Size=layout.LightSize==0?0:((i+pattern)%layout.LightSize)+1;
   lights[i].Light.Attenuation2=8.0f;lights[i].Light.AttenuationBase=4.0f;
   for(unsigned long p=0;p<2;++p){unsigned long type=(pattern+i+p)%4;lights[i].Light.Types[p]=(CShaderGenericLight::EParameterType)type;lights[i].Light.Parameters[p]=(type==1)?C4DVector(2.0f,0.0f,0.0f,7.0f):C4DVector(1.0f,2.0f,4.0f,8.0f);}
  }
  for(int i=0;i<4;++i)m.LightAttenuations.push_back(C4DVector(float(i+1),2.0f,4.0f,8.0f));
  std::vector<Upload> calls;unsigned long last_used=0;
  for(unsigned long i=0;i<count;++i){if(!active[i])continue;last_used=i+1;
   if(!old_enabled[i]){for(unsigned long p=0;p<layout.LightSize;++p)expected[i*layout.LightSize+p]=C4DVector(0.0f,0.0f,0.0f,1.0f);}
   else for(unsigned long p=0;p<lights[i].Light.Size;++p){C4DVector v=lights[i].Light.Parameters[p];switch(lights[i].Light.Types[p]){
    case CShaderGenericLight::PARAMTYPE_DIRECTION:v=ObjectCoordinates(v,scale,turn,false);{float n=(float)std::sqrt(v.X*v.X+v.Y*v.Y+v.Z*v.Z);v.X/=n;v.Y/=n;v.Z/=n;}break;
    case CShaderGenericLight::PARAMTYPE_POSITION:v=ObjectCoordinates(v,scale,turn,true);break;
    case CShaderGenericLight::PARAMTYPE_ATTENUATION:v.Y*=scale*scale;v.Z*=scale;break;
    default:break;}
    expected[i*layout.LightSize+p]=v;
   }
  }
  // Expected uploads are maximal contiguous active runs, independent of the implementation's update_begin bookkeeping.
  for(unsigned long i=0;i<count;){if(!active[i]){++i;continue;}unsigned long begin=i;while(i<count&&active[i])++i;Add(calls,20+begin*layout.LightSize,expected+begin*layout.LightSize,(i-begin)*layout.LightSize);}
  if(last_used>1){unsigned long n=((last_used+2)/4)*2;C4DVector v[4];for(unsigned long i=0;i<n;++i){v[i]=m.LightAttenuations[i];if((i&1)==0){v[i].X/=scale*scale;v[i].Y/=scale*scale;v[i].Z/=scale*scale;v[i].W/=scale*scale;}}Add(calls,80,v,n);}
  unsigned long expected_flags=(m.UpdateFlags&~0x100UL)|0x200UL;
  if((flags&2)&&layout.CameraPos.Count){C4DVector camera=ObjectCoordinates(C4DVector(m.CameraPos,0),scale,turn,true);camera.W=scale;Add(calls,100,&camera,1);expected_flags|=8;}
  m.UpdateObjectSpaceLighting(flags);
  assert(m.UpdateFlags==expected_flags&&m.LightCount==count&&m.CurrentVSConstantLayout==&layout&&m.D3DDevice==&dev);
  assert(std::memcmp(&saved_matrix,&m.WorldMatrix,sizeof(saved_matrix))==0);
  assert(dev.uploads.size()==calls.size());for(unsigned long i=0;i<calls.size();++i){assert(dev.uploads[i].first==calls[i].first&&dev.uploads[i].data.size()==calls[i].data.size());for(unsigned long p=0;p<calls[i].data.size();++p)assert(Equal(dev.uploads[i].data[p],calls[i].data[p]));}
  for(int i=0;i<16;++i)assert(Equal(buffer[i],expected[i]));
  for(unsigned long i=0;i<8;++i){assert(lights[i].IsEnabled==old_enabled[i]);assert(lights[i].UpdateFlag==(i<count&&active[i]?false:old_update[i]));assert(lights[i].Light.Attenuation2==8&&lights[i].Light.AttenuationBase==4);}
  ++cases;
 }
 assert(cases==6912);std::puts("00989cb0_TEST PASS");
}