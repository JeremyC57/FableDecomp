#include <stdio.h>
#include <stdlib.h>
static unsigned trial;
#define CHECK(x) do {if(!(x)){printf("case %u line %d\n",trial,__LINE__);exit(1);}}while(0)
namespace std {
template<class A,class B>struct pair{A first;B second;};
template<class T>struct vector{T v[12];unsigned n;vector():n(0){}unsigned size()const{return n;}T&operator[](unsigned i){CHECK(i<n);return v[i];}};
template<class T>struct list{T v[12];unsigned n;typedef T*iterator;list():n(0){}bool empty()const{return !n;}T*begin(){return v;}T*end(){return v+n;}};
}
struct C2DCoordI{long X,Y;};struct C3DVector{float X,Y,Z;C2DCoordI ToC2DCoordI()const{C2DCoordI v={(long)X,(long)Y};return v;}};
struct C2DBoxI{long x,y,r;C2DBoxI(C2DCoordI p,long rad):x(p.X),y(p.Y),r(rad){}};
enum{THING_TYPE_PLAYER_CREATURE=2,TCI_SHOT_DEFINITION=119};struct CTCPhysicsBase{float radius;float GetRadius()const{return radius;}};
struct CThing;struct CTCCarrying{CThing* w;CThing* GetThingInPrimarySlot()const{return w;}};
struct CTCWeapon{bool melee;bool IsMeleeWeapon()const{return melee;}};
struct CThing{bool live,shot;long flags,type;C3DVector pos;CTCPhysicsBase physics;CTCCarrying*c;CTCWeapon*w;
 long GetType(){return type;}long GetTypeFlags(){return flags;}bool HasTC(int t){CHECK(t==119);return shot;}
 CTCPhysicsBase*GetPhysicsTC(){return &physics;}const C3DVector&GetPos(){return pos;}const C3DVector&PeekPos(){return pos;}
 bool PeekTC(const CTCCarrying**pp){*pp=c;return c!=0;}bool PeekTC(const CTCWeapon**pp){*pp=w;return w!=0;}};
struct CThingGameObject:CThing{};struct CThingCreatureBase:CThingGameObject{};
template<class T>bool IS_THING_ALIVE(T*p){return p&&p->live;}
struct CRegionDef{bool CheckAllThingsOnMapForCombatCollision;};
template<class T>struct CDefPointer{T*p;bool IsNull()const{return !p;}T*operator->()const{return p;}};
struct CRegion{CDefPointer<const CRegionDef> ptr;CDefPointer<const CRegionDef>GetPRegionDef()const{return ptr;}};
struct CWorldMap{CRegion region;int reads;long GetLoadedRegion(){++reads;return 13;}const CRegion&GetRegion(long i){CHECK(i==13);++reads;return region;}};
struct CThingFilter_IsNot{const CThing&t;CThingFilter_IsNot(const CThing&a):t(a){}};
struct CThingFilter_CombatIsStrikeToStrike{const CThing&t;CThingFilter_CombatIsStrikeToStrike(const CThing&a):t(a){}};
struct CThingFilter_CombatOverlappingSphere{const C3DVector&p;float radius;CThingFilter_CombatOverlappingSphere(const C3DVector&a,float r):p(a),radius(r){}};
struct CThingFilter_CombatCanCollideOrInteractWithThing{const CTCPhysicsBase&p;bool a,b;CThingFilter_CombatCanCollideOrInteractWithThing(const CTCPhysicsBase&t,bool x,bool y):p(t),a(x),b(y){}};
template<class A,class B,class C,class D>struct CFilter_Bind4{const A&a;const B&b;const C&c;const D&d;CFilter_Bind4(const A&w,const B&x,const C&y,const D&z):a(w),b(x),c(y),d(z){}};
static CThingCreatureBase actor;static CThingGameObject objs[6];static unsigned nobj,points,calls,extension;static bool prone;
static float radius_sum,expected_attack;static int kind,which[96],who[96],seg[96];
struct Search{template<class T,class F>void fill(const F&f,std::list<T*>&out)const{
 CHECK(&f.a.t==&actor&&&f.b.t==&actor&&&f.c.p==&actor.pos&&&f.d.p==&actor.physics);
 CHECK(f.c.radius==(radius_sum+5)*1.25f&&f.d.a==!prone&&!f.d.b);for(unsigned j=0;j<nobj;++j)out.v[out.n++]=&objs[j];}
 template<class T,class F>void GetAllThings(const F&f,std::list<T*>&o)const{CHECK(!kind);kind=1;fill(f,o);}
 template<class T,class F>void GetAllThingsInRoughArea(const C2DBoxI&b,const F&f,std::list<T*>&o)const{CHECK(!kind);kind=2;CHECK(b.x==(long)actor.pos.X&&b.y==(long)actor.pos.Y&&b.r==(long)((radius_sum+actor.physics.radius+2)*1.2f));fill(f,o);}};
