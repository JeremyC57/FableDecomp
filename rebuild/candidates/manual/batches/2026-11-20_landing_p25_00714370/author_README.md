# 0x00714370 NTCScriptedControl::CActionMoveTo::OnActivate (p25F)

**Grade:** RELOCATION_MATCH (948/948), no flag.

**Revision 2 (after main landed revision 1 in a28016b16):** the batch now holds a refined source, and main should replace the landed one with it.
- The `radii` local, which is not in Ego_d, is gone: `Proximity = std::max(Proximity, <sum of the two GetRadius()>)`.
- FABLE_RETAIL_CTHING_GETTC_EXPANDED is dropped: it was byte-neutral, and the default CThing.h GetTC / PeekTC branch gives the same bytes.
- Line 2 now carries the measured DIFFER(878v948).
- Re-verified: strict vland with `--recheck-landed` (the row is now catalogued) passes, the test passes, and the mutants are 19/19 killed.
  The std::max mutant is `std::max(Proximity, ` -> `(`.
- The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/`, fresh LANDVERIFY_WORK; `vland.log`), with no warnings.
- The real-object behaviour test passes.
- **NAME_FIXES:** one. 0x00800500 is ?HasRoute@CTCPreCalculatedNavigationRoute@@QBE_NXZ (returns bool), not the landed QBEJXZ (long).

## What changed against the draft (pending/2026-10-22_linux_hand280, DIFFER(950v948), local stand-in model)
- **A full port onto the shared headers.** CThing.h with the expanded GetTC / PeekTC, CTCPhysicsBase.h, CTCParamsPhysicsBase.h,
  CTCCarryable.h, CTCUserActionPointBase.h (CTCSingleUserActionPoints), CTCCreatureNavigation.h, CScriptThing.h, CTCScriptedControl.h and
  NTCScriptedControl_CActionMoveTo.h for `this`.
- **Only one class is local:** CTCPreCalculatedNavigationRoute, which has no header. It is modelled from Ego_r.pdb (Route +0x20 retail).
- **The draft's stand-in callees are respelled to the catalogue / PDB names:**

  | draft | now |
  |---|---|
  | IsInScriptedMovement / EnterScriptedMovement | NCreatureMode::IsAbleToMove / RemoveMovementBlockingModes (__fastcall) |
  | Detach | CTCBase::Clone |
  | GetName | GetClassTypeName (slot 22) |
  | GetBounds | GetRHSet |
  | CPhysicsSnapshot | CTCParamsPhysicsBase |
  | ClaimActionPoint | GetFreeActionPoint |
  | HasRoute / GetEndPosition | HasRoute / GetRouteDestination |
  | FollowRoute | InitialiseFromPreCalculatedRoute |
  | CancelAllActions | CleanupAllActions |
  | Failed | Terminated |
  | PControl | PTCScript |
- **Locals and blocks are Ego_d's:** ptc_physics; {old_position, tc_params, old_rhs}; {ptc_carryable {ptc_single_user_action_points {rhset}}};
  {ptarget_thing}; {ptc_navigation {ptc_pre_calculated_route}}. The action-point arm keeps Ego_d's two failure blocks (lines 713..718,
  723..728).
- **The draft's frame residue (0x54 v 0x50) is gone with the real GetTC.** The last residue was the proximity max. Retail selects an address
  (0x007145F9..0x00714603), which is STLport's reference-returning `std::max`. The draft's conditional expression gives DIFFER(936v948)
  (`evidence/p1_ternary_diff.txt`).

## Header changes (add-only opt-ins in `hdr/`, `hdr.diff`; asserts pinned with pin_asserts.py: 2 pinned)
| opt-in | header | what |
|---|---|---|
| FABLE_RETAIL_CTCPARAMSPHYSICSBASE_INLINE_CTOR | CTCParamsPhysicsBase.h | the ctor body, plain inline (retail expands it at 0x00714428) |
| FABLE_RETAIL_CACTIONMOVETO_ONACTIVATE | NTCScriptedControl_CActionMoveTo.h | the slot-2 override declaration |
| FABLE_RETAIL_CTCSCRIPTEDCONTROL_CLEANUPALLACTIONS | CTCScriptedControl.h | declaration of the landed 0x00713630 |
| FABLE_RETAIL_CTCCARRYABLE_ISCARRIED | CTCCarryable.h | ?IsCarried@CTCCarryable@@QBE_NXZ (PDB name), inline, `PCarryingTC != 0` (retail tests +0x10 in place) |

- newly_flagged_rows.tsv has the header line only: every opt-in sits behind its own macro, so no includer changes.

## Inline-budget ablation (line 2)
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| without FABLE_RETAIL_CTCPARAMSPHYSICSBASE_INLINE_CTOR (the ctor declared only, so it is called) | DIFFER(878v948) |
| `Proximity < radii ? radii : Proximity` instead of std::max | DIFFER(936v948) |
| with a named `float radii` local (revision 1) | RELOCATION_MATCH (neutral; dropped, not in Ego_d) |
| with FABLE_RETAIL_CTHING_GETTC_EXPANDED (revision 1) | RELOCATION_MATCH (neutral; dropped) |

- The other three opt-ins are declarations and an accessor; they decide no inlining. There are no flags or pragmas.

## Mutants and test
- **Mutants:** 19 of 19 killed (`mutants/00714370.out`). The `!=` to `==` mutant only failed to link (the test stubs no operator==), so
  it was replaced by a string mutant, which is killed.
- **Test:** `test/00714370.cpp` links the real row object.
  - The fixtures are real CVectorMap component tables for the expanded GetTC / PeekTC, a fake physics TC and script-thing vtables, and
    /alternatename stubs.
  - 17 runs cover the move check, the physics swap (checking the CTCParamsPhysicsBase copy), the carried / free-point / no-point / no-TC
    exits with and without cleanup, the proximity widening for flag 8, type 5 and type 3 (and not type 4, a dead target or a larger
    proximity), the pushable switch, and the route start / no-route / no-navigation / dead-route paths.
