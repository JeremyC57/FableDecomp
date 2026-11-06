# Landing 2026-11-05y: chunk 798 (3 of 6 rows landed, 3 parked)

One expert agent authored the rows (sharing the lane with the chunk 796, then 799, agents). All three landed rows are plain C++ and
grade `genuine`; the agent paired every relocation with its retail target and checked the string literals.

| Address | Name | Grade |
|---|---|---|
| 0x00D35090 | NScript::CQ_FireHeartScript::Main | RELOCATION_MATCH PASS (5 grades; a declared-only explicit specialization keeps CCountedPointer<CScriptThing>::operator= out of line) |
| 0x00D39330 | NScript::CQ_FireHeartScript::PlayCutscene | RELOCATION_MATCH PASS (first grade, on the WaspBoss Outro shared-header model) |
| 0x00ED5830 | NScript::CV_SwordInTheStoneScript::SwordPullCutscene | RELOCATION_MATCH PASS (2 grades) |

- **Headers** (`hdr_README.md`):
  - four new per-quest headers: engine/CQ_FireHeartScript.h (0x1A4), CV_SwordInTheStoneScript.h (0x80),
    CQ_HeroSoulsThunderScript.h (0x58, nested CHeroThunder), CQS_GuardianTrophyDealerInfoScript.h (0x4C, nested CGTDI_Maze);
  - two opt-in branches used only by the parked PhaseTwo: FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO and
    FABLE_RETAIL_STLP_VECTOR_SWAP_OUTOFLINE (stlport/fable_containers.h; the overlay already carried chunk 796's STLP_SORT_FASTCALL).
  - The overlay was additive against the current tree (no merge needed). hdrcheck2 reads `default=same`, and only the two new macros
    read DIFF. The 4 landed AmbushTraders includers still grade RELOCATION_MATCH + PASS (`pending/2026-11-05_linux_hand798/SIBLINGS.tsv`).
- **vland warnings:** 2 RELOC_NAME, both wrong landed labels the agent proved (0x00704580 `_global::CCountedPointer_Set_00704580`,
  0x00CDBFB0 `CRetailTree_erase...`): queued.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05y.tsv` (the six rows, plus unlanded FireHeart callees and the binding vtable
    0x012CCE90). It overrides the older `CScriptBase::PlayCutscene` label of 0x00D39330 (`labels_chunk201_202_corrections_2026-10-20.tsv`).
  - 7 queue lines (tag `land0511y`): vector<TeleportData>::swap 0x00F0CA30 (landed as SThreeWordRecord::Swap), TeleportData's ctor
    0x00F0D470, the map dtor 0x00CDBFB0, IsDistanceBetweenThingsUnder 0x00CBE2FF, CheckGameEnding's prefix.
  - **Open:** 0x00704580 now has three bound spellings (`CCountedPointer<CScriptThing>`, `<CScriptThingImp>` and the untemplated
    stand-in). The new row binds the CScriptThing spelling, as queue lines 582 and `land0511p` already propose; the body may be an
    ICF fold of several T, so a cvdump check of the PDB public decides (queued CHECK).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord X / Y / Z case.

## Parked (`pending/2026-11-05_linux_hand798/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00F0C140 NScript::CQ_AmbushTradersScript::PhaseTwo | DIFFER(2208v2285), 13 attempts | where the IsActiveThreadTerminating exits and their inline vector / CScriptThing dtors fall; the post-loop `~vector`; `TeleportInfo.size()` called where retail inlines it |
| 0x00D7B510 NScript::CQ_HeroSoulsThunderScript::CHeroThunder::Main | DIFFER(3533v3524), 12 attempts | 21 lines: only the closing `while (!T) NewScriptFrame();`, which repeats its test at the loop head where retail jumps to one bottom test (as parked 0x00D82640) |
| 0x00E27E90 NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Main | DIFFER(3595v3595), 10 attempts | per-site inlining: the CScriptThing copy is called inside the inlined CIsThingAlive ctor but expanded for SetIsPushableByHero; the ScriptedThingBase ctor is expanded in the hit branch but called (0x007E72A0) in the talk branch |
