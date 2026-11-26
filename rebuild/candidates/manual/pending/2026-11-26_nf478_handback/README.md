# NF478 hand-back (2026-11-26): CInputProcess slot respells for bT (r467d / r467b)

`README_agent.md` is the agent's full report. `HELD_b1m.tsv` lists the 4 held rows. `evidence/bT_unresolved_after_nf478.tsv` lists what still blocks NF477's bT. `NAME_FIXES.tsv` lines that are still open are in NAME_FIXES_QUEUE as nf478.

## Landed: 3 header opt-ins + 34 rows (+35 proven)
- **b1, 15 rows:** the `QAE` ProcessInput rows on local class models, now the PDB `UAE` virtual at slot 4 on `engine/<Class>.h`.
  - New add-only opt-ins in `CInputProcessGameBase.h`: `_ISEVENTASSIGNABLESPECIALMOVEBUTTON` and `_GETMOUSEWHEELMOVEMENT`.
  - New add-only opt-in in `CInputProcessJumpingAndRolling.h`: `_PROCESSPARTS`.
- **b2, 6 rows:** Init / ProcessWhilePaused / ProcessInput / Update stand-ins moved onto the family opt-in.
- **b2d, 1 row:** 0x00687DB0 becomes `?IsInputEventToBeSentToThisInputProcess@CInputProcessGameBase@@MBE_NABVCInputEvent@@@Z` (both PDBs; retail vftable 0x0125BE98 slot 8).
  - The coordinator set its catalog Module to CInputProcessGameBase.
  - Its best-guess line in `descriptive_names.tsv` is removed, because the address is now proven.
- **b3, 10 rows:** the GetPInventory overrides, now `return PInventory;` through the existing CTC inventory headers.
- **b4, 1 row:** 0x0068D840 Update. Its callee moves to the PDB two-argument `IsGameActionEventInQueue`; one table replace.
- **b5, 1 row:** 0x00689130 moves off its NativeInput namespace model onto `CTCTargetingPlayer.h`.
  - New add-only opt-in: `_SETASTARGETNEXTTHINGINDIRECTION`.
  - Table: 2 deletes and 1 replace.
- **Gates (agent):**
  - vland: 34 of 34 PASS on both sides (13 MATCH, 21 RELOCATION_MATCH);
  - objproof: only the self and the intended callees change;
  - `.drectve` unchanged;
  - census: 109 retail slot dwords, plus 172 landed objects' symbol tables, show no other binder;
  - 0 pick-any reach flips.
- **Coordinator checks:**
  - lander: 34 of 34 landed, 0 held;
  - bindings_multi: 1;
  - funclets: 1,753 checked, 0 broken;
  - s units: 205 checked, 0 failed;
  - descriptive_check: 2,029 lines, 0 FAIL.
  - Equiv: see `EQUIV_CHECK.txt`.
- **Tracker (rerun):**
  - 33 rows move rtti_identified → agree_pdb;
  - 0x00687DB0 moves agree_descriptive → agree_pdb;
  - 0x00A03B40 moves split_fixable → agree_pdb;
  - 0x00687D50 and 0x008023C0 move split_unknown → split_fixable.
  - The same run shows 0x009E2590 agree_pdb → split_fixable. That comes from Account B's new row 0x006C9350, merged in the same window, not from NF478: it calls 0x009E2590 through a free stand-in. The respell is queued (NAME_FIXES_QUEUE nf478).
- **T3:** the nf319 / nf323 / nf324 lines for 0x0068CB70, 0x00690D20, 0x0068D830, 0x006A9170 and 0x006896A0 are closed by REL, and kept in the landings' `T3_CLOSED.tsv`.

## Held
- **b1m (lock `COORD_HOLD_NF478`):** 0x0068AE30, 0x0068B2E0, 0x0068B7C0, 0x00691130. Fully gated, but declined under R-NF478-1 (below).
- **bT** (NF477, 66 full vftables) is still held. It waits on:
  - b1m;
  - 0x0048D390's 4-byte `??_7CInputProcessJumpingAndRolling`;
  - 0x0048CEA0 / 0x0048CEC0 (R-NF478-2);
  - Combat's 30-byte `??_G`;
  - the ctor units 0x00687A30 / 0x006A9140, for 0x00687DF0 and 0x006A91D0.
  - Its link gate has not been rerun.
- **Stale:** `rebuild/include/engine/RECONCILE.tsv` line 704 describes 0x00689B00's model before b3. It is a gen_class_headers by-product; regenerate it with the next header generation.

## Rulings (coordinator, on precedent)
- **R-NF478-1: declined.** The four screen rows call a made-up inline helper `GetMousePosition`, but the PDB names the inline `GetMouseMovement`. A made-up name is allowed only where no original name exists (owner, 2026-11-10), so a file-local `static inline` under the made-up name does not qualify. Route: port the rows onto the PDB `GetMouseMovement` (T3 nf307).
- **R-NF478-2: granted as a best guess.** 0x0048CEC0 is respelt to `??1CInputProcessZTarget@@UAE@XZ`, registered in `descriptive_names.tsv` with basis `one-source:fablewin-pdb`. This follows the owner's best-guess rule (2026-11-25), and the landed name is provably wrong: `??1CHitParameters@@QAE@XZ` is already the name of 0x00789358.
  - The evidence: the FableWin.pdb name, the Ego_r layout and `??_G` teardown order, and a sole retail caller (ZTarget's `??_G` 0x0048CEA0).
  - 0x0048CEA0 is respelt with it.
  - Queued, not done: naming is paused (owner, 2026-11-26).

## Agent rule note
NF478 ran `git log --oneline -5` once (read-only). It made no repo writes and ran no other git command.
