# NF460 hand-back (2026-11-26): name-fix queue pass (nf452 / nf453 / nf455 / nf457 / nf458 lines)

`QUEUE_RESULTS.tsv` gives a verdict for each of the 66 queue lines. `NOTES.md` covers the method and resume point; `RULINGS_NEEDED.tsv` holds the agent's questions.

## Landed: 8 rows (+12 proven)
- **bA_tc_alloc_units, 4 rows:** the TC factories 0x004D4927, 0x004D3E43, 0x004D4987 and 0x004D2D98 move onto their PDB `?Alloc` selves on `engine/CTCBase.h`. Their ctors (0x00702AA0, 0x00770E50, 0x00787DBC, 0x0072C6E0) are now called as `(CThing&)`. 4 table replaces.
- **bC (4 of 5 rows):**
  - 0x008FDB55 / 0x008FDB7D now name the CAIStateGroup_Clean base ctor.
  - 0x00529CA0 calls `~CThingPhysicalSwitch` directly.
  - 0x00AF7B40 calls `NParticleEngine::CParticle`'s destructor and its class `operator delete`.
  - 4 table replaces.
- **Coordinator checks:**
  - bindings_multi: 1;
  - descriptive_check: 2,026 lines, 0 FAIL.
  - Tracker: 12 split_fixable → agree_pdb, with nothing worse.
- **Queue:** 8 NAME_FIXES_QUEUE lines are marked done. 0x0085BBA0 was already landed by NF459 b7.

## Rulings (coordinator, on standing precedent)
- **r460c: 0x006F5610 held.** Its shared-header warning would swap to a local model of CCreatureAction_StrafeForward, the r456b pattern. Its table delete was left out of the applied batch.
- **r460d: bB (11 rows) declined as built.** Six NParticleEngine factories use local models plus `reinterpret_cast` to an incomplete `CPSCBase` where class headers exist. Redo them on header opt-ins that declare `Alloc`, as the Orbit header does.
- **r460a: held.** The creature-mode `Alloc` selves need `CTCCreatureModeManager::CModeBase` without libcp, which means a containers-free view (r451a). The CModeStunned identity unit (0x00740160 / 0x008ED950 / 0x007427F0) rides with it.
- **r460b: owner item.** 0x0099AF00 `PB_W` vs `PBG` is the `wchar_t` question (70 call sites).
- **The rest:** on the T3 nf460 family line; the rows are under COORD_HOLD_NF460. Of the 66 queue lines, 38 were not reached (`QUEUE_RESULTS.tsv`, smallest census first).
