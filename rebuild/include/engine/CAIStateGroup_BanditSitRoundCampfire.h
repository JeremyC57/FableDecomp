#ifndef FABLE_ENGINE_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_H
#define FABLE_ENGINE_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_H
#include "engine/CAIStateGroupBase.h"
#include "engine/CIntelligentPointer.h"
class CThingObject;
// Both original PDB class records; retail native fields start at14 after CAIStateGroupBase.
class CAIStateGroup_BanditSitRoundCampfire:public CAIStateGroupBase {
#ifdef FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_ISSTILLVALID
 // Opt-in (NF404, row 0x0093A890; needs FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS for the base slot): the slot-5 override Ego_r.pdb
 // 0x0095DCE0 / FableWin.pdb 0x02B7DC90 ?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ (public; retail vtable 0x0128B78C
 // slot 5 = 0x0093A890: IS_THING_ALIVE(PCampfire +0x14)). Declaration only (an override: no vtable change).
public:
 virtual bool IsStillValid() const;
#endif
private:
 CThingObject* GetPNearCampfire()const;
 CIntelligentPointer<CThingObject> PCampfire;
 long SeatPosIndex;bool GotSeatPos;bool Sitting;
 unsigned long FrameToNextSitOnSeat,FrameToNextCheckIsRunnable;
 bool WasCarryingWeapon;
};
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_BanditSitRoundCampfire)==48);
#endif
