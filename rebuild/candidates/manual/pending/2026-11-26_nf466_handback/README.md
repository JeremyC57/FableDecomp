# NF466 hand-back (2026-11-26): speed-up C1 in bulk (access letter / const respells)

`C1_DISPOSITION.tsv` has a verdict for each of the 267 pool addresses. `NOTES_agent.md` is the agent's notes and rerun chain.

## Landed: c1a / c1b / c1c, 99 rows over 76 targets
- **What changed:** each row's own spelling, or its callers' spelling, moves to the unique PDB symbol that differs only by access letter or `const`.
- **Gates:**
  - Full caller census from retail references (rel32 and absolute), plus each caller object's whole symbol table: 0 BAD. It found 36 targets with callers the table does not list.
  - vland grade and test unchanged; WARN identical or smaller.
  - objproof ALL_EQ, with `.drectve` compared byte for byte.
  - Link: duplicates and unresolved never go up.
- **Sandbox:** apply_table --dry 0 misses, multi 1, descriptive_check 0 FAIL, clash 0.
- **After landing:**
  - SELF_SYMBOLS applied (37 lines);
  - bindings_multi 1; funclets 1,704, 0 broken; s units 159, 0 failed; descriptive_check 2,030, 0 FAIL.
  - **Tracker:** 47 agree_unverified → agree_pdb and 29 split_unknown → split_fixable, nothing worse. In those 29, every caller now binds the PDB name and only the row's own stand-in self is left.
- One test edit: `00892f70_test.cpp`'s `IsQuestFailed` stub became `const`.

## Not landed: bWF_watchfor2 (4 WatchFor binders)
- The binders 0x00D167C0 / 0x00D16BB0 / 0x00E9DAC0 are named in the unit column of T3 lines nf301 / nf326 / nf374.
- The lander's `--release-t3` releases only lines whose column-2 address is a batch row. So these stay held until the lines are closed by their own ports or the owner decides (r466d).
- The batch is kept in the agent folder.

## Rulings
- **r466a: declined.** 0x00F09640 would raise duplicates from 3 to 4 (r457e). Give the functor one shared model first.
- **r466b: open.** The 5 targets whose PDB symbol is in only one name set stay held. The coordinator will check them against the A2 / r455c precedent.
- **r466c:** informational. A callee-only unit may swap the old spelling for the PDB one in the unresolved set at the same address (r464c).
- **r466d: not applied.** See above.

## Resume
1. The 13 virtual targets (RTTI placement).
2. The 22 virtual / static declaration targets.
3. The 12 const compile failures and the 8 parser skips.
4. One `_hdr` batch for the 4 header targets plus 0x00BF6530.
5. Deputy2Killed / Acolyte.
