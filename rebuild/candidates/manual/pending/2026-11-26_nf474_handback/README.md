# NF474 hand-back (2026-11-26): NF467's b3 rework on the existing retail views, 11 more full-size vftables

`README_agent.md` is the agent's own report. `VFTABLES_FIXED.tsv` lists the tables.

## Landed: b0_hdr (9 headers) + b3r_a (22 rows, 11 units), one commit after equiv_check
- **Headers:** each existing retail view (NF330 / NF429 `_VIRTUAL_MODEL`, or the older full view) gets a nested `FABLE_RETAIL_CAISTATEGROUP_<X>_CTOR` that declares only the default ctor.
  - Each has an `#error` if it is used without its view; moved asserts are pinned.
  - `g++ -E -P` is identical for every HEAD includer, and the 23 row includers grade identically.
- **Units:** MinionHasInformation, MinionListenToStory, ChickenBeg, LookAtInterestingThings, SeekPerceivedThing, CreatureRangedCombatUseWillShield, TestTestTest, OpenAndCloseShop, MinionGuardGreetPlayer, MinionReactToPlayerInteraction and MinionCuriosity.
  - Each ctor emits its full 14-slot, 56-byte `??_7`, equal to retail.
  - Each `??_G` home is a thin row.
  - OpenAndCloseShop and MinionCuriosity have no header, so they are local classes on CAIStateGroupBase.h (r462a), with Ego_r sizes 0x18 / 0x20.
- **Gates (agent):**
  - vland 22 / 22 RELOCATION_MATCH PASS; WARN 31 → 11 (the 11 go with the relabel);
  - ctor bytes unchanged; `.drectve` unchanged;
  - link: duplicates 0 → 0, unresolved 123 → 116;
  - census 0 BAD.
- **Coordinator, after landing:**
  - 11 relabels and 22 SELF_SYMBOLS lines;
  - bindings_multi 1; funclets 1,744, 0 broken; s units 199, 0 failed; descriptive_check 2,030, 0 FAIL.
  - **Tracker:** +11, rtti_identified → agree_pdb (the `??_G` homes), nothing worse.

## Held: b3r_hold (22 rows, 11 units, r467b)
Each needs its slot row respelt first. The list is in `README_agent.md`; 4 of the slot targets are on T3 lines.

## Rulings (coordinator)
- **r474a: resolved at landing.** The 22 ctor locks were NF473's, a finished agent, and the coordinator releases a finished agent's locks at landing. NF474 was right not to override them.
- **r474b: granted.** An unreferenced pick-any `??0C3DVector@@QAE@XZ` is allowed: no table line, self symbol or Leaf names it, and no landed row defines it strongly (r470c).
- **r474c: granted for the held units.** A `_CTOR` sub-macro may also declare retail overrides the view lacks, spelt from Ego_r + FableWin, add-only.
- **r474d: the PDB spelling wins.** 0x00934100 IsStillValid must keep `bool` (`_N`). Find a C++ spelling with a bool return that gives retail's bytes ("never park on" rule). Until then the MinionKidGroupTag unit is HOLD; an `int` return would emit a non-PDB name.
