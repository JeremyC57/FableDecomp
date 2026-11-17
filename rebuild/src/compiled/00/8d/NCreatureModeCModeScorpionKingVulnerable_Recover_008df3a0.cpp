// NCreatureMode::CModeScorpionKingVulnerable::Recover() -- the scorpion king tries a tail plunge at its target; on success the owner's queued animations are cleared and mode 0x37 dropped, otherwise the belly hit location is closed and the body one reopened
// Leaf: ?Recover@CModeScorpionKingVulnerable@NCreatureMode@@AAEXXZ
// Label: the catalog row "CCreatureAction_BriarRose::Execute" (forge-autoname4) is wrong: the only caller is 0x008E0D08 inside
//   NCreatureMode::CModeScorpionKingVulnerable's FrameUpdate, `this` matches Ego_r.pdb CModeScorpionKingVulnerable exactly
//   (PCreature +0x78, bRecovering +0x7c, PTarget +0x7d packed), and the class's private Recover (AAEXXZ) is the direct-call member.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DF3A0, 359 bytes (368 with padding), `ret`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CCreatureAction_ScorpionTailPlunge(CThingCreatureBase&, CThingCreatureBase&)
//   0x008E4640 (a 0x120-byte temporary), CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900,
//   CVectorMap::LowerBound 0x0040F020 (inlined interface lookups 0x5A / 0x31 / 0x7F), CTCAnimationComplex::GetAnimation
//   0x0070B460, CAnimateThing 0x0070D510 (clears the queues at +0x58/+0x64), CTCCreatureModeManager::RemoveMode 0x0073ABA0,
//   CTCHitLocations::GetPHitLocationWithDefName 0x00815820 ("HIT_LOCATION_SCORPION_KING_BELLY" 0x01285A28,
//   "HIT_LOCATION_SCORPION_KING" 0x01285A0C; implicit temporaries), CHitLocation::SetEnabled 0x00816390.
// NF65 (round 7): ported onto the shared headers, as the landed sibling FrameUpdate 0x008E0A10 (engine/NCreatureMode_CModeScorpionKingVulnerable.h,
//   Recover private there; CThing / CThingCreatureBase / CCombatActionBase / CTCAnimationComplex / CAnimateThing / CTCCreatureModeManager /
//   CTCHitLocations / CHitLocation / CIntelligentPointer / CCharString from their headers). The self symbol is now the private
//   ?Recover@CModeScorpionKingVulnerable@NCreatureMode@@AAEXXZ (was the local public QAEXXZ). CCreatureAction_ScorpionTailPlunge has no shared
//   header: it stays a local class (no own members modelled beyond its 0xc tail) with the Ego_r.pdb / FableWin.pdb ctor spelling
//   ??0CCreatureAction_ScorpionTailPlunge@@QAE@AAVCThingCreatureBase@@0@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CANIMATETHING_CLEARQUEUES
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"

class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase {   // retail sizeof 0x120 (no shared header)
public:
    CCreatureAction_ScorpionTailPlunge(CThingCreatureBase& creature, CThingCreatureBase& target);   // 0x008E4640
private:
    unsigned char _own[0xc];
};

void NCreatureMode::CModeScorpionKingVulnerable::Recover()
{
    if (PCreature->SetCurrentAction(CCreatureAction_ScorpionTailPlunge(*PCreature, *PTarget.Get()))) {
        CTCAnimationComplex* animComplex = 0;
        ParentThing.GetTC(TCI_ANIMATION, &animComplex);
        animComplex->GetAnimation().ClearQueues();
        CTCCreatureModeManager* modeManager = 0;
        PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
        modeManager->RemoveMode(0x37);
    } else {
        CTCHitLocations* hitLocations = 0;
        PCreature->GetTC(TCI_HIT_LOCATIONS, &hitLocations);
        NHitLocations::CHitLocation* belly = hitLocations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING_BELLY");
        NHitLocations::CHitLocation* body = hitLocations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING");
        belly->SetEnabled(false);
        body->SetEnabled(true);
    }
}
