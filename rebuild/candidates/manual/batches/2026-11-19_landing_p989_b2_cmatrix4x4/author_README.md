# p989 batch b2 -- the Ego_d AlignedSetMultiply body (header item + 5 rows)

Supersedes b1 (option (a), the FITTED rhs copy). Claims posted by the coordinator for 0x00A9E1E0, 0x00BD2D90 and 0x00ADEFC0.

## Contents
| path | lands as |
|---|---|
| hdr/engine/CMatrix4x4AlignedSSE.h | rebuild/include/engine/CMatrix4x4AlignedSSE.h (today's copy + nested opt-in `FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD`) |
| hdr/engine/CMatrix4x4.h | rebuild/include/engine/CMatrix4x4.h (today's copy; AlignedPreMultiplyTransposed inline text respelled to Ego_d `matrix` / `m`) |
| src/CMatrix4x4_AlignedPreMultiplyTransposed_00adefc0.cpp | NEW row (codex pass24 parked, DIFFER 472v471 there) |
| src/C3DAnimationManager_ApplyParentTransformations_00a9e1e0.cpp | RE-PORT of the landed file (same name, same leaf) |
| src/CEngineInternalPrimitiveAnimatedMesh_GetBoneTransformations_00bd2d90.cpp | RE-PORT of the landed file (same name, same leaf) |
| src/CParticleProgram_ClingToMesh_00ae1320.cpp | NEW row (hand748 / hand3c parked) |
| src/CParticleProgram_SetupSimulation_00ae29d0.cpp | NEW row (hand748 / hand3c parked) |
| test/<stem>_test.cpp + <stem>.mutants.py | rebuild/tests/00/xx/<stem>_test.cpp (mutants: one-decision edits, each must FAIL) |
| payload.json, oracle.tsv | mkpayload-style (tools/mkpayload_b2.py; target_source / target_test per row) |
| NAME_FIXES.tsv | 0x013D2880 `g_SSEAvailable` -> `GEnableSIMD` (Ego_d.pdb); 29d0 context slot names |

0x00989230 (the escalated row) is NOT in this batch: under the Ego_d body it needs one more unit (see below, ESCALATE_00989230.md).

## Grades (official, ATTEMPTS.log 20:35-20:36, FABLE_HDR_OVERLAY=b2/hdr)
All five RELOCATION_MATCH (0x00ADEFC0 through tools/qw.py = q.py plus the Fable.exe bytes in W/oracle, since no repo oracle has that address).
Tests: 00adefc0 PASS (3/3 mutants FAIL; real header, both arms + FABLE_DUAL_PATH_TEST twin; also PASS with FABLE_PORTABLE forced under VC7.1),
00a9e1e0 PASS (6/6), 00bd2d90 PASS (7/7), 00ae1320 PASS (7/7), 00ae29d0 PASS (7/7).

## Every landed expander, regraded under b2/hdr (tools/regress.sh; these do not opt in and keep their bytes)
0x00A5CB90 (/Zi), 0x00A9D480, 0x00A9D580, 0x00A9D750, 0x00AA0FA0, 0x00AA2110, 0x00A9E1E0 (landed text), 0x00BD2D90 (landed text): all RELOCATION_MATCH.
(grep of rebuild/src for AlignedSetMultiply / AlignedSetMultiplyTransposed / AlignedPreMultiplyTransposed / ALIGNEDSETMULTIPLY / FABLE_RETAIL_CMATRIX4X4.)
After landing, the two re-ports replace the landed a9e1 / bd2d texts; 0x00AA0FA0 / 0x00AA2110 stay on the ACCESSORS default (not probed under the new body).

## Why this body (evidence, not a fit)
- Ego_d.pdb record of CMatrix4x4::AlignedSetMultiply (debug length 567): params `lhs`, `rhs`; SSE scope `const float* src1`, `float* dest`,
  `const float* src2`; x87 scope `CMatrix4x4& m` { `long c1` { `long c2` } }. The opt-in is exactly that text (pointers declared in retail's
  store order src1, src2, dest; `&lhs.E11` as the landed AlignedPreScale bodies spell row pointers).
- Calibrator: 0x00ADEFC0 is the out-of-line AlignedPreMultiplyTransposed -- the multiply with no other code around it. A unit sweep on it
  (probes/ef/u_*) shows the clean period-4 phase on all four x87 columns; retail = ACCESSORS + 1 = this body exactly, and its byte gap
  (472 v 471 in codex pass24) was only x87 term encodings.
- With it, each caller matches when written with its OWN Ego_d locals (no fitted local anywhere except the a9e1 call spelling below).

## Ego_d evidence per local, and every FITTED / Inferred line
| row | locals (Ego_d record) | fitted / inferred |
|---|---|---|
| 0x00ADEFC0 | `matrix` (param), `m` (CMatrix4x4) -- record length 70, Engine_Primitive_Manager_Animated_Meshes.obj | none |
| 0x00AE1320 | sim, anim, anim_manager, bone_morphs, mesh; no_bones, mesh_vertices, no_vertices, bone_transforms; {c_bone} (length 234) | none. No copy local -> the product is the inline AlignedPreMultiplyTransposed (an Ego_d proc; retail out-of-line copy 0x00ADEFC0). GetBoneCount() vs .BoneCount: same bytes, descriptive choice |
| 0x00AE29D0 | Ego_d record of this overload cut short | **Inferred**: text follows twin 0x00AE1320's record (same names, same PMT loop); constraint args by CParticleContext Ego_d slot names |
| 0x00BD2D90 | render_info; {no_bones, bone_morphs, interpolate, mesh {c_bone {bone_transform}}} (length 592) | none (the landed fitted locals mesh_pointer / animation_manager / transform are gone). render_info = GetRenderInfo() (Ego_d proc; &RenderInfo gives the same bytes) |
| 0x00A9E1E0 | src, dest, mesh, transpose; no_bones, local_matrix; 2 x {c_bone {parent_index}} (length 1077) | **Fitted**: the row-vector branch is spelled `dest[c_bone].AlignedSetMultiplyTransposed(dest[parent_index], local_matrix)` (same product and SSE operands as `AlignedSetMultiply(local_matrix, dest[parent_index])`); the plain call leaves x87 column 0 of that loop off. t1n1 (transposed branch plain, normal branch Transposed) also matches. Every matching site of this class goes through the Transposed forwarder, so a header-side effect on column 0 is not ruled out. |

Inline budget: FORCEINLINE on AlignedSetMultiplyTransposed / AlignedPreMultiplyTransposed are the existing nested opt-ins (landed bd2d / a9e1); 1320 / 29d0 / adefc0 use them the same way.

## Unit table (mod 4 x87 column phase; measured, see NOTES.md)
| construct | units |
|---|---|
| live `if (x & K) x |= M;` | +1 each |
| bare store | +2 |
| flags written as one statement (vs two) | +3 |
| Ego_d body vs ACCESSORS body | +1 (0x00ADEFC0, 1320, 29d0, row) |
| Ego_d body + named rhs copy (old fitted M_RHSREF) | +2 |
| std::min(range.Count, 4ul) in the row | +1 (but +10 bytes: address select) |
| if-else, call, loop, `&&`, dead asserts, dead `hr` locals, casts, ++/post-inc, data-arg spellings, SSE-arm spellings | 0 |
| forwarder choice (Transposed vs plain) | column 0 only |

## Open
- 0x00989230 under the Ego_d body: 635 v 636, ONLY x87 differs; needs exactly +1 unit at zero bytes (probes/ru/u1 = retail). Candidates
  tried in probes/rv, rv2 (see ESCALATE_00989230.md). Fallback (coordinator / advisor call): the old M_RHSREF copy as a FITTED, row-only opt-in.
- Lane limits found: VC7.1 under Wine hangs on any fatal `#error` (the two #error pins in the header were checked by inspection only);
  this host has no 32-bit libc headers, so the gxx -m32 portable steps of portable_twins_check.py did not run here.
