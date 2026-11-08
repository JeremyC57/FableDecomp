# Local continuation attempts

All attempts below preserve their source and header snapshot in HISTORY. Setup failures, baseline reproductions and comment-only regrades are explicitly included; this is not a count of independent source-shape strategies. Prior Claude sessions are separately preserved under PRIOR_HANDOFFS. Historical source comments and old summaries are not current status.

| Tag | Status | Compiled bytes | Outcome |
|---|---|---:|---|
| baseline | DIFFER | 74965 | Original baseline or initial tool-setup check; inspect status before counting as a compiled strategy. |
| slide_named_values | DIFFER | 74941 | Names fraction, offset, position and facing; saves bytes but still wrong x87 spill/order. |
| slide_vector_sum_order | DIFFER | 74965 | Commuted vector sum, no useful change. |
| prefix_nested_guards | DIFFER | 74911 | Removes conditional-cleanup machinery native retains; rejected despite smaller output. |
| prefix_named_condition | DIFFER | 74965 | Named conjunction, same selected bytes as baseline. |
| prefix_conditional_operator | DIFFER | 74967 | Conditional expression adds bytes; not selected. |
| scale_stored | DIFFER | 74965 | Operator* stored-result header shape does not reproduce native x87 allocation. |
| prefix_continue_combined | DIFFER | 74965 | Equivalent continue guard, no useful local correction. |
| slide_single_factor | DIFFER | 74933 | Named conversion removes duplicated x87 load but misses native Y spill; alternative only. |
| slide_compound_vector | DIFFER | 74965 | Compound vector expression, no useful correction. |
| prefix_equal_true | DIFFER | 74965 | Explicit ==true canonicalized; no improvement. |
| prefix_positive_length | DIFFER | 74965 | Changes condition shape incorrectly; rejected. |
| slide_scaled_reference | DIFFER | 74969 | Reference to scaled temporary worsens size; not selected. |
| slide_scalar_components | DIFFER | 74901 | Smaller code but differs in intermediate rounding/spill shape; not selected. |
| prefix_inline_named_bool | DIFFER | 74965 | Named bool inside prefix helper canonicalized; no improvement. |
| prefix_inline_returns | DIFFER | 74965 | Return-chain helper shape canonicalized; no improvement. |
| slide_factor_reference | DIFFER | 74941 | Factor reference still has wrong x87 spill/order. |
| slide_named_offset_reference | DIFFER | 74941 | Offset reference still has wrong x87 spill/order. |
| crowd_split_loop_guard | DIFFER | 74965 | Split wait-loop guard canonicalized; no correction. |
| crowd_explicit_exits | DIFFER | 74965 | Explicit exits canonicalized; no correction. |
| vector_ctor_body | DIFFER | 74965 | Constructor assignments in body yield identical caller code. |
| vector_copy_body | DIFFER | 74965 | Copy-constructor assignments in body yield identical caller code. |
| prefix_explicit_string | DIFFER | 74957 | Named prefix removes/changes cleanup shape and perturbs slots; not selected. |
| thing_alive_visible | DIFFER | 74955 | Faithful same-TU pointer/dead-bit helper body recovers pThing slot sharing: frame 0x18e4 -> 0x18e0. |
| current_checkpoint_01 | DIFFER | 74955 | Exact recompilation after source status-comment update; not a new independent strategy. |
| setflag_branch_assignment | DIFFER | 74906 | Duplicate flag assignment in the real if/else arms restores native store timing and exact frame 0x18dc; selected. |
| slide_named_offset_only | DIFFER | 74906 | Names only scaled vector; byte-identical to setflag checkpoint. |
| slide_copy_scale | DIFFER | 74906 | Copy then operator*=; byte-identical to setflag checkpoint. |
| slide_step_reference | DIFFER | 74910 | Alias step by const reference: changes second interpolation site slightly, no full correction; not selected. |
| slide_factor_compound | DIFFER | 74882 | Latest alternate: named float fraction and copied position += step*fraction; 74882 bytes, still DIFFER. Compiler completed before wrap-up; local x87 residue not fully reviewed. Preserved as promising alternate, not selected. |
| current_checkpoint_02 | DIFFER | 74906 | Final selected-source verification after comment updates; not a new independent strategy. |
