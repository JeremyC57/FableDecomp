// CAIStateGroup_StealthSeekEnemy::IsStillRunnable(CTCPerceiveThing*) -- require perceived live attackable enemy and willingness to attack
// Retail0x0093C4E0,275bytes. GetPItemA01B50;IsStopping8FD010;GetPerceivedThing7446A0;
// LowerBound4365B0;IsThingAttackable7F6170;AreThingsEnemies76D2F0;IsWillingToAttackTarget8648A0.
#include "engine/CAIStateGroup_StealthSeekEnemy.h"
#include "engine/CAIBrain.h"
#include "engine/CTCPerceiveThing.h"
struct CInterfaceMap { struct Entry { long type; void* component; }; Entry *first,*last; Entry* LowerBound(const long*); };
struct CThing {
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
struct CTCTargetingAI { bool IsThingAttackable(const CThing*) const; };
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