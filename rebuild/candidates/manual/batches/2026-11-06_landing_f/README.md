# Landing 2026-11-06f: chunk 805 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows (sharing the lane with the chunk 804, then 806, agents). The six rows came from the gap scan
unnamed. The agent proved every name from vtable slot 1 (the landed Alloc stores the vtable) and both PDB name sets. The landed rows
are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D41D00 | NScript::CQ_GuildTrainingScript::CSkillTarget::Main | RELOCATION_MATCH PASS (14 attempts; a named per-site `C3DVector step(...)` built member-wise gives retail's 0x19C frame; `<` compares on the hit-score ladder; 28 strings in retail order) |
| 0x00EA36B0 | NScript::CV_LostTraderScript::CLostTraderTrader::Main | RELOCATION_MATCH PASS (2 attempts; a visible CTimer ctor body keeps the timer index in ebp; 25 strings in retail order) |

- **Headers** (`hdr_README.md`): four repo headers gain one opt-in branch each; the default paths are textually unchanged (every
  added line, forward declarations included, sits inside the new `#if`/`#ifdef`). hdrcheck2 reads `cpp-err` for these script
  headers standalone (they need the NScript context), so the coordinator read each diff by hand; the agent rechecked the 13 landed
  includers with the overlay (SIBLINGS.tsv, all MATCH / RELOCATION_MATCH and PASS).
  - engine/CQ_GuildTrainingScript.h: FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_ENTITIES (CAppleGirl, CSkillTarget; needs `_MAIN`).
  - engine/CV_LostTraderScript.h: FABLE_RETAIL_CV_LOSTTRADERSCRIPT_TRADER (the trader's three private helpers).
  - engine/CQ_EndGameFocalSitesScript.h: FABLE_RETAIL_CQ_ENDGAMEFOCALSITESSCRIPT_JACKOFBLADES (CEndGameJackOfBlades).
  - engine/CQ_HangingTreeGoodScript.h: FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT_GUARD (the full CHTG_Guard).
- **vland warnings:** 1 RELOC_NAME on 0x00704580 (placeholder label, relabelled below); 2 RELOC_ALIAS on the `rand` import thunk.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-06f.tsv` (the six Mains plus two corrections: 0x00EA3490
    `NScript::CV_LostTraderScript::WatchForQuestFailure`, not the propagated `CV_BeardyBaldyScript::WatchForQuestFinished`; 0x00704580
    `CCountedPointer<CScriptThing>::operator=`, not `_global::CCountedPointer_Set_00704580`). apply_labels renamed both in the catalog
    and the manifest.
  - 0x00D2D930 is `CEndGameJackOfBlades::Main`, not `CPermanentBarrier::Main` as the chunk table said (CPermanentBarrier::Main is the
    landed 0x00D2D830). chunk_history.tsv and the pending rows.tsv carry the corrected name.
  - 3 queue lines (tag `land0611f`); the one to act on: 0x00704580 is bound under two other spellings by the landed 0x00CF8970
    (`CCountedPointer<CScriptThingImp>`) and 0x00CFD6C0 (non-template); FableWin.pdb has only the CScriptThing one.
- **Landing:** `vland --land` (`logs/land.log`); labels applied (`logs/apply_labels.log`); the manifest sync dry run prints 0;
  bindings_multi_check prints only the known NullCoord case; the new bindings add no symbol at a second address and no second symbol
  at a target.

## Parked (`pending/2026-11-06_linux_hand805/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D3D150 NScript::CQ_GuildTrainingScript::CAppleGirl::Main | DIFFER(4252v4252), 11 attempts | register choice only: the MANY / NO_MORE apples arms take the vtable temporary in eax / ecx where ours uses edx / eax |
| 0x00EC9D30 NScript::CV_SickChildScript::CMansLover::Main | DIFFER(4708v4708), 12 attempts | block layout only: retail falls through into the answer==1 give-letter block; ours lays the else arm first |
| 0x00D706E0 NScript::CQ_HangingTreeGoodScript::CHTG_Guard::Main | DIFFER(4755v4755), 10 attempts | retail expands `~CIsThingAlive` around an out-of-line `~CScriptThing` 0x004AA840; ours calls `~CIsThingAlive` (the documented CIsThingAlive inline case) |
| 0x00D2D930 NScript::CQ_EndGameFocalSitesScript::CEndGameJackOfBlades::Main | DIFFER(5005v5094), 11 attempts | the implicit MovieBase dtor at the four cutscene exits: ours calls the COMDAT 0x006E7B80, retail expands it (the chunk 797 / 800 / 804 long-Main gap); no `/d2inlS` value (diagnostic only) reproduces it |

**Review item:** reloc_bindings.tsv binds the compiler's 0.0f literal 0x0122DEDC (`__real@00000000`) to nine global names in landed
rows (already an open item in docs/HANDOFF.md).
