# NF472 hand-back (2026-11-26): live stand-in binders of 0x008FD288 (step 1)

`README_agent.md` is the agent's own report, with the identity proofs: the PDB compiler-generated flags, RTTI slot-0 `??_G` callers and member offsets.

## Landed: bU1, bU2, bU4, bU5, 21 rows, +4 proven
- **bU1:** 0x008FECEC is the implicit `~CAIStateGroup_MinionReactToCriminal`, a thin row of the callee-respelt owner 0x008FECD0.
- **bU2:** 0x00900533 is `~CAIStateGroup_BalverineCornered`, a thin row of 0x00900517. Ctor 0x009004E0 goes declaration-only.
- **bU4:** 0x008FD56A is `~CAIStateGroup_MinionSitOnSeatBase`, a thin row of 0x008FF271. Its 7 `??_G` callers are respelt and 6 local-model ctors go declaration-only.
- **bU5:** 0x00913490 is `??_GCAIStateGroup_FlyingThingBase`, a thin row of ctor 0x009126C0 (r472a).
- **Gates (agent):**
  - vland 25 / 25 RELOCATION_MATCH PASS; WARN never grows;
  - objproof: intended changes only, `.drectve` unchanged;
  - caller census 177 OK, 0 BAD; copy census: NEQ copies of the new names 9 → 0;
  - link: duplicates 1 → 1, unresolved 26 → 11.
- **Coordinator, after landing:**
  - catalog relabel and SELF_SYMBOLS applied;
  - bindings_multi 1; funclets 1,726, 0 broken; s units 181, 0 failed; descriptive_check 2,030, 0 FAIL.
  - **Tracker:** 0x008FD56A / 0x008FECEC / 0x00900533 / 0x00913490 moved to agree_pdb, nothing worse.
  - 0x008FD288 stays split_fixable (2 spellings) until bU3 lands.

## Held: bU3_workbase (4 rows), r472b declined
- 0x008FDA24 is `~CAIStateGroup_WorkBase<CAIStateGroupBase>` (identity proven), but its only caller 0x0091A570 cannot follow without a new self spelling.
- The batch would move 0x0091A570's self to a made-up free `__fastcall` name, `?CAIStateGroup_BuyFromSeller_ScalarDeletingDestructor@@YIPAXPAXHI@Z`.
- Swapping one stand-in for another to close a different address is not "names right the first time".
- **Route:** port 0x0091A570 onto its real `??_GCAIStateGroup_BuyFromSeller` (in NAME_FIXES), then land bU3 with it.

## Rulings (coordinator)
- **r472a: granted.** A `??_G` that a landed ctor row already emits byte- and relocation-equal to retail, in the same retail TU, may be its thin row. The ctor row's local model is existing HEAD debt (r459b); no new local model text is added. The ctor's header port is queued.
- **r472b: declined**, as above.
