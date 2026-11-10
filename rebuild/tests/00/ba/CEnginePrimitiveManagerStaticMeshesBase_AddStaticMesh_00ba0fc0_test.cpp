#include <cstdio>
#include <cstdlib>
static void check(bool ok){if(!ok)std::abort();}
static int constructed=0,deleted=0,notifications=0;
class CEnginePrimitiveHandle;
class C3DMeshInfo{};
class CMatrix3x4{public:float M[12];};
class CEngineFadeDistance{public:float FadeBeginDistance,MaxDrawDistance;};
class CRGBColour{public:unsigned char B,G,R,A;};
class CEnginePersistentPrimitiveBase{
public:
 unsigned long IVRefCount;
 CEnginePersistentPrimitiveBase():IVRefCount(1){}
 virtual ~CEnginePersistentPrimitiveBase(){++deleted;}
 virtual void Delete()const{delete this;}
 void AddRef()const{++const_cast<CEnginePersistentPrimitiveBase*>(this)->IVRefCount;}
 long Release()const{return --const_cast<CEnginePersistentPrimitiveBase*>(this)->IVRefCount;}
 long GetRefCount()const{return IVRefCount;}
 virtual void NotifyReleased(const CEnginePrimitiveHandle&){check(IVRefCount==1);++notifications;}
};
template<class T>class CIVCountedPointer{
 T* Object;
public:
 CIVCountedPointer():Object(0){}
 CIVCountedPointer(const CIVCountedPointer& other):Object(other.Object){if(Object)Object->AddRef();}
 ~CIVCountedPointer(){Release();}
 void Release(){CEnginePersistentPrimitiveBase* object=Object;if(object){if(object->Release()==0)object->Delete();Object=0;}}
 void Reset(T* object){Release();Object=object;}
 unsigned long GetRefCount()const{return Object?Object->GetRefCount():0;}
 T* operator->()const{return Object;}
};
class CBaseClass{public:virtual ~CBaseClass(){}};
class CEnginePrimitiveHandle:public CBaseClass{
 CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
public:
 CEnginePrimitiveHandle(){}
 virtual ~CEnginePrimitiveHandle(){Clear();}
 void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data.Reset(0);}
 void SetData(CEnginePersistentPrimitiveBase* data){Clear();Data.Reset(data);}
 CEnginePersistentPrimitiveBase* Get()const{return Data.operator->();}
};
class CEngineInternalPrimitiveBase:public CEnginePersistentPrimitiveBase{
public:
 long Type;unsigned long RenderLayerMask,EnginePrimitiveFlags;
 float AlphaMipBias;CEngineFadeDistance FadeDistance;
 unsigned char Alpha,AlphaRef;bool NonPersistent;
 CEngineInternalPrimitiveBase(long type,unsigned long mask):Type(type),RenderLayerMask(mask),NonPersistent(false){}
 void SetNonPersistent(bool non_persistent){NonPersistent=non_persistent;}
};
class CEngineInternalPrimitiveOcclusionBase:public CEngineInternalPrimitiveBase{
public:
 CIVCountedPointer<CEnginePersistentPrimitiveBase> OcclusionInfo[2];
 CEngineInternalPrimitiveOcclusionBase(long type,unsigned long mask):CEngineInternalPrimitiveBase(type,mask){}
};
class CEnginePrimitiveThemeChannelDesc{public:unsigned char ChannelIndex0,ChannelIndex1,ChannelBlendFactor,Pad;CEnginePrimitiveThemeChannelDesc():ChannelIndex0(0),ChannelIndex1(0),ChannelBlendFactor(0){}};
class CEngineInternalPrimitiveMeshBase:public CEngineInternalPrimitiveOcclusionBase{
public:
 const C3DMeshInfo* MeshInfo;const C3DMeshInfo* ShadowMeshInfo;float Scale;CRGBColour TintColour;
 CIVCountedPointer<CEnginePersistentPrimitiveBase> SimulationInfo,MeshEffect;CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;
 CEngineInternalPrimitiveMeshBase(long type,unsigned long mask):CEngineInternalPrimitiveOcclusionBase(type,mask),MeshInfo(0){}
};
class CEngineInternalPrimitiveStaticMesh:public CEngineInternalPrimitiveMeshBase{
public:
 CMatrix3x4 ObjectTransform;
 CEngineInternalPrimitiveStaticMesh():CEngineInternalPrimitiveMeshBase(8,10){++constructed;}
};
class CEnginePrimitiveManagerStaticMeshesBase{
public:
 CEngineInternalPrimitiveBase* seen;unsigned long mode;int calls;bool retain;
 CEnginePrimitiveManagerStaticMeshesBase():seen(0),mode(9),calls(0),retain(false){}
 void AddPrimitive(CEngineInternalPrimitiveBase& p,unsigned long value){check(p.IVRefCount==1);seen=&p;mode=value;++calls;if(retain)p.AddRef();}
 CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo*,const C3DMeshInfo*,const CMatrix3x4&,float,const CEngineFadeDistance&,long,const CRGBColour&,long,float,bool,unsigned long);
};
CEnginePrimitiveHandle CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh(const C3DMeshInfo* mesh_info,const C3DMeshInfo* shadow_mesh_info,
 const CMatrix3x4& transform,float scale,const CEngineFadeDistance& fade_dist,long alpha,const CRGBColour& tint_colour,
 long alpha_ref,float alpha_mip_bias,bool persistent,unsigned long epf_flags)
{
 CEngineInternalPrimitiveStaticMesh* primitive=new CEngineInternalPrimitiveStaticMesh;
 primitive->MeshInfo=mesh_info;
 primitive->ShadowMeshInfo=shadow_mesh_info;
 primitive->ObjectTransform=transform;
 primitive->Scale=scale;
 primitive->FadeDistance=fade_dist;
 primitive->EnginePrimitiveFlags=epf_flags;
 primitive->Alpha=(unsigned char)alpha;
 primitive->TintColour=tint_colour;
 primitive->AlphaRef=(unsigned char)alpha_ref;
 primitive->AlphaMipBias=alpha_mip_bias;
 primitive->SetNonPersistent(true);
 primitive->SetNonPersistent(!persistent);
 CEnginePrimitiveHandle handle;
 handle.SetData(primitive);
 bool non_moving=(epf_flags&1)!=0;
 AddPrimitive(*primitive,non_moving);
 return handle;
}

