// CTCWillLightning::SetAICasting(long) -- mark the will button timer as AI-cast, then set the lightning spell up for that level
// Leaf: ?SetAICasting@CTCWillLightning@@QAEXJ@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079F960, 9 bytes: `mov byte ptr [ecx+5Ah],1 ; jmp 0x0079F810` (the level argument left in place).
// Identity (two sources): Ego_r.pdb ?SetAICasting@CTCWillLightning@@QAEXJ@Z 0x007A4CC0 directly after ?SetLevel@CTCWillLightning@@AAEXJ@Z
//   0x007A4BC0, as retail 0x0079F960 follows the landed SetLevel 0x0079F810 (its jump target); the landed caller
//   CCreatureActionCastWillLightningSpell::Initialise 0x008B9C90 binds it (as the misspelt ?SetAICast@..., NAME_FIXES.tsv). The
//   catalog's "CEditInputProcessCopyPaste::AddToInterface" is propagated.
// Shape: the inline CWillButtonHelperTimer::SetAICasting() (IsAICasting = true; retail has no out-of-line copy) on m_tTimer (+0x2c,
//   IsAICasting at timer +0x2e), then the tail call SetLevel(level).
// Callees: ?SetLevel@CTCWillLightning@@AAEXJ@Z 0x0079F810 (private in both PDBs; landed binding ?SetLevel@CTCWillLightning@@QAEXJ@Z, NAME_FIXES.tsv).
// Header: engine/CTCWillLightning.h (FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING, new), engine/CWillButtonHelperTimer.h
//   (FABLE_RETAIL_CWILLBUTTONHELPERTIMER_SETAICASTING, new).
#define FABLE_RETAIL_CTCWILLLIGHTNING_AICASTING
#include "engine/CTCWillLightning.h"

void CTCWillLightning::SetAICasting(long level)
{
    m_tTimer.SetAICasting();
    SetLevel(level);
}