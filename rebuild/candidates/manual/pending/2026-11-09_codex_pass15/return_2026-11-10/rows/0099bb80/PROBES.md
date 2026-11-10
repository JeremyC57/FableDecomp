# 0099bb80 attempts

Selected008: relocation match,197/197, all seven calls audited; VC7.1 and native64 g++ tests PASS.

| Trial | Result | Size | Hunks |
|---|---|---|---|
| 001_shared_string_vector | DIFFER | 127/197 | 10 |
| 002_inline_budget200 | DIFFER | 194/197 | 4 |
| 003_refcount_local_data | DIFFER | 199/197 | 2 |
| 004_named_fill_value | DIFFER | 199/197 | 2 |
| 005_explicit_fill_value | DIFFER | 199/197 | 2 |
| 006_assign_size_at_resize | DIFFER | 199/197 | 2 |
| 007_unsigned_size_signed_comparison | DIFFER | 199/197 | 2 |
| 008_reference_length_temporary | MASKED_MATCH_PENDING_AUDIT | 197/197 | 0 |
| 009_named_default_allocator | DIFFER | 199/197 | 2 |
| 010_zero_before_length | DIFFER | 199/197 | 4 |
| 011_size_initialized_to_growth | DIFFER | 199/197 | 2 |
| 012_vector_resize_via_insert | COMPILE_FAIL | -/- | - |
| 013_final_annotated | MASKED_MATCH_PENDING_AUDIT | 197/197 | 0 |

001 rebased to real shared bodies. Its blank Flags line exposed a grader parser bug that consumed the next line as ignored compiler inputs; fixed before002.002 is one inline-budget experiment, not another source shape.003 uses the real data pointer local for refcounting.004/005 change the fill argument lifetime,006 combines length assignment with resize,007 tests unsigned length with signed comparison,008 binds length to a const reference,009 names the allocator,010 constructs the fill value before measuring length,011 accumulates size,012 fails because the reduced shared header lacks the three-argument insert overload. These are not all successful distinct behavioral implementations. No extra attempts are required after a validated match.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
