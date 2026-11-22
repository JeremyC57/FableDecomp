#ifndef FABLE_ENGINE_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_H
#define FABLE_ENGINE_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
class CThingObject;
// Both original PDB class records; retail native fields start at14 after CAIStateGroupBase.
class CAIStateGroup_BanditSitRoundCampfire:public CAIStateGroupBase {
private:
 CThingObject* GetPNearCampfire()const;
 CIntelligentPointer<CThingObject> PCampfire;
 long SeatPosIndex;bool GotSeatPos;bool Sitting;
 unsigned long FrameToNextSitOnSeat,FrameToNextCheckIsRunnable;
 bool WasCarryingWeapon;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_BanditSitRoundCampfire)==48);
#endif
