// CTCInventoryAbilities::CycleAssignedExpressions(bool forward) -- page the assignable expression buttons one page forward or back through the prioritised expressions (wrapping at either end, giving up after a full turn without an expression), re-assign the buttons from the new first index and play the menu click
// Grade: DIFFER(445v442) -- register naming only, on shared headers: retail this=edi and the button/count/page-start web in ebx (pushed after the empty test), ours this=ebx and that web in edi; retail tail-merges the early-return ~vector<long> free into the final free call (`push eax ; jmp`), ours keeps its own call; the min/max temporaries share the `original` / `checked` slots in retail. Tried (5 grades): plain (443), same-TU GetPrioritisedExpressionDefs stand-in (445, zero register now esi as retail), shared button variable (445), count declared before the clear loop (436: this=edi as retail but the size computed early), inline (long)defs.size() (445).
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

std::vector<long>& FindPrioritisedExpressionDefs_s5eed8c();   // stand-in callee (see below)
#pragma auto_inline(off)
// Same-TU stand-in for GetPrioritisedExpressionDefs (0x005EED8C, an earlier function of this TU, landed separately): it fills
// the caller's vector in place and keeps no pointer to it, so VC7.1 knows the local does not escape the call (as the
// GetPrioritisedAbilityDefs stand-in of the twin row 0x005F64DD). Only CycleAssignedExpressions is graded.
__declspec(noinline) void CTCInventoryAbilities::GetPrioritisedExpressionDefs(std::vector<long>& defs) const
{
    defs.swap(FindPrioritisedExpressionDefs_s5eed8c());
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
        SetExpressionToButton(*first > 0 ? *first : 0, button);
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
