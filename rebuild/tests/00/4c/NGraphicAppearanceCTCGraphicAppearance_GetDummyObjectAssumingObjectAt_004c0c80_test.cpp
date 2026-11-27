#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_ASSERT(x) ((void)(x))
typedef const char* CCharString;
struct C3DVector {int v; C3DVector(int x=0):v(x){}};
struct CRightHandedSet {int v; CRightHandedSet(int x=0):v(x){} void Init(C3DVector u,C3DVector l){v=u.v*10+l.v;}};
struct CMeshInfo {int Index;bool primary,loaded,found,animated;};
struct CSortPrimaryMeshesFirst {bool operator()(CMeshInfo a,CMeshInfo b)const{return a.primary>b.primary;}};
struct CSortPrimaryMeshesLast {bool operator()(CMeshInfo a,CMeshInfo b)const{return a.primary<b.primary;}};
static bool has,ign,policy;static int started;static CMeshInfo pool[2];
struct CTCAnimationComplex {const CTCAnimationComplex& PeekAnimation()const{return *this;} bool AnimsCanBeIgnored()const{return ign;}};
struct Thing {bool PeekTC(const CTCAnimationComplex** p)const{static CTCAnimationComplex c;*p=&c;return has;}const Thing* GetPhysicsTC()const{return this;} CRightHandedSet GetRHSet()const{return 17;}};
C3DVector GetRenderPositionForThing(const Thing&,float f){if(f!=1)abort();return 99;}
struct C3DAnimationInterpolator {int StartState,EndState;};
struct World {int* GetAnimationManager(){static int id=246;return &id;}};
struct C3DMeshObject {CMeshInfo m; bool HasDummyObject(CCharString s){return s[0]=='d'&&m.found;}long GetNoDummyObjects(CCharString){return m.Index;}bool IsAnimated(){return m.animated;}
void GetDummyObjectPositionStatic(CCharString,long n,C3DVector& p,C3DVector& l,C3DVector& u){p=m.Index*10+n;l=1;u=2;}
void GetDummyObjectPositionAnimated(CCharString,long n,int s,int e,float f,int& id,C3DVector& p,C3DVector& l,C3DVector& u,bool b){if(s!=4||e!=7||f!=1.0f||id!=246||b)abort();p=m.Index*20+n;l=3;u=4;}};
namespace NGraphicAppearance {struct CTCGraphicAppearance {bool GetDummyObjectAssumingObjectAt(const CCharString& name,const C3DVector& at_pos,const CRightHandedSet& at_rhs,long n,unsigned long flags,C3DVector& point,CRightHandedSet& rhset)const ;
bool IsToReduceLODForDummiesAndHelpers()const{return policy;}const Thing& GetParentThing()const{static Thing t;return t;}
void GetAllMeshes(std::vector<CMeshInfo>& v)const{v.assign(pool,pool+2);}
bool IsResourcesLoadedForMesh(CMeshInfo m,bool start)const{if(!m.loaded&&start)++started;return m.loaded;}
void GetMeshObject(CMeshInfo m,float t,C3DMeshObject& o)const{if(t!=.25f)abort();o.m=m;}
void GetMeshObjectAssumingObjectAt(CMeshInfo m,const C3DVector& p,const CRightHandedSet& r,C3DMeshObject& o)const{if(p.v!=19||r.v!=31)abort();o.m=m;}
bool GetAnims(C3DAnimationInterpolator& a)const{a.StartState=4;a.EndState=7;return true;} World& PeekWorld()const{static World w;return w;}};}
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObjectAssumingObjectAt(const CCharString& name,const C3DVector& at_pos,const CRightHandedSet& at_rhs,long n,unsigned long flags,C3DVector& point,CRightHandedSet& rhset)const {
 bool force_accurate=true;
 if(flags&1)force_accurate=false;
 bool ignore_animation=false;
 if(flags&4)ignore_animation=true;
 bool reduced_lod=false;
 if(!force_accurate&&IsToReduceLODForDummiesAndHelpers()) {
  const CTCAnimationComplex* ptc_anim;
  if(GetParentThing().PeekTC(&ptc_anim)) {
   if(ptc_anim->PeekAnimation().AnimsCanBeIgnored())reduced_lod=true;
  }
 }
 std::vector<CMeshInfo> meshes_used;
 GetAllMeshes(meshes_used);
 if(flags&8)std::sort(meshes_used.begin(),meshes_used.end(),CSortPrimaryMeshesFirst());
 else if(flags&16)std::sort(meshes_used.begin(),meshes_used.end(),CSortPrimaryMeshesLast());
 for(std::vector<CMeshInfo>::iterator it=meshes_used.begin();it!=meshes_used.end();++it){
  CMeshInfo& mesh_info=*it;
  if(mesh_info.Index!=0) {
   bool start_loading=(flags&2)==0;
   if(!IsResourcesLoadedForMesh(mesh_info,start_loading)&&!start_loading)return false;
   C3DMeshObject object;
   GetMeshObjectAssumingObjectAt(mesh_info,at_pos,at_rhs,object);
   if(object.HasDummyObject(name)){
    C3DVector look,up;
    
    if(object.IsAnimated()&&!ignore_animation){
     if(reduced_lod){
      point=GetRenderPositionForThing(GetParentThing(),1.0f);
      rhset=GetParentThing().GetPhysicsTC()->GetRHSet();
     }else{
      C3DAnimationInterpolator anims;
      const bool success=GetAnims(anims);
      FABLE_RETAIL_ASSERT(success);
      object.GetDummyObjectPositionAnimated(name,n,anims.StartState,anims.EndState,1.0f,*PeekWorld().GetAnimationManager(),point,look,up,false);
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
      rhset.Init(up,look);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
     }
    }else{
     object.GetDummyObjectPositionStatic(name,n,point,look,up);
     rhset.Init(up,look);
    }
    return true;
   }
  }

 }
 return false;
}

int main(){NGraphicAppearance::CTCGraphicAppearance g;
for(int s=0;s<256;++s)for(int f=0;f<64;++f)for(int n=0;n<3;++n){
pool[0].Index=(s&1)?0:1;pool[0].primary=false;pool[0].loaded=(s&2)!=0;pool[0].found=(s&4)!=0;pool[0].animated=(s&8)!=0;
pool[1].Index=2;pool[1].primary=true;pool[1].loaded=true;pool[1].found=(s&16)!=0;pool[1].animated=true;
has=(s&32)!=0;ign=(s&64)!=0;policy=(s&128)!=0;started=0;
C3DVector p(-2);CRightHandedSet r(-3);CCharString name="dummy";bool got=g.GetDummyObjectAssumingObjectAt(name,C3DVector(19),CRightHandedSet(31),n,f,p,r);
int ep=-2,er=-3,starts=0;bool want=false;for(int k=0;k<2;++k){int ix=(f&8)?1-k:k;CMeshInfo m=pool[ix];if(!m.Index)continue;if(!m.loaded){if(f&2)break;++starts;}if(!m.found)continue;if(0&&n>=m.Index)break;want=true;
if(m.animated&&!(f&4)){if((f&1)&&policy&&has&&ign){ep=99;er=17;}else{ep=m.Index*20+n;er=43;}}else{ep=m.Index*10+n;er=21;}break;}
if(got!=want||p.v!=ep||r.v!=er||starts!=started)return 1;}
puts("004c0c80_TEST PASS");return 0;}