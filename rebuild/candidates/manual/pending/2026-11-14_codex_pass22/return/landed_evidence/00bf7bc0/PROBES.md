# 00BF7BC0 probes

Original whole-function identity and locals are in both PDB extracts and ORIGINAL_LOCALS.json. No baseline is counted as a source attempt.

## Original packed-distance site ladder

Native00BF7BE6..00BF7C2E homes box/point pointers, reloads them intoECX/EDX, packs lanes(Z,0,X,Y), and usesMAXPS/MINPS/SUBPS/MULPS/SHUFPS/ADDPS/ADDSS without an aligned stack frame. Plain02 expands the actual member and emits scalar x87 comparisons/arithmetic; the first packed load is absent. VC7.1 has no auto-vectorizer. The following mandatory per-site spellings investigate distinct scalar expression structures; variants within a family are spellings, not independently claimed whole-function strategies. Every full source/object/disassembly is retained under history/00bf7bc0. Scalar equivalence is checked on the native valid finite ordered-box domain; the final shared SSE helper also preserves hardware NaN/equality semantics.

- plain01: ordinary inline original member visibility, 280bytes with out-of-line distance call. Diagnostic/capture control, not a different kernel spelling.
- plain02: force original expanded member, component max then min,746bytes. Kernel spelling1. FABLE_FORCEINLINE expands the proven original helper rather than adjusting any caller allocation.

- sol2_plain03_sequential_axis_clamp: DIFFER, 695bytes; packed kernel=False. 

- sol2_plain04_exclusive_axis_clamp: DIFFER, 695bytes; packed kernel=False. 

- sol2_plain05_nested_clamp_ternary: DIFFER, 711bytes; packed kernel=False. 

- sol2_plain06_reversed_nested_ternary: DIFFER, 711bytes; packed kernel=False. 

- sol2_plain07_distance_lower_upper_branches: DIFFER, 727bytes; packed kernel=False. 

- sol2_plain08_distance_upper_lower_branches: DIFFER, 727bytes; packed kernel=False. 

- sol2_plain09_distance_nested_ternary: DIFFER, 727bytes; packed kernel=False. 

- sol2_plain10_distance_positive_branch: DIFFER, 731bytes; packed kernel=False. 

- sol2_plain11_outside_max_zero: DIFFER, 727bytes; packed kernel=False. 

- sol2_plain12_separate_positive_parts: DIFFER, 761bytes; packed kernel=False. 

- sol2_plain13_squared_outside_branches: DIFFER, 727bytes; packed kernel=False. 

- sol2_plain14_vector_clamp: DIFFER, 718bytes; packed kernel=False. 

- sol2_plain15_vector_nested_clamp: DIFFER, 716bytes; packed kernel=False. 

- sol2_plain16_vector_max_then_min: DIFFER, 729bytes; packed kernel=False. 

- sol2_plain17_array_loop_clamp: DIFFER, 775bytes; packed kernel=False. 

- sol2_plain18_array_loop_max_min: DIFFER, 743bytes; packed kernel=False. 

- sol2_plain19_array_loop_distance: DIFFER, 765bytes; packed kernel=False. 

- sol2_plain20_array_loop_signed_distance: DIFFER, 759bytes; packed kernel=False. 

- sol2_plain21_array_loop_squared: DIFFER, 743bytes; packed kernel=False. 

- sol2_plain22_max_min_reversed_operands: DIFFER, 698bytes; packed kernel=False. 

- sol2_plain23_upper_clamp_then_lower: DIFFER, 698bytes; packed kernel=False. 

- sol2_plain24_distance_abs_identity: DIFFER, 711bytes; packed kernel=False. 

- sol2_plain25_center_extent_distance: DIFFER, 743bytes; packed kernel=False. 

- sol2_plain26_double_intermediate_clamp: DIFFER, 719bytes; packed kernel=False. 

- sol2_plain27_component_square_accumulate: DIFFER, 711bytes; packed kernel=False. 

- sol2_plain28_component_squared_vector: DIFFER, 743bytes; packed kernel=False. 

- sol2_plain29_two_stage_vector_delta: DIFFER, 695bytes; packed kernel=False. 

- sol2_plain30_conditional_direct_squares: DIFFER, 781bytes; packed kernel=False. 

