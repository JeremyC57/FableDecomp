# Chunk 2041 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Early-return arms that end `LeanAngle = 0.0f; return;` each (instead of one trailing statement) give the constant 0 enough IL
  references for its own callee-saved register (0x006AB82D `xor ebp,ebp ; cmp ecx,ebp`, `mov [esi+1FCh],ebp` in five epilogues).
- `GetPhysicsTC()->SetRHSet(CRightHandedSet(up, GetPhysicsTC()->GetFacingVector()))` loads the physics component once into a callee-saved
  register (0x006AB84C `mov esi,[esi+60h]`); `PhysicsTC->` member reads reload it after the inner call while keeping the vtable.
- Identical `SetRHSet(CRightHandedSet(C3DVector(0,0,1), local))` tails in two early-return arms are cross-jumped by VC7.1; the jump target
  splits the absorbing arm's scheduling region (a no-code tuple 0x2C3 ends it), so its constant stores stay late. Letting an if/else
  share one `LeanAngle = 0; return;` tail stops the merge and restores retail's single region (0x006ABB40..0x006ABBA8).
- A same-TU-visible body of an out-of-line callee that takes a local's address keeps the local from escaping, so the frame packer can
  give it a word shared with a later local: GFGetAngleXYTo's header body (plain inline, still called; 0x00661540) let the z-target
  direction reuse the target position's words (Y / Z at [esp+48h] / [esp+4Ch]).
- `C3DVector v = other; v.Z = 0.0f; v.GetSquaredMagnitude()` squares Y first (Y loaded and stored first, 0x006ABC5F); `v.Initialise(
  other.X, other.Y, 0.0f)` gives the same plus, in 0x006AB820, retail's x87 operand order for the following dot product and
  `(facing ^ direction).Z` cross (c2 record-id tie-breaks; member-by-member stores flip the cross product's first term).
- Outer dead-name sweeps (k reference locals) flip only some x87 tie-breaks: in 0x006AB820 the MovementAcceleration check flips at k >= 6,
  the squares / dot / cross never; the natural fix came from how the direction vector is built (Initialise) and from operator^.
- An inline `T& GFLimit(T&, const L&, const U&)` (0x0048DAE0, plain, out of line for nine callers) is called in a cold arm (trace
  c1 24 / 27, scores 83 / 93); retail's written-out clamps (`if (x < lo) x = lo; else if (x > hi) x = hi;`) need no inline decision. A
  clamp whose bound is a call re-evaluated in the assignment (four GetMaxTurnSpeed calls, 0x006ABD4F..0x006ABD8C) is written out too.
- c2 scheduler: a PAREN tuple (`(float)(a*b)`) between an x87 fmul and its fstp lets the fstp issue right after the fmul (one cycle
  early); the next ready `fld` then outranks an integer copy of the stored value. Removing the paren (`(float)cos(x)*(y)`) delays the
  fstp so the copy pairs with it (0x006ABB7D..0x006ABB8F) -- but the shared InitialiseFromAngles body needs the parens for four landed
  rows, so this one swap stays (traces: tools/sched.sh).
- FableWin's COMDAT run right after a function lists the inline helpers it first instantiates (after ?ResolveFacingDirection@:
  GFGetRoundedSignedAngle, GFArcCos, GFWrapReturn<float>, GFMod<float>); retail keeps unreferenced out-of-line copies of two of them
  (0x006ADDB0, 0x005DB180) that prove the expanded bodies. GFGetRoundedSignedAngle's plain-inline cost (c1 87) exceeds 74 at any
  frequency: FABLE_FORCEINLINE with an Inline-budget line.

Review at landing:
- 0x006AB820 is parked (DIFFER 2328v2328, one 4-line scheduling swap); differ/ holds the source and its passing test.
- NAME_FIXES.tsv: 0x006A99D0 GetMaxTurnSpeed is IBEMXZ (protected const) in both PDBs; the landed self row spells QAEMXZ.
- hdr/README.md: 6 opt-ins / 1 new header (CPlayerMovementDef.h) on top of the chunk 2039 overlay; 0x006ACA30 re-graded RELOCATION_MATCH.
- Landed 0x006A9690 names physics slot 74 (+0x128) SetRHSet in its local model; the RigidBody vtable and CTCPhysicsBase.h put SetRHSet at
  75 (+0x12C). Catalog 0x006A9520 "CThingAICreature::GetTargetPosition" probably is CThingPlayerCreature::GetTargetPosition (this TU).
- If the InitialiseFromAngles body is ever re-examined: the paren-free Y line fixes 0x006AB820 and breaks 0x00A22CA0 / 0x00A0C570 /
  0x00A14040 / 0x00A14260 as authored.
