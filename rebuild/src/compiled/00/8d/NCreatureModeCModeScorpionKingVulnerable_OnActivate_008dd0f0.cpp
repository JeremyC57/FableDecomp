// Leaf: ?OnActivate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_THING_TYPE_IDS
#define FABLE_RETAIL_SCORPION_KING_ACTIVATE_PASS30
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_INITIALISE_SCALE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CANIMINIT_CTORS
#define FABLE_RETAIL_CANIMINIT_CTOR_SIZE_BODY
#include "engine/CAnimInit.h"
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
#define FABLE_RETAIL_CTCHITLOCATIONS_WITHDEFNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
// Inline-budget: GetTCDriver<CTCDPhysicalObstruction> at 007890EA expands HasTC
// and calls bitset::test. The two calls inside OnActivate expand both.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCBoss.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CAnimationEntry.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimInit.h"
#include "engine/CAnimateThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingManager.h"
#include "fable_assert.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"

void NCreatureMode::CModeScorpionKingVulnerable::OnActivate()
{
    float random_seconds = 5.0f;
    CTCBoss* ptc_boss = 0;
    PCreature->GetTC(&ptc_boss);
    RecoveryCount = static_cast<long>(CMainGameComponent::GetConstantFPS() * random_seconds);
    CModeBase::OnActivate();
    FABLE_RETAIL_ASSERT(PCreature->GetType() == THING_TYPE_AI_CREATURE);
    CThingAICreature* pai_creature = GFDebugPDynamicCast(PCreature, (CThingAICreature*)0);
    pai_creature->SuspendAI(true, 0);
    CollisionRadius = pai_creature->GetPhysicsTC()->GetRadius();
    pai_creature->GetPhysicsTC()->SetRadius(CollisionRadius * 0.5f);
    CTCHitLocations* p_TC_hit_locations = 0;
    PCreature->GetTC(&p_TC_hit_locations);
    NHitLocations::CHitLocation* p_hit_location_belly = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING_BELLY");
    NHitLocations::CHitLocation* p_hit_location_normal = p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING");
    FABLE_RETAIL_ASSERT(p_hit_location_belly);
    FABLE_RETAIL_ASSERT(p_hit_location_normal);
    p_hit_location_belly->SetEnabled(true);
    p_hit_location_normal->SetEnabled(false);

    C3DVector claw_pos, shoulder_pos(PCreature->GetPos());
    float claw_radius = 0.5f;
    shoulder_pos -= PCreature->GetPhysicsTC()->GetFacingVector() * 1.0f;
    PCreature->GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_pincer_right", 0, 0, claw_pos);
    claw_pos.Z = shoulder_pos.Z;
    CThing* p_obstruction_thing = CTCDPhysicalObstruction::Create(claw_pos, shoulder_pos, claw_radius);
    if (p_obstruction_thing) {
        CTCDPhysicalObstruction* ptcd_obstruction = 0;
        p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
        ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(p_obstruction_thing);
    }

    PCreature->GetTCGraphicAppearance()->GetHelperPoint("weapon_pos_pincer_left", 0, 0, claw_pos);
    claw_pos.Z = shoulder_pos.Z;
    p_obstruction_thing = CTCDPhysicalObstruction::Create(claw_pos, shoulder_pos, claw_radius);
    if (p_obstruction_thing) {
        CTCDPhysicalObstruction* ptcd_obstruction = 0;
        p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
        ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        Obstructions.push_back(p_obstruction_thing);
    }

    CTCAnimationComplex* ptc_animation = 0;
    GetParentThing().GetTC(&ptc_animation);
    const CAnimationEntry& anim_entry = pai_creature->GetAnimFromName("TAIL_CURL", 0);
    CAnimInit anim_init(0x21, 0, anim_entry.BankIndex, ANIM_UPDATE_NONE, false, anim_entry.GetAnimateThingFlags(), 100);
    CAnimateThing& anim = ptc_animation->GetAnimation();
    anim.AddCombinationAnim(anim_init, 0, 0, true, false);
    TailCurlFinished = false;

    C3DVector emitter_pos(GetParentThing().GetPos());
    CDefPointer<const CSpecialEffectsDef> pspecial_fx(NULL_DEF);
    long fx_index = 0;
    GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx);
    if (pspecial_fx.Get()) {
        if (pspecial_fx->GetEffectFromString("EYE_VULNERABLE", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                PEyeEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        fx_index = 0;
        if (pspecial_fx->GetEffectFromString("EYE_VULNERABLE_FLASH", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_EYE", 0, 0.0f);
            }
        }
        fx_index = 0;
        if (pspecial_fx->GetEffectFromString("STING_IDLE", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, emitter_pos, false);
            if (IS_THING_ALIVE(p_emitter)) {
                PStingEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter))
                    ptcd_emitter->AttachToThing(GetParentThing(), 10, "EMITTER_STING", 0, 0.0f);
            }
        }
        pspecial_fx->GetEffectFromString("STING_IDLE", TailEffectIndex);
    }
    FrameCount = 0;
    EffectsDone = false;
}