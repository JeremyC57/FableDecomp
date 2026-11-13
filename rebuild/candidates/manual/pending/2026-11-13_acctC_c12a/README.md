# acctC lane, hand-back c12a (2026-11-13): 1 row matched, 5,619 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x005EEFF2 | CTCInventoryAbilities::GetMagicListUIComponent | 5,619 | RELOCATION_MATCH | PASS |

Catalogued as `_gapscan::sub_005eeff2` (never worked); name proven from Ego_r.pdb + FableWin.pdb (LABELS.tsv). Re-verified by the
lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`; `coordverify.log`: 1 win,
no RELOC_IDENTITY / RELOC_CONFLICT). Real length, no oracle here. Two agents (C11 wrote it to identical code, C12 closed the frame).

## Flag: `// Flags: /Gr` -- review item
Retail's `vector<EHeroAbility>(size_type)` calls a `__fastcall` `uninitialized_fill_n` COMDAT at 0x005F8220, which only comes out
under `/Gr`; STLport does not declare it `__fastcall`. So the CTCInventoryAbilities TU looks compiled `/Gr`. Consistency check by
the coordinator: the sibling 0x005F1B10 (hand-back c9a, staged without a flag) also grades RELOCATION_MATCH PASS with `/Gr`, so one
TU-wide `/Gr` is consistent with both rows; the open sibling 0x005F59C7 grades the same either way.

## What closed it
- Fresh source from the dossier; same-TU helper bodies GetCategory 0x005EDE37, GetAbilityPDef 0x005EDFD9, GetMiscThingsDef
  0x005EE3B3 (each grades MATCH / RELOCATION_MATCH on its own).
- STLport bodies as shared-header opt-ins (coordinator decision; not in the row file): `hdrdiff/fable_containers.h.diff` adds
  `FABLE_RETAIL_STLP_LIST_BASE_VISIBLE_NOINLINE` (0x005F88ED / 0x005F8908) and `FABLE_RETAIL_STLP_VECTOR_SIZECTOR_VISIBLE_NOINLINE`
  (0x005F8EC0).
- Frame: `spellContainer` read from its named local at 0x005EF489 (not `container.Get()`), and separate fill / swap loop counters.

## Review items
- NAME_FIXES.tsv: 0x005EDE93 is `GetAbilityLevel(EHeroAbility) const` (both PDBs), not `GetAbilityLevel(long)`; 0x009AD390 is
  `GetGlobalIndexFromClassIndex` (both PDBs), not `GetDefGlobalIndex`.
- `open_005f59c7/`: 0x005F59C7 ConstructPCExpressionsList, DIFFER(2649v2652) after four agents (about 175 grades): identical code,
  register choice in the final block differs; forcing two allocator priorities reproduces retail. Untried next steps in NOTES_C12.md.
