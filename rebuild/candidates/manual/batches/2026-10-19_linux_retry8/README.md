# Retry batch 8 (2026-10-19): parked rows of chunks 190–198

129 parked DIFFER rows (best sources from `pending/2026-10-19_linux_hand190..198/differ/`) were re-tried by one Opus 5.5
agent with the GOTCHAS recipes; 17 now grade RELOCATION_MATCH with passing tests and were landed through
`verify_and_land.py --land` (identity check on; `verify.log`). The formulations that fixed them are in LESSONS.md; the
112 still parked are grouped by cause in NOTES.md (about 45 register colouring, 15 scheduling, 6 inlined counted-pointer
release `dec ; jne` reuse, 13 frame-slot sharing, 5 x87 operand order, the rest folding/narrowing, cross-call value
holding, /GS or inline-asm lane gaps). Nine improved-but-unmatched drafts replaced the parked copies. About 30 rows
(mostly chunk-190 register swaps and known lane gaps) were not re-tried this pass (marked in STATUS.tsv).
