# Probe history

The chronological records below distinguish source changes, compile failures, and repeat checks. The instruction count is a diagnostic alignment. A zero masked-byte residue and an independent operand audit determine a match.

| Probe | Outcome | Compiled bytes | Aligned instructions | Experiment |
|---|---|---:|---:|---|
| baseline | LEAF_REVIEW |  |  | Compile the supplied draft on the unmodified package headers. |
| reconstructed | DIFFER | 1815 | 315 | reconstructed |
| operators_0 | DIFFER | 1815 | 315 | operators 0 |
| operators_1 | DIFFER | 1817 | 315 | operators 1 |
| operators_2 | DIFFER | 1824 | 315 | operators 2 |
| operators_3 | DIFFER | 1851 | 303 | operators 3 |
| operators_4 | DIFFER | 1815 | 315 | operators 4 |
| operators_5 | DIFFER | 1862 | 298 | operators 5 |
| operators_6 | DIFFER | 1862 | 298 | operators 6 |
| operators_7 | DIFFER | 1862 | 298 | operators 7 |
| operators_8 | DIFFER | 1926 | 296 | operators 8 |
| native_float_constants | DIFFER | 1824 | 539 | Use single-precision constants at the native x87 sites. |
| best_distance_order | DIFFER | 1824 | 543 | best distance order |
| avoidance_distance_order | DIFFER | 1824 | 549 | Reorder the distance sum at the first divergent x87 operation. |
| donor_result_ADD | DIFFER | 1833 | 501 | donor result ADD |
| donor_result_SUB | DIFFER | 1826 | 548 | donor result SUB |
| donor_result_MUL | DIFFER | 1860 | 489 | donor result MUL |
| radii_order | DIFFER | 1824 | 551 | radii order |
| donor_kinematic | MASKED_EQUAL | 1824 | 552 | Use the same GetVector/SetVector sequence as the original CKinematic assignment. |
| review_1 | MASKED_EQUAL | 1824 | 552 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| handback_d558d19cdc_fa3d8b4 | MASKED_EQUAL | 1824 | 552 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
