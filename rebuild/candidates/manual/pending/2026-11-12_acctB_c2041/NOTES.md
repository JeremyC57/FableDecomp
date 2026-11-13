# Hand chunk 2041 (CThingPlayerCreature .. CThingPlayerCreature): 0 of 1 byte-exact (1 parked at 2328v2328, 652 of 656 instructions in place)

Every grade, probe and test used `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2041/hdr` (hdr/README.md: the chunk 2039 overlay
plus 6 opt-ins / 1 new header of mine). The same-TU row 0x006ACA30 (chunk 2039) re-graded under the final overlay: RELOCATION_MATCH.

## What matched (0)

## Parked for Fable 5.1 (1)
- 006ab820 CThingPlayerCreature::ResolveFacingDirection DIFFER(2328v2328): retail 0x006ABB81 `mov edx,[esp+48h]` / 0x006ABB85 `fld qword
  [0122ED70h]` / 0x006ABB8B `mov [esp+6Ch],edx` / 0x006ABB8F `fsin`, ours `fld` / `fsin` / `mov` / `mov` (the z-target arm's Y copy into the
  new right-handed set scheduled after the Z sine instead of before it); all other instructions, allocation and frame words equal
  | tried: ~40 row spellings of the z-target arm (angle named / inline / const, direction first / temp / named set / explicit trig, set
  named, PhysicsTC member, CRightHandedSet ctor bodies, grouping parentheses), c2 scheduler trace (cause below) | next: the cause is the
  shared InitialiseFromAngles body's `(float)(cos(..)*(cosYZ*length))` PAREN tuple between the Y fmul and its fstp (the fstp issues one
  cycle early, then `fld 0.0` outranks the Y-copy mov 117760 v 48128); the paren-free body gives retail's bytes for this row but breaks
  four landed users (0x00A22CA0, 0x00A0C570 via the header; 0x00A14040, 0x00A14260 with their local copies). Either retail's z-target
  arm builds the direction some other way (not found), or those four rows have another spelling that matches with the paren-free body.
  Differ source differ/006ab820.cpp, passing test differ/006ab820.test.cpp (g++ and VC7.1).

## Skipped
- none. Length check: the oracle length 2328 is right (`ret` at 0x006AC137, int3 padding to 0x006AC140 = ResetThingReactions). No SPLITS.tsv.

## Label corrections
- NAME_FIXES.tsv: 0x006A99D0 is ?GetMaxTurnSpeed@CThingPlayerCreature@@IBEMXZ (protected const, both PDBs); the landed self row declares
  it QAEMXZ.
