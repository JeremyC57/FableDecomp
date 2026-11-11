// Self-contained C++03 dependency fixture. Selected production body is copied below.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
static long cases=0;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"case %ld line %d: %s\n",cases,__LINE__,#x);std::exit(1);}}while(0)
struct C3DVector {float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}float GetMagnitude2D()const{return(float)std::sqrt(X*X+Y*Y);}};
struct CThingCreatureBase;struct CTCPerceiveThing;struct CTCTurncoat;
struct CThing {bool dead,incap,enemy,valid,noticed;unsigned long flags;int score;C3DVector pos;U64 uid;CThing():dead(false),incap(false),enemy(true),valid(true),noticed(true),flags(8),score(0),uid(0){}bool IsDead()const{return dead;}unsigned long GetTypeFlags()const{return flags;}const C3DVector&GetPos()const{return pos;}U64 GetUniqueID()const{return uid;}};
template<class T>struct CIntelligentPointer {T*p;CIntelligentPointer(T*q=0):p(q){}T*Get()const{return p;}T*operator->()const{return p;}T&operator*()const{return*p;}CIntelligentPointer&operator=(T*q){p=q;return*this;}};
template<class T>bool operator!=(const CIntelligentPointer<T>&p,int){return p.Get()!=0;}template<class T>bool operator!=(const CIntelligentPointer<T>&p,const T*q){return p.Get()!=q;}template<class T>bool IS_THING_ALIVE(const T&p){return p!=0&&!p->IsDead();}
struct CThingCreatureBase:CThing{CThingCreatureBase*last;CThingCreatureBase():last(0){}CThingCreatureBase*GetPLastAttackedByCreature()const{return last;}};
struct CThingAICreature:CThingCreatureBase{CTCPerceiveThing*perception;CTCTurncoat*turncoat;CThingAICreature():perception(0),turncoat(0){}bool PeekTC(const CTCPerceiveThing**out)const{*out=perception;return perception!=0;}bool PeekTC(const CTCTurncoat**out)const{if(!turncoat)return false;*out=turncoat;return true;}};
struct CThingPlayerCreature:CThingCreatureBase{};
struct CTCPerceiveThing{float radius;bool aware;const CThing*thing;CTCPerceiveThing():radius(8),aware(false),thing(0){}float GetExtendedSightRadius()const{return radius;}bool IsAwareOfThing()const{return aware;}const CThing*GetPerceivedThing()const{return thing;}};
struct CTCTurncoat{bool active;bool IsTurncoated()const{return active;}};
namespace NCreatureMode {bool IsIncapacitated(const CThing&t){return t.incap;}}
struct CTCEnemy {static bool AreThingsEnemies(const CThing&,const CThing&t){return t.enemy;}};
template<class A,class B>B GFDebugPDynamicCast(A a,B){return static_cast<B>(a);}
struct CTargetingDef{float PreferPlayerWeight,PreferNonCombatantsWeight,PreferLastAttackerWeight,PreferCurrentBestEnemyWeight;CTargetingDef():PreferPlayerWeight(1),PreferNonCombatantsWeight(2),PreferLastAttackerWeight(4),PreferCurrentBestEnemyWeight(8){}};
static const CThingAICreature*expected_parent;
struct CThingFilter_In2DRange{const C3DVector&pos;float radius;CThingFilter_In2DRange(const C3DVector&p,float r):pos(p),radius(r){}bool operator()(const CThing&t)const{float x=t.pos.X-pos.X,y=t.pos.Y-pos.Y;return x*x+y*y<=radius*radius;}};
struct CThingFilter_IsValidAttackTargetForCreature{const std::vector<const CThing*>&excluded;bool awareness;CThingFilter_IsValidAttackTargetForCreature(const CThingAICreature&p,const std::vector<const CThing*>&e,bool a):excluded(e),awareness(a){CHECK(&p==expected_parent);}bool operator()(const CThingCreatureBase&t)const{if(!t.valid||(awareness&&!t.noticed))return false;for(unsigned i=0;i<excluded.size();++i)if(excluded[i]==&t)return false;return true;}};
template<class A,class B>struct CFilter_Bind2{const A&a;const B&b;CFilter_Bind2(const A&x,const B&y):a(x),b(y){}template<class T>bool operator()(const T&t)const{return a(t)&&b(t);}};
template<class A,class B>CFilter_Bind2<A,B>GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
struct CThingCompare_BestCreatureToAttack{CThingCompare_BestCreatureToAttack(const CThingCreatureBase&p,const CThing*,float a,float b,float c,float d){CHECK(&p==expected_parent&&a==1&&b==2&&c==4&&d==8);}template<class T>T*operator()(T*a,T*b)const{if(!a)return b;if(!b)return a;return a->score>b->score?a:b;}};
struct CThingSearchTools{std::vector<CThingAICreature*>pool;mutable int searches;CThingSearchTools():searches(0){}template<class F,class C,class T>bool GetPBestLeafThing(const F&filter,const C&compare,T**out)const{++searches;T*best=0;for(unsigned i=0;i<pool.size();++i)if(filter(*pool[i]))best=compare(best,pool[i]);*out=best;return best!=0;}};
struct CPlayer{CThingPlayerCreature*controlled,*player;CThingPlayerCreature*GetPControlledCreature()const{return controlled;}CThingPlayerCreature*GetPPlayerCharacter()const{return player;}};
struct CPlayerManager{CPlayer p;CPlayer&GetMainPlayer(){return p;}};
struct CMainGameComponent{static CMainGameComponent*current;CPlayerManager pm;static CMainGameComponent*Get(){return current;}CPlayerManager&GetPlayerManager(){return pm;}static long GetConstantFPS(){return 4;}};CMainGameComponent*CMainGameComponent::current=0;
struct CWorld{long frame;long GetFrame()const{return frame;}};
struct CWorldMap{bool in;bool IsPosInMap(const C3DVector&)const{return in;}};
struct CPlayerGui{int updates;static CPlayerGui instance;static CPlayerGui&Get(){return instance;}void UpdateMiniMapMarkers(bool full){CHECK(!full);++updates;}};CPlayerGui CPlayerGui::instance;
struct CTCTargetingAI{CThingAICreature&parent;CIntelligentPointer<CThingCreatureBase>PBestEnemyCreatureForAttack,PScriptTargetOverride;std::list<CIntelligentPointer<CThing> >ThingsToNotAttack;CWorld world;CWorldMap map;CThingSearchTools tools;CTargetingDef def;CTCTargetingAI(CThingAICreature&p):parent(p){}CThingAICreature&GetParentThing(){return parent;}const CWorldMap&PeekWorldMap()const{return map;}CWorld&GetWorld(){return world;}const CTargetingDef&GetTargetingDef()const{return def;}CPlayerManager&GetPlayerManager(){return CMainGameComponent::current->pm;}const CThingSearchTools&PeekThingSearchTools()const{return tools;}void UpdatePBestEnemyCreatureForAttack(bool);};

