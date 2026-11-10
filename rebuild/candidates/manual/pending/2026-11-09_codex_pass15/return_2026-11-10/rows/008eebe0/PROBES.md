# 008eebe0 attempts

Selected:013_group_angular_result. Normal RTM131/131 masked match; all four literal relocations independently PASS. Main re-verification pending.

| Trial | Result | Bytes | Strict hunks |
|---|---|---|---|
| 001_shared_headers | DIFFER | 129/131 | 1 |
| 002_double_magnitude | DIFFER | 133/131 | 2 |
| 003_reuse_magnitude_local | DIFFER | 129/131 | 1 |
| 004_magnitude_reference | DIFFER | 129/131 | 1 |
| 005_float_fabs | DIFFER | 129/131 | 1 |
| 006_magnitude_before_angular | DIFFER | 127/131 | 4 |
| 007_split_quotient | DIFFER | 129/131 | 1 |
| 008_angular_reference | DIFFER | 129/131 | 1 |
| 009_recompute_angular_expression | DIFFER | 127/131 | 4 |
| 010_explicit_ratio_narrowing | DIFFER | 129/131 | 1 |
| 011_negative_scale_product | DIFFER | 133/131 | 3 |
| 012_reversed_compare | DIFFER | 131/131 | 3 |
| 013_group_angular_result | MASKED_MATCH_PENDING_AUDIT | 131/131 | 0 |
| 014_double_absolute_input | DIFFER | 129/131 | 1 |
| 015_double_angular | DIFFER | 125/131 | 1 |
| 016_double_scale | DIFFER | 137/131 | 4 |
| 017_double_aspeed | DIFFER | 133/131 | 2 |
| 018_reference_aspeed | DIFFER | 129/131 | 1 |
| 019_compound_scale_subtract | DIFFER | 129/131 | 1 |
| 020_group_quotient | DIFFER | 129/131 | 1 |
| 021_group_scale_use | DIFFER | 129/131 | 1 |
| 022_assign_angular_quotient | DIFFER | 129/131 | 1 |
| 023_main_independent_regrade | MASKED_MATCH_PENDING_AUDIT | 131/131 | 0 |

The winning edit groups the entire final non-clamped scale: `scale = (1.0f - angular / aspeed);`. Grouping only the quotient or use is inert. No operation, precision, branch, flag, helper or type change is needed.

Actual C2 DAG5 baseline: fxch,fdiv,fsubr,fstp,fmul,fstp,return. Winning shape: fxch,fdiv,fsubr,fxch,PAREN(0x269),fstp,fmul,fstp,return. Thus the grouping inserts a PAREN between the explicit exchange and discard, preventing their coalescing to fstp st(1). All emitted instructions then match retail. The trace proxy is diagnostic only and each diagnostic leaf is checked identical to normal unmodified RTM output.

The self-contained behavior test copies the exact selected function and required shared-header declarations/math bodies. 1344 cases check all clamp/equality/non-clamp combinations of7 velocity vectors,6 strengths,8 signed angular velocities including signed zero, and4 radii. Zero sign and unchanged fields are checked. Native64 g++03 and VC7.1 /Od /W3 PASS.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
