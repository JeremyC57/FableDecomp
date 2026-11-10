// CTCInventoryAbilities::CycleAssignedAbilities(bool forward) -- clear the assignable ability buttons, then page one row forward or back through the prioritised ability defs (wrapping, giving up after a full turn without a def), re-assign the buttons from the new first index and play the menu click
// Grade: DIFFER(508v508) -- same length; left: (1) the before-the-start wrap `count - buttons` is computed in place in edi (`sub edi,eax`) where retail uses ecx (`mov ecx,edi ; sub ecx,eax`), and the search iterator's `mov edi,[ebp-4]` copy sits before the `checked < buttons` pre-test where retail sinks it below the `jle`; (2) the assign arm's shared pushes: retail hoists `push 0 (false) ; mov ecx,ebx ; push edi (button)` above the null test (0x005F661A..0x005F661D) and pushes only the ability in each arm, ours pushes all three in each arm.
// Leaf: ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F64DD, 506 bytes, thiscall `ret 4`; frame-pointer size TU
//   (`optimize("y", off)` + `("s", on)`, as the TU-mates 0x005F6423 / 0x005F694D).
//   Identity: Ego_r.pdb / FableWin.pdb ?CycleAssignedAbilities@CTCInventoryAbilities@@QAEX_N@Z (called by the landed
//   ResetAssignedAbilities 0x005F694D).
// Callees: GetPrioritisedAbilityDefs 0x005F4B30 (vector by reference), GetNumAssignableSpecialMoveButtons 0x005EDCF0 (the
//   landed callers' GetNumAbilitiesPerRow), SetAbilityToButton(EHeroAbility, long, bool) 0x005F6423, ~vector<CDefPointer<const
//   CHeroAbilityDef> > 0x005F9F14, CSoundInit() 0x0041C930, CThing::GetSampleBank 0x004C7A80, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, CASoundBank::GetSoundIndexFromSymbol 0x00A01920; through PSoundManager 0x013B8394
//   CASoundManager::Get2DEmitter (slot 9, +0x24) and CASoundEmitter::PlaySample (slot 3, +0x0C) of SND_MENU_04 (looped flag
//   set, not attached) on gain-fader group 0x20 (slot names: engine/CASoundManager.h, engine/CASoundEmitter.h).
// Layout: CurrentAssignedAbilityLevel +0x144 (Ego_r.pdb name; retail offset); CHeroAbilityDef::Ability +0x3C; CSoundInit
//   GainFaderBits +0x14 (engine/CSoundInit.h, Ego_r.pdb).
// Shape: the level is wrapped to `std::min(original, 0L)` past the end and `count - perRow` (at least 0) before the start, then
//   stepped by its own remainder (`level += level % perRow`, retail 0x005F65A2); the search builds a def handle copy only to
//   test it; the symbol string temporary is homed in the dead `forward` slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_BUTTONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PRIORITISED
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#include "engine/CTCInventoryAbilities.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CThing.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"

std::vector<CDefPointer<const CHeroAbilityDef> >& FindPrioritisedAbilityDefs_s5f4b30();   // stand-in callee (see below)
#pragma auto_inline(off)
// Same-TU stand-in for GetPrioritisedAbilityDefs (0x005F4B30, an earlier function of this TU, landed separately): it fills the
// caller's vector in place and keeps no pointer to it, so VC7.1 knows the local does not escape the call and keeps its end
// pointer in a CPU location across the later out-of-line calls (retail 0x005F651F `mov edi,[ebp-18h]`, reused for the size at
// 0x005F6546 after the GetNumAssignableSpecialMoveButtons call). Only CycleAssignedAbilities is graded.
__declspec(noinline) void CTCInventoryAbilities::GetPrioritisedAbilityDefs(std::vector<CDefPointer<const CHeroAbilityDef> >& defs) const
{
    defs.swap(FindPrioritisedAbilityDefs_s5f4b30());
}
#pragma auto_inline(on)

void CTCInventoryAbilities::CycleAssignedAbilities(bool forward)
{
    std::vector<CDefPointer<const CHeroAbilityDef> > defs;
    GetPrioritisedAbilityDefs(defs);
    long button;
    for (button = 0; button < GetNumAssignableSpecialMoveButtons(); ++button)
        SetAbilityToButton(HERO_ABILITY_NULL, button, false);
    if (defs.empty())
        return;
    long original = std::max(CurrentAssignedAbilityLevel, -GetNumAssignableSpecialMoveButtons());
    long count = defs.size();
    std::vector<CDefPointer<const CHeroAbilityDef> >::iterator first;
    for (;;) {
        if (forward)
            CurrentAssignedAbilityLevel += GetNumAssignableSpecialMoveButtons();
        else
            CurrentAssignedAbilityLevel -= GetNumAssignableSpecialMoveButtons();
        if (CurrentAssignedAbilityLevel >= count)
            CurrentAssignedAbilityLevel = std::min(original, 0L);
        else if (CurrentAssignedAbilityLevel < 0) {
            CurrentAssignedAbilityLevel = count - GetNumAssignableSpecialMoveButtons();
            if (CurrentAssignedAbilityLevel < 0)
                CurrentAssignedAbilityLevel = 0;
        }
        CurrentAssignedAbilityLevel += CurrentAssignedAbilityLevel % GetNumAssignableSpecialMoveButtons();
        first = defs.begin();
        if (CurrentAssignedAbilityLevel > 0)
            first += CurrentAssignedAbilityLevel;
        std::vector<CDefPointer<const CHeroAbilityDef> >::iterator it = first;
        for (long checked = 0; checked < GetNumAssignableSpecialMoveButtons(); ++checked, ++it)
            if (it != defs.end())
                if (CDefPointer<const CHeroAbilityDef>(*it).Get())
                    goto found;
        if (CurrentAssignedAbilityLevel == original)
            return;
    }
found:
    for (button = 0; first != defs.end(); ++first) {
        CDefPointer<const CHeroAbilityDef> def = *first;
        // two calls (retail 0x005F6614..0x005F6629: one null test shared with the def copy's AddRef, the arms push only the
        // ability into one cross-jumped call); a `?:` argument gives a select and another register web (512 bytes)
        if (def.IsNull())
            SetAbilityToButton(HERO_ABILITY_NULL, button, false);
        else
            SetAbilityToButton(def->Ability, button, false);
        if (++button == GetNumAssignableSpecialMoveButtons())
            break;
    }
    if (PSoundManager) {
        CSoundInit init;
        init.GainFaderBits = 0x20;
        CASoundBank& bank = GetParentThing().GetSampleBank();
        long sample = bank.GetSoundIndexFromSymbol("SND_MENU_04");
        PSoundManager->Get2DEmitter().PlaySample(bank, sample, true, init, false);
    }
}
