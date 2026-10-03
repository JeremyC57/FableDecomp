# `core/` — bit-exact portable engine core (C++23)

Reconstructed retail functions for the portable port (x64 Windows / Android). Unlike the
`rebuild/src/compiled` parity lane, sources here need no VC7.1: they target any modern
compiler and are verified **behaviourally** against the user's own `Fable.exe` by the
Unicorn oracle (`../oracle`). They are not counted toward retail byte-parity coverage.

## Bit-exact floating point

Retail runs x87 with control word `0x027F` (53-bit precision, round-to-nearest). A port
reproduces it bit-for-bit by doing register arithmetic in `double`, rounding to `float` exactly
where retail does `fstp DWORD` (`x87::store`), and keeping the retail operation order. The
library is compiled with `-ffp-contract=off` so ARM64/Clang never fuses multiply-adds.

The oracle tests feed thousands of deterministic inputs per function, including exact
power-of-two values chosen to cancel, so a reordered sum is detected (verified by mutation).

## Functions (all bit-exact vs retail)

| Retail | Port | Notes |
|---|---|---|
| `0x00A0DB60` cosine loop | `math::CosineTable` | `Math_InitializeCosineLookup`; 1024 entries + wrap |
| inlined lookup | `math::TableCos` / `TableSin` | fistp floor + linear interpolation |
| `0x00A88B60` | `CQuaternion::operator*` | |
| `0x00A88C10` | `CQuaternion::operator*=` | |
| `0x00A88C50` | `CQuaternion::Equals` | per-component tolerance 1e-4 |
| `0x00A88CB0` | `CQuaternion::IsIdentity` | |
| `0x00A88D10` | `CQuaternion::SetFromAxisAngle` | angle unit: 1.0 = one turn; table trig |
| `0x00A52450` | `C3DKeyframe::ToMatrix(CMatrix3x4&, bool)` | catalogued upstream as `CQuaternion::ToMatrix`; 2nd arg is a scaling flag |
| `0x00AA39A0` | `C3DKeyframe::ToMatrix(CMatrix4x4&)` | catalogued upstream as `CQuaternion::ToMatrix`; 2nd retail arg unused |
| `0x00987BF0` | `CPreTransposedBoneMatrix::Transform` | catalogued upstream as `CQuaternion::operator*` |
| `0x00ADDFE0` | `CPreTransposedBoneMatrix::TransformInPlace` | catalogued upstream as `CQuaternion::operator*` |

## Catalogue corrections found while porting

- `0x00A52450`, `0x00AA39A0`: receiver is `C3DKeyframe` (reads Rotation +0x00, Position +0x10,
  Scaling +0x20), not `CQuaternion`.
- `0x00987BF0`, `0x00ADDFE0`: receiver is a 3×4 row-major `CPreTransposedBoneMatrix`, not a
  quaternion multiply.
