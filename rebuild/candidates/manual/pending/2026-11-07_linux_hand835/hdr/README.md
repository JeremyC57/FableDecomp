# hdr/ -- header overlay of chunk 835 (expert retry)

Every file is a fresh copy of `rebuild/include/engine/<Class>.h` plus one opt-in block; the default path is textually unchanged
(`diff rebuild/include/engine/X.h hdr/engine/X.h` shows only the added `#ifdef ... #endif` lines). No generated header edited.

| header | change | path | evidence | users | includers rechecked |
|---|---|---|---|---|---|
| engine/CV_LostTraderScript.h | adds `CCharString GetRandomBanditName();` (0x00EA5690) inside `#ifdef FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME` after `WatchForQuestFailure()` | opt-in (new, chunk 835) | Ego_r.pdb `?GetRandomBanditName@CV_LostTraderScript@NScript@@QAE?AVCCharString@@XZ`; the landed row 0x00EA5690 (hidden CCharString result, `this` unread); retail 0x00EA5230 calls it with `mov ecx,[esi+14h]` (ParentClass&) | src/00ea5230.cpp | the six landed includers (00ea5930 00ea51f0 00ea2e80 00ea59e0 00ea36b0 00ea3590) rechecked read-only with this overlay: see SIBLINGS.tsv |
| engine/C3DVector.h | the hand829 opt-in `FABLE_RETAIL_C3DVECTOR_INITIALISEFROMANGLES` block (InitialiseFromAngles body), carried unchanged; my diagnostic body variants (IFA_V1/V2/V3) were removed before hand-back | opt-in (chunk 829, not landed yet) | FableWin.pdb `?InitialiseFromAngles@C3DVector@@QAEABV1@MMM@Z`; the landed 0x00A0C450 / 0x00A14260 local bodies | differ/0068cb90.cpp only | not rechecked here (3,261 landed includers; the block is opt-in and the default path is byte-identical to the repo header -- `diff` shows only the added block). hand829 carries the same block. |
| engine/CV_ChapelOfEvilScript.h | the hand824 opt-in `FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL` nested class CCOE_AcolyteEvil (+ `#include "engine/CTimer.h"` inside the block), carried unchanged | opt-in (chunk 814/824, not landed yet) | Ego_r.pdb layout, entity vtable 0x012E8468, Alloc 0x00E6A500 | differ/00e6bc50.cpp only | the one landed includer 00e693f0 rechecked read-only: see SIBLINGS.tsv |

Overlay honoured check: a deliberate `ThingX` rename in a diagnostic copy of CIsThingAlive.h turned 0x00DE2610 SRC_FAIL (C2614), then reverted;
the diagnostic overlay dir (hdr_diag/) is deleted and never part of the hand-back.
