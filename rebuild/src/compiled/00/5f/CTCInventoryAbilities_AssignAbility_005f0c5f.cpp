// CTCInventoryAbilities::AssignAbility(long ability, unsigned long slot) -- put an ability in a quick slot: when the ability it replaces is held in no other slot, give that ability's menu entry a fresh "unassigned" icon; then store the new ability, delete the new ability's unassigned icon and forget it
// Leaf: ?AssignAbility@CTCInventoryAbilities@@QAEXJK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F0C5F, 358 bytes, thiscall `ret 8`; frame-pointer size TU (optimize "s").
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>(CComponent*)
//   0x00429C15 / ReleaseData 0x004291DE, map<EHeroAbility,CComponent*>::operator[] 0x005F9EA5 / _M_find 0x005F8444 /
//   _Rb_tree::erase(iterator) 0x0057926B, CComponent slots 3 FindChild, 60 AddChildDynamically, 62 SetDeletion, CDeletion
//   ctor 0x0042BED4 / copy 0x0042CD84 / dtor 0x0042ABCA.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CTCInventoryAbilities.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"

void CTCInventoryAbilities::AssignAbility(long ability, unsigned long slot)
{
    {
        EHeroAbility current = AssignedAbilities[slot];
        bool assignedElsewhere = false;
        for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
            if (AssignedAbilities[i] == current && i != slot)
                assignedElsewhere = true;
        }
        if (!assignedElsewhere && ability != current && current != 0) {
            CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));
            AbilitiesMenuEntries[current]->AddChildDynamically(icon);
            UnassignedIcons[current] = icon.Get();
        }
    }
    AssignedAbilities[slot] = static_cast<EHeroAbility>(ability);
    std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(static_cast<EHeroAbility>(ability));
    if (found != UnassignedIcons.end()) {
        NUISystem::CComponent* icon = found->second->FindChild("UI_UNASSIGNED_SPELL_ICON");
        if (icon) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            icon->SetDeletion(deletion);
        }
        UnassignedIcons.erase(found);
    }
}