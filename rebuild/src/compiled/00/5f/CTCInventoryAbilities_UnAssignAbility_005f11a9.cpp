// CTCInventoryAbilities::UnAssignAbility(unsigned long slot) -- take the ability out of a spell quick slot: the slot falls back to its saved ability, or, when slot and save already agree, both are cleared; when the removed ability is then held in no slot any more its menu entry gets a fresh "unassigned" icon; the unassigned icon of the ability now in the slot is deleted and forgotten
// Leaf: ?UnAssignAbility@CTCInventoryAbilities@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F11A9, 375 bytes, thiscall `ret 4`; frame-pointer size TU (optimize "s"),
//   the function between AssignAbilityPC 0x005F0DC5 and UnAssignAbilityPC 0x005F1320 (same body shape as the latter).
// Name: Ego_r.pdb + FableWin.pdb ?UnAssignAbility@CTCInventoryAbilities@@QAEXK@Z.
// Callees: 0x0099EBF0 CCharString(const char*, long), 0x0099EAE0 ~CCharString, 0x0041E5F2 NUISystem::CManager::GetInstance,
//   0x0041DB1D CManager::CreateComponent, 0x00429C15 CCountedPointer<CComponent>(CComponent*), 0x004291DE
//   CCountedPointer<CComponent>::ReleaseData, 0x005F9EA5 std::map<EHeroAbility,CComponent*>::operator[], 0x005F8444
//   _Rb_tree::_M_find (behind map::find), 0x0057926B _Rb_tree::erase(iterator), 0x0042BED4 NUISystem::CDeletion(),
//   0x0042CD84 CDeletion(const CDeletion&), 0x0042ABCA std::list<CComponent*>::~list (the CDeletion destructor),
//   CComponent vslots 3 FindChild, 60 AddChildDynamically, 62 SetDeletion.
// Shape: retail reads AssignedAbilities[slot] into a scratch register (ecx), compares it with the saved ability (eax),
//   then copies it to the loop register (edx) and homes it at [ebp-4] for the two map::operator[] keys, and re-reads each
//   vector's begin pointer for the stores. VC7.1 emits exactly that when the slot is read twice before the branch (one
//   value for the comparison, one for the key local) and the loop value is a copy of the key local; a single read lets it
//   keep &AssignedAbilities[slot] in a register instead. Local names are descriptive.
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
    EHeroAbility current = AssignedAbilities[slot];
    EHeroAbility saved = AssignedAbilitiesSave[slot];
    EHeroAbility removed = AssignedAbilities[slot];
    EHeroAbility ability = removed;
    if (current != saved) {
        AssignedAbilities[slot] = saved;
    } else {
        AssignedAbilities[slot] = static_cast<EHeroAbility>(0);
        AssignedAbilitiesSave[slot] = static_cast<EHeroAbility>(0);
    }
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