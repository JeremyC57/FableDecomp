# Name-fix agent NF231 (2026-11-22, round 10): the nf230 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Authoring agent AUTH20 is running
(locks AUTH20: 0x00B4D370, 0x00BD3070, 0x00502E90): do not touch those. Codex pass 27 rows are off limits, and so is 0x0042BEC0 (needs a ruling).

## Your brief is NF230's with overrides
Read `$S/w22/agentNF230/BRIEF.md` and the briefs it chains to (NF229 ... NF215 "Lane", `$S/w22/agentNF231/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF231`, names -> `NF231`. On a conflict: this file > NF230 > earlier. nf230 LANDED (`batches/2026-11-22_landing_nf230/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf230_handback/`). Start from the repo tree as it is now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f.

## Entries (ENTRIES.tsv), in this order
1. **0x00430340** (q12625): the self row spells `?SetQuit@CNewFrontendGameComponent@@QAEXXZ`; retail's is `UAEXXZ` (FableWin, Ego_d, RTTI
   vtable slot 4). Port it onto the header's `_SETQUIT` opt-in; census every binder, vtable slots included; one spelling.
2. **0x0042EBB6 Shutdown** (q12627): the full header port NF230 listed (NOTES): type FileNames / FileTimes as vector<CWideString> /
   vector<CDateAndTime> (Ego_r + Ego_d) as add-only opt-ins, move the five local models onto their headers, name the two header-less stand-ins
   from the PDBs. The in-place version landed; this one must grade the same and remove the local models.
3. **Tests** (q12628) for the 9 rows still on HEAD host-model tests (0x0041D98A, 0x00446720, 0x00599D5C, 0x00542140, 0x0059A238, 0x0041D0A8,
   0x0042EBB6, and 0x0054F410 / 0x0054F370 which still model `?g_pNewFrontendGameComponent`), as far as context allows.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF230 file set and the decisions you need.
