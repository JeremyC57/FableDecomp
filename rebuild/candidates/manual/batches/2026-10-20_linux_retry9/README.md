# Retry batch 9 (2026-10-20): parked rows of chunks 190–204, Fable 5.1 pass

184 parked rows (153 unique addresses; the 112 of chunks 190–198 had one Opus retry already, retry 8) were re-tried by a
Fable 5.1 agent briefed to derive formulations from how VC7.1 decides shapes rather than permute. 8 rows now grade
MATCH/RELOCATION_MATCH with passing tests and were landed through `verify_and_land.py --land` (identity check on,
`verify.log`); three improved drafts replaced the parked copies. NOTES.md records every hypothesis tried per row so the
next pass does not repeat them; LESSONS.md has the formulations that worked and the gap statements (inline-once
destructor cost band, x87 operand rank through inline operators, frame-slot reuse of address-taken locals).
