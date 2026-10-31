# Wave 2 section 4b retry sweep: notes

Rows: 8 attempted, 2 matched (008bc1a0, 008c6db0), 6 improved (008d5aa0, 008dcc80, 008de7a0, 008de080, 008e8060,
008dd0f0), 0 held unchanged. Per-row grade history and residue are in STATUS.tsv and in each differ/ file's header.

## Per-lever win counts (graded improvements)
| lever | wins | rows |
|---|---|---|
| block-scoped locals and temps so VC7.1 shares stack slots | 5 | 008c6db0, 008de7a0, 008de080, 008e8060, 008dcc80 |
| same-TU `CDefClassBase::GetSubDef<T>` body + shared engine/CDefPointer.h | 2 | 008c6db0 (match), 008dcc80 |
| C3DVector ctor-form operators, operand order, dot-product grouping | 3 | 008c6db0, 008de080 (x2) |
| GetInterface shape (null-initialised result, bool + out, bitset::test form, key as a temporary) | 4 | 008d5aa0 (x2), 008de080 (x2) |
| repeat an expression instead of naming a local (VC7.1 CSEs it: fpatan, (float)fps) | 2 | 008de080, 008de7a0 |
| pointer local held in a callee-saved register (physics, ParentThing) | 2 | 008de080, 008dcc80 |
| accumulated `lengthSquared = X*X; += Z*Z; += Y*Y` (VC7.1 reorders a single sum) | 1 | 008de080 |
| reuse one float variable (slot order) | 1 | 008de080 |
| user copy ctor on a by-value functor (built in place, no rep movsd) | 1 | 008de7a0 |
| `std::partition` as a __forceinline wrapper over the fastcall `__partition` | 1 | 008de7a0 |
| `vector::operator[]` through `*(begin() + n)` | 1 | 008de7a0 |
| IS_THING_ALIVE nested-if template | 1 | 008d5aa0 |
| extern epsilon global (not a folded literal) | 1 | 008c6db0 |
| move onto shared headers (CDefClassBase/CDefPointer) | 1 | 008bc1a0 |

## Descriptive (unproven) names used
- Globals: g_FireMissileWeaponFrame_013bafd0 (write-only frame stamp), g_epsilon_0129ba3c (1e-4f).
- Members: CTCTrapBase::State (+0x5C), CTCShotDefinitionMultiArrow::FirstArrow, CTCPhysicsBase::Radius (+0x34),
  CThing::ThingType (+0x10), CAnimFrameInfo::Frame / Offset.
- Virtual slots (CTCPhysicsBase): SetPosition (30), SetPositionImmediately (31), SetRHSet (32), SetAngleXY (66);
  GetFacingVector (70) follows the landed rows.
- Types / helpers: CIsOutsideIcicles (partition predicate), NULL_DEF (header placeholder enumerator),
  GetObstructionDriver, GetAngleXY, WrapAngle, C3DVector::FastNormalise, CCardinalSpline::GetNumberOfControlPoints;
  AddCombinationAnim / CAnimInit::Init / GetHelperPoint parameter names.
- Inferred (one source): CTCTrapBase::IsOpen 0x00787C1B (see NAME_FIXES.tsv).

## Extent fix
008dd0f0 had no catalog length: 1513 bytes (ret at 0x008DD6D8, int3 pad to the next start 0x008DD6E0).
BOUNDS_FIX.tsv + oracle_extra.tsv carry the row; it was graded over the true extent.

## Tooling
fa.sh <addr> <file>: compile with /FAs into tmp/<base>.asm through an explicit z:\ path (g.sh cds into the repo, so a
bare /FAs once wrote a stray .asm into the checkout; it was moved out at once and fa.sh checks for strays).
