#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __int64 long long
#endif
static void check(bool b){if(!b){std::puts("FAIL");std::exit(1);}}
struct E {int k,v;E(int a,int b):k(a),v(b){}bool operator==(const E&o)const{return k==o.k&&v==o.v;}};static std::vector<E>trace;
class CCharString{ public:std::string s;CCharString(const char*p,long n=-1):s(p){check(n==-1);}CCharString&operator=(const char*p){s=p;return *this;}};
class CTCCombatLeader{ public:unsigned long frame,reaction;void SetFrameOfLastEventReaction(unsigned long f){frame=f;trace.push_back(E(2,int(f)));}void SetLastEventReaction(unsigned long r){reaction=r;trace.push_back(E(3,int(r)));}};
class CThing{ public:int id;bool dead,down;unsigned long flags;CThing():id(0),dead(false),down(false),flags(8){}bool IsDead()const{return dead;}unsigned long GetTypeFlags()const{return flags;}};
class CThingCreatureBase:public CThing{ public:CTCCombatLeader*leader;bool GetTC(CTCCombatLeader**o){*o=leader;return leader!=0;}};
template<class T>class CIntelligentPointer{ public:T*p;CIntelligentPointer(T*q=0):p(q){}T*Get()const{return p;}T*operator->()const{return p;}T&operator*()const{return *p;}};
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>&p){return p.p&&!p.p->dead;}
template<class A,class B>B GFDebugPDynamicCast(A a,B){return static_cast<B>(a);}
namespace NCreatureMode{static bool IsKnockedDown(const CThing&t){trace.push_back(E(1,t.id));return t.down;}}
class CCombatDialogueDef{ public:int refs;long FoeKnockedDownByColleague,MenLosing,Encourage;};
template<class T>class CDefPointer{ public:T*p;CDefPointer(T*q):p(q){if(p)++const_cast<CCombatDialogueDef*>(p)->refs;}CDefPointer(const CDefPointer&o):p(o.p){if(p)++const_cast<CCombatDialogueDef*>(p)->refs;}~CDefPointer(){if(p)--const_cast<CCombatDialogueDef*>(p)->refs;}const T*operator->()const{return p;}};
class CombatDialogueManager{ public:CCombatDialogueDef*def;bool speechResult;int refsAtSpeech;CDefPointer<const CCombatDialogueDef>GetPDialogueDef(){return CDefPointer<const CCombatDialogueDef>(def);}bool SaySpeech(CThingCreatureBase&c,unsigned long s){check(c.id==100);refsAtSpeech=def->refs;trace.push_back(E(4,int(s)));return speechResult;}};
struct CCombatantInfo{struct D{std::map<unsigned __int64,CThingCreatureBase*>Attackers;}DefenderInfo;};
class CCombatManager{ public:CCombatantInfo info;CombatDialogueManager dialogue;int lookups;const CCombatantInfo&GetCombatantInfo(const CThingCreatureBase&c)const{check(c.id==200);++const_cast<CCombatManager*>(this)->lookups;return info;}CombatDialogueManager&GetCombatDialogueManager(){return dialogue;}};
class CWorld{ public:unsigned long seed;static long frame;static long GetFrame(){return frame;}unsigned long*GetWorldSeed()const{return &const_cast<CWorld*>(this)->seed;}};long CWorld::frame;
static unsigned long GFRandom(unsigned long n,unsigned long*p){unsigned long v=(*p*0x24a1+0x24df)&0xffffffffUL;*p=((v>>13)|(v<<19))&0xffffffffUL;trace.push_back(E(5,0));return n?*p%n:0;}
class CCombatActionBase{ public:CIntelligentPointer<CThingCreatureBase>PCreature;CIntelligentPointer<CThing>PTarget;CCombatManager*manager;CWorld*world;std::string animation;CCombatManager&GetCombatManager(){return *manager;}CWorld&GetWorld(){return *world;}void SetCombatAnimation(const CCharString&s){animation=s.s;trace.push_back(E(6,0));}void Initialise(){trace.push_back(E(7,0));}};
class CCreatureAction_GenericLeaderCommand:public CCombatActionBase{ public:void Initialise();};
void CCreatureAction_GenericLeaderCommand::Initialise()
{
 bool cheer=false;
 bool embarassed=false;
 CCharString animation_name("LEADER_IDLE");
 CTCCombatLeader* pCombatLeaderTC;
 if(PCreature->GetTC(&pCombatLeaderTC)) {
  if(IS_THING_ALIVE(PTarget)&&(PTarget->GetTypeFlags()&8)) {
   CThingCreatureBase* pcreature_defender=GFDebugPDynamicCast(PTarget.Get(),(CThingCreatureBase*)0);
   const CCombatManager& comb_man=GetCombatManager();
   const std::map<unsigned __int64,CThingCreatureBase*>& combatants=comb_man.GetCombatantInfo(*pcreature_defender).DefenderInfo.Attackers;
   for(std::map<unsigned __int64,CThingCreatureBase*>::const_iterator it=combatants.begin();it!=combatants.end();++it) {
    const CThingCreatureBase* pattacker=it->second;
    if(NCreatureMode::IsKnockedDown(*pattacker))embarassed=true;
   }
   if(NCreatureMode::IsKnockedDown(*PTarget))cheer=true;
  }
  CDefPointer<const CCombatDialogueDef> p_dialogue_def=GetCombatManager().GetCombatDialogueManager().GetPDialogueDef();
  if(cheer) {
   animation_name="LEADER_CHEER";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(0);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->FoeKnockedDownByColleague);
  } else if(embarassed) {
   animation_name="LEADER_DISAPPOINTED";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(1);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->MenLosing);
  } else if(GFRandom(100,GetWorld().GetWorldSeed())<20) {
   animation_name="LEADER_GIVE_ORDERS";
   pCombatLeaderTC->SetFrameOfLastEventReaction(GetWorld().GetFrame());
   pCombatLeaderTC->SetLastEventReaction(2);
   CThingCreatureBase* creature=PCreature.Get();
   GetCombatManager().GetCombatDialogueManager().SaySpeech(*creature,p_dialogue_def->Encourage);
  }
 }
 SetCombatAnimation(animation_name);
 CCombatActionBase::Initialise();
}

