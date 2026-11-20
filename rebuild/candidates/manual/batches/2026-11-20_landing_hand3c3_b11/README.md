# Landing (Account A, 2026-11-20, on the owner's instruction "land the matches here in main chat now")

- Strict vland `--land` (LANDVERIFY_STRICT_HEADERS=1): 0x00B16410 RELOCATION_MATCH PASS (`logs/vland_land.log`), catalog CompilerFlags
  `/O2 /Oy /W3 /d2inlT230`; oracle line read from Fable.exe (1671 bytes, `oracle.tsv`).
- Headers: engine/GFFloatAbs.h and engine/GFIsNonZero.h are new; C2DVector.h and CMatrix3x4.h take B's additions only (0 removed lines
  against today's tree). hdrcheck2 (`logs/hdrcheck.log`): the default path and every existing macro are token-identical; the one changed
  macro, FABLE_RETAIL_CMATRIX3X4_PREROTATE, is new and no landed file uses it.
- One definition: none of the object's 26 defined symbols is bound to another landed row, so there is no owner to link against. Six
  landed rows define GFFloatAbs (one also GFIsNonZero) locally as `inline` (pick-any, no LNK2005): 0x006E3440, 0x006823D0, 0x006D1250,
  0x00AC8C80, 0x00AC7200, 0x007D6870. Queued as a shared-header port onto the new headers.
- B's NAME_FIXES.tsv is the cumulative hand3c3 list, already in the queue.

# b11 (hand3c3, Account B, 2026-11-20): NParticleEngine::CPSCOrbit::COrbitState::Update -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00B16410 | NParticleEngine::CPSCOrbit::COrbitState::Update(const COrbitSettings&, CMatrix3x4&, float) | 1671 | RELOCATION_MATCH (`// Flags: /d2inlT230`) | PASS (60,000 cases) | 307 / 309 (2 equivalent) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1): `RELOCATION_MATCH PASS`; one RELOC_NAME warning, a second catalog spelling of
0x0129BA3C (`?g_NormaliseEpsilon@@3MB`; the PDB name is ?DELTA_NOTIONAL_ZERO@@3MB, see NAME_FIXES.tsv). Purity: retail_asm
through the shared engine/GFFloatToLongNear.h only (inside GFFastCos / GFFastSin; `// Retail-asm:` line with the eight fld / fistp
sites; no asm in the row). Grades: 5 (plain inline without the flag 842v1671; the port 1669v1671; the match; two comment-only finals).
No local class model: the row is the COrbitState of engine/NParticleEngine_CPSCOrbit.h (Update was already declared there).

## What the parked diagnosis missed
The parked draft (1674v1671, "register colouring / frame-slot packing") modelled every class locally, wrote the conditions as
GFFloatAbs compares and gave the matrix its own __forceinline rotations. On the shared headers the function is the Ego_d line
table statement for statement and the residue is gone.

