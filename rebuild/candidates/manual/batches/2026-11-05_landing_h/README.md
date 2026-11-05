# Landing 2026-11-05h: chunk 781 (5 of 6 rows: the CV_DemonDoorsScript TU and the trophies intro watcher)

Authored by one expert agent. All five rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E6FC40 | NScript::CV_DemonDoorsScript::CDF_Dragon::Main | RELOCATION_MATCH PASS, 1664 bytes including its 8-entry jump table |
| 0x00E702C0 | NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::Alloc | RELOCATION_MATCH PASS |
| 0x00E70360 | NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::GetParentScript | MATCH PASS |
| 0x00E74D70 | NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::Main | RELOCATION_MATCH PASS |
| 0x00E9CFD0 | NScript::CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main | RELOCATION_MATCH PASS |

- **The CDF_Dragon split.** The 1828-byte ledger row was Main plus its jump table, then Alloc and GetParentScript of
  CDF_SummonsCreatures. Main is graded with its table. `verify_land_jumptable.py` (read-only, `logs/jumptable_00e6fc40.log`)
  agrees with vland, and the agent checked every case-to-target pair against the retail table at 0x00E702A0 by hand.
- **Key spellings:** CDF_Dragon::Main needs the movie PImp destructor 0x006E7AB0 declared out of line, while SummonsCreatures::Main
  in the same retail file must not have that declaration; SummonsCreatures::Main uses the existing
  FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR opt-in; the watcher needs a `__forceinline` CTimer::operator=.
- **Header:** engine/CQ_HangingTreeEvilScript.h completes the nested CHTE_FreedomFighter from the Ego_r layout (size 0x20). It is a
  default-path change, but no landed source includes the header; the parked chunk 744 row 0x00D69480 builds the same bytes with
  and without it. Only the parked row below needs it.
- **Landing:** `vland --land` (`logs/land.log`); labels `ghidra_out/labels_landing_2026-11-05h.tsv`; `bindings_multi_check.py` 0.

## Parked (`pending/2026-11-05_linux_hand781/`)
- **0x00D6A560 CQ_HangingTreeEvilScript::CHTE_FreedomFighter::Main:** DIFFER(2058v2058) after 20 spellings. One `lea ecx,[esp+2Ch]`
  sits above the terminate check's `jne` in retail's follow-the-guard wait loop; every other byte and relocation is equal.

## Names
3 queue lines (tag `land0511h`): the two ledger scopes, and 0x0143E8E0 = `?NullCoord@CScriptThing@@1VC3DVector@@A` (Ego_r dynamic
initializer), which ten landed files spell g_ZeroVector / g_ZeroPos. Open: 0x0122DEDC (0.0f) has 30+ spellings and no PDB name; it
needs a naming decision like the 2026-11-01 EPSILON / TWO_PI ones.
