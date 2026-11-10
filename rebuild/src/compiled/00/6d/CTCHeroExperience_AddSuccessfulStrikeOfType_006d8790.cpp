// CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool increase) -- record a successful strike of one damage type: push it on the attack history, restart the experience-attenuation timer and, unless the hero has the multi-strike TC, add |damage| x attack variation x the type's combo multiplier to the experience multiplier and update the combo
// Leaf: ?AddSuccessfulStrikeOfType@CTCHeroExperience@@AAEXW4EDamageAttribute@@M_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D8790, 124 bytes, thiscall, `ret 0Ch`.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_MULTISTRIKE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STRIKEOFTYPE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_TREE_CONSTFIND_TEMPLATE_BODY
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(fabs)
#endif
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CThing.h"
#include "engine/CMainGameComponent.h"

void CTCHeroExperience::AddSuccessfulStrikeOfType(EDamageAttribute type, float damage, bool increase)
{
    AddAttackOfTypeToAttackHistory(type);
    const CComboMultiplierData& data = GetActiveComboMultiplierData();
    ExperienceAttenuationTimer = (long)(CMainGameComponent::GetConstantFPS() * data.ExperienceMultiplierNumSecondsForAttenuation);
    if (!GetParentThing().HasTC(TCI_MULTI_STRIKE)) {
        std::map<EDamageAttribute, float>::const_iterator found = data.ComboMultiplier.find(type);
        if (found != data.ComboMultiplier.end()) {
            float strength = (float)fabs(damage);
            ExperienceMultiplierParam += (GetAttackVariationMultiplier()) * strength * found->second;
            UpdateComboMultiplier(increase);
        }
    }
}