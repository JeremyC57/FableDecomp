#include <stdio.h>
#include <vector>
#include <functional>
#include <algorithm>
#ifdef _MSC_VER
#define min _cpp_min
#endif
static bool failed;static int line_failed;
#define CHECK(x) do{if(!(x)){failed=true;line_failed=__LINE__;return;}}while(0)
class C3DVector{ public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}};
class CThing;class CTCNoise{ public:const CThing*creator;float range;const CThing*GetCreator()const{return creator;}float GetDistanceCanBeHeardFrom()const{return range;}};
class CTCVillage{ public:long end;long GetFrameBribeEnds()const{return end;}};
class CTCVillageMember{ public:CTCVillage*v;CTCVillage*GetPVillage()const{return v;}};
enum ETCInterfaceType{MEMBER=180};
class CThing{ public:C3DVector pos;long created;bool dead,scan,has,lookup;CTCVillageMember*member;const CTCNoise*noise;const C3DVector&GetPos()const{return pos;}bool HasTC(ETCInterfaceType)const{return has;}bool GetTC(CTCVillageMember**p){*p=member;return lookup;}bool PeekTC(const CTCNoise**p)const{*p=noise;return noise!=0;}long GetWorldFrameCreatedAt()const{return created;}bool IsIncludedInThingScans()const{return scan&&!dead;}};
class CThingNoise:public CThing{ public:};
template<class T>bool IS_THING_ALIVE(T*p){return p&&!p->dead;}template<class T>bool IS_THING_DEAD(T*p){return !IS_THING_ALIVE(p);}
// Layout: complete original Ego23ef4/FW25f3a,size4, genuine unary-function base.
class CThingFilter_NoiseCameFromThing:public std::unary_function<CThing,bool>{
 const CThing&Thing;
public:CThingFilter_NoiseCameFromThing(const CThing&thing):Thing(thing){}
 bool operator()(const CThing&thing)const{const CTCNoise*ptc_noise=0;return thing.PeekTC(&ptc_noise)&&ptc_noise->GetCreator()==&Thing;}
};
// Layout: complete original Ego23f00/FW25f46,size16.
class CThingFilter_NoiseCanBeHeardFromPosition:public std::unary_function<CThing,bool>{
 C3DVector Position;float ListenersHearingRange;
public:CThingFilter_NoiseCanBeHeardFromPosition(const CThingFilter_NoiseCanBeHeardFromPosition&);
 CThingFilter_NoiseCanBeHeardFromPosition(const C3DVector&pos,float range):Position(pos){ListenersHearingRange=range;}
 bool operator()(const CThing&thing)const{const CTCNoise*ptc_noise;if(thing.PeekTC(&ptc_noise)){float distance_squared=(thing.GetPos()-Position).GetSquaredMagnitude();float noise_range=ptc_noise->GetDistanceCanBeHeardFrom();float range=std::min(noise_range,ListenersHearingRange);return distance_squared<=range*range;}return false;}
};
// Layout: complete original Ego38abb/FW3cbcd,size12; Pos is genuinely unused in this comparator.
class CThingCompare_GetMostRecentlyCreatedThing{
 C3DVector Pos;
public:CThingCompare_GetMostRecentlyCreatedThing(){}
 CThingCompare_GetMostRecentlyCreatedThing(const CThingCompare_GetMostRecentlyCreatedThing&);
 template<class T>T*operator()(T*first,T*second)const{
  if(IS_THING_DEAD(first)&&second)return second;
  if(IS_THING_DEAD(second))return first;
  return first->GetWorldFrameCreatedAt()>second->GetWorldFrameCreatedAt()?first:second;
 }
};

template<class A,class B>class CFilter_Bind2{ public:const A&F1;const B&F2;CFilter_Bind2(const A&a,const B&b):F1(a),F2(b){}template<class T>bool operator()(const T&t)const{return F1(t)&&F2(t);}};
template<class A,class B>CFilter_Bind2<A,B>GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
class CThingSearchTools{ public:std::vector<CThingNoise*>things;int calls;CThingNoise*selected;bool kill;template<class F,class C,class T>bool GetPBestLeafThing(const F&filter,const C&compare,T**out){++calls;T*best=0;for(unsigned i=0;i<things.size();++i){T*thing=things[i];if(thing->IsIncludedInThingScans()&&filter(*thing))best=compare(best,thing);}*out=best;selected=best;if(kill&&best)best->dead=true;return best!=0;}};
class CWorld{ public:long frame;long GetFrame()const{return frame;}};
class CPerceivedThingDef{ public:float AwarenessTimer;};
class CTCPerceiveThing{ public:CThing*parent;const CThing*perceived;CThingSearchTools*tools;CWorld*world;const CPerceivedThingDef*PDef;bool Heard,DirectAwareness,second_null;long FramesSinceDirectlyPerceivedThing;float AwarenessTimer,SoundRadius;C3DVector LastPerceivedPos;int perceived_calls,last_calls,react_calls;CThing&GetParentThing(){return *parent;}CWorld&GetWorld(){return *world;}const CThing*GetPerceivedThing(){++perceived_calls;return second_null&&perceived_calls>1?0:perceived;}CThingSearchTools&GetThingSearchTools(){return *tools;}bool IsAwareOfThing()const{return AwarenessTimer!=0.0f;}bool IsAwarenessOfThingDirect()const{return DirectAwareness;}float GetSoundRadius()const{return (SoundRadius);}void SetLastAwareInfo(){++last_calls;LastPerceivedPos=perceived->pos;AwarenessTimer=PDef->AwarenessTimer;}void SetAsDirectlyAwareOfPerceivedThing(){if(GetPerceivedThing()){DirectAwareness=true;FramesSinceDirectlyPerceivedThing=0;SetLastAwareInfo();}}void ReactToAlerts(){++react_calls;}void UpdateHearing();};
void CTCPerceiveThing::UpdateHearing()
{
 Heard=false;
 CTCVillageMember* tc_village_member=0;
 if(GetParentThing().HasTC((ETCInterfaceType)0xb4)&&GetParentThing().GetTC(&tc_village_member)) {
  CTCVillage* ptc_village=tc_village_member->GetPVillage();
  if(ptc_village && GetWorld().GetFrame()<ptc_village->GetFrameBribeEnds())return;
 }
 const CThing* pperceived_thing=GetPerceivedThing();
 CThingNoise* pnoise=0;
 GetThingSearchTools().GetPBestLeafThing(
  GFFilter_Bind2(CThingFilter_NoiseCameFromThing(*pperceived_thing),CThingFilter_NoiseCanBeHeardFromPosition(GetParentThing().GetPos(),GetSoundRadius())),
  CThingCompare_GetMostRecentlyCreatedThing(),&pnoise);
 if(IS_THING_ALIVE(pnoise)) {
  Heard=true;
  if(!(IsAwareOfThing()&&IsAwarenessOfThingDirect())) {
   LastPerceivedPos=pnoise->GetPos();AwarenessTimer=PDef->AwarenessTimer;
  } else SetAsDirectlyAwareOfPerceivedThing();
 }
 ReactToAlerts();
}

