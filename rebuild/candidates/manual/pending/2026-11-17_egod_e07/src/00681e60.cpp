// CTCCreatureOpinionOfHero::SetPersonalityOverride(long personality_def, bool replace_existing_override) -- switch the creature to an OPINION_PERSONALITY def (unless it already has an override and replacing was not asked): take the def's attitude mask (wife attitudes always off) and rescale the run-out of every transient opinion offset by the new / old run-out time scaling; 0 (or a bad def) resets the mask to all-enabled
// Leaf: ?SetPersonalityOverride@CTCCreatureOpinionOfHero@@QAEXJ_N@Z
// Inline-budget: FABLE_RETAIL_STLP_FILL_FASTCALL (stlport/fable_containers.h) -- not an inline control but a calling convention: retail's
//        variable-value fill is the __fastcall instance ??$fill@PA_N_N@std@@YIXPA_N0AB_N@Z (0x00685430; the opt-in body grades MATCH there)
//        called at 0x00681F04; VC7.1 itself calls it and expands the constant-true fill (rep stos). Without the opt-in: DIFFER(626v626).
// Retail: 0x00681E60, 626 bytes (ret 8).
// Names: Ego_r / Ego_d; Ego_d locals def_mgr (const CGameDefinitionManager&), pdef_old, found_attitude (vector<bool>), c1 (both branches),
//        end_it / it / attitude_type (the attitude map walk), end_it / it / transient / relative_time_scaling / remaining_runout_delta /
//        new_runout_frames (the transient list walk). Structure from e05's local-model RELOCATION_MATCH (ref/00681e60).
// FITTED: the plain `{ }` block around the def-path body after the bad-def early return (Ego_d puts found_attitude, the c1 loop and both
//        walks one scope below pdef_old; the exact original construct that opened that scope is unknown).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSTYPENAME
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CDEFSTRING_NE_DEFSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_FILL_FASTCALL
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SETPERSONALITYOVERRIDE_INDEX
#define FABLE_RETAIL_COPINIONTRANSIENTOFFSET_TRAITS
#define FABLE_RETAIL_COPINIONPERSONALITYDEF_ATTITUDEMAP
#include "fable_string.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/COpinionPersonalityDef.h"
#include "engine/COpinionOfHeroDef.h"
#include "engine/CTCCreatureOpinionOfHero.h"

void CTCCreatureOpinionOfHero::SetPersonalityOverride(long personality_def, bool replace_existing_override)
{
    const CGameDefinitionManager& def_mgr = GetWorld().PeekDefinitionManager();
    if (!replace_existing_override && PPersonalityDef->GetInstantiationName() != def_mgr.GetDefNameFromGlobalIndex(PDef->Personality))
        return;
    if (personality_def)
    {
        CDefPointer<const COpinionPersonalityDef> pdef_old = PPersonalityDef;
        if (!(def_mgr.GetDefClassTypeNameFromGlobalIndex(personality_def) == "OPINION_PERSONALITY") || !def_mgr.GetDef(personality_def, &PPersonalityDef))
        {
            SetPersonalityOverride(0, true);
            return;
        }
        {
            std::fill(AttitudeMask.begin(), AttitudeMask.end(), PPersonalityDef->AttitudeEnabledDefaultForNotNone);
            for (long c1 = OPINION_ATTITUDE_TYPE_WIFE_FIRST; c1 < OPINION_ATTITUDE_TYPE_LAST; ++c1)
                AttitudeMask[c1] = false;
            std::vector<bool> found_attitude(OPINION_ATTITUDE_TYPE_LAST, false);
            {
                std::map<EOpinionAttitudeType, bool>::const_iterator end_it = PPersonalityDef->AttitudeEnabled.end();
                for (std::map<EOpinionAttitudeType, bool>::const_iterator it = PPersonalityDef->AttitudeEnabled.begin(); it != end_it; ++it)
                {
                    EOpinionAttitudeType attitude_type = it->first;
                    if (attitude_type < OPINION_ATTITUDE_TYPE_LAST)
                    {
                        found_attitude[attitude_type] = true;
                        AttitudeMask[attitude_type] = it->second;
                    }
                }
            }
            {
                std::list<COpinionTransientOffset>::iterator end_it = TransientOffsets.end();
                for (std::list<COpinionTransientOffset>::iterator it = TransientOffsets.begin(); it != end_it; ++it)
                {
                    COpinionTransientOffset& transient = *it;
                    float relative_time_scaling = PPersonalityDef->PersonalityTraits[transient.Opinion].TimeScalingRunOut;
                    if (pdef_old->PersonalityTraits[transient.Opinion].TimeScalingRunOut > 0.0001f)
                        relative_time_scaling /= pdef_old->PersonalityTraits[transient.Opinion].TimeScalingRunOut;
                    float remaining_runout_delta = transient.FramesToRunOut * transient.OffsetPerFrameRunOut;
                    long new_runout_frames = (long)(transient.FramesToRunOut * relative_time_scaling);
                    if (new_runout_frames < 1)
                        new_runout_frames = 1;
                    transient.FramesToRunOut = new_runout_frames;
                    transient.OffsetPerFrameRunOut = remaining_runout_delta / new_runout_frames;
                }
            }
        }
    }
    else
    {
        std::fill(AttitudeMask.begin(), AttitudeMask.end(), true);
        for (long c1 = OPINION_ATTITUDE_TYPE_WIFE_FIRST; c1 < OPINION_ATTITUDE_TYPE_LAST; ++c1)
            AttitudeMask[c1] = false;
    }
}
