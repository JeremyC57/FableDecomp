# hand3c b27 (2026-11-19, Account B): 0x00D8A240 NScript::CQ_HerosOldHouseScript::WatchForBooty, RELOCATION_MATCH + test PASS

- **Identity:** two sources, FableWin.pdb 0x037BA020 and Ego_r.pdb 0x00DAF540 (register_scripts.obj): `?WatchForBooty@CQ_HerosOldHouseScript@NScript@@QAEXXZ`. The landed Main 0x00D89E60 spawns it by name.
- **Source:** FableWin's statement form, with Ego_d's locals.
  - The locals are `_Fail_Val` (TRetVoid, declared first) and `HiddenBooty` (CScriptThing).
  - The audit replaced the author's `booty` / `TRetVoid().Return()` spelling with these. The author had queried Ego_d without the `NScript::` prefix and found no record.
- **Inline budget:** `// Flags: /d2inlT57`, under the owner rule of 2026-11-08 (preference 2: a per-row flag, source untouched).
  - The residue was purely which ~CScriptThing return sites are expanded and which are called.
  - Measured window, unchanged by the audit's fix: none DIFFER(662v610), T56 DIFFER(561v610), T57..T63 RELOCATION_MATCH, T64 DIFFER(655v610). A T0..T70 sweep matches only at 57..63, so 57 is the smallest.
  - Precedent: /d2inlT57 is already landed on 0x00D7FE90, and 174 landed rows carry d2inl flags.
  - The c2 trace numbers quoted in the source header (c1 = 60, site scores) were measured on the author's earlier spelling. The flag window is identical on the landed spelling, but the trace was not rerun.
- **Plain C++ first:** see the author's NOTES and the audit's three extra no-flag spellings on the Ego_d form, all DIFFER.
- **Header:** engine/CQ_HerosOldHouseScript.h gains a declaration-only opt-in, FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_WATCHFORBOOTY. Asserts are pinned at 74, 75 and 77. The landed includers 0x00D8BCD0 and 0x00D8ABF0 regrade RELOCATION_MATCH with the opt-in on and off.
- **Mutants:** 23/23 killed by the author's set. The audit's set killed 33 of 34; the one survivor, `return;` vs `return _Fail_Val.Return();` at the end, emits the same code.
- **Name fixes:** none.
