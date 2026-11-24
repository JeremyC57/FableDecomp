# Name-fix agent NF229 (2026-11-22, round 10): the nf228 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Authoring agent AUTH18 is running
(locks AUTH18: 0x0093D270, 0x00B4D370, 0x00BD3070, 0x00502E90): do not touch those. Codex pass 27 rows are off limits, and so is 0x0042BEC0 (needs a ruling).

## Your brief is NF228's with overrides
Read `$S/w22/agentNF228/BRIEF.md` and the briefs it chains to (NF227 ... NF215 "Lane", `$S/w22/agentNF229/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF229`, names -> `NF229`. On a conflict: this file > NF228 > earlier. nf228 LANDED (`batches/2026-11-22_landing_nf228/README.md`;
hand-back `pending/2026-11-22_nf228_handback/`). Start from the repo tree as it is now.
Coordinator rulings (nf226-nf228): vtable-slot relocations are binders; a folded alias lands with its split queued; size-pragma scaffolding
is accepted when the owner row sets the same pragma itself or the pragma restores the TU default; test-only defines / `/alternatename` are OK
in tests only; an opt-in name resting on one PDB needs the retail body as the second source and a NOTES line saying so.

## Entries (ENTRIES.tsv), in this order
1. **The seven second-layer self rows on placeholders** (q12608-q12614: 0x0042B8FC, 0x0042A264, 0x0042A237, 0x0042AC9F, 0x0042ACEC,
   0x0042AD10, 0x0042AE33) and **0x0042AC0A** (q12615): onto their STLport spellings, one spelling per address, the nf228 pattern (explicit
   instantiation of the header member under the binder row's defines / pragma). Then the held table line for 0x0042B8FC (q12617) can be added.
2. **0x013B871C** (q12616): the six landed rows that still use the `?g_pNewFrontendGameComponent` stand-in in code (0x0042EBB6, 0x00558170,
   0x00599D5C, 0x0059A238, 0x0041D0A8, 0x00446720): port each onto CNewFrontendGameComponent.h's static (`Get()` / the PDB member). Compile-check
   the list first; it came from a source grep.
3. **Tests** (q12618) for 0x0042BE50 and the 12 CManager / CObservable vtable-slot rows, as far as context allows: self-contained, a case per
   condition, no local model of a class that has a header.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Never write into /home/user/FableDecomp; never run git; wait on jobs by PID (kill -0), never pgrep -f. Hand back with the NF228 file set.
