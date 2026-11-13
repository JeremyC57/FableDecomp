# Hand chunk 3022 (C3DAnimationManager): 3 of 3 byte-exact with ONE helper text set

## What matched (3)
- 0x00A9EB90 BlendAnimations, 0x00A9F540 CalcMeshTransformations, 0x00AA0090 CalcBlendedMeshTransformations: all RELOCATION_MATCH
  (q.py + read-only vland, behaviour PASS under g++ and VC7.1). The block from GFFastArcCos to BlendWith (GFFastArcCos, GFFastSin,
  SetSlerpFromIdentity, PostApplyRelativeAnimationAlpha, Dot, CalculateSlerpWeights, SetSlerp, BlendWith, plus the comment above it)
  is now byte-for-byte the same text in all three src files (checked with a script; cand/H_common.txt).

## Helpers that differed between the rows before (diffed first)
- GFFastArcCos: 0x00A9EB90 named values `a`/`b`; the two twins used the one-line text.
- SetSlerp: 0x00A9EB90 `to.Dot(from)` + `CQuaternion q; ...; *this = q;`; the twins direct stores + `from.Dot(to)`.
- CalculateSlerpWeights was only in 0x00A9EB90; SetSlerpFromIdentity / PostApplyRelativeAnimationAlpha / BlendWith only in the twins.
  GFFastSin and Dot were already the same.

## Common set and its evidence (FITTED v PROVEN)
- GFFastSin (named a/b, two-statement index step): PROVEN-by-size (chunk 2044, sibling-relative), unchanged.
- GFFastArcCos `float a = g_ArcCosTable[i]; float b = g_ArcCosTable[i + 1]; return ((1.0f - frac) * a + frac * b) * 0.15915494f;`:
  FITTED. Same style as GFFastSin; FableWin's copy has asserts (119i), so size says nothing. Out of line it RELOCATION_MATCHes
  0x00A22530. Matrix of 13 ArcCos texts over the three rows (cand/ac/results.txt): only the named-value text, or the table-pointer
  text `const float* table = &g_ArcCosTable[i];` (tP: 0x00A9EB90 left with one SIB swap at 0x00A9EC5A), come close.
- SetSlerp (direct stores, `from.Dot(to)`): PROVEN-by-size. VC7.1 /Od 104i/323b v FableWin 114i/323b, the same +10i/+0b as its siblings
  CalculateSlerpWeights 75/85 and SetSlerpFromIdentity 87/97. The `*this = q` text is 111i/346b (wrong). Out of line it
  RELOCATION_MATCHes 0x00A22590.
- BlendWith `CQuaternion rotation; rotation.SetSlerp(Rotation, other.Rotation, t); Rotation = rotation; Position =
  Position.Interpolate(other.Position, t);`: PROVEN against retail's out-of-line copy. Compiled out of line it RELOCATION_MATCHes
  0x00AA3C50 (probe/users/00aa3c50.cpp). Chunk 2044's text with the named `from` reference (FableWin 48i SAME) gives 671v671 there
  (dot order), and 2044's original text gives 669v671. FableWin is a later build, so the retail copy wins.
- CalculateSlerpWeights (unchanged; RELOCATION_MATCH v 0x00AA3390) and SetSlerpFromIdentity (unchanged; v 0x00AA3560 and 0x00B05620).
  PostApplyRelativeAnimationAlpha (`other_position` named; FableWin 32i SAME; no retail out-of-line copy known): FITTED, unchanged.
  Dot: plain one-expression text, unchanged (FableWin +2i/+8b, unexplained, as in chunk 2044).
- Landed users regraded with these texts substituted (probe/users/mk.py builds them; grades.log "CHECK landed user ... v2"):
  0x00A22530, 0x00A22590, 0x00AA3390, 0x00AA3560, 0x00AA3C50, 0x00B05620 are all RELOCATION_MATCH. The GFFastSin users were verified
  by chunk 2044 with the same text.

## What changed in each row
- 0x00A9EB90: uses the twins' SetSlerp text. The scalar loop becomes `CQuaternion& rotation = from[i].Rotation; CQuaternion slerp;
  slerp.SetSlerp(rotation, to[i].Rotation, t); rotation = slerp;`. Retail builds q in frame words 0x40..0x4C and copies it
  (0x00A9F009..0x00A9F05D). Writing `CQuaternion rotation; rotation.SetSlerp(from[i].Rotation, ...); from[i].Rotation = rotation;`
  instead gives 1335v1337. Adds the BLEND / SLERPFROMIDENTITY / MULASSIGN / ADDASSIGN / POSTAPPLY / C4DVECTOR opt-ins so the full
  block compiles (byte-neutral). Window: +1..+3 dead locals still match, +4 breaks.
- 0x00A9F540: each of the four bone loops names its keyframe (`{ C3DKeyframe& keyframe = keyframes[bone]; ApplyX(keyframe, ...); }`).
  Under the named ArcCos the BlendWith dot otherwise loads X first (retail 0x00A9FA56 order is Y, Z, W, X). Window: +1..+8 dead
  floats all match. Tried and failing: named sequence reference, named `enabled`, `bool partial` (all broke other code), and
  dead-long rulers (no effect). Before this the dead-float ruler showed a narrow 2..3 / 6 / 9 window.
- 0x00AA0090: the row body is unchanged. Only the helper block changed (named ArcCos, BlendWith without the reference).
  Window: +1..+4 dead floats. The keyframe-reference loop form does NOT fit this row (DIFFER 2390v2390, q products), so the twins
  keep different loop spellings.
- Tests: the GFFastArcCos copies in test/00a9f540.cpp and test/00aa0090.cpp are rewritten in the named form (same results). All
  three tests pass under g++ and VC7.1.

## Retail-asm
Code is instruction-identical to the prior matches, so the fistp / rdtsc sites the GFFloatToLongNear / GFGetCycleCount /
GFFloatToLongFloor helpers reach are unchanged. The `// Retail-asm:` lines are kept as they were.

## Parked for Fable 5.1 (0)
None.

## Names
Unchanged from chunks 3021 and 2044. One new local name, `slerp`, in 0x00A9EB90's scalar loop; it is descriptive.

## Label corrections
None.
