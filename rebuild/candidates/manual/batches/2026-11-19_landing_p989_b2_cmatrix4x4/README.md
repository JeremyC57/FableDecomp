# p989 b2 (2026-11-19, Account B): the CMatrix4x4 x87 column-order class. 3 new rows, 2 re-ports and one header opt-in, all RELOCATION_MATCH + test PASS, independently audited

## The finding
VC7.1 orders the four x87 products of each element in AlignedSetMultiply by a count of the function's live symbols, with period 4.
- The out-of-line 0x00ADEFC0 AlignedPreMultiplyTransposed has no other code around it. Retail's order there is exactly the order Ego_d.pdb's own AlignedSetMultiply text gives, so that text is the shared body, not a per-row fit:
  - params `lhs`, `rhs`;
  - SSE `const float* src1`, `src2` and `float* dest`, declared in retail's store order;
  - x87 `CMatrix4x4& m = *this` with `long c1` / `long c2`.
- With that body, every user written in its own Ego_d locals matches.

## Header
- engine/CMatrix4x4AlignedSSE.h: new nested opt-in FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD, inside ..._ACCESSORS, with #error pins. The pins were checked by reading only, because VC7.1 under Wine hangs on #error.
- engine/CMatrix4x4.h: the AlignedPreMultiplyTransposed respelling to Ego_d's `matrix` / `m` is under the same opt-in (audit fix), with today's text in the #else.
- The default path is unchanged: hdrcheck2 reports every macro same, and the 113 landed sources and tests that mention CMatrix4x4 preprocess identically.
- The six other landed expanders recheck RELOCATION_MATCH on the installed headers: 0x00A5CB90 (/Zi), 0x00A9D480, 0x00A9D580, 0x00A9D750, 0x00AA0FA0, 0x00AA2110.

## Rows
| Row | Kind | Notes |
|---|---|---|
| 0x00ADEFC0 CMatrix4x4::AlignedPreMultiplyTransposed | new (was parked) | FableWin 0x02E8E0F0 + the Ego_d public and record; Ego_d locals `matrix`, `m`. The class calibrator |
| 0x00AE1320 CParticleProgram::ClingToMesh | new (hand-off) | Ego_r 0x00BED000 + FableWin 0x03337C60; Ego_d locals. The audit strengthened the test: 18/18 non-equivalent mutants killed |
| 0x00AE29D0 CParticleProgram::SetupSimulation | new (hand-off) | Ego_r 0x00BEEBC0 + FableWin 0x03335000. Its complete Ego_d record (the second copy of the name) gives param `dest`; audit fix, nothing inferred |
| 0x00A9E1E0 C3DAnimationManager::ApplyParentTransformations | RE-PORT (claimed) | Ego_d locals. `// Inferred:` (audit fix: was `// Fitted:`): both branches call through the AlignedSetMultiplyTransposed forwarder. The measured alternatives are in the source; no natural spelling matches. Purity drops from retail_asm to genuine (audit REVIEW: purity.py does not see the helper through the forwarder) |
| 0x00BD2D90 CEngineInternalPrimitiveAnimatedMesh::GetBoneTransformations | RE-PORT (claimed) | exactly Ego_d's locals (render_info, no_bones, bone_morphs, interpolate, mesh, c_bone, bone_transform); the landed text's locals were fitted |

The re-ports went through repair_remove.py and were re-landed under their landed file names and leaves. Before removal they rechecked RELOCATION_MATCH under strict headers.

## Not in this batch
- 0x00989230 is prepared separately (pending/2026-11-19_acctB_snapshot/p989_b3) and waits on Account A's FableWin callee lists.
- 0x00988E80 is escalated.

## Names
NAME_FIXES.tsv: 0x013D2880 g_SSEAvailable -> GEnableSIMD (Ego_d public; already bound to both names). It also lists five stale caller bindings the new rows expose: 0x00AE1320, 0x00AE29D0, 0x00AE2510, 0x00ADF610 and 0x00A5BEF0. All are for Account A.
