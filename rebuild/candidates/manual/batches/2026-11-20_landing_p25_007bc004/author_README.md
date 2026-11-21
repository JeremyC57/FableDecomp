# 0x007BC004 NThingShot::CTCShotDefinitionNymphFun::InitMovement (p25R, 2026-11-21)

**Grade:** RELOCATION_MATCH (342/342). Strict vland (LANDVERIFY_STRICT_HEADERS=1, empty overlay, fresh work dir): WIN, behaviour PASS,
no warning (`vland.log`). **No header change** (no overlay), no flag, no Inferred line, no NAME_FIXES.
Catalogue module is `CTCShotDefinitionNymphFun`; line 1 carries the namespace (NThingShot), as the PDB symbol does.

## What closed it
The parked drafts (Q12/Q13 343v342, the local-model r2 342v342 with a 0x50 v 0x5C frame) were rewritten on the recipe of the landed
same-TU sibling `CTCShotDefinitionProjectile::InitMovement` 0x007BBB3B, in Ego_d's statement shape:
- Ego_d line table (`egod_lines/`): 86 ptargetthing (14) / 88 alive (19) / 90 ptc_phys (21) / 91 fire_at_pos (17) / 93 `+= 0.5f` (12) /
  94 height (25) / 96 line_to_target (21) / 97..99 176 B, no retail bytes / 100 RandomiseShotDirection (50) / 101 normalise (8) /
  102 up (27) / 103 normalise (8) / 104 ResetRHSet (34) / 106 speed (36) / 107 applied_acceleration (19) / 108 (12) / 109 (16).
  Locals: ptargetthing, { fire_at_pos, ptc_phys, applied_acceleration, line_to_target, up, speed } -- the source uses exactly these.
- The sibling's own table (`egod_lines/007bbb3b_sibling.lines.txt`) has the same 179-B three-line statement after line_to_target (its
  line 201, an ASSERT the landed row documents as emitting nothing) and the same 50 / 8 / 27 / 8 / 34 sizes for the randomise ..
  ResetRHSet lines: so `GetShotAccuracyPercentage()` and `up = line_to_target.CrossWithUp().Cross(line_to_target)` are the sibling text.
- Speed line 106 (36 B): `PShotDef->Speed / CMainGameComponent::GetConstantFPS()` is 30 B at VC7.1 /Od (+6 = VC10's float spill, the
  calibration in LESSONS); `GetShotDef().Speed` is 27. Both are byte-identical at /O2; the Ego_d-consistent one is kept.
- Defines from the sibling: CRIGHTHANDEDSET_CTOR_INLINE and C3DVECTOR_CROSS_MEMBER_XY_GROUP (+ the TU's `optimize("y", off)` /
  `("s", on)` pragmas that every landed shot-definition row carries).

## Ablation (one change at a time; sources in `evidence/`)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| without C3DVECTOR_CROSS_MEMBER_XY_GROUP (`7b_v1.cpp`) | DIFFER(342v342): `mov eax,[ebp-8]` before `fild ConstantFPS`, `mov ecx,ebx` before `lea/push` (0x007BC115..28) |
| CRIGHTHANDEDSET_CTOR (declaration only) for CTOR_INLINE (`7b_ab1.cpp`) | DIFFER(342v342): frame 0x50 v 0x5C (the set temporary shares the randomise result's slot) |
| `GetShotDef().Speed` (`7b_ab3.cpp`) | RELOCATION_MATCH (same bytes; rejected on Ego_d line 106) |
| `(long)AccuracyPercentage` for GetShotAccuracyPercentage() (`7b_ab5.cpp`) | DIFFER(340v342) |
| no optimize pragmas (`7b_ab4.cpp`) | DIFFER(417v342) |
| the XY_GROUP's job by other no-code tuples: `(GetConstantFPS())`, `(float)GetConstantFPS()`, SCALE_X_PAREN (`7b_a/e/c.cpp`) | DIFFER(342v342) |

## Tests
`test/007bc004.cpp` links the REAL row object (`tools/rowtest.py`; vland behaviour PASS). It defines every import, builds the shot /
parent / physics memory at retail offsets with a fake physics vtable (slots 32 / 38), and checks 11 live cases against a double-precision
reference (height query at target + 0.5, randomise arguments and seed, RH set up / forward, acceleration = forward * speed / fps, call
order, gravity off) plus a dead target (no call at all).
**Mutants:** 14/14 KILLED (`mutants/007bc004.out`).
