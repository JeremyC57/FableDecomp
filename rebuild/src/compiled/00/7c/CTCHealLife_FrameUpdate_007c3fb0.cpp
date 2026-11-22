// CTCHealLife::FrameUpdate()
// Leaf: ?FrameUpdate@CTCHealLife@@UAEXXZ
// Flags: (none)
// Inferred: FableWin 0x02713C79 has the same constant-false branch before the iterator scope as StartHealingEffect.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCENEMY_ARETHINGSENEMIES
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCHEALLIFE_START_ORIGINAL
#define FABLE_RETAIL_CTCHEALLIFE_FRAMEUPDATE_PASS26
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_STLP_LIST
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesHealLifeDef.h"
#include "engine/CTCEnemy.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/CTCHealLife.h"

void CTCHealLife::FrameUpdate()
{
    CThingCreatureBase* pcreature = GFDebugPDynamicCast(&GetParentThing(), (CThingCreatureBase*)0);
    pcreature->MakeNoise("SIMPLE_NOISE");
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    CCharString effectname = GetLevelEffectName();
    long index = 0;
    if (false) {} else
    for (std::list<CIntelligentPointer<CThingCreatureBase> >::iterator it = ThingsToHeal.begin(), endit = ThingsToHeal.end();
            it != endit; ++it) {
        CIntelligentPointer<CThingCreatureBase> something = *it;
        if (IS_THING_ALIVE(something) && IS_THING_ALIVE(PHealingTarget)) {
            if (!CTCEnemy::AreThingsEnemies(*PHealingTarget, *something)) {
                if (HealingEffects[index] == 0 && PDef->GetSubDef(&pspecial_effects) &&
                        pspecial_effects->GetEffectFromString(effectname, effect)) {
                    CIntelligentPointer<CThing> fx(CTCSpecialAbilities::CreateParticleEffect(effect, something->GetPos()));
                    if (IS_THING_ALIVE(fx)) {
                        CTCDParticleEmitter* ptc_emitter;
                        fx->GetTC(&ptc_emitter);
                        ptc_emitter->AttachToThing(*something, 0x28, "", 0, 0.0f);
                        HealingEffects[index] = fx;
                    }
                }
                HealthIncreaseAccum[index] += PDef->HealthIncreasePerFrame[SpellLevel];
                if (HealthIncreaseAccum[index] > 1.0f) {
                    float man = (float)floor(HealthIncreaseAccum[index]);
                    something->ModifyHealth(man, true);
                    HealthIncreaseAccum[index] -= man;
                }
            }
        }
        else if (IS_THING_ALIVE(HealingEffects[index])) {
            CTCDParticleEmitter* ptcd_particle_emitter = 0;
            HealingEffects[index]->GetTC(&ptcd_particle_emitter);
            ptcd_particle_emitter->EnterTimelineEnd();
        }
        ++index;
    }
    TotalHealthAccum += PDef->HealthIncreasePerFrame[SpellLevel];
    if (TotalHealthAccum >= PDef->TotalHealthIncrease[SpellLevel] || !IS_THING_ALIVE(PHealingTarget))
        pcreature->RemoveTC(TCI_HEAL_LIFE);
}