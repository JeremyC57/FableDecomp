# Name-fix agent NF234 (2026-11-22, round 10): the nf233 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Running: authoring agent AUTH23
(Fable 5.2 trial; locks AUTH23: 0x0066B370, 0x00A68DE0, 0x00839420, 0x00651273, 0x00816410, 0x00502E90) and tools agent C2T2 (0x00B4D370 /
0x00BD3070). Do not touch those rows or `$S/w22/auth2*/`, `$S/w22/c2t*/`. Codex pass 27 rows and 0x0042BEC0 are off limits.

## Your brief is NF233's with overrides
Read `$S/w22/agentNF233/BRIEF.md` and the briefs it chains to (NF232 ... NF215 "Lane", `$S/w22/agentNF234/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF234`, names -> `NF234`. On a conflict: this file > NF233 > earlier. nf233 LANDED (`batches/2026-11-22_landing_nf233/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf233_handback/`). Start from the repo tree as it is now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Every grade
under `timeout 900`. Disk is tight (~3.4 GB free): clean your own scratch as you go (guarded, literal paths).

## Entries (ENTRIES.tsv), in this order
1. **0x00495780** (q12650): grade the approved form (CCountedPointer.h CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY + DELETEDATA_DECL_ONLY
   opt-ins; NF233 drafts in `pending/2026-11-22_nf233_handback/work/e2/`). Every pick-any copy must equal its owner; 0x00412450 then 2 -> 1.
2. **The remaining CGameComponent / DeviceResetCallback spellings** (q12642-q12645): 0x00415930 SetQuit `MAE`, the vtables 0x0122ECA4 /
   0x0122DE50 (rows still emitting stand-ins: 0x00495620, 0x00A0A110, 0x00418B7A and the other local vtable models NF233 lists), and the
   CMainGameComponent sibling vtables (q12645). Census first; port each emitter onto the headers (NF233's CTOR_DTOR / DTOR_INLINE pattern).
3. **PClass pair unit** (q12647 / q12651): 0x013B8760, 0x01228AF1 (atexit dtor), 0x0121A2DC (initializer) together.
4. **CEngineInterpolationInfoSet nesting** (q12646 / q12649): a header claim, add-only (a nested opt-in), proven against the two PDBs.
5. **Tests** (q12652) as far as context allows.
0x004330D2's `??_G` self leaf (q12641 / q12648) needs a device for the row itself: write up what would work, do not land a borrowed name.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF233 file set and the decisions you need.
