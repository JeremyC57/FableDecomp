// Standalone C++03 behavioral fixture; production leaf body is copied verbatim below.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <utility>
#define CHECK(x) do { if(!(x)){printf("failure line %d case %lu\n",__LINE__,cases);exit(1);} }while(0)
static unsigned long cases;
class C3DVector { public:float X,Y,Z; C3DVector(){} C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} };
class CCharString { public:int Key; CCharString(int k=0):Key(k){} };
class CWeaponDef { public:bool UseExtendedProjectileTrails;CCharString ProjectileWeaponHelperNameTip,ProjectileWeaponHelperNameBase; CWeaponDef():UseExtendedProjectileTrails(false),ProjectileWeaponHelperNameTip(1),ProjectileWeaponHelperNameBase(2){} };
static int refs;
template<class T> class CDefPointer { public:T* Object;CDefPointer():Object(0){}~CDefPointer(){if(Object)--refs;}T* operator->()const{return Object;} };
class CThingBaseDef { public:const CWeaponDef* Def;bool GetSubDef(CDefPointer<const CWeaponDef>* p)const{p->Object=Def;if(Def)++refs;return Def!=0;} };
class CCreatureActionBase { public:bool Strike;virtual bool IsStrikeEventActive()const{return Strike;} virtual bool IsStrikeAction()const{return !Strike;} };
class CTCTrapBase { public:bool Open;bool IsOpen()const{return Open;} };
class CThing;
class CTCCarryable { public:const CThing* By;const CThing* GetPCarriedByThing()const{return By;} };
namespace NThingShot {class CTCShotDefinitionBase { public:};}
static bool tip_ok,base_ok;
static int helper_calls,interpolation_calls,initialise_calls,activity_calls,pair_calls;
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
 bool GetHelperPoint(const CCharString& name,long index,unsigned long flags,C3DVector& pos,float f)const {
  CHECK(index==0&&flags==0);++helper_calls;pos=C3DVector((float)name.Key+f,4.0f,8.0f);return name.Key==1?tip_ok:base_ok;
 }
};
}
class CThing { public:
 bool Alive,Shot;unsigned long TypeFlags;CThingBaseDef Base;CTCTrapBase* Trap;CTCCarryable* Carry;NGraphicAppearance::CTCGraphicAppearance Graphic;
 CThing():Alive(true),Shot(false),TypeFlags(0),Trap(0),Carry(0){Base.Def=0;}
 bool GetTC(const NThingShot::CTCShotDefinitionBase** out){static NThingShot::CTCShotDefinitionBase s;if(Shot)*out=&s;return Shot;}
 bool PeekTC(const CTCTrapBase** out)const{if(Trap)*out=Trap;return Trap!=0;}
 bool PeekTC(const CTCCarryable** out)const{if(Carry)*out=Carry;return Carry!=0;}
 unsigned long GetTypeFlags()const{return TypeFlags;}
 const CThingBaseDef& PeekBaseDef()const{return Base;}
 NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return &Graphic;}
};
class CThingCreatureBase: public CThing { public:
 bool Performing;CCreatureActionBase Action;
 CThingCreatureBase():Performing(false){Action.Strike=false;}
 bool IsPerformingAnyAction()const{return Performing;}
 const CCreatureActionBase* PeekPCurrentAction()const{return &Action;}
};
template<class P,class T> T* GFDebugPDynamicCast(P p,T*){return static_cast<T*>(p);}
template<class T> bool IS_THING_ALIVE(T p){return p&&p->Alive;}
class CWorld { public:static long Frame;static long GetFrame(){return Frame;}};long CWorld::Frame=12;
class CMainGameComponent{ public:static long GetConstantFPS(){return 4;}};
class CGlobalDef{ public:long WeaponTrialNumFramesToFadeOut;};
class CGameDefinitionManager{ public:CGlobalDef Def;const CGlobalDef& PeekGlobalDef()const{return Def;}};
struct CEnginePrimitiveWeaponTrail {class CWeaponTrailEntry{ public:C3DVector Top,Bottom;float CreationTime;CWeaponTrailEntry(const C3DVector& a,const C3DVector& b,float t):Top(a),Bottom(b),CreationTime(t){}};};
namespace {
float TRAIL_EXISTENCE_TIME_SECONDS=1.0f;
long NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE=2;
long MAX_PROJECTILE_TRAIL_SIZE=2;
std::pair<C3DVector,C3DVector> GetProjectileTrailElementFromPoint(const C3DVector& p){++pair_calls;return std::pair<C3DVector,C3DVector>(p,C3DVector(p.X+2.0f,p.Y+2.0f,p.Z+2.0f));}
}
class CTCWeaponTrail {
public:
 typedef CEnginePrimitiveWeaponTrail::CWeaponTrailEntry Entry;
 long NoTrails;std::vector<unsigned char> ActiveTrail;std::vector<long> InactiveTicks;std::vector<std::list<Entry> > Trail;std::list<Entry> ProjectileWeaponTrail;long MinProjectileTrailTimer;bool WeaponTrailActive;
 CThingCreatureBase* Parent;CGameDefinitionManager Manager;CWorld World;unsigned ActiveMask;
 CTCWeaponTrail():NoTrails(0),MinProjectileTrailTimer(-1),WeaponTrailActive(false),Parent(0),ActiveMask(0){Manager.Def.WeaponTrialNumFramesToFadeOut=2;}
 CThing& GetParentThing(){return *Parent;}
 const CWorld& PeekWorld()const{return World;}
 const CGameDefinitionManager& GetDefinitionManager()const{return Manager;}
 void InitialiseTrails(){++initialise_calls;}
 bool IsTrailActiveForThing(long n,const CThing* trap,const CThingCreatureBase* creature)const{++activity_calls;return (trap||creature)&&(ActiveMask&(1u<<n));}
 void CalculateInterpolatedWeaponHelpers(long n,float f,C3DVector& top,C3DVector& bottom)const{++interpolation_calls;top=C3DVector((float)(10*n)+f,20,30);bottom=C3DVector((float)(10*n)+f+2,22,32);}
 void UpdateWeaponTrails();
};
void CTCWeaponTrail::UpdateWeaponTrails()
{
 if(NoTrails==-1){InitialiseTrails();return;}
 float time=((float)PeekWorld().GetFrame())/CMainGameComponent::GetConstantFPS();
 const NThingShot::CTCShotDefinitionBase* ptc_shot_defn;
 if(GetParentThing().GetTC(&ptc_shot_defn)) {
  NGraphicAppearance::CTCGraphicAppearance* ptc_graph=GetParentThing().GetTCGraphicAppearance();
  CDefPointer<const CWeaponDef> pweapon_def;
  if(GetParentThing().PeekBaseDef().GetSubDef(&pweapon_def)) {
   bool extended_trails=false;
   if(pweapon_def->UseExtendedProjectileTrails) {
    float base_interp=1.0f;
    C3DVector pos_a;
    bool got_a=ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameTip,0,0,pos_a,base_interp);
    C3DVector pos_b;
    bool got_b=ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameBase,0,0,pos_b,base_interp);
    if(got_a&&got_b)ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(pos_a,pos_b,time));
   } else {
    if(ProjectileWeaponTrail.empty()) {
     float base_interp=0.0f;C3DVector pos_b;
     if(ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameBase,0,0,pos_b,base_interp)) {
      std::pair<C3DVector,C3DVector> element=GetProjectileTrailElementFromPoint(pos_b);
      ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(element.first,element.second,time));
     }
    }
    float tip_interp=1.0f;C3DVector pos_a;
    if(ptc_graph->GetHelperPoint(pweapon_def->ProjectileWeaponHelperNameTip,0,0,pos_a,tip_interp)) {
     std::pair<C3DVector,C3DVector> element=GetProjectileTrailElementFromPoint(pos_a);
     float time_fraction=1.0f/CMainGameComponent::GetConstantFPS();
     ProjectileWeaponTrail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(element.first,element.second,time+time_fraction));
    }
   }
  }
  if(ProjectileWeaponTrail.size()>MAX_PROJECTILE_TRAIL_SIZE)ProjectileWeaponTrail.pop_front();
  return;
 }
 if(!ProjectileWeaponTrail.empty()) {
  if(MinProjectileTrailTimer==-1)MinProjectileTrailTimer=15;
  if(MinProjectileTrailTimer--==0)ProjectileWeaponTrail.clear();
 }
 const CThingCreatureBase* pcreature_actioneer=0;
 const CThing* ptrap=0;
 bool quit_out=true;
 const CTCTrapBase* ptc_trap;
 const CTCCarryable* ptc_carryable;
 if(GetParentThing().PeekTC(&ptc_trap)&&ptc_trap->IsOpen())ptrap=&GetParentThing();
 else if(GetParentThing().GetTypeFlags()&8)pcreature_actioneer=GFDebugPDynamicCast(&GetParentThing(),(CThingCreatureBase*)0);
 else if(GetParentThing().PeekTC(&ptc_carryable)) {
  if(IS_THING_ALIVE(ptc_carryable->GetPCarriedByThing())&&(ptc_carryable->GetPCarriedByThing()->GetTypeFlags()&8))
   pcreature_actioneer=GFDebugPDynamicCast(ptc_carryable->GetPCarriedByThing(),(const CThingCreatureBase*)0);
 }
 if(IS_THING_ALIVE(pcreature_actioneer)&&pcreature_actioneer->IsPerformingAnyAction()) {
  const CCreatureActionBase* paction=pcreature_actioneer->PeekPCurrentAction();
  if(!paction->IsStrikeEventActive())pcreature_actioneer=0;
 }else pcreature_actioneer=0;
 if(ptrap||pcreature_actioneer)WeaponTrailActive=true;
 if(!WeaponTrailActive)return;
 bool active_elements=false;
 C3DVector top_pos,bottom_pos;
 long c1=0;
 if(c1<NoTrails) {
 for(;c1<NoTrails;++c1) {
  bool active=IsTrailActiveForThing(c1,ptrap,pcreature_actioneer);
  std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>& current_trail=Trail[c1];
  const CGlobalDef& glob_def=GetDefinitionManager().PeekGlobalDef();
  if(!active)ActiveTrail[c1] ? InactiveTicks[c1]=0 : InactiveTicks[c1]++;
  if(active||InactiveTicks[c1]<glob_def.WeaponTrialNumFramesToFadeOut) {
   if(!ActiveTrail[c1]&&active)current_trail.clear();
   float fraction=0.0f;
   float time_fraction=1.0f/CMainGameComponent::GetConstantFPS();
   for(long c2=0;c2<NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;++c2) {
    fraction+=1.0f/NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE;
    CalculateInterpolatedWeaponHelpers(c1,fraction,top_pos,bottom_pos);
    current_trail.push_back(CEnginePrimitiveWeaponTrail::CWeaponTrailEntry(top_pos,bottom_pos,time+time_fraction*fraction));
   }
  }
  ActiveTrail[c1]=active;
  while(!current_trail.empty()&&time-current_trail.front().CreationTime>TRAIL_EXISTENCE_TIME_SECONDS)current_trail.pop_front();
  if(!current_trail.empty())active_elements=true;
 }
 }
 if(!active_elements)WeaponTrailActive=false;
}

