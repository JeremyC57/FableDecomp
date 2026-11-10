#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){} };
static bool same(float a,float b){return memcmp(&a,&b,4)==0;}
static void equal(const C3DVector& a,const C3DVector& b){check(same(a.X,b.X)&&same(a.Y,b.Y)&&same(a.Z,b.Z));}
class CCharString {public:int id;CCharString(int i):id(i){} };const CCharString MAIN_FOCAL_POSITION_NAME(0),TARGETING_CURSOR_POSITION_NAME(1);
static int object_live,object_made,anims_live,anims_made,render_calls,anim_calls,manager_calls,lod_calls,tc_calls,anim_ignore_calls;
static int resources[3],helper_queries[2],meshes_queries;static float expected_time,last_render_time;
static bool reduce,has_tc,anim_ignore;static bool loaded[3],animated[3];static int helper_masks[3];
static C3DVector positions[3][2],animated_position,render_position;
class C3DAnimationManager{};static C3DAnimationManager animation_manager;
class C3DAnimationBlendState {public:int number;};
class C3DAnimationInterpolator {public:C3DAnimationBlendState StartState,EndState;C3DAnimationInterpolator(){++anims_live;++anims_made;}~C3DAnimationInterpolator(){--anims_live;}};
class CAnimateThing {public:bool AnimsCanBeIgnored()const{++anim_ignore_calls;return anim_ignore;}};
class CTCAnimationComplex {public:CAnimateThing animation;const CAnimateThing& PeekAnimation()const{return animation;}};static CTCAnimationComplex animation_tc;
class CThing {public:template<class T>bool PeekTC(T** p)const{++tc_calls;if(!has_tc)return false;*p=&animation_tc;return true;}};
class CWorld {public:C3DAnimationManager* GetAnimationManager()const{++manager_calls;return &animation_manager;}};
static const CThing* expected_thing;
namespace {
#ifdef _MSC_VER
C3DVector __fastcall GetRenderPositionForThing(const CThing& thing,float interpolate)
#else
C3DVector GetRenderPositionForThing(const CThing& thing,float interpolate)
#endif
{check(&thing==expected_thing);++render_calls;last_render_time=interpolate;return render_position;}
}
namespace NGraphicAppearance {class CMeshInfo {public:long Index;float SizeX;unsigned char PrimaryMesh:1;CMeshInfo(long index,bool primary):Index(index),SizeX(1),PrimaryMesh(primary){} };}
class CSortPrimaryMeshesFirst {public:bool operator()(const NGraphicAppearance::CMeshInfo& lhs,const NGraphicAppearance::CMeshInfo& rhs){return lhs.PrimaryMesh;}};
class CSortPrimaryMeshesLast {public:bool operator()(const NGraphicAppearance::CMeshInfo& lhs,const NGraphicAppearance::CMeshInfo& rhs){return !lhs.PrimaryMesh;}};
class C3DMeshObject {public:int id;C3DMeshObject(){++object_live;++object_made;}~C3DMeshObject(){--object_live;}
 bool HasHelperPoint(const CCharString& name)const{++helper_queries[name.id];return (helper_masks[id]&(1<<name.id))!=0;}
 long GetNoHelperPoints(const CCharString&)const{return 1;}
 bool IsAnimated()const{return animated[id];}
 C3DVector GetHelperPointPositionStatic(const CCharString& name,long)const{return positions[id][name.id];}
 C3DVector GetHelperPointPositionAnimated(const CCharString&,long,const C3DAnimationBlendState& a,const C3DAnimationBlendState& b,float interpolate,C3DAnimationManager& manager)const{check(a.number==11&&b.number==22&&same(interpolate,expected_time)&&&manager==&animation_manager);return animated_position;}
};
namespace NGraphicAppearance {class CTCGraphicAppearance {public:CThing thing;CWorld world;bool two_meshes;
 const CThing& GetParentThing()const{return thing;}
 const CWorld& PeekWorld()const{return world;}
 bool IsToReduceLODForDummiesAndHelpers()const{++lod_calls;return reduce;}
 void GetAllMeshes(std::vector<CMeshInfo>& meshes)const{++meshes_queries;meshes.push_back(CMeshInfo(0,false));meshes.push_back(CMeshInfo(1,false));if(two_meshes)meshes.push_back(CMeshInfo(2,true));}
 bool IsResourcesLoadedForMesh(const CMeshInfo& info,bool start)const{resources[info.Index]+=start?10:1;return loaded[info.Index];}
 void GetMeshObject(const CMeshInfo& info,float interpolate,C3DMeshObject& object)const{check(same(interpolate,expected_time));object.id=(int)info.Index;}
 bool GetAnims(C3DAnimationInterpolator& anims)const{++anim_calls;anims.StartState.number=11;anims.EndState.number=22;return false;}
 bool GetHelperPoint(const CCharString&,long,unsigned long,C3DVector&,float)const;
 C3DVector GetFocalPos(float)const;
};}
static void reset(){object_live=object_made=anims_live=anims_made=render_calls=anim_calls=manager_calls=lod_calls=tc_calls=anim_ignore_calls=meshes_queries=0;resources[0]=resources[1]=resources[2]=0;helper_queries[0]=helper_queries[1]=0;}
bool NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name,long n,unsigned long flags,C3DVector& point,float interpolate)const {
 bool reduced_lod=false;
 bool force_accurate=true;
 if(flags&1)force_accurate=false;
 bool ignore_animation=false;
 if(flags&4)ignore_animation=true;
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
 for(std::vector<CMeshInfo>::iterator it=meshes_used.begin();it!=meshes_used.end();++it) {
  CMeshInfo& mesh_info=*it;
  if(mesh_info.Index!=0) {
   bool start_loading=(flags&2)==0;
   bool return_if_not_loaded=!start_loading||(flags&32)!=0;
   if(!IsResourcesLoadedForMesh(mesh_info,start_loading)&&return_if_not_loaded)return false;
   C3DMeshObject object;
   GetMeshObject(mesh_info,interpolate,object);
   if(object.HasHelperPoint(name)) {
    if(n>=object.GetNoHelperPoints(name))return false;
    if(object.IsAnimated()&&!ignore_animation) {
     if(reduced_lod)point=GetRenderPositionForThing(GetParentThing(),1.0f);
     else {
      C3DAnimationInterpolator anims;
      GetAnims(anims);
      C3DAnimationManager* anim_manager=PeekWorld().GetAnimationManager();
      point=object.GetHelperPointPositionAnimated(name,n,anims.StartState,anims.EndState,interpolate,*anim_manager);
     }
    }else point=object.GetHelperPointPositionStatic(name,n);
    return true;
   }
  }
 }
 return false;
}
namespace NGraphicAppearance {
C3DVector CTCGraphicAppearance::GetFocalPos(float interpolate)const {
 C3DVector pos;
 if(GetHelperPoint(MAIN_FOCAL_POSITION_NAME,0,0,pos,interpolate))return pos;
 if(GetHelperPoint(TARGETING_CURSOR_POSITION_NAME,0,0,pos,interpolate))return pos;
 pos=GetRenderPositionForThing(GetParentThing(),interpolate);
 return pos;
}
}
int main(){
 const unsigned int bits[]={0u,0x80000000u,0x3f000000u,0xbf800000u,0x4a000000u,0x7f800000u,0x7fc12345u};const float times[]={-1,0,0.5f,1,2};long cases=0;
 for(int have=0;have!=4;++have)for(int x=0;x!=7;++x)for(int y=0;y!=7;++y)for(int z=0;z!=7;++z)for(int tm=0;tm!=5;++tm){
  float a,b,c;memcpy(&a,bits+x,4);memcpy(&b,bits+y,4);memcpy(&c,bits+z,4);positions[1][0]=C3DVector(a,b,c);positions[1][1]=C3DVector(c,a,b);render_position=C3DVector(b,c,a);expected_time=times[tm];
  NGraphicAppearance::CTCGraphicAppearance appearance;appearance.two_meshes=false;expected_thing=&appearance.thing;helper_masks[1]=have;loaded[1]=true;animated[1]=false;reduce=has_tc=anim_ignore=false;reset();C3DVector got=appearance.GetFocalPos(expected_time);equal(got,(have&1)?positions[1][0]:(have&2)?positions[1][1]:render_position);check(helper_queries[0]==1&&helper_queries[1]==((have&1)?0:1)&&render_calls==((have==0)?1:0));if(have==0)check(same(last_render_time,expected_time));check(object_live==0&&anims_live==0);++cases;
 }
 positions[1][0]=C3DVector(1,2,3);positions[2][0]=C3DVector(4,5,6);animated_position=C3DVector(7,8,9);render_position=C3DVector(10,11,12);expected_time=0.25f;
 for(unsigned long flags=0;flags!=64;++flags)for(int features=0;features!=8;++features)for(int lm=0;lm!=4;++lm)for(int hm=0;hm!=4;++hm)for(int anim=0;anim!=2;++anim)for(long n=-1;n!=3;++n){
  NGraphicAppearance::CTCGraphicAppearance appearance;appearance.two_meshes=true;expected_thing=&appearance.thing;reduce=(features&1)!=0;has_tc=(features&2)!=0;anim_ignore=(features&4)!=0;loaded[1]=(lm&1)!=0;loaded[2]=(lm&2)!=0;helper_masks[1]=(hm&1)?1:0;helper_masks[2]=(hm&2)?1:0;animated[1]=animated[2]=anim!=0;reset();C3DVector point(-90,-91,-92);bool got=appearance.GetHelperPoint(MAIN_FOCAL_POSITION_NAME,n,flags,point,expected_time);
  bool want=false;int chosen=0,objects=0;int r1=0,r2=0;int order[2];order[0]=(flags&8)?2:1;order[1]=3-order[0];
  for(int k=0;k!=2;++k){int id=order[k];bool start=(flags&2)==0;if(id==1)r1+=start?10:1;else r2+=start?10:1;if(!loaded[id]&&(!start||(flags&32)!=0))break;++objects;if(helper_masks[id]){if(n<1){want=true;chosen=id;}break;}}
  check(got==want&&resources[1]==r1&&resources[2]==r2&&object_made==objects&&object_live==0&&anims_live==0);check(meshes_queries==1);check(lod_calls==((flags&1)?1:0));check(tc_calls==(((flags&1)&&reduce)?1:0));check(anim_ignore_calls==(((flags&1)&&reduce&&has_tc)?1:0));
  bool animated_path=want&&anim&&!(flags&4);bool reduced=animated_path&&(flags&1)&&reduce&&has_tc&&anim_ignore;
  if(want)equal(point,reduced?render_position:animated_path?animated_position:positions[chosen][0]);else equal(point,C3DVector(-90,-91,-92));check(render_calls==(reduced?1:0));if(reduced)check(same(last_render_time,1.0f));check(anim_calls==((animated_path&&!reduced)?1:0)&&manager_calls==anim_calls&&anims_made==anim_calls);++cases;
 }
 check(cases==72396);puts("004c1060_TEST PASS");return 0;
}