typedef std::vector<std::pair<C3DVector,C3DVector> > Trail;
struct CTCWeaponTrail{void GetLastWeaponTrailPoints(Trail&v,long idx){CHECK(idx==19);++calls;v.n=points;for(unsigned j=0;j<v.n;++j){v.v[j].first.X=(float)j;v.v[j].second.Y=(float)(100+j);}}};
static void ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(Trail&v,const CThing&t){CHECK(&t==&actor&&v.n==points);++extension;}
struct CCombatActionBase{CThingCreatureBase*PCreature;CThing*PTarget;CTCWeaponTrail*trail;CWorldMap map;Search tools;int n;
 CWorldMap&GetWorldMap(){return map;}const Search&PeekThingSearchTools()const{return tools;}bool IsToHitProneThings()const{return prone;}
 CTCWeaponTrail*GetWeaponTrailUsedInStrikeAction(long&idx){idx=19;return trail;}
 void event(int t,float r,const std::pair<C3DVector,C3DVector>&a,const std::pair<C3DVector,C3DVector>&b,CThingGameObject&h){
 CHECK(r==expected_attack&&a.first.X+1==b.first.X&&b.second.Y==b.first.X+100);CHECK(n<96);which[n]=t;who[n]=(int)(&h-objs);seg[n++]=(int)b.first.X;}
#define EVENT(name,tag) void ProcessPossible##name##StrikeEvent(float r,const std::pair<C3DVector,C3DVector>&a,const std::pair<C3DVector,C3DVector>&b,CThingGameObject&h){event(tag,r,a,b,h);}
 EVENT(Creature,1) EVENT(ProneThings,2) EVENT(Shot,3) EVENT(Object,4)
 void PerformPossibleDamageAction(float);};
