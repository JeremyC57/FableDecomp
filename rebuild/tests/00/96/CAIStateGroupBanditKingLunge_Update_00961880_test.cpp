#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
// Portable fixture declarations retain member relationships on a 64-bit host.
// VC7.1 below uses the candidate's canonical shared headers directly.
#define FABLE_ENGINE_CAISTATEGROUP_BANDITKINGLUNGE_H
#define FABLE_ENGINE_CAISTATEGROUPBASE_H
#define FABLE_ENGINE_CAIBRAIN_H
class CAIBrain;
class CAIStateGroupBase { public:
 void* __vftable; CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData;
 bool Terminated; unsigned char Priority; bool Enabled; unsigned char padding;
};
struct CAIStateGroup_BanditKingLunge : CAIStateGroupBase {
 void* Player[2]; long TimeOfLastLunge;
};
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
#endif
// CAIStateGroup_BanditKingLunge::Update() -- wait for idle, face the player and perform a lunge, then advance the boss phase and record its time.
// Retail 0x00961880, 313 bytes excluding alignment. Shared group/base/brain headers supply the real PDB members.
// CThing's donor component map is +0x48 but retail is +0x44 (presence words start +0x20), so the narrow retail component shell is intentional.
// CThingAICreature's generated header flattens these inherited fields into padding. No CThing, pointer or boss shared headers exist.
// The action's retail temporary occupies 0x114 bytes; the generated CCreatureAction_BanditKingLunge header is 0x12c, so an opaque retail action shell preserves its audited constructor/destructor boundary.
#include "engine/CAIStateGroup_BanditKingLunge.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CTCBoss { public: long GetPhase(); void SetNextBossPhase(long,bool); void ClearPhaseCounter(); }; // 0x007AED10, 0x007AEDA0, 0x007AED50
struct CThingRetail;
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry* first; Entry* last; Entry* LowerBound(const long*); }; // 0x0040F020
struct CThingRetail {
 unsigned char inherited[0x20]; unsigned long UsingComponents[9]; CInterfaceMap Components;
 bool IsPerformingAnyAction(); // 0x006622A0
 template<class T> __forceinline void GetInterface(T** out,long type) {
  if(UsingComponents[type>>5] & (1UL << (type & 31))) {
   long key=type; CInterfaceMap* map=&Components; CInterfaceMap::Entry* it=map->LowerBound(&key);
   if(it==map->last || type<it->type) it=map->last;
   *out=(T*)it->component;
  }
 }
};
struct CBaseIntelligentPointer {
 virtual void v00(); CThingRetail* item;
 CBaseIntelligentPointer(); // 0x00A01B10
 ~CBaseIntelligentPointer(); // 0x00A01C10; by-value pointer argument is destroyed by its callee
 CThingRetail* GetPItem() const; // 0x00A01B50
 void SetPItem(CThingRetail*); // 0x00A01B90
};
struct CConstThingPointer : CBaseIntelligentPointer {
 virtual void v00();
 CConstThingPointer(const CBaseIntelligentPointer& p) { SetPItem(p.GetPItem()); }
};
struct CRetailBanditKingLungeAction {
 unsigned char state[0x114];
 CRetailBanditKingLungeAction(CThingRetail*,CThingRetail*); // 0x009619C0
 ~CRetailBanditKingLungeAction(); // 0x0062E900
};
long ReadGameTime(); // 0x0049D870
struct BanditKingLungeMethods : CAIStateGroup_BanditKingLunge {
 __forceinline CAIStateGroupBase* Base() { return (CAIStateGroupBase*)this; }
 __forceinline CThingRetail* Creature() { return (CThingRetail*)Base()->PBrain->Creature; }
 __forceinline CBaseIntelligentPointer& Target() { return *(CBaseIntelligentPointer*)Player; }
 long AIAction_DoNothing(); // 0x0097E200
 long AIAction_LookAtThing(long,CConstThingPointer,long,float); // 0x0097D6F0
 bool CanLungeAtPlayer(CThingRetail*); // 0x009613A0
 long AIAction_CombatPerformActionAndWait(CRetailBanditKingLungeAction*,bool); // 0x0097DA40
 void* GetWorld(); // 0x008FCFC0
 void Terminate(); // 0x008FCF40
 void Update();
};
void BanditKingLungeMethods::Update()
{
 while(Creature()->IsPerformingAnyAction()) {
  long result=AIAction_DoNothing();
  if(result!=4 && result!=2) goto finish;
 }
 if(AIAction_LookAtThing(1,Target(),23,0.0027777f)==2 && CanLungeAtPlayer(Target().GetPItem())) {
  CAIBrain* brain=Base()->PBrain;
  CTCBoss* boss;
  ((CThingRetail*)brain->Creature)->GetInterface(&boss,173);
  CThingRetail* actor=(CThingRetail*)brain->Creature;
  AIAction_CombatPerformActionAndWait(&CRetailBanditKingLungeAction(actor,Target().GetPItem()),false);
  boss->SetNextBossPhase(boss->GetPhase()+1,false);
  GetWorld();
  TimeOfLastLunge=ReadGameTime();
  boss->ClearPhaseCounter();
 }
finish:
 Terminate();
}

