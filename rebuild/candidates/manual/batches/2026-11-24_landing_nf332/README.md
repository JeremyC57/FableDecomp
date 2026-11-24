# Landing nf332 (Account A, 2026-11-24): T1 / T2 tracker batch, 66 rows onto PDB spellings (40 table lines)

Hand-back: `pending/2026-11-24_nf332_handback/` (NOTES.md first); tools: `naming_batch/agent_tools/nf332/`.
- **Scope:** 239 free entries, screened against T3, locks and Codex reservations. NF332 reached 66.
- **Results:**
  - 55 done: all 47 access entries, the 16 CThing& constructor signature entries, and standin_alias entries 99 / 111;
  - 2 done-partial;
  - 7 skipped-held;
  - 2 skipped-port;
  - 0 rejected.
  - The other 173 entries are untouched and open for the next batch.

## What changed
- **Rows:** 66 rows, of which 37 are self renames (RENAMES section 1).
  - Access / virtual / const letters, and callers respelled onto PDB names.
  - Three tests change with no line-count change: 00b0a750 calls its now protected const member through a test-only accessor;
    008915e0 / 008915b0 respell their stubs.
- **0x008915B0** (CGameScriptInterface::SetBoastAsFailed) called 0x004AF880 under the `SetBoastAsCompleted` spelling, which binds
  0x004AF8F0, a different function. It now binds the PDB `SetBoastAsFailed`. Fixing entry 99 alone would have given that spelling two
  targets.
- **Table:** `TABLE_CHANGES.tsv`, applied with `apply_table.py`: 34 replace, 2 add, 1 rewitness, 3 delete, 0 misses. No new multi-target
  symbol; `bindings_multi_check` shows only the pre-existing `?NullCoord` line.
- No header, flag or catalog change; the catalog names already equal the renamed selves.

## Proof
- **Agent, per row:**
  - HEAD / new RELOCATION_MATCH at one work path;
  - objsec ALL_EQ, `.drectve` included, with DEF / UND sets the same;
  - link check: the PDB spellings resolve, with no new duplicate.
- **Agent, hand-back cross-check:** every old table line exists, and every new symbol is an exact string in both PDB name sets.
- **Coordinator:**
  - all 69 `orig/` copies (tests included) equal the repo;
  - recheck on the landed tree: 66 / 66 RELOCATION_MATCH PASS (`logs/recheck.log`);
  - all 37 self renames grade under their new PDB symbol.

## Rulings (coordinator)
- **D3 (accepted):** the four CTCTavernGame selves 007d7b40 / 007cf460 / 007d4a90 / 007daaa0 move from the placeholder `vfunc_30` to
  `?LeaveGame@<class>@@UAEXXZ`.
  - Evidence: both PDB name sets, plus retail RTTI slot 30.
  - No table line, source, header or vftable used the old spelling.
  - This is the self-rename-with-every-binder form the unit route and NF329 land (NF304 Q6). NF282 D2 is about re-scoping a self onto a
    made-up owner.
- **D1 (136) / 160:** done-partial. The remaining binder needs an add-only const opt-in on `engine/CTCHaste.h` (136), or a
  CTCPhysicsBase port (160).
- **D4 / D5:** the cited binders already bind the PDB spelling through their shared headers. Nothing to do.
- **D6:** the 16 global `CTCX_Alloc` factories are queued in NAME_FIXES_QUEUE as `nf332`. They are ports to
  `?Alloc@CTCX@@SIPAVCTCBase@@AAVCThing@@@Z`.
- **Slip, recorded:** one scratch file was written outside the agent folder and removed at once. No repo write and no git.
