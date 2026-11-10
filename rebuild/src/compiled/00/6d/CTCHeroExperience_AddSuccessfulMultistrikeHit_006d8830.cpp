// CTCHeroExperience::AddSuccessfulMultistrikeHit(const CHitParameters&) -- add attack-history-aware experience for each damage attribute.
// Leaf: ?AddSuccessfulMultistrikeHit@CTCHeroExperience@@QAEXABVCHitParameters@@@Z
// Flags:
// Retail: 0x006d8830,321 bytes,thiscall,ret4.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_MULTISTRIKE
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CThing.h"
#include "engine/CHitParameters.h"
#include "engine/CMainGameComponent.h"
void CTCHeroExperience::AddSuccessfulMultistrikeHit(const CHitParameters& hit)
{
    for (std::map<EDamageAttribute, float>::const_iterator it = hit.GetDamageMap().begin(); it != hit.GetDamageMap().end(); ++it) {
        EDamageAttribute type = it->first;
        float damage = it->second;
        AddAttackOfTypeToAttackHistory(type);
        const CComboMultiplierData& data = GetActiveComboMultiplierData();
        ExperienceAttenuationTimer = (long)(CMainGameComponent::GetConstantFPS() * data.ExperienceMultiplierNumSecondsForAttenuation);
        std::map<EDamageAttribute, float>::const_iterator found = data.ComboMultiplier.find(type);
        if (found != data.ComboMultiplier.end()) {
            float strength = (float)fabs(damage);
            ExperienceMultiplierParam += (GetAttackVariationMultiplier()) * strength * found->second;
            UpdateComboMultiplier(true);
        }
    }
}