- sol2_plain31_negative_zero_inside_distance: DIFFER, 716bytes; packed kernel=False. 

- sol2_intrinsic01_nested_intrinsic_clamp: DIFFER, 602bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic02_staged_intrinsic_clamp: DIFFER, 602bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic03_separate_clamped_and_delta: DIFFER, 602bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic04_load_then_high_half: DIFFER, 602bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic05_set_ps_lanes: DIFFER, 751bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic06_setr_ps_lanes: DIFFER, 751bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic07_unpack_three_scalar_loads: DIFFER, 651bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic08_scalar_shuffle_reduction: DIFFER, 603bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic09_spill_squared_lanes: DIFFER, 595bytes; MAXPS/MINPS=True, aligned frame=True. 

- sol2_intrinsic10_xyz_lane_layout: DIFFER, 752bytes; MAXPS/MINPS=True, aligned frame=True. 

## Selected whole function

- sol2_01_original_shared_kernel: 546/546 bytes,168/168 instructions. Original PDB member/caller structure plus existing C3DBoundingBoxSSE.h, after the full30plain+10intrinsic site ladder. Initial forced-inline form, superseded by the review ladder below. The native FSQRT and absence of calls prove expansion; forcing it is unnecessary once the real threshold is identified.
- sol2_02_portable_macro_statement: same exact546bytes, former selected source. Added the macro statement semicolon required by FABLE_PORTABLE; same native output. This portability syntax correction is not a distinct source strategy.

All existing shared-header helper definitions that are emitted but unreferenced are listed in RELOCATION_AUDIT.json. Target has no function relocations and no reachable helper debts. The only five target relocations independently bind the three real original globals. Immediate1.0e30f is exact0x7149F2CA.

## Tests and header proof

Standalone full-body test:11760 target cases, real shared SSE path versus portable twin for20512 additional kernel cases, VC71 /Od /W3 /GR- without /EHsc, and g++64 C++03. Exact output00bf7bc0_TEST PASS. Three original method bodies are copied verbatim and checked by audit script. The fixture has real ninth LOD backing storage for native lookup-before-bound behavior. Target coordinates are finite and boxes ordered. Signed-zero and non-NaN Inf results compare all32bits. Multi-NaN payload identity was disproved in an exploratory test; final NaN checks classify NaN, and NATIVE_BEHAVIOR.md discloses the existing helper limitation. Compiler fixture diagnostics (macro operand expression and missing portable semicolon) are not source strategies; strict-payload exploratory assertion failure is preserved.

42 old/default header profiles preprocess identically. Every combination of prior header branches is covered.85 moved old static-assert names were pinned using mandatory pin_asserts.py. No new packing. Parent performs the combined existing-caller regression.

## Main review: inline-device ladder and final plain-inline source

The initial generic assumption that VC7.1 cannot expand an asm-bearing inline member was too broad at this site. The actual flag ladder proves that BOTH original distance members can stay plain inline. Selected source now uses /d2inlT282 and the required line2 Inline-budget note; FABLE_FORCEINLINE is absent from the row. This only restores original callee expansion, not register or allocation layout.

- sol2_review_inline_plain:280bytes, original distance call remains.
- sol2_review_inline_threshold512:546exact; threshold4096:546exact; size512:546exact.
- sol2_review_inline_scope255:280bytes, original distance call remains, scoped inline_depth255/reset diagnostic.
- Threshold boundary probes, all source bodies fixed:128=280,256=280,384=546exact,320=546exact,288=546exact,272=280,280=278,284=546exact,282=546exact,281=278. At280/281 the distance call still remains; the callee's separately emitted body has expanded further, changing its caller-side scheduling slightly. First threshold that removes the distance call is282. These are compiler diagnostics, not distinct source strategies or site spellings.
- sol2_03_plain_inline_budget282: final source546/546bytes,168/168instructions, five exact independently named data bindings, no reachable helpers. Inline methods are ordinary inline. The final test uses normal fable_compiler.h and engine/C3DBoundingBoxSSE.h includes, through an explicit immutable include root in run_tests_sol2.py.

Original43 compiler invocations plus16 inline-review diagnostics/final confirmation =59 worker invocations, one original whole-function strategy,30 plain kernel spellings and10 intrinsic kernel spellings. All histories are preserved.
