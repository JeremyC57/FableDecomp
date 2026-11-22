# Name-fix agent NF227 (2026-11-22, round 10): the nf226 follow-ups (CManager vtable-slot self rows, CObservable, the map / tree placeholders)

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent; authoring agents AUTH15 (0x008E18D0)
and AUTH16 (0x00CF4EC0) are running (locks AUTH15 / AUTH16). Codex pass 27 rows and 0x00A4EFC0 are off limits.

## Your brief is NF226's with overrides
Read `$S/w22/agentNF226/BRIEF.md` and the briefs it chains to (NF225 ... NF215 "Lane", `$S/w22/agentNF227/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF227`, names -> `NF227`. On a conflict: this file > NF226 > earlier. nf226 is applied to the working tree (landing:
`batches/2026-11-22_landing_nf226/README.md`; hand-back `pending/2026-11-22_nf226_handback/`). Start from the working tree as it is.

## Coordinator rulings you work under (nf226)
- Vtable-slot relocations ARE binders. A self row whose leaf differs from the spelling its vtable slot binds is a split to close.
- A folded alias (a copy the table maps to the address retail folded it into) lands; the split is queued.
- Size-pragma scaffolding that restores the TU default before the pick-any copies is accepted.

## Entries (ENTRIES.tsv), in order
1. **The 12 CManager vtable-slot self rows** (0x0055CA40, 0x0055C9C0, 0x0055CAE0, 0x0055CA90, 0x0055CA00, 0x0041DED2, 0x0042D592, 0x0041D98A,
   0x0041E3B2, 0x0055C950, 0x0042D4F0, 0x0042D5AA): census, then port each onto engine/NUISystem_CManager.h so its leaf is its slot's PDB
   spelling (both PDBs; virtualness and access from the PDB). Placeholder / descriptive names take the PDB name. One spelling per address.
2. **0x0042AB01** (`_List_base<CActionParam*>` ctor): respell the remaining `??0CActionParamList` binder onto the STLport spelling.
3. **NF226's NAME_FIXES lines** (CObservable 0x0042BEC0 / 0x0042BE7B, ClearObservers 0x0055C950 if not covered by 1, the tree / map
   placeholders, 0x00495610's InitGraphics binder, the 0x004299A8 / 0x013B871C splits), in the order listed, as far as context allows.
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Never write into /home/user/FableDecomp; never run git. Report briefly when done or when context gets heavy.
