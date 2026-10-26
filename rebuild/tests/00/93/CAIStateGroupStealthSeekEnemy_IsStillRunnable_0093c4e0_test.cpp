#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define FABLE_ENGINE_CAISTATEGROUP_STEALTHSEEKENEMY_H
#define FABLE_ENGINE_CAIBRAIN_H
#define FABLE_ENGINE_CTCPERCEIVETHING_H
struct CAIStateGroup_StealthSeekEnemy { unsigned char _pad_0x04[sizeof(void*)]; unsigned char PEnemy[2*sizeof(void*)]; };
class CThingAICreature;
class CAIBrain { public: CThingAICreature* Creature; };
class CTCPerceiveThing { public: bool ExtendedLineOfSight; float AwarenessTimer; };
#endif
// BEGIN EXACT CANDIDATE
// CAIStateGroup_StealthSeekEnemy::IsStillRunnable(CTCPerceiveThing*) -- require perceived live attackable enemy and willingness to attack
// Retail0x0093C4E0,275bytes. GetPItemA01B50;IsStopping8FD010;GetPerceivedThing7446A0;
// LowerBound4365B0;IsThingAttackable7F6170;AreThingsEnemies76D2F0;IsWillingToAttackTarget8648A0.
#include "engine/CAIStateGroup_StealthSeekEnemy.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPerceiveThing.h"
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*) const; };
class CThing { public:
    unsigned char prefix[0x20]; unsigned long bits[9]; CInterfaceMap interfaces;
    unsigned char middle[0x45]; unsigned char dead;
    template<class T> __forceinline bool GetInterface(T** out,long type) {
        if(bits[type>>5]&(1UL<<(type&31))) {
            long key=type; CInterfaceMap* map=&interfaces; CInterfaceMap::Entry* it=map->LowerBound(&key);
            if(it==map->last || it->type>type) it=map->last;
            *out=(T*)it->component; return true;
        } return false;
    }
};


struct CBaseIntelligentPointer { virtual ~CBaseIntelligentPointer(); void* Handle; CThing* GetPItem() const; };
struct CTCPerceiveThingMethods : CTCPerceiveThing { CThing* GetPerceivedThing() const; };
class CTCTargetingAI { public: bool IsThingAttackable(const CThing*) const; };
bool __fastcall AreThingsEnemies(CThing*,CThing*);
bool __fastcall IsWillingToAttackTarget(CThing*,CThing*);
struct CAIStateGroup_StealthSeekEnemy_Adapter : CAIStateGroup_StealthSeekEnemy {
 CAIBrain* Brain() { return *(CAIBrain**)&_pad_0x04[0]; }
 CBaseIntelligentPointer& Enemy() { return *(CBaseIntelligentPointer*)PEnemy; }
 __forceinline bool AreEnemies() { CThing* c=(CThing*)Brain()->Creature; return AreThingsEnemies(Enemy().GetPItem(),c); }
 __forceinline bool WillAttack() { CThing* c=(CThing*)Brain()->Creature; return IsWillingToAttackTarget(c,Enemy().GetPItem()); }
 bool IsStopping(); bool IsStillRunnable(CTCPerceiveThingMethods*);
};
bool CAIStateGroup_StealthSeekEnemy_Adapter::IsStillRunnable(CTCPerceiveThingMethods* perception) {
 if(Enemy().GetPItem() && !(Enemy().GetPItem()->dead&1) && !IsStopping()) {
  if(perception->ExtendedLineOfSight || perception->AwarenessTimer!=0.0f) {
   CThing* enemy=Enemy().GetPItem();
   if(perception->GetPerceivedThing()==enemy) {
    CTCTargetingAI* targeting;
    if(((CThing*)Brain()->Creature)->GetInterface(&targeting,8) && targeting->IsThingAttackable(Enemy().GetPItem())) {
     return AreEnemies() && WillAttack();
    }
   }
  }
 }
 return false;
}
// END EXACT CANDIDATE
static int errors,scenario,reads,lookups,attacks,enemies,willing;
static CThing enemy,creature,replacement,other;
static CAIBrain brain;
static CTCPerceiveThingMethods perception;
static CTCTargetingAI targeting;
static void Check(bool b) { if(!b) ++errors; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { ++reads; if(scenario==16 && reads==5) brain.Creature=(CThingAICreature*)&replacement; return scenario==1?0:&enemy; }
bool CAIStateGroup_StealthSeekEnemy_Adapter::IsStopping() { return scenario==3; }
CThing* CTCPerceiveThingMethods::GetPerceivedThing() const { Check(this==&perception); return scenario==5?&other:&enemy; }
CInterfaceMap::Entry* CInterfaceMap::LowerBound(const long* key) const { ++lookups; Check(*key==8); return scenario==14?last:first; }
bool CTCTargetingAI::IsThingAttackable(const CThing* t) const { ++attacks; Check(this==&targeting && t==&enemy); return scenario!=7; }
bool __fastcall AreThingsEnemies(CThing* a,CThing* b) { ++enemies; Check(a==&enemy && b==&creature); return scenario!=8; }
bool __fastcall IsWillingToAttackTarget(CThing* a,CThing* b) { ++willing; Check(a==(scenario==16?&replacement:&creature) && b==&enemy); return scenario!=9; }
int main() {
 CAIStateGroup_StealthSeekEnemy_Adapter state;
 *(CAIBrain**)state._pad_0x04=&brain; new(state.PEnemy) CBaseIntelligentPointer;
 CInterfaceMap::Entry entry[2]; entry[0].component=&targeting; entry[1].component=&targeting;
 creature.interfaces.first=entry; creature.interfaces.last=entry+1;
 for(scenario=1;scenario<=16;++scenario) {
  reads=lookups=attacks=enemies=willing=0; brain.Creature=(CThingAICreature*)&creature; enemy.dead=scenario==2?1:0;
  for(int i=0;i<9;++i) creature.bits[i]=0;
  if(scenario!=6) creature.bits[0]=1UL<<8;
  entry[0].type=scenario==15?9:8;
  perception.ExtendedLineOfSight=scenario!=4 && scenario!=11 && scenario!=12 && scenario!=13;
  perception.AwarenessTimer=scenario==11?1.0f:scenario==12?-1.0f:0.0f;
  if(scenario==13) { union { unsigned int u; float f; } q; q.u=0x7fc00000; perception.AwarenessTimer=q.f; }
  bool result=state.IsStillRunnable(&perception); Check(result==(scenario>=10));
  if(scenario>=10) Check(reads==6 && lookups==1 && attacks==1 && enemies==1 && willing==1);
  if(scenario==8) Check(enemies==1 && willing==0);
  if(scenario==9) Check(willing==1);
 }
 ((CBaseIntelligentPointer*)state.PEnemy)->~CBaseIntelligentPointer();
 if(errors) { std::printf("0093c4e0_TEST FAIL %d\n",errors); return 1; }
 std::printf("0093c4e0_TEST PASS\n"); return 0;
}