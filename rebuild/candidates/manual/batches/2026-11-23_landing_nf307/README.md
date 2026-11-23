# Landing nf307 (Account A, 2026-11-23): T1 signature, NF304's continuation (23 rows, 15 table lines)

Hand-back: `pending/2026-11-23_nf307_handback/`; helpers under `naming_batch/agent_tools/nf307/` (new: mkoff.py also lists the neighbours'
witnesses, cmpcomdat.py compares pick-any bodies; assemble.py takes work/tc_skip.tsv for rewitness lines). Entries 1-40 of 174 reached:
15 done, 3 done-partial, 18 skipped-port, 4 skipped-held, no rejects; no header or test change.

- Landed (23 of the 24 hand-back rows): pointer -> reference (entries 9, 11, 13, 14, 15, 23, 37), `unsigned long` for `long` (16), a
  shared enum header + enumerators (19 EHeroExperienceStatCategory, 28 EHeroAbility; NF300 Q1, `evidence/ppdiff_enum.txt`), the local `long`
  member retyped to EOpinionDeedType (5 / 6; NF304 Q2) with 0x00681AE0's self fix (full Leaf; moves five binders HEAD left unresolved onto it),
  CCreatureNavOutput nested into CTCCreatureNavigation (7; NF300 Q2), NF304's held entry 4 / 8 variants (0x00449E60: the CPlayerSave
  ??0 / ??1 copies are byte- and relocation-equal to 0x00449F90's, NF304 Q1 proven per copy), and entry 36's LookAtPos unit (five untabled
  PBV binders 0x00714900 / 0x00714CC0 / 0x008D8EA0 / 0x008DA610 / 0x008DB240 with the witness 0x008DB460).
- HELD at landing: 0x006B8640 (NF304 entries 1-3) and its 3 table replaces (`logs/table_held_006b8640.tsv`, the row under
  `pending/.../held/`): its re-scope also renames three local inline getters of the stand-in CScreenEffectPriorityStack to names that are not
  PDB names, which NF304 Q1 does not cover; the full fix re-scopes CScreenEffectPriorityStack into NScreenEffect with the FrameUpdate binding
  at 0x00880E50, queued T3 as one unit.
- Proof: RELOCATION_MATCH (0x00681AE0 MATCH) at HEAD and new; objsec ALL_EQ (`.drectve` included, no table emitted); per-row symbol diff
  shows only the respelled symbols; links HEAD v new: no new duplicate, nothing HEAD resolved left unresolved. Staleness: all 24 orig/
  copies equal HEAD at landing (after nf305 / nf306).
- Table: 7 replace, 5 delete, 3 rewitness (KnowsOfDeed `QAE_NJJ_N` onto 0x007BB150, GetNextNavPosOnRoute `AU` and LookAtPos `PBV` onto
  0x00713EE0; both witnesses compiled at landing).
- Rulings: D1 held (above). D2 0x00713EE0 takes the entry 7 / 36 edits in the continuation. D3 entries 38 / 39 may land as one unit: the
  binder 0x00766480 passes the enum value (`(EObjectAugmentationType)0xC` or the OR of the enumerators) and the selves 0x00766050 / 0x007660C0
  are retyped to the PDB enum with every binder (NF304 Q6). D4 0x007BB150's typedef is queued (NAME_FIXES). D5 0x004D5896 binding the PDB
  ctor that 0x007CD810's TU emitter also defines lands; 0x007CD810 is queued for the nf86 / nf88 pick-any scaffolding.
- Queued: 20 T3 lines (18 ports, the 0x006B8640 unit, the 0x007CD810 scaffolding); NAME_FIXES_QUEUE: 0x007BB150, 0x008168B0.

## Recheck
`logs/recheck.log` (landed tree, 23 rows + witnesses 0x007BB150 / 0x00713EE0): 25 PASS (RELOCATION_MATCH / MATCH), 0 DIFFER, 0 RELOC_CONFLICT; both witnesses bind their rewitnessed spellings (`logs/witnesses_head.log`).
