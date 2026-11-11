#ifndef FABLE_ENGINE_NHEROSTATMODIFIEREVENT_CHITTHING_H
#define FABLE_ENGINE_NHEROSTATMODIFIEREVENT_CHITTHING_H
// Original classes/signatures: both PDB type records in evidence/00839940.
#include "engine/EHeroExperienceStatCategory.h"
class CTCHeroStats;class CCreatureStatsDef;
namespace NHeroStatModifierEvent {
class CBase {public:CBase();~CBase(){}virtual void ModifyHeroStats(CTCHeroStats&)const=0;};
class CHitThing:public CBase {public:
 CHitThing(const CCreatureStatsDef&);
 virtual void ModifyHeroStats(CTCHeroStats&)const;
 void SetAsUnarmed(bool value){Unarmed=value;}
 void SetStatToIncrement(EHeroExperienceStatCategory stat){StatToIncrement=stat;}
 void SetStatIncrementAmount(float amount){IncrementAmount=amount;}
private:
 const CCreatureStatsDef& StatsDefOfHitThing;
 bool Unarmed;EHeroExperienceStatCategory StatToIncrement;bool DoStatIncrement;float IncrementAmount;
};
}
// Layout: native event ctor/store offsets +4/+8/+c/+10/+14; original PDB ref member belongs to CHitThing, not its base.
FABLE_STATIC_ASSERT(sizeof(NHeroStatModifierEvent::CBase)==4);
FABLE_STATIC_ASSERT(sizeof(NHeroStatModifierEvent::CHitThing)==0x18);
#endif