int main(){unsigned long cases=0;
 for(int component=0;component<2;++component)for(int target=0;target<4;++target)for(int count=0;count<4;++count)for(int mask=0;mask<16;++mask)for(int seed=0;seed<128;++seed)for(int result=0;result<2;++result){
 CThingCreatureBase leader,defender,allies[3];leader.id=100;leader.leader=0;defender.id=200;defender.dead=target==2;defender.flags=target==3?0:8;defender.down=(mask&8)!=0;CCombatDialogueDef def;def.refs=1;def.FoeKnockedDownByColleague=101;def.MenLosing=102;def.Encourage=103;CTCCombatLeader tc;tc.frame=999;tc.reaction=999;leader.leader=component?&tc:0;
 CCombatManager cm;cm.lookups=0;cm.dialogue.def=&def;cm.dialogue.refsAtSpeech=0;cm.dialogue.speechResult=result!=0;
 for(int i=0;i<count;++i){allies[i].id=i;allies[i].down=(mask&(1<<i))!=0;cm.info.DefenderInfo.Attackers.insert(std::make_pair((unsigned __int64)(2*i+1),&allies[i]));}
 CWorld world;world.seed=seed==127?0xffffffffUL:(unsigned long)seed;CWorld::frame=seed==127?-1:seed*2;
 CCreatureAction_GenericLeaderCommand a;a.PCreature.p=&leader;a.PTarget.p=target?&defender:0;a.manager=&cm;a.world=&world;
 unsigned long expectedSeed=world.seed;int branch=-1;bool scanned=component&&target==1;std::vector<E>expected;
 if(scanned){bool ally=false;for(int i=0;i<count;++i){expected.push_back(E(1,i));if(mask&(1<<i))ally=true;}expected.push_back(E(1,200));if(mask&8)branch=0;else if(ally)branch=1;}
 if(component&&branch<0){unsigned long v=(expectedSeed*0x24a1+0x24df)&0xffffffffUL;expectedSeed=((v>>13)|(v<<19))&0xffffffffUL;expected.push_back(E(5,0));if(expectedSeed%100<20)branch=2;}
 if(branch>=0){expected.push_back(E(2,int((unsigned long)CWorld::frame)));expected.push_back(E(3,branch));expected.push_back(E(4,101+branch));}expected.push_back(E(6,0));expected.push_back(E(7,0));trace.clear();a.Initialise();
 check(trace==expected);const char*names[]={"LEADER_CHEER","LEADER_DISAPPOINTED","LEADER_GIVE_ORDERS"};check(a.animation==(branch<0?"LEADER_IDLE":names[branch]));check(world.seed==expectedSeed);check(def.refs==1);check(cm.lookups==int(scanned));check(cm.info.DefenderInfo.Attackers.size()==unsigned(count));check(tc.reaction==(branch<0?999UL:(unsigned long)branch));check(tc.frame==(branch<0?999UL:(unsigned long)CWorld::frame));check(cm.dialogue.refsAtSpeech==(branch<0?0:2));++cases;
 }
 check(cases==131072);std::puts("008bf800_TEST PASS");return 0;
}