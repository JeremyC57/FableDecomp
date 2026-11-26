# NF477 hand-back (2026-11-26): CInputProcess family vftables (r467d)

`README_agent.md` is the agent's full report. `evidence/slot_audit.tsv` is the slot audit of all 66 family tables. `HELD_bT.tsv` lists the table unit that is held. `NAME_FIXES.tsv` lists the 42 respells bT waits on; they are now NAME_FIXES_QUEUE nf477.

## Landed: 69 headers + 37 rows (+37 proven)
- **b0_hdr, add-only:**
  - one family opt-in, `FABLE_RETAIL_INPUTPROCESS_VTABLE`, declaration only;
  - `engine/CAInputProcess.h` gets the retail base list (vftable 0x0129CA10, 9 slots);
  - the 66 class headers plus GameBase and ControlSpirit each get a block declaring the slots whose retail target differs from the base, with the PDB access letter;
  - 14 duplicates of existing opt-ins are guarded, and `#error` fires without the prerequisite views;
  - 173 moved asserts are pinned.
  - The coordinator checked all 69 `orig/` copies against the repo before installing them.
- **bD, 3 thin `??_G` rows of 0x00488D10** (the nf424 pattern): 0x0048B8D0 DebugControls, 0x0048B4E0 InventoryBase and 0x0048BC20 PhotojournalCapture.
  - Each is relabelled to `` `scalar deleting destructor' ``.
- **bR, 34 slot-target self respells** from free stand-ins to the PDB members. Bodies are unchanged.
  - The 5 CAInputProcess base rows;
  - 14 ProcessWhilePaused overrides;
  - 7 empty ProcessInput, 1 Update and 6 Init;
  - 0x00689C20's tail jump, which now names `?ProcessInput@CInputProcessInventoryBase@@UAE..`.
- **Gates (agent):**
  - vland: bR 34 of 34 PASS (33 MATCH, 1 RELOCATION_MATCH), bD 3 of 3 RELOCATION_MATCH PASS;
  - objproof: only the self symbol changes;
  - `.drectve` unchanged;
  - link: duplicates 0 → 0;
  - census: 37 rows, 0 BAD (every retail reference is a vftable slot).
- **Coordinator checks:**
  - lander: 37 of 37 landed, 0 held;
  - bindings_multi: 1;
  - funclets: 1,753 checked, 0 broken;
  - s units: 205 checked, 0 failed;
  - descriptive_check: 2,030 lines, 0 FAIL.
  - Tracker (rerun): exactly 37 rows move to agree_pdb (33 from rtti_identified, 4 from agree_unverified), with nothing worse.
  - Equiv: see `EQUIV_CHECK.txt`.
- **T3:** 31 nf324 "header does not declare X" lines are closed by `REL=nf324` and kept in the landing's `T3_CLOSED.tsv`.

## Held: bT, the table unit (61 rows: 0x00488D10 plus 60 thin rows; lock `COORD_HOLD_NF477`)
66 tables are built and slot-proofed (0 BAD): 4 bytes become 36, 40 or 44 bytes (`VFTABLES_FIXED.tsv`). bT lands after these:
- **r467b:** 37 slot rows are still landed under misspelt selves. These are body or local-model ports, queued as NAME_FIXES_QUEUE nf477.
- **0x0048D390** still emits a 4-byte `??_7CInputProcessJumpingAndRolling` from a local model.
- **Combat's `??_G` copy** is 30 bytes against retail's 40 (header member-destructor chain).
- **bT's link gate has not run yet.**

## Rulings (coordinator, on precedent)
- **R-NF477-1:** the trivial virtuals at 0x004863A0..0x00486C30 and 0x0048B3A0 land as out-of-line rows at their retail addresses. Each has its own retail body and vftable references, the bytes are identical either way, and a row per retail address is the lane rule.
- **R-NF477-2:** bR's respells land before bT. r467b says "with, or after"; this is the "after" case.
- **R-NF477-3:** open. The identity of 0x0048CEC0 (`~CInputProcessZTarget` against the landed `??1CHitParameters@@QAE@XZ`) is queued as an identity check (NAME_FIXES_QUEUE nf477). 0x0048CEA0 stays out of bD until it is settled.

## Also in this commit: Account B's 0x0085ED90 callee respell (owner: B's naming fixes are applied in the main chat)
B's new row declared `resize(unsigned int, const vector&)` inside `vector<T>`. That names the outer vector, so the object bound `..QAEXIABV12@@Z`, which no row defines; the landed 0x0085ED20 defines the `const T&` form. The tracker showed 0x0085ED20 falling from agree_std to split_unknown.
- **Fix:** `const T&` and `T value;` in the row and its test.
- The leaf is still the same 36 bytes. Its one relocation now names 0x0085ED20's self.
- The recheck is RELOCATION_MATCH PASS, with the test passing.
- The table line is respelt, and 0x0085ED20 is back to agree_std.
