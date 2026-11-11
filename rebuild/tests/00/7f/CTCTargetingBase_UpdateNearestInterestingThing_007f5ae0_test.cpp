#include <stdio.h>
#include <math.h>
#include <string.h>
#ifdef _MSC_VER
typedef __int64 I64;typedef unsigned __int64 U64;
#else
typedef long long I64;typedef unsigned long long U64;
#define __int64 long long
#endif
static int failed,line_failed,world_calls,frame_calls,search_calls,volume_calls,map_calls,layer_calls,fov_calls,set_calls,contains_calls;
static char trace[64];static int tn;static void note(char c){trace[tn++]=c;trace[tn]=0;}
#define CHECK(x) do{if(!(x)){failed=1;line_failed=__LINE__;return;}}while(0)
class C2DCoordI {public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C3DVector {public:float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}float GetMagnitude2D()const{float squared=X*X;squared+=Y*Y;return (float)sqrt(squared);}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
class C2DBoxI {public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}bool ContainsPoint(const C2DCoordI&p)const{note('C');++contains_calls;return p.X>=TLX&&p.Y>=TLY&&p.X<BRX&&p.Y<BRY;}};
class CThing {public:C3DVector pos;U64 uid;int type;bool dead;const C3DVector&GetPos()const{return pos;}U64 GetUniqueID()const{return uid;}int GetType()const{return type;}};
class CThingCreatureBase:public CThing {public:bool fov;bool IsThingInFOV(const CThing&,bool head)const{if(head)failed=1;++fov_calls;note('F');return fov;}};
class CThingPhysical:public CThing{};
template<class T>class CIntelligentPointer{public:T*item;T*Get()const{return item;}T*operator->()const{return item;}T&operator*()const{return *item;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}
template<class T>bool IS_THING_ALIVE(T*p){return p&&!p->dead;}
class CThingFilter_IsInterestingToCreature {public:const CThingCreatureBase&SourceCreature;CThingFilter_IsInterestingToCreature(const CThingCreatureBase&c):SourceCreature(c){}};
class CThingFilter_IsInCreatureFOV {public:const CThingCreatureBase&SourceCreature;bool IncludeHeadAngle;CThingFilter_IsInCreatureFOV(const CThingCreatureBase&c,bool h):SourceCreature(c),IncludeHeadAngle(h){}};
class CThingFilter_IsPerceptible{};
template<class A,class B,class C>class CFilter_Bind3{public:const A&F1;const B&F2;const C&F3;CFilter_Bind3(const A&a,const B&b,const C&c):F1(a),F2(b),F3(c){}};
template<class A,class B,class C>CFilter_Bind3<A,B,C>GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
class CThingCompare_Nearest{public:C3DVector Pos;CThingCompare_Nearest(const C3DVector&p):Pos(p){}};
static CThingCreatureBase*expected_parent;static CThingPhysical*search_result;static bool volume_ok,layer_ok;static long frame_value,period_value;static C2DBoxI*dummy_box;
class CThingFilter_IsInSameVolumeContextAsThing {public:CThingFilter_IsInSameVolumeContextAsThing(const CThing&t,bool b){if(&t!=expected_parent||b)failed=1;}bool operator()(const CThing&t)const{++volume_calls;note('V');if(&t!=search_result)failed=1;return volume_ok;}};
class CWorldMap{public:long GetNavigationLayerAt(const C3DVector&p,const CThing*t)const{++layer_calls;note('L');if(t)failed=1;if(&p==&search_result->pos)return 0;if(&p!=&expected_parent->pos)failed=1;return layer_ok?0:1;}};
class CWorld{public:static long GetFrame(){++frame_calls;note('f');return frame_value;}const CWorldMap&PeekWorldMap()const{++map_calls;note('M');static CWorldMap map;return map;}};
class CMainGameComponent{public:static long GetConstantFPS(){return period_value;}};
class CThingSearchTools{public:template<class F,class C,class T>bool GetPBestThingInRoughArea(const C2DBoxI&a,const F&filter,const C&compare,T**out)const{++search_calls;note('Q');long x=(long)expected_parent->pos.X,y=(long)expected_parent->pos.Y;if(a.TLX!=x-4||a.TLY!=y-4||a.BRX!=x+4||a.BRY!=y+4||&filter.F1.SourceCreature!=expected_parent||&filter.F2.SourceCreature!=expected_parent||filter.F2.IncludeHeadAngle||compare.Pos.X!=expected_parent->pos.X||compare.Pos.Y!=expected_parent->pos.Y||compare.Pos.Z!=expected_parent->pos.Z)failed=1;*out=search_result;return *out!=0;}};
class CTCTargetingBase{public:CThing*parent;bool ScriptOverrideIgnoreEverything;CIntelligentPointer<CThing>PNearestInterestingThing,PScriptOverrideInterestingThing;CThing*set_value;
 CWorld&GetWorld()const{++world_calls;note('W');static CWorld w;return w;}CThing&GetParentThing(){return *parent;}const CThingSearchTools&PeekThingSearchTools()const{static CThingSearchTools tools;return tools;}void SetPNearestInterestingThing(CThing*p){++set_calls;note('S');set_value=p;PNearestInterestingThing.item=p;}void UpdateNearestInterestingThing();};
void CTCTargetingBase::UpdateNearestInterestingThing()
{
 if((GetWorld().GetFrame()+(__int64)GetParentThing().GetUniqueID())%CMainGameComponent::GetConstantFPS()!=0)return;
 if(ScriptOverrideIgnoreEverything)return;
 const C3DVector& pos=GetParentThing().GetPos();
 if(PScriptOverrideInterestingThing!=0) {
  if((PScriptOverrideInterestingThing->GetPos()-pos).GetMagnitude2D()<4.0f) {
   SetPNearestInterestingThing(PScriptOverrideInterestingThing.Get());return;
  }
 }
 const CThingCreatureBase& parent_creature=static_cast<const CThingCreatureBase&>(GetParentThing());
 C2DBoxI area(pos.ToC2DCoordI(),4);
 CThingPhysical* pnearest_thing;
 PeekThingSearchTools().GetPBestThingInRoughArea(area,
  GFFilter_Bind3(CThingFilter_IsInterestingToCreature(parent_creature),CThingFilter_IsInCreatureFOV(parent_creature,false),CThingFilter_IsPerceptible()),
  CThingCompare_Nearest(pos),&pnearest_thing);
 if(IS_THING_ALIVE(pnearest_thing)) {
  if(!CThingFilter_IsInSameVolumeContextAsThing(parent_creature,false)(*pnearest_thing))pnearest_thing=0;
  else {
   const CWorldMap& wmap=GetWorld().PeekWorldMap();
   if(wmap.GetNavigationLayerAt(pnearest_thing->GetPos(),0)!=wmap.GetNavigationLayerAt(parent_creature.GetPos(),0))pnearest_thing=0;
  }
 }
 if(PNearestInterestingThing!=0 && (!pnearest_thing || pnearest_thing->GetType()!=2) && parent_creature.IsThingInFOV(*PNearestInterestingThing,false)) {
  const C3DVector& current=PNearestInterestingThing->GetPos();
  if(area.ContainsPoint(C2DCoordI((long)current.X,(long)current.Y)))return;
 }
 SetPNearestInterestingThing(pnearest_thing);
}

static void one(int clock,int ignore,int over,int found,int vol,int layer,int current,int fov,int posindex,int seed)
{
 long periods[]={1,3,60};long frames[]={-16,0,1,16};U64 ids[]={0,1,((U64)1<<32)+1,((U64)1<<63)+1000};float positions[]={-3.75f,-0.5f,0,8.25f};
 CThingCreatureBase parent;parent.pos=C3DVector(positions[posindex],positions[3-posindex],16);parent.uid=ids[(clock/4)%4];parent.type=2;parent.dead=false;parent.fov=fov!=0;
 CThing override_thing;override_thing.pos=parent.pos;override_thing.pos.Z=128;float distances[]={0,0,3,4,5};override_thing.pos.X+=distances[over];override_thing.uid=17;override_thing.type=1;override_thing.dead=seed!=0;
 CThingPhysical candidate;candidate.pos=C3DVector(64,32,16);candidate.uid=18;candidate.type=(found==1||found==3)?2:1;candidate.dead=found>=3;
 CThing old;old.pos=parent.pos;old.pos.X+=(current==2?4:0);old.uid=19;old.type=1;old.dead=seed!=0;
 CTCTargetingBase obj;obj.parent=&parent;obj.ScriptOverrideIgnoreEverything=ignore!=0;obj.PNearestInterestingThing.item=current?&old:0;obj.PScriptOverrideInterestingThing.item=over?&override_thing:0;obj.set_value=0;
 expected_parent=&parent;search_result=found?&candidate:0;volume_ok=vol!=0;layer_ok=layer!=0;frame_value=frames[clock%4];period_value=periods[clock/16];
 failed=0;tn=0;trace[0]=0;world_calls=frame_calls=search_calls=volume_calls=map_calls=layer_calls=fov_calls=set_calls=contains_calls=0;
 I64 signed_id=(parent.uid>>63)?-(I64)(~parent.uid)-1:(I64)parent.uid;I64 sum=signed_id+frame_value;bool due=(sum/period_value)*period_value==sum;
 bool work=due&&!ignore;bool use_override=work&&over&&distances[over]<4;
 CThing*best=found?&candidate:0;bool check_volume=work&&!use_override&&best&&!candidate.dead;
 if(check_volume&&(!volume_ok||!layer_ok))best=0;
 bool check_fov=work&&!use_override&&current&&(!best||best->type!=2);
 long ox=(long)old.pos.X,oy=(long)old.pos.Y;long px=(long)parent.pos.X,py=(long)parent.pos.Y;
 bool keep=check_fov&&fov&&ox>=px-4&&oy>=py-4&&ox<px+4&&oy<py+4;
 CThing*expected=(current?&old:0);int expected_sets=0;if(use_override){expected=&override_thing;expected_sets=1;}else if(work&&!keep){expected=best;expected_sets=1;}
 obj.UpdateNearestInterestingThing();CHECK(!failed);CHECK(obj.PNearestInterestingThing.item==expected);CHECK(set_calls==expected_sets);CHECK(frame_calls==1);CHECK(search_calls==(work&&!use_override?1:0));CHECK(volume_calls==(check_volume?1:0));CHECK(map_calls==(check_volume&&vol?1:0));CHECK(layer_calls==2*map_calls);CHECK(world_calls==1+map_calls);CHECK(fov_calls==(check_fov?1:0));CHECK(contains_calls==(check_fov&&fov?1:0));
 char expected_trace[64]="Wf";if(use_override)strcat(expected_trace,"S");else if(work){strcat(expected_trace,"Q");if(check_volume){strcat(expected_trace,"V");if(vol)strcat(expected_trace,"WMLL");}if(check_fov){strcat(expected_trace,"F");if(fov)strcat(expected_trace,"C");}if(!keep)strcat(expected_trace,"S");}CHECK(strcmp(trace,expected_trace)==0);CHECK(obj.PScriptOverrideInterestingThing.item==(over?&override_thing:0));CHECK(obj.parent==&parent);CHECK(parent.uid==ids[(clock/4)%4]);
}
int main(){int cases=0;for(int c=0;c<48;++c)for(int i=0;i<2;++i)for(int o=0;o<5;++o)for(int f=0;f<5;++f)for(int v=0;v<2;++v)for(int l=0;l<2;++l)for(int t=0;t<3;++t)for(int a=0;a<2;++a)for(int p=0;p<4;++p)for(int s=0;s<2;++s){one(c,i,o,f,v,l,t,a,p,s);if(failed){printf("007f5ae0_TEST FAIL line%d case%d\n",line_failed,cases);return 1;}++cases;}if(cases!=460800)return 2;puts("007f5ae0_TEST PASS");return 0;}