void CTCTargetingAI::UpdatePBestEnemyCreatureForAttack(bool force_update)
{
 const C3DVector& pos=GetParentThing().GetPos();
 const CThingAICreature& parent_creature=*GFDebugPDynamicCast(&GetParentThing(),(const CThingAICreature*)0);
 bool overridden_by_script=false;
 CThingCreatureBase* pold_best_enemy=PBestEnemyCreatureForAttack.Get();
 if(IS_THING_ALIVE(PScriptTargetOverride)&&!NCreatureMode::IsIncapacitated(*PScriptTargetOverride)) {
  const CTCTurncoat* ptc_turncoat;
  if(GetParentThing().PeekTC(&ptc_turncoat)&&ptc_turncoat->IsTurncoated()) {
   if(CTCEnemy::AreThingsEnemies(parent_creature,*PScriptTargetOverride)) {
    PBestEnemyCreatureForAttack=PScriptTargetOverride;
    overridden_by_script=true;
   }
  } else {
   PBestEnemyCreatureForAttack=PScriptTargetOverride;
   overridden_by_script=true;
  }
 }
 if(!overridden_by_script) {
  std::vector<const CThing*> exclusion_list;
  if(!ThingsToNotAttack.empty()) {
   exclusion_list.reserve(ThingsToNotAttack.size());
   for(std::list<CIntelligentPointer<CThing> >::iterator it=ThingsToNotAttack.begin();it!=ThingsToNotAttack.end();++it) {
    CThing* pthing=it->Get();
    exclusion_list.push_back(pthing);
   }
  }
  const CTCPerceiveThing* tc_perception;
  parent_creature.PeekTC(&tc_perception);
  float radius=tc_perception->GetExtendedSightRadius();
  if(PeekWorldMap().IsPosInMap(pos)) {
   float dis_to_player=(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature()->GetPos()-pos).GetMagnitude2D();
   long update_frequency_attenuator=1+4*(long)(dis_to_player/radius);
   CThingCreatureBase* pbest_for_attack=PBestEnemyCreatureForAttack.Get();
   if(force_update||((unsigned long)GetWorld().GetFrame()+GetParentThing().GetUniqueID())%(unsigned long)(CMainGameComponent::GetConstantFPS()*update_frequency_attenuator)==0) {
    pbest_for_attack=0;
    float prefer_player_weight=GetTargetingDef().PreferPlayerWeight;
    float prefer_non_combatant_weight=GetTargetingDef().PreferNonCombatantsWeight;
    float pref_last_attacker=GetTargetingDef().PreferLastAttackerWeight;
    float pref_cur_target=GetTargetingDef().PreferCurrentBestEnemyWeight;
    CThingCompare_BestCreatureToAttack comparison_filter(parent_creature,PBestEnemyCreatureForAttack.Get(),prefer_player_weight,prefer_non_combatant_weight,pref_last_attacker,pref_cur_target);
    CThingPlayerCreature* pplayer_creature=GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    CThingAICreature* pthing_ai_creature=0;
    {
     CThingFilter_In2DRange search_range(pos,radius);
     CThingFilter_IsValidAttackTargetForCreature search_attack(parent_creature,exclusion_list,true);
     if(!PeekThingSearchTools().GetPBestLeafThing(GFFilter_Bind2(search_range,search_attack),comparison_filter,&pthing_ai_creature))pthing_ai_creature=0;
    }
    if(IS_THING_ALIVE(pplayer_creature)) {
     CThingFilter_In2DRange range_filter(pos,radius);
     CThingFilter_IsValidAttackTargetForCreature attack_filter(parent_creature,exclusion_list,true);
     if(!range_filter(*pplayer_creature)||!attack_filter(*pplayer_creature))pplayer_creature=0;
    } else pplayer_creature=0;
    if(pplayer_creature&&pthing_ai_creature)pbest_for_attack=comparison_filter.operator()<CThingCreatureBase>(pthing_ai_creature,pplayer_creature);
    else if(pplayer_creature)pbest_for_attack=pplayer_creature;
    else if(pthing_ai_creature)pbest_for_attack=pthing_ai_creature;
   }
   if(PBestEnemyCreatureForAttack!=pbest_for_attack)PBestEnemyCreatureForAttack=pbest_for_attack;
   if(!IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&IS_THING_ALIVE(parent_creature.GetPLastAttackedByCreature())&&CThingFilter_IsValidAttackTargetForCreature(parent_creature,exclusion_list,false)(*parent_creature.GetPLastAttackedByCreature())) {
    CThingCreatureBase* attacker=parent_creature.GetPLastAttackedByCreature();
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    CIntelligentPointer<CThingCreatureBase> target(attacker);
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    PBestEnemyCreatureForAttack=target;
   }
   if(!IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&tc_perception->IsAwareOfThing()) {
    const CThing* pperceived_thing=tc_perception->GetPerceivedThing();
    if(IS_THING_ALIVE(pperceived_thing)&&(pperceived_thing->GetTypeFlags()&8)) {
     const CThingCreatureBase* pCreatureConst=GFDebugPDynamicCast(pperceived_thing,(const CThingCreatureBase*)0);
     CThingCreatureBase* pCreature=const_cast<CThingCreatureBase*>(pCreatureConst);
     if(CThingFilter_IsValidAttackTargetForCreature(parent_creature,exclusion_list,false)(*pCreature))PBestEnemyCreatureForAttack=pCreature;
    }
   }
  }
 }
 if(PBestEnemyCreatureForAttack!=pold_best_enemy) {
  if(IS_THING_ALIVE(pold_best_enemy)&&(pold_best_enemy->GetTypeFlags()&32))CPlayerGui::Get().UpdateMiniMapMarkers(false);
  else if(IS_THING_ALIVE(PBestEnemyCreatureForAttack)&&(PBestEnemyCreatureForAttack->GetTypeFlags()&32))CPlayerGui::Get().UpdateMiniMapMarkers(false);
 }
}

