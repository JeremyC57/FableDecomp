// CTCQuickAccessMenu::GetMagicListUI — populate the quick-access spell list.
// Leaf: ?GetMagicListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@XZ
// Flags: default
// Fidelity scaffolding: emit the linked size-optimised UI action and STLport
// companions while retaining the speed mode of this quick-access method.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_AVAILABILITY
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_INTERFACETYPE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMSLONG_INLINE_DTOR_PASS27
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCQUICKACCESSMENU_MAGIC_LIST_PASS27
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/GFIntToWideString.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#if defined(_MSC_VER)
#pragma optimize("", on)
#endif
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/CTCQuickAccessMenu.h"

NUISystem::CComponent* CTCQuickAccessMenu::GetMagicListUI()
{
    using namespace NUISystem;
    CTCInventoryAbilities* ptc_abilities = 0;
    GetParentThing().GetTC(&ptc_abilities);
    std::list<EHeroAbility> abilities;
    for (long count = HERO_ABILITY_FORCE_PUSH; count < MAX_NUMBER_OF_HERO_ABILITIES; ++count) {
        if (ptc_abilities->IsAbilityAvailable(static_cast<EHeroAbility>(count)))
            abilities.push_back(static_cast<EHeroAbility>(count));
    }
    CCountedPointer<CComponent> pmagic_actions_list(CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false));
    CComponent* plist_component = 0;
    CManager* pmanager = CManager::GetInstance();
    if (!abilities.empty()) {
        plist_component = pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false);
        CList* plist = static_cast<CList*>(plist_component);
        plist->InitialiseOffsets(abilities.size());
        for (std::list<EHeroAbility>::iterator it = abilities.begin(); it != abilities.end(); ++it) {
            CWideString name;
            CTCInventoryExperience* ptc_experience;
            if (GetParentThing().GetTC(&ptc_experience)) {
                name = ptc_experience->GetExperienceAbilityName(*it);
                name += CWideString::CreateFromCharString(" ");
                name += GFIntToWideString(ptc_abilities->GetAbilityLevel(*it));
            }
            CCountedPointer<CComponent> pmenu_entry(pmanager->CreateMenuEntryComponent(name, 0.0f, 1.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CMenuEntry> pmenu_entry_real;
            pmenu_entry_real.SetStaticCast(pmenu_entry);
            CActionParam* pparam_ability = new CActionParamSLONG(*it);
            pmenu_entry_real->AddUIActionParam(pparam_ability);
            plist->AddScrollingChildAndSetOffsets(pmenu_entry);
        }
    }
    return plist_component;
}

#if defined(_MSC_VER)
#pragma optimize("s", on)
#endif