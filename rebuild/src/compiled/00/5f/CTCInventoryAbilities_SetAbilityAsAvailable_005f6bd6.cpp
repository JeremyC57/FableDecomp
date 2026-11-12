// CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign) -- mark an ability (un)available; on a newly available one record it in ReceivedAbilityOrder (first empty slot, else over the first duplicate), put it on the first free special-move button and flag the tutorial when more abilities than buttons are held; on a newly lost one take it off its button and out of ReceivedAbilityOrder; optionally step the assigned row back one page and re-cycle
// Leaf: ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6BD6, 618 bytes, thiscall `ret 0Ch`; frame-pointer size TU
//   (`#pragma optimize("y", off)` + `optimize("s", on)`, like its neighbours OnInitialActivate 0x005F6B47 / SetAbilityToButton 0x005F6423).
// Identity: Ego_r.pdb 0x00585AD8 and FableWin.pdb both name ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z;
//   caller CThingPlayerCreature::LearnAbility 0x006AC690.
// Callees: CTCBase::PeekPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960,
//   CGamePlayerInterface::GetNumberOfAssignableSpecialMoveButtons() const 0x00445940 (the chain written out once),
//   CVectorMap<long,EHeroAbility>::LowerBound 0x005F7F4C (same-TU noinline body; Find expanded), SetAbilityToButton 0x005F6423,
//   GetNumAssignableSpecialMoveButtons() const 0x005EDCF0, vector<pair<long,EHeroAbility> >::erase 0x005F87C0,
//   std::find<EHeroAbility*,EHeroAbility> 0x005F8150 (-> std::__find 0x005F7ACC), vector<EHeroAbility>::erase 0x005F8750,
//   CycleAssignedAbilities(bool) 0x005F64DD. Expanded inline: IsAbilityAvailable 0x005EDF08, GetNumAssignedReceivedAbilities
//   0x005EE146 (both defined earlier in retail's TU), CVectorMap::ValFind (FableWin.pdb member template).
// Shape: the find/__find COMDATs' bodies are visible in the TU (noinline), so the compiler knows they neither write memory nor keep
//   &ability: ability stays in ebx and `last` is reused after the call. The received-order insert is a found flag with `break`s
//   (the dup scan's outer loop tests the flag after the inner loop), giving retail's i+1 / j homes in the dead parameter slots.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_AVAILABILITY
#define FABLE_RETAIL_STLP_FIND_NOINLINE_BODY_FORWARDER
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_CVECTORMAP_VALFIND
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "fable_string.h"
#include "stlport/fable_algorithm.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#pragma optimize("y", off)
#pragma optimize("s", on)

inline bool CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility ability) const
{
    return Abilities[ability].first >= 0;
}

FABLE_FORCEINLINE long CTCInventoryAbilities::GetNumAssignedReceivedAbilities() const
{
    long assigned = 0;
    for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
        if (ReceivedAbilityOrder[i] != HERO_ABILITY_NULL)
            ++assigned;
    }
    return assigned;
}

void CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign)
{
    bool was_available = IsAbilityAvailable(ability);
    if (available)
        Abilities[ability].first = std::max(0L, Abilities[ability].first);
    else
        Abilities[ability].first = -1;
    if (Initialised) {
        if (available && !was_available) {
            bool added = false;
            for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
                if (ReceivedAbilityOrder[i] == HERO_ABILITY_NULL) {
                    ReceivedAbilityOrder[i] = ability;
                    added = true;
                    break;
                }
            }
            if (!added) {
                for (unsigned long i = 0; i < ReceivedAbilityOrder.size(); ++i) {
                    for (unsigned long j = i + 1; j < ReceivedAbilityOrder.size(); ++j) {
                        if (ReceivedAbilityOrder[j] == ReceivedAbilityOrder[i]) {
                            ReceivedAbilityOrder[j] = ability;
                            added = true;
                            break;
                        }
                    }
                    if (added)
                        break;
                }
            }
            long num_buttons = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons();
            for (long button = 0; button < num_buttons; ++button) {
                if (AbilityButtons.Find(button) == AbilityButtons.end()) {
                    SetAbilityToButton(ability, button, true);
                    break;
                }
            }
            if (GetNumAssignedReceivedAbilities() > GetNumAssignableSpecialMoveButtons())
                WaitingToGiveTutorial = true;
        }
        else if (was_available && !available) {
            CVectorMap<long, EHeroAbility>::iterator it = AbilityButtons.ValFind(ability);
            if (it != AbilityButtons.end())
                AbilityButtons.erase(it);
            std::vector<EHeroAbility>::iterator r = std::find(ReceivedAbilityOrder.begin(), ReceivedAbilityOrder.end(), ability);
            if (r != ReceivedAbilityOrder.end())
                ReceivedAbilityOrder.erase(r);
        }
    }
    if (reassign) {
        CurrentAssignedAbilityLevel = std::max(CurrentAssignedAbilityLevel - GetNumAssignableSpecialMoveButtons(), -GetNumAssignableSpecialMoveButtons());
        CycleAssignedAbilities(true);
    }
}