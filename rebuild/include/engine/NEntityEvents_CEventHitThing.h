#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTHITTHING_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTHITTHING_H
#include "engine/NEntityEvents_EEventType.h"
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/CIntelligentPointer.h"
namespace NEntityEvents {
// Original complete own-field set; native heap24bytes and both PDBs.
class CEventHitThing:public CEventBase {
public:
 CEventHitThing(const CThing&,CThing&,bool,bool,bool,const CThing*,bool,bool);
 virtual EEventType GetType()const;
private:
 CIntelligentPointer<CThing> PThingHit;
 bool HitEnemy,UsedMeleeWeapon,UsedRangedWeapon,Flourish,Decapitate;
};
}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitThing)==24);
#endif
