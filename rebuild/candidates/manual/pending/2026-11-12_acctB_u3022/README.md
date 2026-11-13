# acctB hand-back: ONE helper text set for 0x00A9EB90, 0x00A9F540 and 0x00AA0090 (supersedes two earlier folders)

**This folder replaces the row sources and tests of `2026-11-12_acctB_r3021` (0x00A9EB90) and
`2026-11-12_acctB_r2044` (0x00A9F540, 0x00AA0090).** Those two hand-backs matched with different row-local texts
of `GFFastArcCos` and `SetSlerp`; retail had one. Land the three rows from here.

| address | result |
|---|---|
| 0x00A9EB90 `C3DAnimationManager::BlendAnimations` | RELOCATION_MATCH 1337v1337, test PASS (g++ and VC7.1) |
| 0x00A9F540 `C3DAnimationManager::CalcMeshTransformations` | RELOCATION_MATCH 2288v2288, test PASS (g++ and VC7.1) |
| 0x00AA0090 `C3DAnimationManager::CalcBlendedMeshTransformations` | RELOCATION_MATCH 2390v2390, test PASS (g++ and VC7.1) |

Re-verified by the acctB local coordinator (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` holds the grade, both test runs and the read-only vland log. Purity and padded-struct greps clean. Nothing is landed. vland: 3 wins, no warnings. The helper block from `GFFastArcCos` through `BlendWith` is byte-identical text in
the three sources (checked by script).

## The common set and its evidence

- **`SetSlerp`: direct stores, `from.Dot(to)`. PROVEN:** FableWin size 323 bytes equals ours (the `*this = q` text
  0x00A9EB90 used is 346), and compiled out of line it matches retail 0x00A22590.
- **`BlendWith`:** `CQuaternion rotation; rotation.SetSlerp(Rotation, other.Rotation, t); Rotation = rotation; Position = Position.Interpolate(other.Position, t);`
  **PROVEN:** the only text that matches retail's uncalled copy 0x00AA3C50.
- **`CalculateSlerpWeights`, `SetSlerpFromIdentity`: PROVEN** by their out-of-line copies 0x00AA3390 / 0x00AA3560.
- **`GFFastSin`** (named table values): proven by size in chunk 2044, unchanged.
- **`GFFastArcCos` with named table values: FITTED.** FableWin's copy carries asserts, so its size says nothing.
  13 texts were graded across the three rows; every other text breaks at least one row. Its out-of-line copy
  matches 0x00A22530.

## Row changes that go with it (both FITTED natural spellings)

- 0x00A9EB90 scalar loop: `CQuaternion slerp; slerp.SetSlerp(rotation, to[i].Rotation, t); rotation = slerp;`
  (retail builds the quaternion in frame words and copies it back at 0x00A9F009..0x00A9F05D).
- 0x00A9F540: each of the four bone loops names its keyframe (`C3DKeyframe& keyframe = keyframes[bone];`).

None of the rows sits on an edge (extra dead locals: +1..+3, +1..+8, +1..+4 still match). Every landed user of
these helpers still grades RELOCATION_MATCH with the texts substituted in: 0x00A22530, 0x00A22590, 0x00AA3390,
0x00AA3560, 0x00AA3C50, 0x00B05620. The instruction streams are unchanged, so the fistp / rdtsc sites and the
`// Retail-asm:` lines are as in the earlier folders (asm evidence tables: `2026-11-12_acctB_r2044`).

**Headers:** chunk 2044's overlay unchanged (README note only); 0x00A9EB90 now also defines six existing opt-ins.
