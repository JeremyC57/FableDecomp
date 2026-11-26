# NF476 hand-back (2026-11-26): MinionReactToAlert unit, T3 nf374 0x008FCEC0 closed

`README_agent.md` is the agent's full report. `HELD_b1_hold.tsv` lists the two rows held back. `evidence/` holds the header preprocess check and the pick-any measurements for 0x00902045.

## Landed: 3 headers + 7 rows (+6 proven)
- **b0_hdr, add-only:** CAIStateGroup_MinionFaceHero.h, CAIStateGroup_BanditKingLunge.h and CAIStateGroup_BanditSitRoundCampfire.h.
  - Each gets a nested `FABLE_RETAIL_CAISTATEGROUP_<X>_INITIALISE` slot-1 declaration in the PDB spelling.
  - Moved size asserts are pinned to their original lines.
  - The coordinator checked every `orig/` copy against the repo before installing.
- **b1, 7 rows in one unit:** 0x008FD50D (ctor), 0x008FF19C (thin `??_G`), 0x0090F0D0, 0x00902045, 0x0090F690, 0x009612E0 and 0x0093A500.
  - Each binder now binds the landed self `?Initialise@CAIStateGroupBase@@UAE..JW4EScriptAIPriority@@..@Z`.
  - The table loses the `CAIStateGroupBaseI` and `@CAIStateGroupBase_Initialise@28` stand-in lines.
  - The two long-priority stand-ins are rewitnessed: `QAE..JJ` to 0x00902233 and `UAE..JJ` to 0x00957940.
  - 0x008FF19C is relabelled to `` `scalar deleting destructor' ``. SELF_SYMBOLS gets 6 updates and 1 new line (0x0090F690).
- **`??_7CAIStateGroup_MinionReactToAlert@@6B@` is now 56 bytes** (retail's 14 slots). The coordinator compiled all 8 landed rows that model the class. Only 0x008FD50D and 0x008FF19C emit the table, both at 56 bytes.
- **Gates (agent):**
  - 7 of 7 RELOCATION_MATCH PASS on both sides, tests included;
  - objproof ALL_EQ;
  - `.drectve` identical;
  - link: duplicates 0 → 0, unresolved 9 → 5;
  - census: 0 BAD over all 38 landed code callers of 0x008FCEC0.
- **Coordinator checks:**
  - bindings_multi: 1 (NullCoord);
  - funclets: 1,750 checked, 0 broken;
  - s units: 205 checked, 0 failed;
  - descriptive_check: 2,030 lines, 0 FAIL.
  - Tracker (rerun, not the agent's estimate): exactly 6 rows move rtti_identified → agree_pdb (0x008FF19C, 0x0090F0D0, 0x00902045, 0x0090F690, 0x009612E0, 0x0093A500), with nothing worse.
  - Equiv: see `EQUIV_CHECK.txt`.
- **T3:**
  - The nf374 line for 0x008FCEC0 is closed. Its column-2 address is the self, not a batch row, so `--release-t3` does not apply. The coordinator closed it after landing because every binder it names was respelt; the closed text is in the landing's `T3_CLOSED.tsv`.
  - A new nf476 line tracks the split that remains (0x00902233, 0x00957940).

## Held: b1_hold (lock `COORD_HOLD_NF476`, NAME_FIXES_QUEUE nf476)
Both ports grade RELOCATION_MATCH PASS and are objproof ALL_EQ. Each fails the caller census because of HEAD debt:
- **0x0091D820:** 0x0095A890 and 0x0096A250 bind stand-ins of the self.
- **0x009705D0:** the dtor row 0x00902F71 emits an 8-byte local table with `??_E` in slot 1.

## Not started
These are unchanged, and their locks are restored to the before-state:
- IdleAtHome (nf305);
- NF475's bH_hold 4 units;
- MinionWander / CreatureGoUnderground.

## Rulings
- **r476a (advisor decision, owner delegated): the 0x00902045 row-end `#pragma optimize("", on)` is granted under an extension of R-NF409-2 clause 3** (EXTRA_RULES.md).
  - The reset changes four `CIntelligentPointer<const CThingPlayerCreature>` COMDATs:
    - `??0` becomes EQ at 0x006892A0.
    - `??_G` (30 bytes) is identical to the landed row 0x0048A4D0's copy.
    - `??4` (29 → 26 bytes) and `SetPointer` (9 → 5 bytes) have no retail address and no relocation in the object reaches them. With the reset, both are identical to the copies in 0x00688D40, 0x00691D20 and 0x006A9180.
  - The coordinator measured this by compiling all 16 landed rows that use those headers.
- **r476b (coordinator, on precedent):** the 0x009612E0 HEAD test lands as is, since it passes. Its respell onto the PDB names is queued (NAME_FIXES_QUEUE nf476).
- **r476c (coordinator):** confirmed. The `COORD_HOLD_NF475` / `COORD_HOLD_NF473` locks match the before-state in the agent's `LOCKS.txt`.