- Probable (one source each, not in LABELS.tsv): retail 0x006A9520 (unreferenced, in this TU before OnDie; returns PeekPos() + (0, 0,
  PhysicsTC->Radius)) is ?GetTargetPosition@CThingPlayerCreature@@QBE?AVC3DVector@@XZ (Ego_r 0x005464C0 in this class's donor block),
  not the catalog's CThingAICreature::GetTargetPosition.
- Review: landed 0x006A9690 ResolveRelativeMovementAcceleration names CTCPhysicsBase slot 74 (+0x128) `SetRHSet`; the CTCPhysicsRigidBody
  vtable 0x0126996C has 74 SetFacingAnglesFromRHSet 0x007599C0 / 75 SetRHSet 0x00759A00, and engine/CTCPhysicsBase.h puts SetRHSet at
  +0x12C (this row calls +0x12C). A local-model slot name only (no relocation).

## Names and evidence
- Row and callees: LABELS.tsv (6 entries). Descriptive (made-up) names: none in symbols. Locals (sneaking, camera_direction, targeted_pos,
  target_pos, direction, facing, cos_angle, angle, turn, desired_turn, actual_turn, lean_target, lean_step, lean_limit, facing_now,
  facing_angle) are descriptive. EMovementBandType values 1..4 are written as casts (no enumerator names on disk; band meanings from
  the landed GetCurrentMaxMovementSpeed 0x006AB5D0).
- New FableWin-only inline names used: GFGetRoundedSignedAngle, GFArcCos, GFWrapReturn<float>, GFMod<float> (FableWin COMDATs right after
  ResolveFacingDirection), CPlayer::IsPCProjectileWeaponThirdPersonAimingMode, C3DVector::Initialise / InitialiseFromAngles / operator^.

## Decoded outline (retail order)
1. PCurrentAction set, not Finished (+0x61), IsPreventingNormalMovement (slot 49): LeanAngle = 0, SetRHSet(up (0,0,1), GetFacingVector()), return.
2. sneaking = PeekTC<const CTCStealth> && IsSneaking. PC third-person aiming (CPlayer +0x21A): camera manager (CAIGameCameraBase slot 10)
   view direction, Z = 0, normalise, SetRHSet; LeanAngle = 0; return.
3. First-person targeting: targeting->GetTargetedPos(pos, 1.0f) - GetPos(), Z = 0, normalise, SetRHSet; LeanAngle = 0; return.
4. Z-targeting: locked thing (slot 36): heading GFGetAngleXYTo(PeekPos(), focal pos or GetPos()) -> InitialiseFromAngles(h, 0, 1) ->
   SetRHSet; none: SetRHSet(up, GetFacingVector()) (out-of-line set ctor); LeanAngle = 0; return.
5. Otherwise with MovementAcceleration non-zero: turn = sign((facing ^ dir).Z) * GFArcCos(clamped dot) limited to +-GetMaxTurnSpeed()/360,
   rotate facing about Z, SetRHSet; lean target from the band (3 / 4, unclamped turn > 4 degrees, not sneaking) and the movement def; if
   the physics position barely moved (< 0.05 squared) LeanAngle = 0 and reset the set, else GFSmoothTowards; limit LeanAngle by
   LeanAngleLimitAngle/360; tilt up about the facing by LeanAngle. With no acceleration: LeanAngle = 0, upright set.
   Then band 1, or (sneaking or band 2) and |GFGetRoundedSignedAngle(facing angle - acceleration angle)| > 1 degree: acceleration = 0.

All grades/probes with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2041/hdr` (tools/gr.sh sets it).

## Grade log 0x006AB820 (pass 1)
| # | change | result (md.py exact / regs+frame masked) |
|---|--------|--------|
| 1 | first draft (else-if chain PC/FP/ZT/main, GFLimit clamps, plain GFGetRoundedSignedAngle) | DIFFER 2227v2328, 294 / 458 of 656 |
| 2 | stealth StatGetInterfaceType inline (overlay), GetPTargetLockedThing (slot 36), explicit clamps (GFLimit sc 83/93 called), GFGetRoundedSignedAngle FABLE_FORCEINLINE (c1 87) | DIFFER 2253v2328, 309 / 467 |
| 3 | per-arm `LeanAngle = 0.0f; return;` (ebp = 0 constant register as retail), GetPhysicsTC() accessor at every physics call (object loaded once, as retail), angle statement before the sign ternary | DIFFER 2275v2328, 553 / 566 |
| 4 | main direction = MovementAcceleration copy + Z = 0 + 3D GetSquaredMagnitude (fragment: Y-first squares as retail), ZT InitialiseFromAngles inside the SetRHSet call, band block: facing angle named before GetAngleXY (if / else-if, matches retail) | DIFFER 2271v2328, 562 / 573 |
| 5 | `desired_turn == actual_turn` operand order (fucompp order now retail) | DIFFER 2271v2328, 563 / 573 (wasted: should have been a probe) |
| 6 | GFGetAngleXYTo body visible (plain inline header opt-in; body = landed 0x00661540, QLEN check RELOCATION_MATCH; still called): target_pos no longer escapes, direction shares its word ([esp+48h]); ZT arm: one `LeanAngle = 0; return;` after the found / null if-else (no cross-jump of the camera tail into ZT-found -> one scheduling region, retail's early up stores) | DIFFER 2328v2328, 639 / 647 |
| 7 | main direction via C3DVector::Initialise(MA.X, MA.Y, 0.0f), sign from (facing ^ direction).Z (FableWin operator^): free probes showed these give retail's record ids for all four x87 tie-breaks (MA check, squares, dot, cross) | DIFFER 2328v2328, 653 / 653: only 0x006ABB81..8F (fld 0.0 / fsin scheduled before the Y copy mov) |

## The 0x006ABB81 residue (ZT direction: `fld 0.0 ; fsin` before the Y copy `mov edx,[esp+48h] ; mov [esp+6Ch],edx`)
- c2 scheduler trace (tools/sched.sh, copy of agent2031's hook): region from the target-position copy to SetRHSet; ours issues the Y
  `fstp [esp+48h]` at cycle 228 right after its fmul because the shared InitialiseFromAngles body has a PAREN tuple between them
  (`Y=(float)(cos(..)*(cosYZ*length))`); at 229 `fld 0.0` (prio 117760, fsin chain) beats the Y-copy `mov` (48128).
- Free probes: an InitialiseFromAngles body with the cast on the trig result only (`Y=(float)cos(..)*(cosYZ*length)`, or all three
  components so) delays the fstp to 232 where the mov pairs with it: retail's bytes for the whole row (probe q7 / q5 = our grade-7 object
  with exactly the two retail lines). But that body breaks the landed users 0x00A22CA0 (q5: DIFFER 368v368, q7: 370v368) and 0x00A0C570
  (q5: 373v373, q7: 377v373) re-graded under the overlay (original body: both RELOCATION_MATCH; 0x00746EC0 / 0x008BD3F0 stay
  RELOCATION_MATCH either way). One shared body for all rows -> not used.
- Row-level spellings measured (free, zt feature): angle named / inline / const, direction declared first, InitialiseFromAngles as its
  own statement, PhysicsTC member, named set (frame grows to 0x88 or set at [esp+2Ch]), C3DVector temp, CRightHandedSet::Init
  (FableWin) under the ctor (changes the inline split): none moves it.

## Grades per pass
- Pass 1: 7 grades (above). Passes 2 and 3: not used -- after grade 7 the only residue was the 4-line scheduling swap; ~40 further
  row spellings were compile-only probes on our own object (no retail diff) and none moved the deciding instruction, so there was no
  candidate whose grade could differ from grade 7's; grading unchanged objects would have spent grades for nothing.

## Free work on top of the grades
- ~150 compile-only probes (tools/pv.sh, tools/fr.sh, feature extraction tools/feat.py: own output only), 6 inline traces (tools/inl.sh),
  2 c2prio runs (tools/cp.sh), 8 scheduler traces (tools/sched.sh, copy of agent2031's hook), x87 record-id sweeps (tools/sweep.sh: k
  dead reference names; MA check flips at k >= 6, the other three orders do not move with outer names), out-of-line body checks
  (GFGetAngleXYTo v 0x00661540 RELOCATION_MATCH), regression re-grades of landed users of changed header bodies (tools/regr.sh,
  tools/bodyvar.sh: 0x00746EC0, 0x00A22CA0, 0x00A0C570, 0x008BD3F0; local-body probes of 0x00A14040 / 0x00A14260).
- 0x007F26B0 (pending r3002, needs its own overlay) was started once under this overlay and spun; its cl.exe (mine) was killed by PID.