static void reset_counts(){helper_calls=interpolation_calls=initialise_calls=activity_calls=pair_calls=0;CHECK(refs==0);}
static bool vector_equal(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
static void same(const std::list<CTCWeaponTrail::Entry>&a,const std::list<CTCWeaponTrail::Entry>&b){CHECK(a.size()==b.size());std::list<CTCWeaponTrail::Entry>::const_iterator i=a.begin(),j=b.begin();for(;i!=a.end();++i,++j){CHECK(vector_equal(i->Top,j->Top));CHECK(vector_equal(i->Bottom,j->Bottom));CHECK(i->CreationTime==j->CreationTime);}}
static CTCWeaponTrail::Entry seed(float t){return CTCWeaponTrail::Entry(C3DVector(91,92,93),C3DVector(94,95,96),t);}
static void expected_projectile(std::list<CTCWeaponTrail::Entry>&out,bool ext,int name,float f,float when){C3DVector p((float)name+f,4,8);if(ext)out.push_back(CTCWeaponTrail::Entry(C3DVector(2,4,8),C3DVector(3,4,8),when));else out.push_back(CTCWeaponTrail::Entry(p,C3DVector(p.X+2,p.Y+2,p.Z+2),when));}
int main(){
 CWorld::Frame=12;
 // Every projectile path, all helper outcomes, empty/nonempty lists and size caps.
 for(int ready=0;ready<2;++ready)for(int def=0;def<2;++def)for(int ext=0;ext<2;++ext)for(int flags=0;flags<4;++flags)for(int initial=0;initial<5;++initial)for(int cap=0;cap<5;++cap){
  ++cases;reset_counts();CThingCreatureBase parent;parent.Shot=true;CWeaponDef weapon;weapon.UseExtendedProjectileTrails=ext!=0;parent.Base.Def=def?&weapon:0;
  tip_ok=(flags&1)!=0;base_ok=(flags&2)!=0;MAX_PROJECTILE_TRAIL_SIZE=cap;
  CTCWeaponTrail t;t.Parent=&parent;t.NoTrails=ready?0:-1;t.WeaponTrailActive=true;t.MinProjectileTrailTimer=7;
  for(int i=0;i<initial;++i)t.ProjectileWeaponTrail.push_back(seed((float)i));
  std::list<CTCWeaponTrail::Entry> expected=t.ProjectileWeaponTrail;int calls=0,pairs=0;
  if(ready){if(def){if(ext){calls=2;if(flags==3)expected_projectile(expected,true,0,0,3);}else{if(initial==0){++calls;if(base_ok){expected_projectile(expected,false,2,0,3);++pairs;}}++calls;if(tip_ok){expected_projectile(expected,false,1,1,3.25f);++pairs;}}}if(expected.size()>(unsigned)cap)expected.pop_front();}
  t.UpdateWeaponTrails();same(t.ProjectileWeaponTrail,expected);CHECK(helper_calls==calls&&pair_calls==pairs);CHECK(initialise_calls==!ready);CHECK(t.MinProjectileTrailTimer==7&&t.WeaponTrailActive);CHECK(activity_calls==0&&interpolation_calls==0&&refs==0);
 }
 // Parent/holder/trap/action gates, independent active and previous-active bits,
 // fading, expiration boundaries, projectile timer and interpolation counts.
 const long counts[4]={0,1,2,4};const long timers[4]={-1,0,1,15};
 for(unsigned gate=0;gate<16;++gate)for(unsigned mask=0;mask<8;++mask)for(unsigned old=0;old<8;++old)for(int fade=0;fade<3;++fade)for(int count=0;count<4;++count)for(int timer=0;timer<4;++timer){
  ++cases;reset_counts();CThingCreatureBase parent,holder;CTCTrapBase trap;CTCCarryable carry;
  bool is_trap=gate==1||gate==2;trap.Open=gate==1;parent.Trap=is_trap?&trap:0;
  parent.TypeFlags=(gate>=3&&gate<=7)?8:0;parent.Alive=gate!=4;parent.Performing=gate!=5;parent.Action.Strike=gate!=6;
  parent.Carry=gate>=8?&carry:0;carry.By=gate==8?0:&holder;holder.Alive=gate!=9;holder.TypeFlags=gate==10?0:8;holder.Performing=gate!=11;holder.Action.Strike=gate!=12;
  bool trap_reason=gate==1;bool creature_reason=(gate==3||gate==7||gate>=13);bool reason=trap_reason||creature_reason;
  CTCWeaponTrail t;t.Parent=&parent;t.NoTrails=3;t.WeaponTrailActive=(gate&1)!=0;t.ActiveMask=mask;t.MinProjectileTrailTimer=timers[timer];t.Manager.Def.WeaponTrialNumFramesToFadeOut=fade;
  NO_TRAIL_ELEMENTS_PER_WORLD_UPDATE=counts[count];t.ActiveTrail.resize(3);t.InactiveTicks.resize(3);t.Trail.resize(3);
  for(int k=0;k<3;++k){t.ActiveTrail[k]=(old>>k)&1;t.InactiveTicks[k]=k;t.Trail[k].push_back(seed(0));t.Trail[k].push_back(seed(2));t.Trail[k].push_back(seed(3));}
  t.ProjectileWeaponTrail.push_back(seed(0));std::vector<std::list<CTCWeaponTrail::Entry> > expect=t.Trail;std::vector<long> ticks=t.InactiveTicks;std::vector<unsigned char> active=t.ActiveTrail;
  bool run=t.WeaponTrailActive||reason;int samples=0;bool any=false;
  if(run)for(int k=0;k<3;++k){bool now=reason&&((mask>>k)&1);if(!now)ticks[k]=active[k]?0:ticks[k]+1;
    if(now||ticks[k]<fade){if(!active[k]&&now)expect[k].clear();for(long sample=1;sample<=counts[count];++sample){float fraction=(float)sample/(float)counts[count];expect[k].push_back(CTCWeaponTrail::Entry(C3DVector((float)(10*k)+fraction,20,30),C3DVector((float)(10*k)+fraction+2,22,32),3+0.25f*fraction));++samples;}}
    active[k]=(unsigned char)now;
    while(!expect[k].empty()&&expect[k].front().CreationTime<2.0f)expect[k].pop_front();
    if(!expect[k].empty())any=true;
  }
  t.UpdateWeaponTrails();CHECK(t.WeaponTrailActive==(run&&any));CHECK(activity_calls==(run?3:0));CHECK(interpolation_calls==samples&&helper_calls==0&&initialise_calls==0&&refs==0);
  CHECK(t.MinProjectileTrailTimer==(timers[timer]==-1?14:timers[timer]-1));CHECK(t.ProjectileWeaponTrail.size()==(timers[timer]==0?0u:1u));
  for(int k=0;k<3;++k){same(t.Trail[k],expect[k]);CHECK(t.ActiveTrail[k]==active[k]&&t.InactiveTicks[k]==ticks[k]);}
 }
 // Empty projectile list leaves its timer alone; empty/no regular trails deactivate.
 for(int n=0;n<4;++n){++cases;reset_counts();CThingCreatureBase parent;CTCWeaponTrail t;t.Parent=&parent;t.WeaponTrailActive=true;t.NoTrails=n;t.ActiveTrail.resize(n,0);t.InactiveTicks.resize(n,8);t.Trail.resize(n);t.MinProjectileTrailTimer=0;t.UpdateWeaponTrails();CHECK(t.MinProjectileTrailTimer==0);CHECK(!t.WeaponTrailActive);CHECK(interpolation_calls==0&&activity_calls==n);}
 puts("0076efa0_TEST PASS");return 0;
}