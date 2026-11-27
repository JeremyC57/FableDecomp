// 0077a890 CTCDExperienceOrb::Create
// Leaf: ?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z
// Flags: none
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CCREATUREDEF_SIZE_BODY
#define FABLE_RETAIL_CCREATUREDEF_STATGETTYPENAME
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#define FABLE_RETAIL_CEXPERIENCEDEF_PASS22
#define FABLE_RETAIL_CTCDEXPERIENCEORB_PASS22_INITIALACTIVATE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTHINGCREATUREBASE_ATTACKEDBY
#define FABLE_RETAIL_EXPERIENCE_ORB_CREATE_PASS29
#define FABLE_RETAIL_GFDEBUGDYNAMICCAST
#define FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_STLP_VECTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCDExperienceOrb.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatRandom.h"
#include "engine/CCreatureDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureStatsDef.h"
#include "engine/NHeroExperience.h"
void CTCDExperienceOrb::GetExperienceOrbsToCreateForTotalExperience(long experience, std::vector<long>& orbs_to_create_values) const
{
    while (experience > 0) {
        std::map<long, float>::const_iterator lower_it = GetDef().ParticleScalesPerExperiencePoints.lower_bound(experience);
        if (lower_it != GetDef().ParticleScalesPerExperiencePoints.begin())
            --lower_it;
        long threshold_experience = (std::max)(lower_it->first, 1L);
        orbs_to_create_values.push_back(threshold_experience);
        experience -= threshold_experience;
    }
}
namespace {
const C3DVector VELOCITY_TO_START_MOVING_AT(0.3f, 0.0f, 0.4f);
}

void FABLE_FASTCALL CTCDExperienceOrb::Create(CThing& creator)
{
    float weapon_based_multiplier = 1.0f;
    if (creator.GetTypeFlags() & 8) {
        CThingCreatureBase& creature_creator = *GFDebugDynamicCast(&creator, (CThingCreatureBase*)0);
        const CThingCreatureBase* pthing_killed_by = creature_creator.GetPLastAttackedByCreature();
        if (IS_THING_ALIVE(pthing_killed_by))
            weapon_based_multiplier = NHeroExperience::GetWeaponModifiedExperienceMultiplierForThing(*pthing_killed_by);
    }
    long experience_worth;
    CDefPointer<const CCreatureDef> pcreature_def;
    if (creator.PeekBaseDef().GetSubDef(&pcreature_def))
        experience_worth = pcreature_def->GetStatsDef().ExperienceWorth;
    else
        experience_worth = 1;
    experience_worth = (long)(experience_worth * weapon_based_multiplier);
    if (experience_worth) {
        long experience_orb_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("THING_EXPERIENCE_ORB");
        std::vector<CThing*> orb_list;
        std::vector<long> orbs_to_create_values;
        CThing* pexperience_orb = Create(experience_orb_def_index, creator.GetPos());
        if (IS_THING_ALIVE(pexperience_orb)) {
            orb_list.push_back(pexperience_orb);
            CTCDExperienceOrb* ptcd_experience_orb;
            pexperience_orb->GetTC(&ptcd_experience_orb);
            ptcd_experience_orb->GetExperienceOrbsToCreateForTotalExperience(experience_worth, orbs_to_create_values);
        }
        for (long c1 = 0; c1 < orbs_to_create_values.size(); ++c1) {
            if (IS_THING_ALIVE(pexperience_orb)) {
                CTCDExperienceOrb* ptcd_experience_orb;
                pexperience_orb->GetTC(&ptcd_experience_orb);
                ptcd_experience_orb->SetExperiencePoints(orbs_to_create_values[c1]);
                C3DVector initial_velocity = VELOCITY_TO_START_MOVING_AT;
                initial_velocity.RotateXY(GFFloatRandom(1.0f, ptcd_experience_orb->GetPWorldSeed()));
                pexperience_orb->GetPhysicsTC()->SetVelocity(initial_velocity);
            }
            pexperience_orb = 0;
            if (c1 + 1 < orbs_to_create_values.size()) {
                pexperience_orb = Create(experience_orb_def_index, creator.GetPos());
                orb_list.push_back(pexperience_orb);
            }
        }
    }
}