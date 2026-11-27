// 007b1a30 CTCFishing::CreateFish
// Leaf: ?CreateFish@CTCFishing@@IAE_N_N@Z
// Flags: none
// Inline-budget: The lower fish list calls its push_back at the selected branch. The opposite branch uses the native expansion.
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CREATEFISH_PASS29
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CFISHINGDEF_PASS22
#define FABLE_RETAIL_CTCFISHING_PASS22
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CFISHDEF_VIRTUAL_MODEL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_STLP_LIST
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingObject.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CFishDef.h"
#include "engine/CTCFish.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCFishing.h"
#include "engine/GFRandom.h"
bool CTCFishing::CreateFish(bool using_fishing_spot)
{
    CTCHeroExperience* ptc_experience;
    if (GetParentThing().GetTC(&ptc_experience)) {
        long fishing_level = using_fishing_spot ? 1 : ptc_experience->GetFishingLevel();
        std::vector<long> fish_lower;
        std::vector<long> fish_level;
        if (false) {} else for (long i = 0; i < PDef->FishTypes.size(); ++i) {
            CDefPointer<const CDefClassBase> pbase_def = GetDefinitionManager().GetPDefFromGlobalIndex(PDef->FishTypes[i]);
            CDefPointer<const CFishDef> pfish_def;
            if (pbase_def->GetSubDef(&pfish_def)) {
                if (pfish_def->Level == fishing_level)
                    fish_level.push_back(PDef->FishTypes[i]);
                else if (pfish_def->Level < fishing_level) {
                    // Retail expands the subscript and calls the frame-pointer push_back owner at 007b1b57.
                    const long& value=PDef->FishTypes[i];
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                    fish_lower.push_back(value);
#ifdef _MSC_VER
#pragma inline_depth(8)
#endif
                }
            }
        }
        long fish_def_index = 0;
        if (!fish_level.empty())
            fish_def_index = fish_level[GFRandom(fish_level.size(), GetPWorldSeed())];
        else if (!fish_lower.empty())
            fish_def_index = fish_lower[GFRandom(fish_lower.size(), GetPWorldSeed())];
        if (fish_def_index > 0) {
            PFish = CThingObject::Create(fish_def_index, GetParentThing().GetPos(), GetParentThing().GetPlayerNumber(), 0, 0, "");
            if (PFish.Get()) {
                PFish->SetInLimbo(true);
                CTCFish* ptc_fish = 0;
                PFish->GetTC(&ptc_fish);
                long level_difference = fishing_level - ptc_fish->GetPDef()->Level;
                if (fishing_level > 0) {
                    ptc_fish->SetLength(ptc_fish->GetLength() * (1.0f + 0.2f * level_difference));
                    float weight_mult = 1.0f + 0.2f * level_difference;
                    weight_mult = weight_mult * weight_mult * weight_mult;
                    ptc_fish->SetWeight(ptc_fish->GetWeight() * weight_mult);
                }
                CDefPointer<const CRegionDef> pregion_def = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
                if (!pregion_def.IsNull()) {
                    float region_mult = pregion_def->FishWeightMult;
                    float theme_mult = GetWorld().GetEnvironmentFishWeightMult(GetParentThing().GetPos());
                    float rain_mult = 1.0f + GetWorld().GetEnvironment().GetRainStrength() * (PDef->FishWeightRainMult - 1.0f);
                    float snow_mult = 1.0f + GetWorld().GetEnvironment().GetSnowStrength() * (PDef->FishWeightSnowMult - 1.0f);
                    float mist_mult = 1.0f + GetWorld().GetEnvironment().GetMistStrength() * (PDef->FishWeightMistMult - 1.0f);
                    ptc_fish->SetWeight(ptc_fish->GetWeight() * region_mult * theme_mult * rain_mult * snow_mult * mist_mult);
                }
                CurrentFishWeight = (long)ptc_fish->GetWeight();
                return true;
            }
        }
    }
    return false;
}