# Probe history

The chronological records below distinguish source changes, compile failures, and repeat checks. The instruction count is a diagnostic alignment. A zero masked-byte residue and an independent operand audit determine a match.

| Probe | Outcome | Compiled bytes | Aligned instructions | Experiment |
|---|---|---:|---:|---|
| baseline | DIFFER | 1957 | 461 | Compile the supplied draft on the unmodified package headers. |
| engine_vector | COMPILE_FAIL |  |  | engine vector |
| shared_geometry | DIFFER | 2009 | 486 | shared geometry |
| combat_animation_owner | DIFFER | 2009 | 486 | combat animation owner |
| combat_velocity_dot | DIFFER | 2047 | 377 | combat velocity dot |
| combat_flatten_line | DIFFER | 2047 | 377 | combat flatten line |
| combat_flatten_target | DIFFER | 2047 | 377 | combat flatten target |
| combat_cross | DIFFER | 2047 | 377 | combat cross |
| knock_shared | COMPILE_FAIL |  |  | knock shared |
| knock_all_owners | DIFFER | 2104 | 424 | knock all owners |
| knock_turns | DIFFER | 2104 | 424 | knock turns |
| knock_original_vectors | DIFFER | 2104 | 426 | Recover the source full-vector copies, planar flattening and cross product before selecting a reaction. |
| handback_d558d19cdc_7f52cbd | DIFFER | 2104 | 426 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |

## Continued main-chat grades

All 2,028 bytes and 573 instructions match. The 99 primary operands, 15 mapped inline copies, and the extra 125-byte GetComponent at 00867F60 with four operands and its 60-byte LowerBound owner are verified. The 192-case RTM fixture passed. Direct initialization of the three facing vectors uses the caller-provided return storage. Ego_d names cos_thet and both line_to_target locals. The donor IsNonZero reads Y then X then Z. Visible original GetComponent and Find bodies remove a 4-byte stack slot. Slot 32 (+0x80) is ResetRHSet. The old draft invoked SetRHSet at +0x12c.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_knockdonor_implicit | 2112 | 429 | 15 / 0 |
| round2_knockdonor_joint | 2112 | 433 | 15 / 0 |
| round2_knockdonor_slot | 2104 | 426 | 15 / 0 |
| round2_knockdonor_vectors | 2104 | 430 | 15 / 0 |
| round2_knockdot_accumxyz | 2068 | 406 | 15 / 0 |
| round2_knockdot_bothxyz | 2066 | 408 | 15 / 0 |
| round2_knockdot_bothyxz | 2070 | 406 | 15 / 0 |
| round2_knockdot_original | 2070 | 408 | 15 / 0 |
| round2_knockdot_xygroup | 2068 | 406 | 15 / 0 |
| round2_knockdot_yxz | 2031 | 453 | 15 / 0 |
| round2_knockfilter_direct | 2028 | 573 | 15 / 0 |
| round2_knockfilter_find | 2028 | 573 | 15 / 0 |
| round2_knockfilter_paren | 2028 | 573 | 15 / 0 |
| round2_knockfilter_real | 2028 | 573 | 15 / 0 |
| round2_knockfilter_squared | 2062 | 423 | 15 / 0 |
| round2_knockfilter_sub | 2028 | 549 | 15 / 0 |
| round2_knockowners_arithmetic | 2031 | 451 | 15 / 0 |
| round2_knockowners_component | COMPILE_FAIL |  |  /  |
| round2_knockowners_ctors | 2031 | 453 | 15 / 0 |
| round2_knockowners_initialise | 2037 | 451 | 15 / 0 |
| round2_knockowners_mapowner | COMPILE_FAIL |  |  /  |
| round2_knockowners_sum_scope | 2066 | 408 | 15 / 0 |
| round2_knockshape_initexplicit | 2062 | 405 | 15 / 0 |
| round2_knockshape_initonly | 2062 | 401 | 15 / 0 |
| round2_knockshape_inits | 2070 | 408 | 15 / 0 |
