// CTCInventoryAbilities::CycleAssignedExpressions(bool forward) -- page the assignable expression buttons one page forward or back through the prioritised expressions (wrapping at either end, giving up after a full turn without an expression), re-assign the buttons from the new first index and play the menu click
// Grade: DIFFER(447v442) -- registers now retail's (this=edi, page start/count web in ebx pushed after the empty test, button esi) and the assign loop's two-call arm; left: (1) the search loop's iterator is homed at [ebp-0Ch] before the `checked < buttons` call (retail keeps it in eax and enters the rotated loop with `mov eax,ebx ; jmp`), (2) the std::max / std::min temporaries pack into that iterator slot (retail: the max temporary in `original`'s [ebp-8], the min one in `checked`'s [ebp-4]), (3) retail restores ebx before the final ~vector free (`cmp ; pop ebx ; je`) so the early-return free cross-jumps into it (`push eax ; jmp`); ours restores ebx after it and keeps a second `call free ; pop ecx`.
// Leaf: ?CycleAssignedExpressions@CTCInventoryAbilities@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F6793, 440 bytes, thiscall `ret 4`; frame-pointer size TU
//   (`optimize("y", off)` + `("s", on)`, the TU of CycleAssignedAbilities 0x005F64DD).
//   Identity: Ego_r.pdb / FableWin.pdb ?CycleAssignedExpressions@CTCInventoryAbilities@@QAEX_N@Z.
// Callees: GetPrioritisedExpressionDefs 0x005EED8C (vector<long> by reference), GetNumAssignableSpecialMoveButtons 0x005EDCF0,
//   SetExpressionToButton(long, long) 0x005F66D9, CSoundInit() 0x0041C930, CThing::GetSampleBank 0x004C7A80, CCharString(const
//   char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CASoundBank::GetSoundIndexFromSymbol 0x00A01920, free 0x00BFEA14 (the
//   inline ~vector<long>); through PSoundManager 0x013B8394 CASoundManager::Get2DEmitter (slot 9, +0x24) and
//   CASoundEmitter::PlaySample (slot 3, +0x0C) of SND_MENU_04 (looped flag set, not attached) on gain-fader group 0x20.
// Layout: CurrentAssignedExpressionLevel +0x1B8 (Ego_r.pdb name; retail offset); CSoundInit GainFaderBits +0x14.
// Shape: the twin of CycleAssignedAbilities without the row-remainder step and the def handles: an expression is usable when
//   positive; the empty test comes before the buttons are cleared.
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
#include "engine/CThing.h"
#include "engine/CASoundBank.h"
#include "engine/CSoundInit.h"
#include "engine/CASoundManager.h"

#pragma auto_inline(off)
// The real body of GetPrioritisedExpressionDefs (0x005EED8C, landed; an earlier function of this TU, called out of line): with it
// visible c2 knows the caller's vector is only appended to and never captured, so the local stays out of memory across the call.
// Only CycleAssignedExpressions is graded.
void CTCInventoryAbilities::GetPrioritisedExpressionDefs(std::vector<long>& defs) const
{
    for (unsigned long i = 0; i < ReceivedExpressionOrder.size(); ++i)
        defs.push_back(ReceivedExpressionOrder[i]);
}
#pragma auto_inline(on)

void CTCInventoryAbilities::CycleAssignedExpressions(bool forward)
{
    std::vector<long> defs;
    GetPrioritisedExpressionDefs(defs);
    if (defs.empty())
        return;
    for (long button = 0; button < GetNumAssignableSpecialMoveButtons(); ++button)
        SetExpressionToButton(0, button);
    long original = std::max(CurrentAssignedExpressionLevel, -GetNumAssignableSpecialMoveButtons());
    long count = defs.size();
    std::vector<long>::iterator first;
    for (;;) {
        if (forward)
            CurrentAssignedExpressionLevel += GetNumAssignableSpecialMoveButtons();
        else
            CurrentAssignedExpressionLevel -= GetNumAssignableSpecialMoveButtons();
        if (CurrentAssignedExpressionLevel >= count)
            CurrentAssignedExpressionLevel = std::min(original, 0L);
        else if (CurrentAssignedExpressionLevel < 0)
            CurrentAssignedExpressionLevel = count - GetNumAssignableSpecialMoveButtons();
        first = defs.begin();
        if (CurrentAssignedExpressionLevel > 0)
            first += CurrentAssignedExpressionLevel;
        std::vector<long>::iterator it = first;
        for (long checked = 0; checked < GetNumAssignableSpecialMoveButtons(); ++checked, ++it)
            if (it != defs.end() && *it > 0)
                goto found;
        if (CurrentAssignedExpressionLevel == original)
            return;
    }
found:
    for (long button = 0; first != defs.end(); ++first) {
        // two calls (retail 0x005F68B4..0x005F68BC: `push esi ; jle ; push eax ; jmp` / `push 0` into one cross-jumped `call`); a `?:`
        // argument computes the value first (`jg ; xor eax,eax`) and colours `this` ebx instead of edi
        if (*first > 0)
            SetExpressionToButton(*first, button);
        else
            SetExpressionToButton(0, button);
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
