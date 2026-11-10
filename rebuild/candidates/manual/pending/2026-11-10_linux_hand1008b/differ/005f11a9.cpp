// CTCInventoryAbilities::UnAssignAbility(unsigned long slot) -- empty a quick slot (it falls back to the slot's saved ability unless that is the one removed); when the removed ability is held in no slot any more, give its menu entry a fresh "unassigned" icon; then delete the unassigned icon of the ability now in the slot and forget it
// Grade: DIFFER(373v375) -- the head and the scan-loop register, size 373 v 375 (chunk 1008b g7; 358 -> 373 by comparing a re-read AssignedAbilities[slot] instead of the loaded value, which breaks the address CSE into the restore/clear arms): retail homes `removed` (`mov edx,ecx ; mov [ebp-4],edx`) before the je and runs the scan loop on edx, a copy distinct from the load temp; ours stores it after the arms and merges ability with the load temp (eax), so the rotation differs (retail A[slot] in ecx, S[slot] in eax). 30+ spellings probed (v2-v31, s1, s2).
// Leaf: ?UnAssignAbility@CTCInventoryAbilities@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F11A9, 375 bytes, thiscall `ret 4`; frame-pointer size TU (optimize "s").
// Callees: as AssignAbility 0x005F0C5F (CreateComponent 0x0041DB1D, map<EHeroAbility,CComponent*>::operator[] 0x005F9EA5 /
//   _M_find 0x005F8444 / _Rb_tree::erase(iterator) 0x0057926B, CDeletion 0x0042BED4 / 0x0042CD84 / 0x0042ABCA).
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

void CTCInventoryAbilities::UnAssignAbility(unsigned long slot)
{
    EHeroAbility ability = AssignedAbilities[slot];
    if (AssignedAbilities[slot] != AssignedAbilitiesSave[slot]) {
        AssignedAbilities[slot] = AssignedAbilitiesSave[slot];
    } else {
        AssignedAbilities[slot] = static_cast<EHeroAbility>(0);
        AssignedAbilitiesSave[slot] = static_cast<EHeroAbility>(0);
    }
    EHeroAbility removed = ability;
    bool stillAssigned = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
        if (AssignedAbilities[i] == ability) {
            stillAssigned = true;
            break;
        }
    }
    if (!stillAssigned && ability != 0) {
        CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("UI_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[removed]->AddChildDynamically(icon);
        UnassignedIcons[removed] = icon.Get();
    }
    if (AssignedAbilities[slot] != 0) {
        std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(AssignedAbilities[slot]);
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
}
