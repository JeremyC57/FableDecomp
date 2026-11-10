# 00b801f0 current attempts

COMPLETE: selected031_inline_budget_185 is a verified relocation match. Each history directory contains the exact source/header snapshot and compiler result. Older package attempts remain untouched.

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
| 001_shared_containers | DIFFER | 304/523 | 22 |
| 002_inline_budget_200 | DIFFER | 504/523 | 27 |
| 002_inline_budget_500 | DIFFER | 806/523 | 26 |
| 003_real_random_access_copy | DIFFER | 518/523 | 4 |
| 004_original_pod_tag | DIFFER | 518/523 | 4 |
| 005_overflow_external_instantiation | DIFFER | 523/523 | 2 |
| 006_bind_copy_temporary | DIFFER | 523/523 | 2 |
| 007_direct_initialise_copy | DIFFER | 523/523 | 2 |
| 008_fill_value_parenthesis | DIFFER | 523/523 | 2 |
| 009_fill_separate_increment | DIFFER | 523/523 | 2 |
| 010_fill_reference_destination | DIFFER | 523/523 | 2 |
| 011_default_then_assign_copy | DIFFER | 523/523 | 2 |
| 012_guarded_do_fill | DIFFER | 523/523 | 2 |
| 013_postincrement_fill | DIFFER | 523/523 | 2 |
| 014_const_reference_iterator_end | DIFFER | 523/523 | 2 |

| 015_explicit_member_assignment | DIFFER | 493/523 | 35 |
| 016_explicit_member_copy | DIFFER | 481/523 | 32 |
| 017_explicit_copy_and_assignment | DIFFER | 421/523 | 33 |

Previous selected baseline:005_overflow_external_instantiation (historical, DIFFER).

Retry evidence: 2026-11-10 standalone tests pass under VC7.1 /Od /W3 and g++64 -std=c++03. Production baseline restored.
Explicit element assignment/copy bodies produce 493/481/421 bytes and change the broad loop shape, supporting the current implicit trivial element operations. No new element model retained.

Resume compiler trials (all18 prior calls, including both002 variants, retained):

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
|018_fill_value_temporary|DIFFER|523/523|2|
|019_forceinline_fill|DIFFER|523/523|2|
|020_original_vertex_hierarchy|DIFFER|523/523|2|
|021_fill_copy_expression|DIFFER|523/523|2|
|022_fill_reversed_condition|DIFFER|523/523|4|
|023_fill_first_reference|MASKED_MATCH diagnostic only|523/523|0|
|024_fill_local_iterator|MASKED_MATCH|523/523|0|
|025_inline_budget_100|DIFFER|309/523|22|
|026_inline_budget_150|DIFFER|400/523|24|
|027_inline_budget_175|DIFFER|477/523|26|
|028_inline_budget_187|MASKED_MATCH|523/523|0|
|029_inline_budget_181|DIFFER|477/523|26|
|030_inline_budget_184|DIFFER|477/523|26|
|031_inline_budget_185|RELOCATION_MATCH|523/523|0|

Selected031:32 normal candidate compiler calls total. The020 preparation script initially found an existing empty directory and stopped before compilation; fixed before its single actual grade. Scheduler proxy replays are diagnostics and not extra candidate grades.

018 copy temporary at the last fill call and021 per-iteration copy expression are behavior-equivalent for this trivial element but forward back to the same load order.019 forced fill expansion does not help; no forced-inline helper retained.020 restores the genuine two-base element hierarchy from both raw donor PDBs; bytes remain the same but the owning model is now correct.022 reverses the pointer comparison and adds two comparison differences.023 shows first-iterator reference binding can produce the native order, but changes the known STLport function signature, so is excluded from selection.024 achieves the same match through the original by-value signature with a local iterator cursor, preserving the std::fill contract.

The inline budget controls actual helper expansion, not scheduling:100 gives309 bytes;150 gives400;175/181/184 give477 with helper calls;185/187/200 give523 and all193 instructions match. Adjacent184/185 establish the smallest matching inline threshold in the measured expansion interval. INLINE_BUDGET_SEARCH.json records the boundary; the source carries the required marker.

Final complete extent523 bytes,193 instructions,zero strict/loose hunks. AUDIT_FINAL verifies the sole external relocation to real protected std::vector<CTVertexSeaBackground>::_M_insert_overflow00b80400, complete raw donor owning types, and unchanged preprocessed default/prior header branches. The fresh standalone fixture pastes the production header tokens (comments removed) and passes VC7.1 /Od /W3 plus64-bit g++ C++03 -O2 ASan/UBSan. Existing insertion/alias/overflow behavior cases retained. Normal RTM and diagnostic proxy leaf bytes agree; final load-preheader is X,Y,Z. All historical attempts retained.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
