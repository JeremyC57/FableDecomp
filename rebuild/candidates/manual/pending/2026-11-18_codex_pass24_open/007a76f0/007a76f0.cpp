// CTCBattleCharge::CreateStartUpEffect() -- set up battle charge particles and sound.
// Grade: DIFFER(1219v1239) -- The shared owner uses the original battle definition, effect map, noise and particle APIs. The direction construction and three intelligent-pointer effect blocks retain different stack and instruction schedules.
// Leaf: ?CreateStartUpEffect@CTCBattleCharge@@AAEXXZ
// Retail: 0x007A76F0, 1239 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCBATTLECHARGE_STARTUP_PASS24
#define FABLE_RETAIL_CSPECIALABILITIESBATTLECHARGEDEF_STARTUP_PASS24
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_BITSET_INDEX
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/ThingAlive.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCBattleCharge.h"
#include "engine/_quarantine/CSpecialAbilitiesBattleChargeDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

void CTCBattleCharge::CreateStartUpEffect()
{
    CThingCreatureBase& parent_creature = static_cast<CThingCreatureBase&>(GetParentThing());
    parent_creature.MakeNoise("SIMPLE_NOISE");
    long effect = 0;
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    if (PDef->GetSubDef(&pspecial_effects)) {
        C3DVector movement_direction;
        if (DirectionOverride && !IsZTargetting)
            movement_direction = DirectionOverrideVector;
        else if (IS_THING_ALIVE(PTarget))
            movement_direction = PTarget->GetPos() - parent_creature.GetPos();
        else
            movement_direction = parent_creature.GetPhysicsTC()->GetFacingVector();
        CRightHandedSet rhset(C3DVector(0.0f, 0.0f, 1.0f), movement_direction);
        CCharString shield_effect_name("SHIELD_EFFECT_01");
        switch (SpellLevel) {
        case 1: shield_effect_name = "SHIELD_EFFECT_02"; break;
        case 2: shield_effect_name = "SHIELD_EFFECT_03"; break;
        case 3: shield_effect_name = "SHIELD_EFFECT_04"; break;
        }
        if (pspecial_effects->GetEffectFromString(shield_effect_name, effect)) {
            ShieldEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, parent_creature.GetPos(), false));
            if (IS_THING_ALIVE(ShieldEffect)) {
                CTCDParticleEmitter* particle_driver;
                ShieldEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 0x28, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
        if (pspecial_effects->GetEffectFromString("LEFT_FEET_EFFECT", effect)) {
            LeftFeetEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(LeftFeetEffect)) {
                CTCDParticleEmitter* particle_driver;
                LeftFeetEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 8, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
        if (pspecial_effects->GetEffectFromString("RIGHT_FEET_EFFECT", effect)) {
            RightFeetEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(RightFeetEffect)) {
                CTCDParticleEmitter* particle_driver;
                RightFeetEffect->GetTC(&particle_driver);
                particle_driver->AttachToThing(parent_creature, 8, "", 0, 0.0f);
                particle_driver->SetEmitterOrientation(rhset);
                particle_driver->SetAlwaysUpdate(true);
            }
        }
    }
}
