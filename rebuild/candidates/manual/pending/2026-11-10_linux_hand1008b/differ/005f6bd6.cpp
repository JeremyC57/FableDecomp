// CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign) -- make an ability available (level at least 0) or unavailable (-1); once initialised, a newly available ability takes the first empty (else the first duplicated) entry of the received order, the first free special-move button, and raises the tutorial flag when more abilities are received than there are buttons; a newly unavailable one leaves its button and the received order; on request page the assigned row back one row and cycle forward
// Grade: DIFFER(617v618) -- register colouring, size 617 v 618 (chunk 1008b g4, difflines 72). Retail: ability in ebx (pieces: top, the received-order stores, the value search) and the zero constant in edi at the top (`cmp [eax],edi` x2, `mov [ebp-4],edi`; no byte use, so `cmp byte [ebp+0Ch],0` stays immediate); ours: ability in edi, const 0 split to immediates (c2prio: const 0 allowed ebx only, prio -81), entry.first CSE'd into ecx (`test ecx,ecx`). The duplicate scan: retail eliminates i (i*4 in ecx, i+1 in [ebp+0Ch], j in [ebp-4]), ours keeps i in edx. Retail passes &ability ([ebp+8]) to std::find, ours a copy's home [ebp+0Ch]; no not-found `mov eax,edx`.
// Leaf: ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6BD6, 618 bytes, thiscall `ret 0Ch`; frame-pointer size TU (optimize "s").
//   Ego_r.pdb / FableWin.pdb ?SetAbilityAsAvailable@CTCInventoryAbilities@@QAEXW4EHeroAbility@@_N1@Z.
// Callees: CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960,
//   CGamePlayerInterface::GetNumberOfAssignableSpecialMoveButtons 0x00445940, CVectorMap<long,EHeroAbility>::LowerBound 0x005F7F4C,
//   SetAbilityToButton 0x005F6423, GetNumAssignableSpecialMoveButtons 0x005EDCF0 (x3), vector<pair<long,EHeroAbility> >::erase
//   0x005F87C0, std::find<EHeroAbility*, EHeroAbility> 0x005F8150, vector<EHeroAbility>::erase 0x005F8750, CycleAssignedAbilities 0x005F64DD.
// Layout: WaitingToGiveTutorial +0x140, Initialised +0x141, CurrentAssignedAbilityLevel +0x144, ReceivedAbilityOrder +0x154,
//   Abilities +0x160, AbilityButtons +0x16C (engine/CTCInventoryAbilities.h, retail branch).
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_SPECIALMOVEBUTTONS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_STLP_FIND
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "stlport/fable_algorithm.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"

void CTCInventoryAbilities::SetAbilityAsAvailable(EHeroAbility ability, bool available, bool reassign)
{
    std::pair<long, CDefPointer<const CHeroAbilityDef> >& entry = Abilities[ability];
    bool wasAvailable = entry.first >= 0;
    if (available)
        entry.first = std::max(0L, entry.first);
    else
        entry.first = -1;
    if (Initialised) {
        if (available && !wasAvailable) {
            // the received order: the first empty entry, else the later of the first duplicated pair
            unsigned long i;
            for (i = 0; i < ReceivedAbilityOrder.size(); ++i) {
                if (ReceivedAbilityOrder[i] == HERO_ABILITY_NULL) {
                    ReceivedAbilityOrder[i] = ability;
                    goto received;
                }
            }
            for (i = 0; i < ReceivedAbilityOrder.size(); ++i) {
                for (unsigned long j = i + 1; j < ReceivedAbilityOrder.size(); ++j) {
                    if (ReceivedAbilityOrder[j] == ReceivedAbilityOrder[i]) {
                        ReceivedAbilityOrder[j] = ability;
                        goto received;
                    }
                }
            }
received:
            // the first special-move button nothing is assigned to
            long buttons = PeekPlayerManager().GetMainPlayer().PlayerInterface->GetNumberOfAssignableSpecialMoveButtons();
            for (long button = 0; button < buttons; ++button) {
                if (AbilityButtons.Find(button) == AbilityButtons.end()) {
                    SetAbilityToButton(ability, button, true);
                    break;
                }
            }
            long received = 0;
            for (unsigned long k = 0; k < ReceivedAbilityOrder.size(); ++k)
                if (ReceivedAbilityOrder[k] != HERO_ABILITY_NULL)
                    ++received;
            if (received > GetNumAssignableSpecialMoveButtons())
                WaitingToGiveTutorial = true;
        } else if (wasAvailable && !available) {
            EHeroAbility removed = ability;
            CVectorMap<long, EHeroAbility>& buttons = AbilityButtons;
            std::pair<long, EHeroAbility>* button = buttons.begin();
            std::pair<long, EHeroAbility>* last = buttons.end();
            for (; button != last; ++button)
                if (button->second == removed)
                    break;
            if (button != AbilityButtons.end())
                AbilityButtons.erase(button);
            std::vector<EHeroAbility>::iterator orderEnd = ReceivedAbilityOrder.end();
            std::vector<EHeroAbility>::iterator order = std::find(ReceivedAbilityOrder.begin(), orderEnd, removed);
            if (order != orderEnd)
                ReceivedAbilityOrder.erase(order);
        }
    }
    if (reassign) {
        CurrentAssignedAbilityLevel = std::max(CurrentAssignedAbilityLevel - GetNumAssignableSpecialMoveButtons(), -GetNumAssignableSpecialMoveButtons());
        CycleAssignedAbilities(true);
    }
}
