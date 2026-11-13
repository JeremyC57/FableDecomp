# acctB hand-back, author workflow bwf2 (fourth harvest): 2 rows matched, 17,713 bytes

Same lane and layout as `2026-11-12_acctB_bwf2a` (all agents Opus; overlays based on `FullDecomp` at `bb0dc27e9`). Nothing is landed.

| address | name (as matched) | bytes |
|---|---|---|
| 0x00607330 | `CTCInventoryMap::ConstructLogbookList` | 9124 |
| 0x00D30030 | `NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle` (catalog label `CScriptGameResourceObject_LoadGuildComplex` is wrong) | 8589 |

Coordinator (acctB) re-verification: read-only `vland.py`, `LANDVERIFY_STRICT_HEADERS=1`, each row's overlay: both
RELOCATION_MATCH + PASS (`coordverify/`); purity scan clean; no RELOC_CONFLICT; no oracle bytes here. Workflow verifier lenses /
repair not run on these rows yet; a row a repair changes is re-staged. Names and name fixes: `notes/<addr>/NOTES.md`.
