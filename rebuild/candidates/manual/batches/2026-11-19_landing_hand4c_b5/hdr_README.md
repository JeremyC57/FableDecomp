# hand4c header overlay (2026-11-19)
Landed already (removed from the overlay; the repo copy holds them): engine/CThingFilter_OverlappingCircle.h
FABLE_RETAIL_CTHINGFILTER_OVERLAPPINGCIRCLE_BODY; engine/CThingSearchTools.h TCSINROUGHAREA_EGOD (b3) and FOREACHINROUGHAREA_EGOD (b4);
engine/CThingFilter_IsInQuestOrNullQuest.h (b3); engine/CThingFilter_SolidToThing.h SOLIDTOTHING_BODY (b4);
engine/CThingFilter_PhysicsFlagsSolidToThing.h (b4).

Open (batch 5, row 0x006668A0):
- engine/CThingCreatureBase.h (copy of the current repo header): FABLE_RETAIL_CTHINGCREATUREBASE_POSSIBLECOMBATANIMS (new; inside the class
  right after the COMBATANIM block, no assert line moves) -- declaration only of Ego_r.pdb / FableWin.pdb
  ?GetPossibleCombatAnimations@CThingCreatureBase@@QBE_NABVCCharString@@AAV?$vector@PBVCAnimationEntry@@..@std@@@Z (0x006668A0).
  Users: 0x006668A0. No layout change.
