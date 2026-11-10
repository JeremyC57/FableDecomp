# GetPositionMatrixFromRHSet attempts

Prior drafts and their unproven helper were reviewed. All attempts here use the real shared headers.

| Attempt | Shape | Outcome |
|---|---|---|
| 001_shared_matrix_set | Four-vector Set with negated input axes | DIFFER 191/185; 17 strict hunks |
| 002_copy_axes_then_negate | Copy axes into matrix then negate first two rows | DIFFER 213/185; 13 strict hunks |
| 003_direct_axes | Named Up/Forward/Right and direct matrix fields | DIFFER 185/185; 1 strict hunks |
| 004_direct_axes_components | Construct copied axes from individual components | DIFFER 185/185; 2 strict hunks |
| 005_right_const_reference | Bind GetRight result to const reference; direct assignments | BYTE_MATCH_PENDING_AUDIT 185/185; 0 strict hunks |
| 006_negative_right | Negate right before matrix stores | DIFFER 201/185; 10 strict hunks |

Selected005. No callee or global relocations. Behavior checks recorded alongside the selected grading evidence.

## Final handoff authority

Current status: VERIFIED_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
