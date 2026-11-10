# 0074e140 current attempts

COMPLETE: selected035_group_zero_sine is a verified relocation match. Each history directory contains the exact source/header snapshot and compiler result. Older package attempts remain untouched.

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
| 001_shared_classes_literal_pitch | COMPILE_FAIL | -/- | - |
| 001b_shared_accessor | DIFFER | 245/245 | 2 |
| 002_actual_adjust_body | DIFFER | 245/245 | 2 |
| 003_group_random_offset | DIFFER | 245/245 | 2 |
| 004_group_yaw_product | DIFFER | 245/245 | 2 |
| 005_reference_cosine | DIFFER | 245/245 | 2 |
| 006_reference_yaw | DIFFER | 245/245 | 2 |
| 007_initialise_vector | DIFFER | 245/245 | 2 |
| 008_copy_ctor_body | DIFFER | 245/245 | 2 |
| 009_copy_ctor_initialise | DIFFER | 245/245 | 2 |
| 010_inline_comdat_adjust | DIFFER | 245/245 | 2 |
| 011_named_return_reference | DIFFER | 245/245 | 2 |
| 012_copy_ctor_named_components | DIFFER | 245/245 | 2 |
| 013_copy_x_group | DIFFER | 245/245 | 2 |
| 014_copy_x_float_cast | DIFFER | 245/245 | 2 |
| 015_copy_x_unary_plus | DIFFER | 245/245 | 2 |
| 016_copy_return_temporary | DIFFER | 245/245 | 2 |

| 017_copy_yxz | DIFFER | 245/245 | 3 |
| 018_copy_yzx | DIFFER | 245/245 | 2 |
| 019_copy_zxy | DIFFER | 245/245 | 2 |
| 020_return_component_constructor | DIFFER | 245/245 | 2 |
| 021_copy_assignment | DIFFER | 247/245 | 3 |
| 022_copy_array_loop | DIFFER | 249/245 | 7 |
| 023_return_assigned_result | DIFFER | 267/245 | 20 |
| 024_return_scalar_components | DIFFER | 245/245 | 2 |
| 025_parenthesised_copy_assignment | DIFFER | 245/245 | 2 |
| 026_grouped_double_negative_copy | DIFFER | 249/245 | 3 |
| 027_grouped_unit_product_copy | DIFFER | 245/245 | 2 |
| 028_copy_initialise_components | DIFFER | 245/245 | 2 |

Previous selected baseline:001b_shared_accessor (historical, DIFFER).

Retry evidence: 2026-11-10 standalone tests pass under VC7.1 /Od /W3 and g++64 -std=c++03. Production baseline restored.
Copy-body reordering changes which first component is loaded through x87, but does not solve the load/store interleave. Explicit component return and scalar intermediates forward back to baseline. Array-loop diagnostic discarded (not a retained portable source shape); generated assignment and return-through-assignment worsen code size.

Resume compiler trials (all prior001..028 and001b evidence retained):

| Trial | Result | Bytes (ours/retail) | Strict hunks |
|---|---|---|---|
| 029_y_comma_x_initializer | DIFFER |245/245|3|
| 030_group_angle_products | DIFFER |245/245|2|
| 031_group_constructor_arguments | MASKED_MATCH |245/245|0|
| 032_group_trig_calls | DIFFER |245/245|2|
| 033_group_trig_casts | MASKED_MATCH |245/245|0|
| 034_group_zero_trig | MASKED_MATCH |245/245|0|
| 035_group_zero_sine | RELOCATION_MATCH |245/245|0|

Selected035 differs from baseline by one natural pair of parentheses: `(float)(sin(0.0))`. Seven new distinct normal RTM trials;36 normal compiler calls total including001b and failed001. Scheduler proxy traces are diagnostic replays, not extra candidate trials.031 was reduced through033/034 to035;032 establishes that grouping yaw trig calls alone does not solve the tail.029's comma initializer was a complete copy constructor but worsened the first copied component; never selected.

The035 scheduler trace, asserted byte-identical to the unmodified RTM object, retains two scheduling DAGs. The additional PAREN tuple changes x87/call timing within the second DAG; it does not create a new scheduling region. Baseline pairs the pointer move and X store before Y load.035 picks the Y load before the X store, matching native. No layout, flags, capture model or callee body changes are needed.

Final independent row audit verifies all12 relocation operands (seven float/zero occurrences plus five named external references), complete245-byte extent, exact private const Leaf and all69 instructions. The standalone fixture embeds the exact final method and passes VC7.1 /Od /W3 and64-bit g++ C++03 -O2 with ASan/UBSan. Existing behavioral cases retained. Production source and owning headers frozen; see AUDIT_FINAL.json and SHA256SUMS.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
