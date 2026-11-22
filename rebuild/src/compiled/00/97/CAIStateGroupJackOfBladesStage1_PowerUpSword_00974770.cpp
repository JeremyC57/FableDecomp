// CAIStateGroup_JackOfBladesStage1::PowerUpSword()
// Leaf: ?PowerUpSword@CAIStateGroup_JackOfBladesStage1@@IAEXXZ
// Flags: default
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_TCDRIVER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION_NOWAIT
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
#define FABLE_RETAIL_CTCDPHYSICALOBSTRUCTION_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_JackOfBladesStage1.h"
#include "engine/CAIBrain.h"
#include "engine/C3DVector.h"
#include "engine/EActionRegisteredType.h"
#include "engine/CSoundPair.h"
class CAnimationEntry;
#include "engine/CCreatureAction_PlayCombatAnimation.h"
#include "engine/CTCBoss.h"
#include "engine/CTCDPhysicalObstruction.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCCarrying.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase_GetSubDef.h"

bool CAIStateGroup_JackOfBladesBase::IsStillRunnable()
{
    if (!IS_THING_ALIVE(PTargetThing) || IsStopping() || !IS_THING_ALIVE(PArenaCentre))
        return false;
    return true;
}

void CAIStateGroup_JackOfBladesStage1::PowerUpSword()
{
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_INTO", true));

    do {
        if (!IsStillRunnable()) {
            Terminate();
            return;
        }

        if (IS_THING_ALIVE(PObstruction)) {
            CTCDPhysicalObstruction* ptcd_obstruction;
            CThing* thing = PObstruction.Get();
            if (thing->HasTC(CTCDPhysicalObstruction::StatGetInterfaceType()))
                ptcd_obstruction = static_cast<CTCDPhysicalObstruction*>(&thing->GetTCDriver());
            ptcd_obstruction->SetLifeTimeSeconds(60.0f);
        }
        AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_LOOP", true));
    } while (ptc_boss->CountLivingHelpers() > 0);

    if (!IsStillRunnable()) {
        Terminate();
        return;
    }

    CDefPointer<const CSpecialEffectsDef> pspecial_fx;
    GetCreature().PeekBaseDef().GetSubDef(&pspecial_fx);
    if (pspecial_fx.Get()) {
        long fx_index;
        if (pspecial_fx->GetEffectFromString("SWORD_OF_AEONS", fx_index)) {
            CThing* p_emitter = CTCDParticleEmitter::Create(fx_index, GetCreature().GetPos(), false);
            if (IS_THING_ALIVE(p_emitter)) {
                PSwordEmitter = p_emitter;
                CTCDParticleEmitter* ptcd_emitter = 0;
                if (p_emitter->GetTCDriver(&ptcd_emitter)) {
                    CTCCarrying* ptc_carrying = 0;
                    GetCreature().GetTC(&ptc_carrying);
                    CThing* psword = ptc_carrying->GetThingInPrimarySlot();
                    ptcd_emitter->AttachToThing(*psword, 10, "WEAPON_FOCUS_01", 0, 0.0f);
                }
            }
        }
    }
    AIAction_PerformAction(CCreatureAction_PlayCombatAnimation(GetCreature(), "SUMMON_OUTOF", true));
}