static int failures, activeChecks, waits, looks, canChecks, lookResult, waitResult;
static int activeBeforeIdle, canLunge, pointerCreates, pointerDestroys, pointerGets, sets;
static int lookups, actionCreates, actionDestroys, performs, phaseGets, phaseSets;
static int worlds, times, clears, terminations, phase=7, nextPhase, lookupMode;
static CThingRetail *expectedActor, *expectedTarget;
static CTCBoss *expectedBoss;
static CInterfaceMap::Entry *expectedEntry;
static int events[64], eventCount;
static void Event(int n) { events[eventCount++]=n; }
static void Check(bool b) { if(!b) ++failures; }
void CBaseIntelligentPointer::v00() {}
void CConstThingPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : item(0) { ++pointerCreates; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++pointerDestroys; }
CThingRetail* CBaseIntelligentPointer::GetPItem() const { ++pointerGets; return item; }
void CBaseIntelligentPointer::SetPItem(CThingRetail* p) { ++sets; item=p; }
bool CThingRetail::IsPerformingAnyAction() {
 Check(this==expectedActor); ++activeChecks; Event(1); return activeChecks<=activeBeforeIdle;
}
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) {
 Check(this==&expectedActor->Components && *key==173); ++lookups; Event(5);
 return lookupMode==2 ? last : expectedEntry;
}
long BanditKingLungeMethods::AIAction_DoNothing() { ++waits; Event(2); return waitResult; }
long BanditKingLungeMethods::AIAction_LookAtThing(long repeat,CConstThingPointer p,long action,float angle) {
 Check(repeat==1 && p.item==expectedTarget && action==23 && angle==0.0027777f);
 ++looks; Event(3); return lookResult;
}
bool BanditKingLungeMethods::CanLungeAtPlayer(CThingRetail* p) {
 Check(p==expectedTarget); ++canChecks; Event(4); return canLunge!=0;
}
CRetailBanditKingLungeAction::CRetailBanditKingLungeAction(CThingRetail* actor,CThingRetail* target) {
 Check(actor==expectedActor && target==expectedTarget); ++actionCreates; Event(6);
}
CRetailBanditKingLungeAction::~CRetailBanditKingLungeAction() { ++actionDestroys; Event(8); }
long BanditKingLungeMethods::AIAction_CombatPerformActionAndWait(CRetailBanditKingLungeAction* a,bool flag) {
 Check(a!=0 && !flag); ++performs; Event(7); return 99;
}
long CTCBoss::GetPhase() { Check(this==expectedBoss); ++phaseGets; Event(9); return phase; }
void CTCBoss::SetNextBossPhase(long value,bool force) {
 Check(this==expectedBoss && !force); nextPhase=value; ++phaseSets; Event(10);
}
void* BanditKingLungeMethods::GetWorld() { ++worlds; Event(11); return 0; }
long ReadGameTime() { ++times; Event(12); return 123456; }
void CTCBoss::ClearPhaseCounter() { Check(this==expectedBoss); ++clears; Event(13); }
void BanditKingLungeMethods::Terminate() { ++terminations; Base()->Terminated=true; Event(14); }
static void Run(int scenario)
{
 activeChecks=waits=looks=canChecks=pointerCreates=pointerDestroys=pointerGets=sets=0;
 lookups=actionCreates=actionDestroys=performs=phaseGets=phaseSets=worlds=times=clears=terminations=0;
 nextPhase=0; eventCount=0; activeBeforeIdle=0; waitResult=4; lookResult=2; canLunge=1; lookupMode=0;
 if(scenario==1) activeBeforeIdle=2;
 if(scenario==2) { activeBeforeIdle=1; waitResult=3; }
 if(scenario==3) lookResult=4;
 if(scenario==4) canLunge=0;
 if(scenario==5) lookupMode=1;
 if(scenario==6) lookupMode=2;
 if(scenario==7) { activeBeforeIdle=2; waitResult=2; }
 BanditKingLungeMethods group;
 CAIBrain brain;
 CThingRetail actor,target;
 CTCBoss boss,wrongBoss;
 CInterfaceMap::Entry entries[2];
 entries[0].type=lookupMode==1?174:173; entries[0].component=&boss;
 entries[1].type=999; entries[1].component=&boss;
 if(lookupMode==1) entries[0].component=&wrongBoss;
 expectedActor=&actor; expectedTarget=&target; expectedBoss=&boss; expectedEntry=entries;
 for(int i=0;i<9;++i) actor.UsingComponents[i]=0;
 actor.UsingComponents[173>>5]=1UL<<(173&31);
 actor.Components.first=entries; actor.Components.last=entries+1;
 brain.Creature=(CThingAICreature*)&actor;
 group.Base()->PBrain=&brain; group.Base()->Terminated=false; group.TimeOfLastLunge=-77;
 CBaseIntelligentPointer* targetPointer=new ((void*)group.Player) CBaseIntelligentPointer;
 targetPointer->item=&target; pointerCreates=0;
 group.Update();
 Check(terminations==1 && group.Base()->Terminated && events[eventCount-1]==14);
 Check(activeChecks==(scenario==2?1:activeBeforeIdle+1));
 Check(waits==activeBeforeIdle);
 bool reachedLook=scenario!=2;
 Check(looks==(reachedLook?1:0));
 Check(pointerCreates==(reachedLook?1:0) && pointerDestroys==(reachedLook?1:0));
 Check(canChecks==((reachedLook && lookResult==2)?1:0));
 bool lunged=scenario!=2 && scenario!=3 && scenario!=4;
 Check(lookups==(lunged?1:0) && performs==(lunged?1:0));
 Check(actionCreates==(lunged?1:0) && actionDestroys==(lunged?1:0));
 Check(phaseGets==(lunged?1:0) && phaseSets==(lunged?1:0));
 Check(worlds==(lunged?1:0) && times==(lunged?1:0) && clears==(lunged?1:0));
 Check(group.TimeOfLastLunge==(lunged?123456:-77));
 if(lunged) {
  Check(nextPhase==phase+1);
  const int tail[]={5,6,7,8,9,10,11,12,13,14};
  for(int j=0;j<10;++j) Check(events[eventCount-10+j]==tail[j]);
 }
 targetPointer->~CBaseIntelligentPointer();
}
int main() {
 for(int i=0;i<8;++i) Run(i);
 if(failures) { std::printf("00961880_TEST FAIL %d\n",failures); return 1; }
 std::printf("00961880_TEST PASS\n"); return 0;
}