# Landing acctB_b26 (Account A, 2026-11-24): 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim, GetMeshAnim from a shared header

Owner (2026-11-24): "Yes move onto shared headers, make sure to check it afterwards." B's staged row carried a verbatim strong copy of
landed CAnimSet::GetMeshAnim 0x0070C900 (the caller's leaf needs the body visible: 2610 v 2395 without it). Both PDBs put GetMeshAnim,
every other CAnimSet method and CalcInterpolatedMeshAnim in animate_thing.obj, so retail compiled the body in the caller's TU. The row now
takes it from `engine/CAnimSet.h` opt-in `FABLE_RETAIL_CANIMSET_GETMESHANIM_INLINE_BODY` (the same body, `inline`): a pick-any COMDAT
equal to retail 0x0070C900 instead of a second strong definition. The other owner decision on this row stands: the `// Inferred:`
always-true operand (`EnableCombinationAnimations && true`, "yes use it and mark inferred").

Checks (owner asked for them):
- vland --land: RELOCATION_MATCH PASS (`logs/verify.log`). pin_asserts then pinned 6 asserts the header insertions moved
  (C3DAnimationTransitionState.h); a rerun pins 0, no warnings. Regraded from the repo after the pin: RELOCATION_MATCH PASS
  (`logs/recheck.log`); pickany_vs_retail 17 EQ, no leaf-referenced NEQ; pk_all no leaf=y NEQ; the GetMeshAnim copy is `pickany`, EQ at
  0x0070C900 (`logs/pickany.log`). Warnings, both known and owner-accepted at B's re-verify: DELTA_NOTIONAL_ZERO label, `_$E2` atexit thunk.
- Headers: B's overlay (11 headers, additive; 2 new) plus the A opt-in in CAnimSet.h. equiv_check over every landed source and test (both
  trees scratch copies): see `logs/equiv.txt`. bindings_multi_check: baseline. funclet_includes_check: 1362 checked, 0 broken.
- Name fixes: every line of B's notes/ and progress/ NAME_FIXES for this row is already queued.
Still open: the landed 0x0070C900 row models its classes locally, so GetMeshAnim's body exists there and in the header; re-rowing
0x0070C900 onto the shared headers is a follow-up.
