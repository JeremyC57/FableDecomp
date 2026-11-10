// CTCInventoryAbilities::UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container) -- empty a PC spell quick slot: when the removed ability is held in no slot any more its menu entry gets a fresh unassigned icon; the unassigned icon of the ability now in the slot is deleted and forgotten, the container's assignment icon is deleted and hidden, and the container gets a fresh empty spell item
// Leaf: ?UnAssignAbilityPC@CTCInventoryAbilities@@QAEXKPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F1320, 518 bytes, thiscall `ret 8`; frame-pointer size TU (optimize "s").
// Callees: as UnAssignAbility 0x005F11A9, plus CComponent slots 13 SetColour / 14 GetColour (CRGBColour by hidden pointer).
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

void CTCInventoryAbilities::UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container)
{
    EHeroAbility ability = AssignedAbilities[slot];
    AssignedAbilities[slot] = static_cast<EHeroAbility>(0);
    AssignedAbilitiesSave[slot] = static_cast<EHeroAbility>(0);
    EHeroAbility removed = ability;
    bool stillAssigned = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
        if (AssignedAbilities[i] == ability) {
            stillAssigned = true;
            break;
        }
    }
    if (!stillAssigned && ability != 0) {
        CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[removed]->AddChildDynamically(icon);
        UnassignedIcons[removed] = icon.Get();
    }
    if (AssignedAbilities[slot] != 0) {
        std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(AssignedAbilities[slot]);
        if (found != UnassignedIcons.end()) {
            NUISystem::CComponent* icon = found->second->FindChild("PC_UNASSIGNED_SPELL_ICON");
            if (icon) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                icon->SetDeletion(deletion);
            }
            UnassignedIcons.erase(found);
        }
    }
    NUISystem::CComponent* assignment = container->FindChild("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER");
    if (assignment) {
        NUISystem::CDeletion deletion;
        deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
        assignment->SetDeletion(deletion);
        CRGBColour colour = assignment->GetColour();
        colour.A = 0;
        assignment->SetColour(colour);
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_SPELL_INSIDE_CONTAINER", false));
    container->AddChildDynamically(item);
}