void CCombatActionBase::PerformPossibleDamageAction(float attack_point_radius)
{
 if(IS_THING_ALIVE(PCreature)) {
  CThingCreatureBase& attacker=*PCreature;
  bool perform_extended_check=(attacker.GetType()==THING_TYPE_PLAYER_CREATURE);
  std::vector<std::pair<C3DVector,C3DVector> > trail_list;
  std::list<CThingGameObject*> thing_list;
  float target_radius; if(IS_THING_ALIVE(PTarget)) target_radius=PTarget->GetPhysicsTC()->GetRadius(); else target_radius=0.0f;
  float creature_radius=PCreature->GetPhysicsTC()->GetRadius();
  float actual_radius=((creature_radius+1.0f)*2.0f+target_radius)*1.2f;
  long region_index=GetWorldMap().GetLoadedRegion();
  const CRegion& region=GetWorldMap().GetRegion(region_index);
  bool hit_dead_creatures=false;
  if(IsToHitProneThings())hit_dead_creatures=true;
  CThingFilter_IsNot filter1(*PCreature);
  CThingFilter_CombatIsStrikeToStrike filter2(*PCreature);
  CThingFilter_CombatOverlappingSphere filter3(attacker.GetPos(),(creature_radius+5.0f+target_radius)*1.25f);
  CThingFilter_CombatCanCollideOrInteractWithThing filter4(*PCreature->GetPhysicsTC(),!hit_dead_creatures,false);
  CFilter_Bind4<CThingFilter_IsNot,CThingFilter_CombatIsStrikeToStrike,CThingFilter_CombatOverlappingSphere,CThingFilter_CombatCanCollideOrInteractWithThing> filter_main(filter1,filter2,filter3,filter4);
  CDefPointer<const CRegionDef> pregion_def=region.GetPRegionDef();
  if(pregion_def.IsNull()||!pregion_def->CheckAllThingsOnMapForCombatCollision){
   C2DBoxI area(PCreature->PeekPos().ToC2DCoordI(),(long)actual_radius);
   PeekThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(area,filter_main,thing_list);
  }else PeekThingSearchTools().GetAllThings<CThingGameObject>(filter_main,thing_list);
  if(thing_list.empty())return;
  long strike_event_index;
  CTCWeaponTrail* tc_weapon_trail=GetWeaponTrailUsedInStrikeAction(strike_event_index);
  if(tc_weapon_trail){
   tc_weapon_trail->GetLastWeaponTrailPoints(trail_list,strike_event_index);
   const CTCCarrying* ptc_carrying;
   if(PCreature->PeekTC(&ptc_carrying)){
    CThing* pthing=ptc_carrying->GetThingInPrimarySlot();
    const CTCWeapon* ptc_weapon;
    if(IS_THING_ALIVE(pthing)&&pthing->PeekTC(&ptc_weapon)&&ptc_weapon->IsMeleeWeapon())
     ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(trail_list,*PCreature);
   }
   if(false){}else for(std::list<CThingGameObject*>::iterator it=thing_list.begin();it!=thing_list.end();++it){
    CThingGameObject& possible_hit_thing=**it;
    if(false){}else for(long c1=1;c1<trail_list.size();++c1){
     long type_flags=possible_hit_thing.GetTypeFlags();
     if(type_flags&8)ProcessPossibleCreatureStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else if(IsToHitProneThings())ProcessPossibleProneThingsStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else if(type_flags&0x80)continue;
     else if(possible_hit_thing.HasTC(TCI_SHOT_DEFINITION))ProcessPossibleShotStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
     else ProcessPossibleObjectStrikeEvent(attack_point_radius,trail_list[c1-1],trail_list[c1],possible_hit_thing);
    }
   }
  }
 }
}
int main(){for(trial=0;trial<32768;++trial){unsigned m=trial*2654435761u;actor.live=(m&31)!=1;actor.type=(m&1)?2:7;actor.physics.radius=(float)(m%13);
 actor.pos.X=(float)((long)(m%97)-54)+0.5f;actor.pos.Y=(float)((long)(m%79)-38)-0.5f;
 CThing target,weapon;target.live=(m&128)!=0;target.physics.radius=(float)((m>>6)%19);weapon.live=(m&256)!=0;CTCCarrying carrying;CTCWeapon weap;
 carrying.w=m&512?&weapon:0;weap.melee=(m&1024)!=0;weapon.w=m&2048?&weap:0;actor.c=m&4096?&carrying:0;
 CCombatActionBase c;c.PCreature=(m%19)?&actor:0;c.PTarget=(m%7)?&target:0;radius_sum=actor.physics.radius+(c.PTarget&&target.live?target.physics.radius:0);
 CRegionDef rd;rd.CheckAllThingsOnMapForCombatCollision=(m&64)!=0;c.map.region.ptr.p=m&32?&rd:0;c.map.reads=0;
 CTCWeaponTrail trail;c.trail=m%5?&trail:0;nobj=(m>>16)%7;points=(m>>19)%8;prone=(m&8)!=0;
 for(unsigned j=0;j<nobj;++j){objs[j].flags=((j+m)%4==0?8:0)|((j+m)%5==0?128:0);objs[j].shot=((m+j)&1)!=0;}
 expected_attack=(float)(m%41);calls=extension=0;kind=c.n=0;c.PerformPossibleDamageAction(expected_attack);
 bool alive=c.PCreature&&actor.live;CHECK(c.map.reads==(alive?2:0));CHECK(kind==(alive?(c.map.region.ptr.p&&rd.CheckAllThingsOnMapForCombatCollision?1:2):0));
 bool active=alive&&nobj&&c.trail;CHECK(calls==(active?1u:0u));CHECK(extension==((active&&actor.c&&carrying.w&&weapon.live&&weapon.w&&weap.melee)?1u:0u));
 int n=0;if(active)for(unsigned j=0;j<nobj;++j){int t=(objs[j].flags&8)?1:prone?2:(objs[j].flags&128)?0:objs[j].shot?3:4;if(t)for(unsigned i=1;i<points;++i){CHECK(n<c.n&&which[n]==t&&who[n]==(int)j&&seg[n]==(int)i);++n;}}
 CHECK(n==c.n);
 }puts("008591e0_TEST PASS");return 0;}