static void one(int village_case,int frame_case,int profile,int aware,int direct,int second_null,int kill)
{
 CThing parent,perceived,other;parent.pos=C3DVector(0,0,0);perceived.pos=C3DVector(32,64,128);other.pos=C3DVector(-32,-64,-128);
 CTCVillage village;village.end=16;CTCVillageMember member;member.v=village_case==3?0:&village;parent.has=village_case!=0;parent.lookup=village_case!=1;parent.member=&member;
 CThingNoise noises[4];CTCNoise tc[4];CThingSearchTools tools;tools.calls=0;tools.selected=0;tools.kill=kill!=0;
 const float positions[]={0,2,4,8};
 for(int i=0;i<4;++i){int pattern=(profile>>(2*i))&3;noises[i].pos=C3DVector(positions[i],0,0);noises[i].created=(i==0?8:i==1?12:i==2?12:16);noises[i].dead=pattern==3;noises[i].scan=pattern!=2;tc[i].creator=pattern==1?&other:&perceived;tc[i].range=i==1?1.0f:8.0f;noises[i].noise=pattern==2?0:&tc[i];tools.things.push_back(&noises[i]);}
 CWorld world;world.frame=frame_case==0?15:frame_case==1?16:17;CPerceivedThingDef def;def.AwarenessTimer=128.0f;
 CTCPerceiveThing c;c.parent=&parent;c.perceived=&perceived;c.tools=&tools;c.world=&world;c.PDef=&def;c.Heard=true;c.DirectAwareness=direct!=0;c.FramesSinceDirectlyPerceivedThing=27;c.AwarenessTimer=aware?32.0f:0.0f;c.SoundRadius=4.0f;c.LastPerceivedPos=C3DVector(-1,-2,-4);c.perceived_calls=c.last_calls=c.react_calls=0;c.second_null=second_null!=0;
 bool early=village_case>=2&&village_case!=3&&world.frame<village.end;
 int expected=-1;if(!early){for(int i=0;i<4;++i){int pattern=(profile>>(2*i))&3;float bound=i==1?1.0f:4.0f;if(pattern==0&&positions[i]<=bound){if(expected<0||noises[i].created>=noises[expected].created)expected=i;}}}
 c.UpdateHearing();CHECK(!c.Heard==bool(early||expected<0||kill));CHECK(tools.calls==(early?0:1));CHECK(c.react_calls==(early?0:1));CHECK(c.perceived_calls==(early?0:1+((expected>=0&&!kill&&aware&&direct)?1:0)));CHECK(tools.selected==(expected<0?0:&noises[expected]));
 bool heard=!early&&expected>=0&&!kill;bool direct_branch=heard&&aware&&direct;bool set_last=direct_branch&&!second_null;CHECK(c.last_calls==(set_last?1:0));CHECK(c.FramesSinceDirectlyPerceivedThing==(set_last?0:27));CHECK(c.DirectAwareness==bool(direct));
 if(heard&&(!direct_branch||set_last)){const C3DVector&p=direct_branch?perceived.pos:noises[expected].pos;CHECK(c.LastPerceivedPos.X==p.X&&c.LastPerceivedPos.Y==p.Y&&c.LastPerceivedPos.Z==p.Z);CHECK(c.AwarenessTimer==128.0f);}else{CHECK(c.LastPerceivedPos.X==-1&&c.LastPerceivedPos.Y==-2&&c.LastPerceivedPos.Z==-4);CHECK(c.AwarenessTimer==(aware?32.0f:0.0f));}
 for(int i=0;i<4;++i){CHECK(noises[i].created==(i==0?8:i==1?12:i==2?12:16));CHECK(noises[i].pos.X==positions[i]&&noises[i].pos.Y==0&&noises[i].pos.Z==0);}
}
int main(){unsigned long cases=0;for(int v=0;v<5;++v)for(int f=0;f<3;++f)for(int p=0;p<256;++p)for(int a=0;a<2;++a)for(int d=0;d<2;++d)for(int n=0;n<2;++n)for(int k=0;k<2;++k){one(v,f,p,a,d,n,k);if(failed){printf("00744fb0_TEST FAIL line%d case%lu\n",line_failed,cases);return 1;}++cases;}if(cases!=61440)return 2;puts("00744fb0_TEST PASS");return 0;}