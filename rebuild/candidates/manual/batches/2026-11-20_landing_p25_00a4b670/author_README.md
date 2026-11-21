# 0x00A4B670 NBBBMaths::IsPosInFOVWithWeight (p25R, 2026-11-21)

**Grade:** RELOCATION_MATCH (314/314). Strict vland (LANDVERIFY_STRICT_HEADERS=1, empty overlay, fresh work dir): WIN, behaviour PASS,
no warning (`vland.log`). **No header change**, no flag, no pragma, no NAME_FIXES. **One Inferred line** (an existing opt-in).

## What closed it
The parked draft a4_r1 (312v314, `evidence/a4_r1_old_local_model.cpp`) was an old local model with its own `__asm` float-to-long. On the
shared headers the body matches at the first grade:
- **GFFastCos** comes from engine/GFFastCos.h. The bare `fistp dword` is the shared retail-asm engine/GFFloatToLongNear.h, so the row
  carries no asm.
- **The body** is C3DVector `operator-`, Normalise (GFFastOneOverSqrt) and the default `Dot`.
- **Names:** Ego_d's parameter names (origin_pos, view_direction, view_angle, test_pos, weight) and locals (cos_half_fov, to_test_pos,
  cos_angle_between) replace the draft's made-up ones.
- **Ego_d lines** (`egod_lines/00a4b670.odcmp_final.txt`, which is off by one from line 251):

  | Ego_d line | statement | size |
  |---|---|---|
  | 252 | cos_half_fov | 27 = 21 + 6 (float return spill) |
  | 253 | to_test_pos | 16 |
  | 254 | Normalise | 8 |
  | 255 | dot | 14 v 15 |
  | 257 / 259 | test / weight | 15 / 20 v 13 / 22 (x87 compare encodings) |
  | 260 | `return true` | 4 |
  | 262 | `weight = 0.0f` | 7 (fldz / fstp) |
  | 263 | `return false` | 2 |

  Line 251 (297 B) emits nothing in retail and has no text.
- **Inferred:** FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARED_ZXY. This is the existing opt-in of row 0x00BAA870: squared length `Z*Z; +=X*X;
  +=Y*Y`, factor spilled. Retail shows exactly that order and the spill (0x00A4B6DE..F0, 0x00A4B743). Every other shared Normalise body
  differs (table below).

## Ablation (sources in `evidence/`)
| change | grade |
|---|---|
| final (NORMALISE_SQUARED_ZXY) | RELOCATION_MATCH |
| default Normalise (`a4_NONE.cpp`) | DIFFER(314v314) |
| NORMALISE_ZXY (`a4_NORMALISE_ZXY.cpp`) | DIFFER(314v314) |
| NORMALISE_NAMED_SQUARED | DIFFER(314v314) |
| NORMALISE_PAREN_SQUARES_XYZ | DIFFER(314v314) |
| NORMALISE_LOCALS | DIFFER(312v314) |

## Tests
- `test/00a4b670.cpp` links the REAL row object.
- **Fixtures.** The test defines GFastCosTable (cos of k/1024 turns) and GFastOneOverSqrtTable. The second is built from the seed formula's
  exponent / mantissa split.
- **Checks.** 8 cases against exact double math:
  - 90° / 45° / 180° / 0.3- / 0.1-turn cones;
  - inside and outside;
  - offset origins;
  - weight = (dot - cos) / (1 - cos) within 2e-3, and 0 outside;
  - inputs untouched.
- **Mutants:** 11/11 KILLED (`mutants/00a4b670.out`). There are 10 in the source and 1 in the opted-in Normalise body (run with
  a copy of C3DVector.h as overlay).
