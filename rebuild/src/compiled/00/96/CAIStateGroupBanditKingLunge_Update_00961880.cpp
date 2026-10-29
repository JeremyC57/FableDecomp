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
 virtual ~CBaseIntelligentPointer(); // 0x00A01C10; by-value pointer argument is destroyed by its callee
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
class CWorld { public: static long __fastcall GetFrame(); }; // 0x0049D870
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
  TimeOfLastLunge=CWorld::GetFrame();
  boss->ClearPhaseCounter();
 }
finish:
 Terminate();
}