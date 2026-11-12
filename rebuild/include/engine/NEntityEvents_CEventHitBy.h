#ifndef FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBY_H
#define FABLE_ENGINE_NENTITYEVENTS_CEVENTHITBY_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/NEntityEvents_CEventHitBase.h"
#include "engine/EHeroAbility.h"
namespace NEntityEvents {
#ifdef FABLE_RETAIL_NENTITYEVENTS_CEVENTHITBY_TYPE
// chunk Q15 (row 0x00738A80): Ego_r.pdb / FableWin.pdb ?StatGetType@CEventHitBy@NEntityEvents@@SI?AW4EEventType@2@XZ (inline: the
// GetEvent<const CEventHitBy, ..> instances compare GetType() with the immediate 1, EVENT_HIT_BY) and ?GetType@..@UBE (slot 1).
// Needs FABLE_RETAIL_NENTITYEVENTS_CEVENTBASE_VSLOTS3 (EEventType, the pure CEventBase::GetType).
class CEventHitBy:public CEventHitBase {public:CEventHitBy(const CHitParameters&);
 static EEventType FABLE_FASTCALL StatGetType(){return (EEventType)1;}
 virtual EEventType GetType()const;
};
#else
class CEventHitBy:public CEventHitBase {public:CEventHitBy(const CHitParameters&);};
#endif
class CEventHitBySpecialAbility:public CEventHitBase {public:CEventHitBySpecialAbility(const CHitParameters&,EHeroAbility);private:EHeroAbility Ability;bool Aggressive;};
}
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitBy)==0x10);
FABLE_STATIC_ASSERT(sizeof(NEntityEvents::CEventHitBySpecialAbility)==0x18);
#endif
