// CTCInventoryAbilities::AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container) -- put a spell in a PC quick-access slot: refuse it when the paired slot already holds it; give the displaced spell (held in no other slot) a fresh unassigned icon in its menu entry; store the spell, delete its own unassigned icon; delete the container's old contents and fill it with a new assignment icon showing the spell, droppable back (unassign) and swappable
// Leaf: ?AssignAbilityPC@CTCInventoryAbilities@@QAEXJKPAVCComponent@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F0DC5, 996 bytes, thiscall `ret 0Ch`; frame-pointer size TU (optimize "s").
// Callees: as AssignAbility 0x005F0C5F, plus operator new 0x00BFEA1A, CAction::CAction 0x0042BE50 / AddParam(param, self)
//   0x0053FDE0 / AddParam(param) 0x0053FDB0, CCountedPointer<CAction>(CAction*) 0x0042AA29 / ReleaseData 0x00429160,
//   list<CActionParam*>::push_back 0x0042B646, CSprite slot 145 SetGraphicIndex, CComponent slots 74 AddActionOnDropped /
//   75 AddActionOnDroppedNowhere and CObserver slot 3 ObserveEvent (on the component's CObserver base, +4).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CTCInventoryAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"

void CTCInventoryAbilities::AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container)
{
    EHeroAbility current = AssignedAbilities[slot];
    bool assignedElsewhere = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
        if (AssignedAbilities[i] == current && i != slot)
            assignedElsewhere = true;
    }
    // Slots pair up (0/1, 2/3, ...): the same spell may not sit in both halves of a pair.
    if (!(slot & 1) && slot < AssignedAbilities.size() - 1) {
        if (AssignedAbilities[slot + 1] == ability)
            return;
    } else if (slot > 0) {
        if (AssignedAbilities[slot - 1] == ability)
            return;
    }
    if (!assignedElsewhere && ability != current && current != 0) {
        CCountedPointer<NUISystem::CComponent> icon(NUISystem::CManager::GetInstance()->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[current]->AddChildDynamically(icon);
        UnassignedIcons[current] = icon.Get();
    }
    AssignedAbilities[slot] = static_cast<EHeroAbility>(ability);
    std::map<EHeroAbility, NUISystem::CComponent*>::iterator found = UnassignedIcons.find(static_cast<EHeroAbility>(ability));
    if (found != UnassignedIcons.end()) {
        NUISystem::CComponent* icon = found->second->FindChild("PC_UNASSIGNED_SPELL_ICON");
        if (icon) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            icon->SetDeletion(deletion);
        }
        UnassignedIcons.erase(found);
    }
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    // The old contents: an empty slot holds the empty spell item, an assigned one the assignment icon. Each arm looks up and
    // deletes its own child; VC7.1 cross-jumps the two arms into the single lookup at 0x005F0F91 (`jne ; push A ; jmp ;
    // push edi`), with the result in ebx as retail (one shared `old` assigned in the arms leaves it memory-only).
    if (current == 0) {
        NUISystem::CComponent* old = container->FindChild("PC_SPELL_INSIDE_CONTAINER");
        if (old) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            old->SetDeletion(deletion);
        }
    } else {
        NUISystem::CComponent* old = container->FindChild("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER");
        if (old) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            old->SetDeletion(deletion);
        }
    }
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CComponent* sprite = item->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
    if (sprite)
        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[ability].second->IconGraphicIndex);
    CCountedPointer<NUISystem::CAction> unassign(new NUISystem::CAction);
    unassign->ActionType = NUISystem::UI_ACTION_TYPE_UNASSIGN_SPELL_PC;
    unassign->AddParam(container, unassign);
    unassign->UIActionParams.push_back(new NUISystem::CActionParamULONG(slot));
    item->AddActionOnDroppedNowhere(unassign);
    item->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CCountedPointer<NUISystem::CAction> swap(new NUISystem::CAction);
    swap->ActionType = NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC;
    swap->UIActionParams.push_back(new NUISystem::CActionParamSLONG(ability));
    swap->UIActionParams.push_back(new NUISystem::CActionParamSLONG(slot));
    swap->AddParam(container);
    item->AddActionOnDropped(swap);
    container->AddChildDynamically(item);
}