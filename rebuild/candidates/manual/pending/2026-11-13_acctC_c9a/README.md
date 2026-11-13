# acctC lane, hand-back c9a (2026-11-13): 1 row matched, 8,383 bytes -- staged, NOT landed

| address | name | length | grade | test |
|---|---|---:|---|---|
| 0x005F1B10 | CTCInventoryAbilities::ConstructPCMagicList | 8,383 | RELOCATION_MATCH | PASS |

Re-verified by the lane coordinator against `FullDecomp` at 55262db83 (read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`;
`coordverify.log`: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT, only RELOC_NAME label warnings). No flags. Real length. No oracle here.
Three agents (C5, C7, C9) on top of the acctB draft DIFFER(8380v8383).

## What closed it (NOTES.md / LESSONS.md)
- `if (count)` for the three category counters; loop-2's empty-ability locals inside the loop body; push_back NOINLINE opt-in;
  GetMiscThingsDef 0x005EE3B3 with its real body in the same file.
- Same-TU inline bodies of GetAbilityLevel 0x005EDE93 and GetAbilityGraphicIndex 0x005EDE7F (texts equal the retail out-of-line
  bodies, declared through the shared header; accepted by the coordinator) give retail's load order at 0x005F2E7C / 0x005F2F3B.
- A named `descriptionText` pointer before SetValue (0x005F3420); the loop-1 bound as `count` set in an if / else.
- Frame slots (the last residue, 167 instructions): `icon->AddActionOnDroppedNowhere(action);` written in BOTH arms of the isSpell
  if / else (the compiler merges the calls; the extra reference changes the frame packer's weight), `FABLE_RETAIL_STLP_MAP` so
  map::operator[]'s body is visible (the key temporary does not escape), counters declared `surroundCount, physicalCount,
  attackCount` in retail's store order.

## Header diff
`hdrdiff/CTCInventoryAbilities.h.diff`: new opt-in `FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC` (declares the row); 22 moved
asserts pinned. Whole-tree `equiv_check.py` to run when applied.

## Review items
- NAME_FIXES.tsv: 0x005F81BE is the TEMPLATE `GetDef<CExpressionDef>` (FableWin.pdb + reloc_bindings + landed callers); the repo
  opt-in `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF` spells it as a non-template overload.
- `open_005f59c7/`: sibling 0x005F59C7 ConstructPCExpressionsList, DIFFER(2649v2652), identical with registers and frame offsets
  masked; register choice differs (textBank in memory in retail). Agent C10 continues it.
