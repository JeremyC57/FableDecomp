#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBY_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBY_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/EHeroAbility.h"
namespace NEntityEvents {
class CEventHitBy:public CEventHitBase {public:CEventHitBy(const CHitParameters&);};
class CEventHitBySpecialAbility:public CEventHitBase {public:CEventHitBySpecialAbility(const CHitParameters&,EHeroAbility);private:EHeroAbility Ability;bool Aggressive;};
}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitBy)==0x10);
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitBySpecialAbility)==0x18);
#endif
