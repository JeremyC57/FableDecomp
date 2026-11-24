# Name-fix agent NF230 (2026-11-22, round 10): the nf229 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Authoring agent AUTH19 is running
(locks AUTH19: 0x00B4D370, 0x00BD3070, 0x00502E90): do not touch those. Codex pass 27 rows are off limits, and so is 0x0042BEC0 (needs a ruling).

## Your brief is NF229's with overrides
Read `$S/w22/agentNF229/BRIEF.md` and the briefs it chains to (NF228 ... NF215 "Lane", `$S/w22/agentNF230/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF230`, names -> `NF230`. On a conflict: this file > NF229 > earlier. nf229 LANDED (`batches/2026-11-22_landing_nf229/README.md`;
hand-back `pending/2026-11-22_nf229_handback/`, whose STATUS.tsv lists what 0x0059A238 needs). Start from the repo tree as it is now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f.

## Entries (ENTRIES.tsv), in this order
1. **0x0042A8E0** (q12620): respell as the pair's `??_G` (scalar deleting dtor) so 0x00429E16's `~pair` binding moves off `@CAssoc_dtor@4`
   (0x00429E16 2 -> 1 spelling). Census every binder of both addresses first.
2. **0x013B871C** (q12621-q12623): the remaining stand-ins, one spelling per address:
   - 0x0059A238 (`?g_` user; nf229 STATUS has what it needs);
   - `?System_013b871c@@3PAUSystem@@A` on 0x00541F20 and 0x00542140, and the public-static `...@@2PAV1@A` on 0x00597BF2;
   - 0x0041D0A8 PlaySoundW (`PSampleBank`) and 0x0042EBB6 Shutdown (FileNames / FileTimes typed, CDefPointer non-const): these need
     header claims in CNewFrontendGameComponent.h. Make them as add-only opt-ins under new macros if the evidence (two sources: Ego_r /
     FableWin / Ego_d types + retail reads) supports them; otherwise write up exactly what is missing.
   When no landed row emits `?g_pNewFrontendGameComponent` any more, the table line goes.
3. **Tests** (q12624) for the 12 CManager / CObservable vtable-slot rows and 0x00446720 / 0x00599D5C, as far as context allows.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF229 file set and the decisions you need.
