# Name-fix agent NF226 (2026-11-22, round 10): the nf225 follow-ups (access specifiers, tests on the shared headers)

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; an authoring agent (AUTH12) is on
0x00CF4EC0 (locks read AUTH12). Codex pass 27 rows and 0x00A4EFC0 (one-row Codex hand-off) are off limits.

## Your brief is NF225's with overrides
Read `$S/w22/agentNF225/BRIEF.md` and the briefs it chains to (NF224 ... NF215 "Lane", `$S/w22/agentNF226/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF226`, names -> `NF226`. On a conflict: this file > NF225 > earlier. nf225 is applied to the working tree now (landing:
`batches/2026-11-22_landing_nf225/README.md`; hand-back `pending/2026-11-22_nf225_handback/`). Start from the working tree as it is.

## Entries (ENTRIES.tsv), in order
1. **The access-specifier fixes (three NAME_FIXES lines):**
   - 0x0041E641 `??1CManager@NUISystem@@IAE@XZ` (protected, non-virtual; all three PDBs): the self row emits a public virtual destructor
     from a row-local model. Port it onto engine/NUISystem_CManager.h.
   - 0x0041E3F6 + 0x0041E5F2: the ctor is `??0CManager@NUISystem@@IAE@XZ` (protected) in the PDBs; both rows spell QAE. Move them together.
   - 0x00494840: its Leaf line says `AAE` (private, Ego_r) but the row emits `QAE`; fix the model so the row emits the PDB leaf.
   Self signatures change only with two sources. Census every binder; one spelling per address at the end.
2. **Tests onto the shared headers** for 0x00494840, 0x0054ED90, 0x0041E5F2, 0x0042FB40, 0x0042DB20 (and 0x0041E641 / 0x0041E3F6 if entry
   1 changes them): each test self-contained (C++03, /Od /W3, no /EHsc, include/ only), a case per condition in the row, printing
   `<addr>_TEST PASS`; no local model of a class that has a header (stub the out-of-line members the test links against instead).
3. **The `~CCountedPointer<CFontBank>` copies** (no retail owner; 53 B in 0x00494840 / 0x0054ED90, 5-B jmp in 0x0042DDB3): report only,
   unless Ego_d / Ego_r evidence settles the inline shape; do not change rows for it otherwise.
For every entry: recheck every changed row, pk.py every new copy, census spellings, link-check, TABLE_CHANGES.tsv. Never write into
/home/user/FableDecomp; never run git. Report briefly when done or when context gets heavy.