## Source shape
Ego_d.pdb parameters / locals and line table (lines 54..143; egod_lines/), one statement per entry:
- the three "non zero" tests are 26-byte lines = one bool call each: `GFIsNonZero(settings.SqueezeAngle)` etc. (FableWin
  ?GFIsNonZero@@YI_NM@Z 0x02F50800 sits next to ?PreXRotate@CMatrix3x4@@QAEXM@Z 0x02F50A10: this TU's COMDAT cluster);
- the squeeze block's four locals in line order temp_x, temp_y, cos_a, sin_a (29 / 41 / 20 / 20 bytes), then two 17-byte sums;
- `next_pos_2d.Clear()` (8 bytes), `long orient = settings.Type & 3` (11), three `NextPosition.Initialise(..)` cases (31 each),
  `if (settings.Type & 4)` (10), TransformPoint (15) / `NextPosition += parent_transform.GetTranslation()` (24), three
  `Pre?Rotate(-NextAngle)` cases (19 each; the angle is in revolutions), SetTranslateOnly (15), the tangent shift.
- COrbitSettings::Type is ONE long in both PDBs (the draft's bitfield is not in the TPI): mask 3 = plane, bit 2 = whole matrix.
- `// Inferred:` (1) `/d2inlT230`: with the default threshold VC7.1 calls the plain-inline shared helpers (842 bytes); retail
  expands all of them. Window: T200 1601, T212 1544, T225 / T228 / T229 1614 bytes; T230, T231, T234, T237, T2000, T9999 are the
  same matching object (the text before the TransformPoint choice was also one object from T250 to T800).
  (2) TransformPoint in its ZYX_SUMS text: the default parenthesised text gives the same TransformPoint instructions here but
  flips two commutative sums inside the PreYRotate expansion (0x00B16967 / 0x00B1698B; 1669 bytes, 66 instructions off). Eight
  rewrites of PreYRotate itself (operand order, `a = b*s + a`, `a = a*c`, ..) are inert: c2 canonicalises them.
  (3) no `break` after the last rotate case (Ego_d line 137 is followed by line 139).

## Header overlay (hdr/, hdr.diff; strictly additive: 0 removed lines; copied from today's rebuild/include)
- engine/CMatrix3x4.h: `FABLE_RETAIL_CMATRIX3X4_PREROTATE` -- ?PreXRotate / ?PreYRotate / ?PreZRotate@CMatrix3x4@@QAEXM@Z as inline
  bodies (and the GFSin / GFCos includes under the same opt-in). Text from the Ego_d line tables (egod_lines/: two 15-byte call
  lines, then five lines of 12 / 15 / 21 / 15 / 15 bytes per column) and locals (`const float angle`, `const float sinA`,
  `float tmp`, `const float cosA`). FITTED: cosA declared before sinA (sinA first spills the cosine: 1726 bytes).
  Needs FABLE_RETAIL_MATH_TRIG_TWO_PI for retail's double scale 0x0124F2B8.
- engine/C2DVector.h: `FABLE_RETAIL_C2DVECTOR_CLEAR` -- ?Clear@C2DVector@@QAEXXZ, `{ X=0.0f; Y=0.0f; }` (Ego_d: one 26-byte line =
  two separate stores).
- NEW engine/GFFloatAbs.h -- ?GFFloatAbs@@YIMM@Z (FableWin 0x01F43D00; Ego_d parameter `n`, lines 3 / 11 / 3 / 4).
- NEW engine/GFIsNonZero.h -- ?GFIsNonZero@@YI_NM@Z (FableWin 0x02F50800; `GFFloatAbs(n) > DELTA_NOTIONAL_ZERO`).
The two new files define functions, not classes, so the strict shared-header check flags no landed row. Six landed rows carry
their own copy of GFFloatAbs (0x006E3440, 0x006823D0, 0x006D1250, 0x00AC8C80, 0x00AC7200, 0x007D6870; 0x00AC7200 also GFIsNonZero)
and can move to the headers when they are next touched. NParticleEngine::GetOneOverTickRate 0x00AEAA00 is declared in the row,
as in the landed rows of this TU (0x00B16D50, 0x00B18820, 0x00B1B0C0, 0x00B1F200): there is no NParticleEngine free-function header.

## Test (test/00b16410.cpp)
Links the real row object. No class: the row is entered through a linker alias and works on raw storage (17-float state, 12-dword
settings, 12-float matrix, each between guard words); GFastCosTable, DELTA_NOTIONAL_ZERO, TWO_PI and GetOneOverTickRate (counted)
are defined by the test under their retail names. An independent model in double arithmetic (its own table lookup, its own
rotations) gives the expected state and matrix: copied fields must be bit-equal, computed floats within 2e-6 of the case's
magnitude bound, the matrix translation bit-equal to the new point, settings / guards unchanged. A third of the cases use the
threshold values of the three tests (0, +-1e-4 exactly, one float above / below, half), a third a small world (unit radius) so a
wrong branch at a threshold shows; the stack under the call is filled with large floats first (a disabled orbit must give a
zero point). The run prints its path counters (disabled 9960, squeeze 27893, plain 22147, no expand 17672, no cycle time 8035,
cycle 11851, clamp 9122, no clamp 3360, planes 14705 / 15148 / 14959 / 15188, whole matrix 30126, translate 29874) and fails if
one is zero. Note for other float rows: VC7.1 does not round a `(float)` cast kept in a register, so the model rounds through
memory (Single()); without that 881 cases with |CycleTime| one float above 1e-4 failed falsely.

## Mutants (mutants/)
309 one-decision mutants (176 of the row text, 133 of the new header bodies): 307 KILLED. The two survivors are equivalent:
- `GFFastCos(-settings.SqueezeAngle)` -> `GFFastCos(settings.SqueezeAngle)`: the cosine is even (the table lookup of the
  mirrored angle agrees to float rounding);
- GFFloatAbs mask `0x7ffffffe`: the result only feeds `> DELTA_NOTIONAL_ZERO` (bits 0x38D1B717, odd), and clearing bit 0 cannot
  move a value across an odd bound.
