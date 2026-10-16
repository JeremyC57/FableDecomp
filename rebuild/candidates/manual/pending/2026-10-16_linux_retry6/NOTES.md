# Retry batch 6 (Fable 5.1, 2026-10-16): 8 parked rows from chunks 165-179
- 0 of 8 matched; every row got 3 tries, notes on the differ grade lines.
- 005ea100 CTCTavernGame::vfunc_42 is closest: frame and both arms match once the def is taken before the branch; only the CDefPointer release shape (no RefCount re-read, def kept in edi) differs.
- 00544210 CSpellContainerList::InitialiseOffsets differs only by the dead `count` argument-slot reuse (store/fild per branch, alpha spilled through the same slot).
- 00729b20 CTCActionUseScriptedHook::OnActionUse: our two SetText arms tail-merge; retail keeps two complete calls.
- 00ca9d00, 006772d0, 007868a0, 007bdac7, 005bdfb1: parked again (multiple-inheritance base order, EH frame slot layout, scheduling).
