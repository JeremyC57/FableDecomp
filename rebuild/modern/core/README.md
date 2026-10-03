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
| `0x00A0DB60` rsqrt loop | `math::InvSqrtTable` | 128 buckets; bucket 64 forced to 0xFF by retail |
| inlined | `math::InvSqrtEstimate` / `FastInvSqrt` | exponent trick + table + one Newton step |
| `0x00A55D80` | `CMatrix3x4::InitialiseSkewedSymmetric` | |
| `0x00A55DF0` | `CMatrix3x4::operator*` | |
| `0x00C1CF20` | `CMatrix3x4::operator*=` | rhs in EDX; catalogued upstream as `operator*` → C3DVector |
| `0x00A560A0` | `CMatrix3x4::IsIdentity` | NaN rows pass (retail flag test) |
| `0x00A56180` | `CMatrix3x4::Equals` | |
| `0x00A56270` | `CMatrix3x4::Orthonormalise` | |
| `0x00A56530` | `CMatrix3x4::operator*(C3DVector)` | |
| `0x00A9D480` | `CPreTransposedBoneMatrix::ScaleRows` | catalogued upstream as `CMatrix3x4::PostScale`; x87 and SSE paths both verified |
| `0x00A9D580` | `CPreTransposedBoneMatrix::ScaleColumns` | catalogued upstream as `CMatrix3x4::PostScale`; x87 and SSE paths both verified |
| `0x00A14440` | `C3DVector::Normalise` | catalogued upstream as `GetScaled` |
| `0x00A14480` | `C3DVector::NormaliseAndGetLength` | catalogued upstream as `GetScaled`; returns length in ST0 |
| `0x00A14510` | `C2DVector::Normalise` | catalogued upstream as `C2DVector::Dot` |
| `0x00A14540` | `C2DVector::FastNormalise` | catalogued upstream as `Normalise`; table estimate |
| `0x00A55F90` | `CMatrix3x4::InitialiseRotation` | `Matrix_RotationAroundAxis`; x87 fsin/fcos → libm (see caveat) |
| `0x00A13C60` | `C3DVector::Rotate` | |
| `0x00A42140` | `CPlane::Initialise(normal, d)` | |
| `0x00A42170` | `CPlane::Initialise(p0, p1, p2)` | |
| `0x00A42280` | `CPlane::InitialiseFromPointAndNormal` | **not in upstream catalogue** |
| `0x00A422C0` | `CPlane::GetIntersectionWithLine` | |
| `0x00A42370` | `CPlane::GetIntersectionWithLineOffset` | **not in upstream catalogue** |
| `0x00A42430` | `CPlane::IsWithinDistance` | **not in upstream catalogue** |
| `0x00A42470` | `CPlane::GetIntersectionWithTriangle` | |
| `0x00A56C40` | `C2DLineF::Set` | **not in upstream catalogue** |
| `0x00A56C60` | `C2DLineF::GetDirection` | **not in upstream catalogue** |
| `0x00A56B80` / `0x00A56BB0` | `C2DLineF::GetLowestX` / `GetHighestX` | return in ST0 |
| `0x00A56BE0` / `0x00A56C10` | `C2DLineF::GetLowestY` / `GetHighestY` | first catalogued upstream as `GetHighestY` |
| `0x00A56FF0` | `C2DLineF::OnLeft` | strict |
| `0x00A57030` | `C2DLineF::Intersects2D` | shared endpoints within 1e-4 (inclusive) count |
| `0x00A57140` | `C2DLineF::GetPointOnInfiniteLine` | |
| `0x00A571A0` | `C2DLineF::GetDistanceToPoint` | 4 branches; all optional-output combinations verified |
| `0x00A57960` | `C2DLineF::GetInfiniteLineLineIntersection` | |
| `0x00A57A40` | `C2DLineF::IsWithinDist` | returns int 0/1 in EAX |
| `0x00A88B60` | `CQuaternion::operator*` | |
| `0x00A88C10` | `CQuaternion::operator*=` | |
| `0x00A88C50` | `CQuaternion::Equals` | per-component tolerance 1e-4 |
| `0x00A88CB0` | `CQuaternion::IsIdentity` | |
| `0x00A88D10` | `CQuaternion::SetFromAxisAngle` | angle unit: 1.0 = one turn; table trig |
| `0x00A52450` | `C3DKeyframe::ToMatrix(CMatrix3x4&, bool)` | catalogued upstream as `CQuaternion::ToMatrix`; 2nd arg is a scaling flag |
| `0x00AA39A0` | `C3DKeyframe::ToMatrix(CMatrix4x4&)` | catalogued upstream as `CQuaternion::ToMatrix`; 2nd retail arg unused |
| `0x00987BF0` | `CPreTransposedBoneMatrix::Transform` | catalogued upstream as `CQuaternion::operator*` |
| `0x00ADDFE0` | `CPreTransposedBoneMatrix::TransformInPlace` | catalogued upstream as `CQuaternion::operator*` |

**Caveat — x87 transcendentals:** `fsin`/`fcos` (used by `Matrix_RotationAroundAxis`) are ported
with libm `sin`/`cos`. Unicorn also emulates them with the host libm, so the oracle cannot certify the
last bit against real x87 hardware for those two instructions; after rounding to float, differences
are expected to be vanishingly rare. Everything else is plain IEEE arithmetic and verified exactly.

## Catalogue corrections found while porting

- `0x00A52450`, `0x00AA39A0`: receiver is `C3DKeyframe` (reads Rotation +0x00, Position +0x10,
  Scaling +0x20), not `CQuaternion`.
- `0x00987BF0`, `0x00ADDFE0`: receiver is a 3×4 row-major `CPreTransposedBoneMatrix`, not a
  quaternion multiply.
- `0x00A9D480`, `0x00A9D580` (`CMatrix3x4::PostScale`): element indices 0-2/4-6/8-10 and the
  preserved column 3 show a 3×4 rows-of-four matrix, not the rows-of-three `CMatrix3x4`.
- `0x00C1CF20` (`CMatrix3x4::operator*` → `C3DVector`): in-place matrix product, rhs in EDX,
  no stack arguments.
- `0x00A14440`/`0x00A14480` (`C3DVector::GetScaled`) are in-place normalisations;
  `0x00A14510` (`C2DVector::Dot`) normalises; `0x00A14540` (`C2DVector::Normalise`) is the fast variant.
- `0x006AD220` (`C3DVector::GetAccurateMagnitude`) reads a vector at `this+0x1B4` of a larger object
  and calls game logic — not a `C3DVector` method (not ported here).
- Missing from the catalogue: `0x00A42280`, `0x00A42370`, `0x00A42430` (CPlane), `0x00A56C40`,
  `0x00A56C60` (C2DLineF), `0x00A144C0` (2D clamp helper).
- `0x00A56BE0` (`C2DLineF::GetHighestY`) returns the lowest Y.
