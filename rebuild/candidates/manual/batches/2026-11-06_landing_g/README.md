# Landing 2026-11-06g: chunk 806 (1 of 6 rows landed, 5 parked)

One expert agent authored the rows (sharing the lane with the chunk 805, then 807, agents). The six rows came from the gap scan
unnamed. The agent proved every name from vtable slot 1 (the landed Alloc stores the vtable) and both PDB name sets. The landed row
is plain C++ and grades `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00DB2B10 | NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife::Main | RELOCATION_MATCH PASS (4 attempts; temporary-dtor flag bits follow source order; stores written before the calls they precede) |

- **Headers** (`hdr_README.md`): the default paths are textually unchanged.
  - engine/CQ_NewOakValeIntroScript.h: FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES2 (CNOVI_AffairWife, CNOVI_TeddyGirl).
  - engine/CQ_BanditCampBossBattleScript.h: FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_FORGER (CBanditForger).
  - New: engine/CQ_BanditCampScript.h (nested CGate1GuardOuter) and engine/CV_OracleScript.h (nested CVO_OracleMarker), both behind
    `#ifdef FABLE_RETAIL_NSCRIPT` with no `#error` branch; no landed row includes them yet (the landed CGate1GuardInner::Main
    0x00D02B80 and the Oracle same-TU rows keep local models: retype debt).
  - The agent rechecked the landed includers 0x00DB09E0, 0x00DB6C60 and 0x00D038E0 with the overlay: all RELOCATION_MATCH PASS
    (SIBLINGS.tsv).
- **vland warnings:** 1 RELOC_ALIAS on the `rand` import thunk.
- **Names.** Labels: `ghidra_out/labels_landing_2026-11-06g.tsv` (the six Mains). 3 queue lines (tag `land0611g`), all on landed rows:
  - 0x00DB0660 AddGoodDeed: the landed GivenTeddy row binds `?AddGoodDeed@CQ_NewOakValeIntroScript@@QAEXXZ` (no NScript); the PDB
    and the new row 0x00DB2B10 use `?AddGoodDeed@CQ_NewOakValeIntroScript@NScript@@QAEXXZ`. Until that row is retyped, the target
    carries two symbols (the per-target binding check shows it).
  - 0x00DB0600 GivenTeddy: the landed row has a global class and the bare Leaf; PDB
    `?GivenTeddy@CNOVI_TeddyGirl@CQ_NewOakValeIntroScript@NScript@@AAEXXZ`.
  - 0x00DAEA70 AddBadDeed: already queued; the new rows use the PDB spelling.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case.
- **Rule note:** the agent ran one read-only `git status` (against the no-git rule); it wrote nothing (agent_NOTES.md).

## Parked (`pending/2026-11-06_linux_hand806/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DAF080 NScript::CQ_NewOakValeIntroScript::CNOVI_TeddyGirl::Main | DIFFER(5451v5504), 7 attempts | only the `Bully` CScriptThing dtor at the natural exit: retail expands it, ours calls 0x004AA840 |
| 0x00D01630 NScript::CQ_BanditCampScript::CGate1GuardOuter::Main | DIFFER(5134v5178), 4 attempts | the implicit movie dtor called out of line (0x006E7B80) where retail expands it (the chunk 797 / 800 / 805 gap) |
| 0x00D0C410 NScript::CQ_BanditCampBossBattleScript::CBanditForger::Main | DIFFER(5255v5373), 4 attempts | frame 0xa0 v 0xa4; retail re-tests `NotAttacked` at the top of loop 1 where ours rotates the loop; movie dtor called 6 times v expanded 5; one block out of line |
| 0x00EBCF70 NScript::CV_OracleScript::CVO_OracleMarker::Main | DIFFER(5492v5572), 7 attempts | retail loads the expression string pointer once across four Compare calls, ours reloads it; compare chain and category branches out of line; frame slots |
| 0x00EC68A0 NScript::CV_SickChildScript::CSickChildsSister::Main | DIFFER(5969v5962), 6 attempts | block placement of the run-to-mother loop test (+7 bytes) and one register swap |

GuardOuter and Forger stopped at 4 attempts (time), below the 10-attempt target: their next ideas are in agent_NOTES.md.
