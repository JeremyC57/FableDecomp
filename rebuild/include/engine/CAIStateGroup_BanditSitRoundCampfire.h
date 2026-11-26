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
#ifdef FABLE_RETAIL_CAISTATEGROUP_BANDITSITROUNDCAMPFIRE_INITIALISE
 // NF476 add-only nested opt-in (row 0x0093A500, T3 nf374): the slot-1 override, Ego_r.pdb 0x0095D920 / FableWin.pdb 0x02B7D150
 // ?Initialise@CAIStateGroup_BanditSitRoundCampfire@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (public virtual;
 // retail vtable 0x0128B78C slot 1 = 0x0093A500). Declaration only; it changes the emitted ??_7 slot 1 of an includer that emits the table,
 // so it stays off the default view. Needs FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS (the base virtual and EScriptAIPriority).
public:
 virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
#endif
private:
 CThingObject* GetPNearCampfire()const;
 CIntelligentPointer<CThingObject> PCampfire;
 long SeatPosIndex;bool GotSeatPos;bool Sitting;
 unsigned long FrameToNextSitOnSeat,FrameToNextCheckIsRunnable;
 bool WasCarryingWeapon;
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CAIStateGroup_BanditSitRoundCampfire)==48);
#endif
