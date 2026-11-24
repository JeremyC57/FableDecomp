# Landing acctB_b25 (Account A, 2026-11-24): Account B's staged GFMain 0x00402510 and CAssassinMarker::Main 0x00D11930

Owner ruling (2026-11-24): B verifies and stages; A lands. Both rows were READY in `pending/2026-11-24_acctB_b24/README.md` and were
missed by the acctB_b24c landing. B ran out of usage after pushing them; nothing new is on FullDecomp-B beyond 4747fcd20a.

| Row | Name | Grade (before landing / landed tree) | Pick-any |
|---|---|---|---|
| 0x00402510 | GFMain (`/Gr /GS`) | RELOCATION_MATCH PASS / RELOCATION_MATCH PASS | 12 / 12 EQ |
| 0x00D11930 | NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main (`/d2inlS15`) | RELOCATION_MATCH PASS / RELOCATION_MATCH PASS | 11 EQ, 6 NEQ none leaf-called |

`logs/verify.log` is the pre-land grade on B's overlay; `logs/recheck.log` the recheck from the repo after landing; `logs/pickany.log`
pickany_vs_retail on the landed objects.

## Headers
- GFMain: B's overlay (12 existing headers with additive opt-ins, 7 new headers with Ego_r layouts; evidence per opt-in in
  `pending/2026-11-24_acctB_b24/hdr/00402510/README.md`). CDefinitionManager.h was 3-way merged against its base aaa2d37855 (no
  conflict); the other eleven were byte supersets of today's files and were copied.
- 0x00D11930: today's CQ_BanditCampBossBattleScript.h plus B's `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER` block and its
  size assert, inserted after the ASSASSIN1 block that b24e added at the same anchor (B's overlay predated it).
- pin_asserts: 0 moved. equiv_check (both trees scratch copies, 57,157 rows): only the two new rows differ. bindings_multi_check: the
  baseline (1 + NullCoord). funclet_includes_check: 1362 checked, 0 broken.

## Decisions (advisor decision, owner delegated)
- **0x00D11930's row-local `map<CCharString,STB>::operator[]` declaration is replaced by the header opt-in**
  `FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY` (EXTRA_RULES 2026-11-20 prefers the header form; B's review item). The row regraded
  RELOCATION_MATCH PASS with the same NEQ set.
- **Its six NEQ copies do not hold it.** None is leaf-called (the combined pick-any rule, COORDINATION_LOG line 1106, holds only rows whose
  leaf calls a NEQ copy). `_M_erase` 0x00CD344A is reach=y, the other five reach=n; all six are in the /d2inlS15 family audit.
- **GFMain keeps the landed data spellings** that Ego_d.pdb proves wrong (B's ESCALATE item 1); the PDB spellings are in
  NAME_FIXES_QUEUE (`acctB_b24 PRIORITY`, and three more added here as `acctB_b25`: 0x0137544A, 0x013B8648, 0x013BC864). The one
  exception, 0x013B864F `?GDisableCapsLock@@3_NA` against the landed `?g_KeyRedefinerFlag@@3EA`, is a type clash (bool& binding) and is
  queued for the respell. B's `notes/00402510/LABELS.tsv` proposals are not applied here.
