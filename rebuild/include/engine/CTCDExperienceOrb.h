#ifndef FABLE_ENGINE_CTCDEXPERIENCEORB_H
#define FABLE_ENGINE_CTCDEXPERIENCEORB_H
// Layout and names: both original PDBs; donor026fd4f0 and native007c1000.
#include "engine/CTCDriverBase.h"
#include "engine/CDefPointer.h"
class CExperienceDef;
class CTCDExperienceOrb:public CTCDriverBase {public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_EXPERIENCE_ORB;}
 long GetExperiencePoints()const{return ExperiencePoints;}void OnCollected(CThing&);
private:CDefPointer<const CExperienceDef>PDef;long ExperiencePoints,CombinationImmunityTimer,LifeTimer;bool DecayOverTime,PickedUp;
};
FABLE_STATIC_ASSERT(sizeof(CTCDExperienceOrb)==32);
#endif
