// NCreatureMode::CModeScorpionKingVulnerable::FrameUpdate() -- base frame update; every third frame add one more tail emitter while the mesh has an EMITTER_TAIL dummy for it; then either start the summon action when the boss is ready for its next phase (dropping the vulnerable mode, or reopening the body hit location when the action is refused) or recover once the tail-curl animation is over
// Leaf: ?FrameUpdate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E0A10, 776 bytes, thiscall, plain `ret`; vtable 0x0128582C slot 3 (cell 0x01285838).
// Identity (two sources): CModeBase slot 3 = FrameUpdate (engine/CTCCreatureModeManager.h); Ego_r.pdb
//   ?FrameUpdate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ 0x00823030; the body calls the class's private Recover 0x008DF3A0.
// Callees: CTCCreatureModeManager::CModeBase::FrameUpdate 0x0073AC50, CVectorMap::LowerBound 0x0040F020 (inline GetTC), CTCBoss::CountEffects
//   0x007AEF10 / AddEffect 0x007AF6C0 / IsReadyForPhaseChange 0x007AEE90 / ClearEffects 0x007AEF60 / IncrementBossPhase 0x007AED20,
//   NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0, CTCDParticleEmitter::Create(long, const C3DVector&, bool)
//   0x006E0880 / AttachToThing 0x006E0BE0, CTCAnimationComplex::GetAnimation 0x0070B460, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CCreatureAction_ScorpionSummon ctor 0x008E46D0, CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0, CTCHitLocations::GetPHitLocationFromName 0x00815820, CHitLocation::SetEnabled
//   0x00816390, CAnimateThing::GetFirstCombinationAnimFromType 0x0070C4E0, Recover 0x008DF3A0, CCharString 0x0099EBF0 / 0x0099EAE0.
// Headers (overlay, FABLE_HDR_OVERLAY=$A/hdr): new engine/NCreatureMode_CModeScorpionKingVulnerable.h; opt-ins FABLE_RETAIL_CTCBOSS_EFFECTS,
//   FABLE_RETAIL_CTCHITLOCATIONS_FROMNAME, FABLE_RETAIL_CHITLOCATION_SETENABLED, FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOSS_EFFECTS
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CTCHITLOCATIONS_FROMNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ScorpionSummon.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCBoss.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"

void NCreatureMode::CModeScorpionKingVulnerable::FrameUpdate()
{
    CModeBase::FrameUpdate();
    ++FrameCount;
    if (!EffectsDone && FrameCount % 3 == 0) {
        // one more emitter along the tail, for as long as the mesh has an EMITTER_TAIL dummy with that index
        CTCBoss* boss = 0;
        PCreature->GetTC(TCI_BOSS, &boss);
        long index = boss->CountEffects();
        C3DVector pos;
        CRightHandedSet orientation;
        if (ParentThing.GetTCGraphicAppearance()->GetDummyObject("EMITTER_TAIL", index, 0, pos, orientation, 1.0f)) {
            CThing* emitter = CTCDParticleEmitter::Create(TailEffectIndex, pos, false);
            if (IS_THING_ALIVE(emitter)) {
                boss->AddEffect(*emitter);
                CTCDParticleEmitter* driver;
                if (emitter->GetTCDriver(&driver))
                    driver->AttachToThing(ParentThing, 10, "EMITTER_TAIL", index, 0.0f);
            }
        } else {
            EffectsDone = true;
        }
    }
    CTCAnimationComplex* animComplex = 0;
    ParentThing.GetTC(TCI_ANIMATION, &animComplex);
    CAnimateThing& animation = animComplex->GetAnimation();
    CTCBoss* boss = 0;
    PCreature->GetTC(TCI_BOSS, &boss);
    if (boss->IsReadyForPhaseChange()) {
        CThingCreatureBase* creature = PCreature;
        CThingCreatureBase* target = PTarget.Get();
        boss->ClearEffects();
        if (creature->SetCurrentAction(CCreatureAction_ScorpionSummon(*creature, *target))) {
            boss->IncrementBossPhase();
            CTCCreatureModeManager* modeManager = 0;
            PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager);
            modeManager->RemoveMode(0x37);
        } else {
            CTCHitLocations* hitLocations = 0;
            PCreature->GetTC(TCI_HIT_LOCATIONS, &hitLocations);
            NHitLocations::CHitLocation* belly = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING_BELLY");
            NHitLocations::CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_SCORPION_KING");
            belly->SetEnabled(false);
            body->SetEnabled(true);
        }
    } else if (!animation.GetFirstCombinationAnimFromType(0x21) || TailCurlFinished) {
        Recover();
    }
}