int main(){
 const long alphas[]={-1,0,127,255,256};const long refs[]={-1,0,128,256};int cases=0;
 C3DMeshInfo mesh,shadow;
 for(int flags=0;flags<8;++flags)for(int ai=0;ai<5;++ai)for(int ri=0;ri<4;++ri)for(int persistent=0;persistent<2;++persistent)for(int seed=0;seed<3;++seed)for(int retain=0;retain<2;++retain){
  CEnginePrimitiveManagerStaticMeshesBase manager;manager.retain=retain!=0;
  CMatrix3x4 transform;for(int i=0;i<12;++i)transform.M[i]=float(seed*16+i)*0.25f;
  CEngineFadeDistance fade;fade.FadeBeginDistance=seed*0.5f;fade.MaxDrawDistance=8.0f+seed;
  CRGBColour tint;tint.B=(unsigned char)(seed+1);tint.G=127;tint.R=255;tint.A=64;
  int before_deleted=deleted,before_notifications=notifications;
  {
   CEnginePrimitiveHandle handle=manager.AddStaticMesh(&mesh,seed?&shadow:0,transform,seed*0.5f,fade,alphas[ai],tint,refs[ri],-seed*0.25f,persistent!=0,(unsigned long)flags);
   CEngineInternalPrimitiveStaticMesh* p=static_cast<CEngineInternalPrimitiveStaticMesh*>(handle.Get());
   check(manager.calls==1&&manager.seen==p&&manager.mode==(unsigned long)(flags&1));
   check(p&&p->IVRefCount==(unsigned long)(1+retain)&&p->Type==8&&p->RenderLayerMask==10);
   check(p->MeshInfo==&mesh&&p->ShadowMeshInfo==(seed?&shadow:0));
   for(int j=0;j<12;++j)check(p->ObjectTransform.M[j]==float(seed*16+j)*0.25f);
   check(p->Scale==seed*0.5f&&p->AlphaMipBias==-seed*0.25f&&p->EnginePrimitiveFlags==(unsigned long)flags);
   check(p->FadeDistance.FadeBeginDistance==seed*0.5f&&p->FadeDistance.MaxDrawDistance==8.0f+seed);
   check(p->Alpha==(unsigned char)alphas[ai]&&p->AlphaRef==(unsigned char)refs[ri]&&p->NonPersistent==!persistent);
   check(p->TintColour.B==seed+1&&p->TintColour.G==127&&p->TintColour.R==255&&p->TintColour.A==64);
   check(!p->ThemeChannelDesc.ChannelIndex0&&!p->ThemeChannelDesc.ChannelIndex1&&!p->ThemeChannelDesc.ChannelBlendFactor);
   check(deleted==before_deleted&&notifications==before_notifications);
  }
  if(retain){check(manager.seen->IVRefCount==1&&deleted==before_deleted&&notifications==before_notifications);check(manager.seen->Release()==0);manager.seen->Delete();}
  else check(notifications==before_notifications+1);
  check(deleted==before_deleted+1);++cases;
 }
 check(cases==1920&&constructed==1920&&deleted==1920);std::puts("00ba0fc0_TEST PASS");return 0;
}