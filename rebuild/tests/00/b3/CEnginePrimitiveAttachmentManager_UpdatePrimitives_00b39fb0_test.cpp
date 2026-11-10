#include <cstdio>
#include <list>
class C3DVector{public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}};
class C3DBoundingSphere{public:C3DVector Center;float Radius;C3DBoundingSphere():Radius(0){}};
class CEngineCamera{};
template<class T>class CArray{};
static int gDestroyed,gParentCalls,gRadiusCalls,gVisibleCalls,gUpdateCalls,gBadArgs;static bool gSphereOK,gRadiusOK,gVisible;static float gInterpolation;static const CArray<CEngineCamera*>* gCameras;
class CEngineFadeDistance{public:float MaxDrawDistance;};
class CEngineInternalPrimitiveBase{public:CEngineFadeDistance FadeDistance;unsigned char Flags;bool GetBoundingSphereRadiusWorldSpace(float& r){++gRadiusCalls;r=2;return gRadiusOK;}void SetEnable(bool b){Flags=(Flags&~1)|(b?1:0);}};
class CEngineInternalAttachedPrimitive{public:enum EParentType{PARENT_TYPE_NULL,PARENT_TYPE_STATIC_MESH,PARENT_TYPE_ANIMATED_MESH};EParentType ParentType;CEngineInternalPrimitiveBase* OutputPrimitive;unsigned long RefCount;CEngineInternalPrimitiveBase& GetOutputPrimitive(){return *OutputPrimitive;}bool GetParentWorldSpaceBoundingSphereAnimatedMesh(C3DBoundingSphere& s,float interp){++gParentCalls;gBadArgs|=interp!=gInterpolation||s.Center.X!=0||s.Center.Y!=0||s.Center.Z!=0||s.Radius!=0;s.Center=C3DVector(4,8,16);s.Radius=3;return gSphereOK;}bool GetParentWorldSpaceBoundingSphere(C3DBoundingSphere&,float);void UpdateOutputPrimitive(float interp){++gUpdateCalls;gBadArgs|=interp!=gInterpolation;}};
class CEngineInternalAttachedPrimitiveInfo{public:CEngineInternalAttachedPrimitive* PrimitiveHandle;CEngineInternalAttachedPrimitiveInfo(CEngineInternalAttachedPrimitive* p=0):PrimitiveHandle(p){}~CEngineInternalAttachedPrimitiveInfo(){++gDestroyed;}bool IsAlive()const{return PrimitiveHandle&&PrimitiveHandle->RefCount>1;}CEngineInternalAttachedPrimitive& GetPrimitive(){return *PrimitiveHandle;}bool GetOuterBoundingSphere(C3DBoundingSphere&,float);};
class CEnginePrimitiveAttachmentManager{public:typedef std::list<CEngineInternalAttachedPrimitiveInfo> TAttachedPrimitiveList;TAttachedPrimitiveList AttachedPrimitives;void UpdatePrimitives(const CArray<CEngineCamera*>&,float);bool IsVisible(const C3DBoundingSphere& s,float distance,const CArray<CEngineCamera*>& cameras)const{++gVisibleCalls;gBadArgs|=s.Center.X!=4||s.Center.Y!=8||s.Center.Z!=16||s.Radius!=7||distance!=64||&cameras!=gCameras;return gVisible;}};
bool CEngineInternalAttachedPrimitiveInfo::GetOuterBoundingSphere(C3DBoundingSphere& dest,float interp){
  CEngineInternalAttachedPrimitive& attach_prim=GetPrimitive();
  CEngineInternalPrimitiveBase& output_prim=attach_prim.GetOutputPrimitive();
  if(attach_prim.GetParentWorldSpaceBoundingSphere(dest,interp)){
   float child_radius;
   if(output_prim.GetBoundingSphereRadiusWorldSpace(child_radius)){
    dest.Radius+=child_radius*2.0;
    return true;
   }
  }
  return false;
 }

