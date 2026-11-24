# Name-fix agent NF228 (2026-11-22, round 10): the nf227 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; no authoring agent is running.
Codex pass 27 rows and 0x00A4EFC0 (Codex one-row hand-off, owner decision pending) are off limits.

## Your brief is NF227's with overrides
Read `$S/w22/agentNF227/BRIEF.md` and the briefs it chains to (NF226 ... NF215 "Lane", `$S/w22/agentNF228/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF228`, names -> `NF228`. On a conflict: this file > NF227 > earlier. nf227 LANDED (`batches/2026-11-22_landing_nf227/README.md`;
hand-back `pending/2026-11-22_nf227_handback/`). Start from the repo tree as it is now.
Coordinator rulings (nf226 / nf227): vtable-slot relocations are binders; a folded alias lands with its split queued; size-pragma
scaffolding is accepted when the owner row sets the same pragma itself or the pragma restores the TU default.

## Entries (ENTRIES.tsv), in this order
1. **The callee self rows still on stand-in spellings** (NF227 NAME_FIXES lines in ENTRIES.tsv: 0x0042A879, 0x0042D246, 0x0042C02F,
   0x0042ACC3, 0x0042AC76, 0x0042B76C, 0x0042B7AD, 0x0042B7EE, 0x0042AB01, and 0x0041E1CD's `??ACLayerTree`) onto their STLport /
   PDB spellings, one spelling per address (0x0042D246: port 0x0041E1CD's local model so the held `??A?$map@JW4EEngine2DLayer@@...` line
   can be added).
2. **0x0042BE7B:** land the ready port (`pending/2026-11-22_nf227_handback/work/0042be7b_f1.cpp`) after a fresh recheck.
3. **0x004299A8 / 0x013B871C** (the two open nf226 lines; NF227 NOTES has the fix for each).
4. **Tests** for the nf227 rows that still carry HEAD tests (q12601 list), as far as context allows: self-contained, a case per
   condition, no local model of a class that has a header.
0x0042BEC0 (implicit ~CObservable as a self row) is NOT yours: it needs a ruling.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Never write into /home/user/FableDecomp; never run git. Report briefly when done or when context gets heavy.