static bool eligible(const CThingCreatureBase*t,const CThing*excluded,bool awareness,bool range){return t&&t->valid&&(!awareness||t->noticed)&&t!=excluded&&(!range||(t->pos.X*t->pos.X+t->pos.Y*t->pos.Y<=64));}
static bool live(const CThing*t){return t&&!t->dead;}
int main(){
 for(unsigned bits=0;bits<32768;++bits){
  ++cases;CThingAICreature parent,ai;CThingCreatureBase old,script,last,perceived;CThingPlayerCreature player,controlled;CTCPerceiveThing perception;CTCTurncoat turncoat;CMainGameComponent game;CMainGameComponent::current=&game;CTCTargetingAI state(parent);expected_parent=&parent;CPlayerGui::instance.updates=0;
  parent.perception=&perception;turncoat.active=(bits&2)!=0;parent.turncoat=(bits&1)?&turncoat:0;state.map.in=(bits&4)!=0;
  state.PScriptTargetOverride=(bits&8)?&script:0;script.dead=(bits&16)!=0;script.incap=(bits&32)!=0;script.enemy=(bits&64)!=0;
  old.dead=(bits&128)!=0;old.flags=(bits&256)?40:8;CThingCreatureBase*old_ptr=(bits&512)?&old:0;state.PBestEnemyCreatureForAttack=old_ptr;
  player.dead=(bits&1024)!=0;player.noticed=(bits&2048)!=0;player.score=6;player.flags=40;game.pm.p.player=(bits&4096)?&player:0;game.pm.p.controlled=&controlled;
  ai.score=(bits&8192)?8:4;ai.valid=(bits&16384)!=0;ai.pos.X=(bits%5==0)?16:4;state.tools.pool.push_back(&ai);
  controlled.pos.X=(float)((bits%4)*8);parent.uid=((U64)1<<32)+(bits%9);state.world.frame=(long)(bits%13);bool force=(bits%7)==0;
  parent.last=(bits%3)?&last:0;last.dead=(bits%5)==1;last.valid=(bits%7)!=1;last.noticed=false;
  perception.aware=(bits%3)==0;perception.thing=(bits%5)?&perceived:0;perceived.dead=(bits%7)==2;perceived.valid=(bits%11)!=3;perceived.flags=(bits%4)?8:0;perceived.noticed=false;
  const CThing*excluded=bits%6==0?static_cast<const CThing*>(&ai):bits%6==1?static_cast<const CThing*>(&player):bits%6==2?static_cast<const CThing*>(&last):bits%6==3?static_cast<const CThing*>(&perceived):0;
  if(excluded){state.ThingsToNotAttack.push_back(CIntelligentPointer<CThing>(0));state.ThingsToNotAttack.push_back(CIntelligentPointer<CThing>(const_cast<CThing*>(excluded)));}
  CThingCreatureBase*expected=old_ptr;int searches=0;
  bool scripted=(bits&8)&&!script.dead&&!script.incap&&(!(parent.turncoat&&turncoat.active)||script.enemy);
  if(scripted)expected=&script;
  else if(state.map.in){
   unsigned long period=4*(1+4*(bits%4));bool refresh=force||((parent.uid+(unsigned long)state.world.frame)%period==0);
   if(refresh){++searches;expected=0;if(eligible(&ai,excluded,true,true))expected=&ai;CThingPlayerCreature*pp=game.pm.p.player;if(live(pp)&&eligible(pp,excluded,true,true)&&(!expected||expected->score<=pp->score))expected=pp;}
   if(!live(expected)&&live(parent.last)&&eligible(parent.last,excluded,false,false))expected=parent.last;
   if(!live(expected)&&perception.aware&&live(perception.thing)&&(perceived.flags&8)&&eligible(&perceived,excluded,false,false))expected=&perceived;
  }
  int updates=(expected!=old_ptr&&((live(old_ptr)&&(old_ptr->flags&32))||(live(expected)&&(expected->flags&32))))?1:0;
  state.UpdatePBestEnemyCreatureForAttack(force);
  CHECK(state.PBestEnemyCreatureForAttack.Get()==expected);CHECK(state.tools.searches==searches);CHECK(CPlayerGui::instance.updates==updates);CHECK(state.PScriptTargetOverride.Get()==((bits&8)?&script:0));CHECK(state.ThingsToNotAttack.size()==(excluded?2u:0u));
 }
 CHECK(cases==32768);std::puts("007f6200_TEST PASS");return 0;
}