bool CEngineInternalAttachedPrimitive::GetParentWorldSpaceBoundingSphere(C3DBoundingSphere& dest,float interp){
 switch(ParentType){case PARENT_TYPE_ANIMATED_MESH:return GetParentWorldSpaceBoundingSphereAnimatedMesh(dest,interp);}
 return false;
}
void CEnginePrimitiveAttachmentManager::UpdatePrimitives(const CArray<CEngineCamera*>& cameras,float interp)
{
 TAttachedPrimitiveList::iterator itr;
 for(itr=AttachedPrimitives.begin();itr!=AttachedPrimitives.end();){
  TAttachedPrimitiveList::iterator next_itr=itr;
  ++next_itr;
  CEngineInternalAttachedPrimitiveInfo& prim_info=*itr;
  if(prim_info.IsAlive()){
   bool visible=false;
   C3DBoundingSphere bsphere;
   if(prim_info.GetOuterBoundingSphere(bsphere,interp)){
    CEngineInternalPrimitiveBase& out_prim=prim_info.GetPrimitive().GetOutputPrimitive();
    float max_dist=out_prim.FadeDistance.MaxDrawDistance;
    if(IsVisible(bsphere,max_dist,cameras)){
     prim_info.GetPrimitive().UpdateOutputPrimitive(interp);
     visible=true;
    }
   }
   prim_info.GetPrimitive().GetOutputPrimitive().SetEnable(visible);
  }else AttachedPrimitives.erase(itr);
  itr=next_itr;
 }
}


int main(){int ok=1,cases=0;CArray<CEngineCamera*> cameras;gCameras=&cameras;gInterpolation=0.5f;
for(int ref=0;ref<4;++ref)for(int parent=0;parent<3;++parent)for(int sphere=0;sphere<2;++sphere)for(int radius=0;radius<2;++radius)for(int visible=0;visible<2;++visible){CEnginePrimitiveAttachmentManager manager;CEngineInternalPrimitiveBase out;out.FadeDistance.MaxDrawDistance=64;out.Flags=0xad;CEngineInternalAttachedPrimitive primitive;primitive.ParentType=(CEngineInternalAttachedPrimitive::EParentType)parent;primitive.OutputPrimitive=&out;primitive.RefCount=ref;manager.AttachedPrimitives.push_back(CEngineInternalAttachedPrimitiveInfo(ref?&primitive:0));gDestroyed=gParentCalls=gRadiusCalls=gVisibleCalls=gUpdateCalls=gBadArgs=0;gSphereOK=sphere!=0;gRadiusOK=radius!=0;gVisible=visible!=0;manager.UpdatePrimitives(cameras,gInterpolation);bool alive=ref>1,p=alive&&parent==2,r=p&&sphere,v=r&&radius,u=v&&visible;ok &= manager.AttachedPrimitives.size()==(alive?1:0)&&gDestroyed==(alive?0:1)&&gParentCalls==(p?1:0)&&gRadiusCalls==(r?1:0)&&gVisibleCalls==(v?1:0)&&gUpdateCalls==(u?1:0)&&!gBadArgs;ok &= out.Flags==(alive?(u?0xad:0xac):0xad);++cases;}
// Alternating expiry ensures saved successor stays valid through actual list erasure.
{CEnginePrimitiveAttachmentManager manager;CEngineInternalPrimitiveBase outputs[6];CEngineInternalAttachedPrimitive primitives[6];for(int i=0;i<6;++i){outputs[i].Flags=0xa4;outputs[i].FadeDistance.MaxDrawDistance=64;primitives[i].RefCount=(i%2)?2:1;primitives[i].ParentType=CEngineInternalAttachedPrimitive::PARENT_TYPE_ANIMATED_MESH;primitives[i].OutputPrimitive=&outputs[i];manager.AttachedPrimitives.push_back(CEngineInternalAttachedPrimitiveInfo(&primitives[i]));}gSphereOK=gRadiusOK=gVisible=true;gDestroyed=gParentCalls=gRadiusCalls=gVisibleCalls=gUpdateCalls=gBadArgs=0;manager.UpdatePrimitives(cameras,gInterpolation);ok &= manager.AttachedPrimitives.size()==3&&gDestroyed==3&&gParentCalls==3&&gRadiusCalls==3&&gVisibleCalls==3&&gUpdateCalls==3&&!gBadArgs;for(int i=0;i<6;++i)ok &= outputs[i].Flags==((i%2)?0xa5:0xa4);++cases;}
{CEnginePrimitiveAttachmentManager manager;gDestroyed=gParentCalls=gRadiusCalls=gVisibleCalls=gUpdateCalls=0;manager.UpdatePrimitives(cameras,gInterpolation);ok &= !gDestroyed&&!gParentCalls&&!gRadiusCalls&&!gVisibleCalls&&!gUpdateCalls;++cases;}
ok &= cases==98;std::printf(ok?"00b39fb0_TEST PASS\n":"00b39fb0_TEST FAIL\n");return ok?0:1;}