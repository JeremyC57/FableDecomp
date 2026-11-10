# Pass17 attempts

All numbered build histories are preserved locally. Flag diagnostics, compile failures and small variants are not counted as independent source strategies. No byte match is claimed.

| Build | Idea | Object / retail | Result |
|---|---|---|---|
| 001_original_rolling_average | Original GetRollingAverage template with double1.0 denominator;124/124 but extra arithmetic-order residue. | 124/124 | DIFFER |
| 002_float_denominator | Float1.0f denominator preserves retail arithmetic;124/124, only post-call fld/mov order differs. Selected base. | 124/124 | DIFFER |
| 003_accumulated_average | Accumulated float arithmetic in template;same best residue. | 124/124 | DIFFER |
| 004_g7_diagnostic | /G7 diagnostic, not a source strategy;126/124. | 126/124 | DIFFER |
| 005_g6_diagnostic | /G6 diagnostic, not a source strategy;unchanged124/124. | 124/124 | DIFFER |
| 006_named_template_result | Named result in original template with double denominator;earlier arithmetic residue returns. | 124/124 | DIFFER |
| 007_stage_string_result | Stage returned string value and radius before member store;unchanged124/124. | 124/124 | DIFFER |
| 008_visible_original_camtools | Full genuine GetCamTools/GetCameraManager/GetCameraTools bodies;90/124 because unused pure calls disappear. | 90/124 | DIFFER |
| 009_visible_camtools_budget1 | Same full helper with /d2inlT1 diagnostic;90/124, no improvement;not separate strategy. | 90/124 | DIFFER |
| 010_camtools_original_outline | Same full helper with scoped auto_inline(off);90/124;not separate strategy. | 90/124 | DIFFER |
| 011_camtools_explicit_outline | Same full helper explicitly noinline;90/124, confirming pure-call elimination rather than only expansion. No device retained. | 90/124 | DIFFER |
| 012_branch_local_radius | Radius assigned inside each if/else arm;124/124, altered branch/load layout. | 124/124 | DIFFER |
| 013_sequenced_radius_initialisation | Sequence string update and radius initialization in one comma expression;unchanged124/124. | 124/124 | DIFFER |
| 014_return_value_reference_lifetime | Bind returned string value to const reference;unchanged124/124. | 124/124 | DIFFER |
| 015_g5_diagnostic | /G5 diagnostic, not a source strategy;unchanged124/124. | 124/124 | DIFFER |
| 016_group_radius_value | Parenthesized radius read;unchanged bytes and diagnostic scheduling graph. Not a new algorithm family. | 124/124 | DIFFER |
| 017_size_optimisation | /O1 /Oy diagnostic, not a source strategy;121/124. | 121/124 | DIFFER |
| 018_float_consistency_diagnostic | /Op diagnostic, not a source strategy;130/124. | 130/124 | DIFFER |
| 019_expansion_timer_snapshot | Snapshot expansion timer between returned string and member store;124/124 but dataflow/order worsens. | 124/124 | DIFFER |
| 020_select_radius_then_decrement | Select radius by conditional expression then decrement positive timer;128/124, extra flow. | 128/124 | DIFFER |

## Compiler evidence

The in-process c2 diagnostic reproduced the unmodified RTM leaf. It reports3 scheduling graphs; immediately after the first GetCamTools call, the integer reload has priority43008 and the DesiredCageRadius fld35840. The compiler picks the reload first. The original helper body removes both otherwise unused pure calls even when kept out of line; it is not a valid solution to this row.

Selected original rolling-average/update shape passes6144 frame-transition cases under bothVC7.1 andg++03. Tests also check both tools-call snapshots and preservation of desired values.

## Further focused retries

- 021_original_tools_type_best_review: Corrected CTools original const manager reference; unchanged124/124. Status DIFFER.
- 022_named_string_tools_receiver: Named tools receiver for first update;116/124, call precedes smoothing and native spill disappears. Status DIFFER.
- 023_string_update_lexical_scope: Lexically scope first string update;unchanged124/124. Status DIFFER.

-024_member_pointer_string_store: staged string result and radius, then pointer-to-member destination;124/124, same first post-call load pair. No gain.

Sol transfer: read ROW, best, every prior/header/test/handback and all neighbours, raw original locals and current24 histories/trace before authoring. Original20–24 preserved.

025_length_destination_reference: current length field through one actual float reference; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

026_length_destination_pointer: current length field through one actual float pointer; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

027_radius_reference_read: desired radius through reference across tools call; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

028_radius_pointer_read: desired radius through pointer across tools call; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

029_float_result_byte_copy: standard four-byte scalar result representation copy; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

030_float_result_swap: real STLport swap, old local value dead; COMPILE_FAIL None/124, first differenceNone. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

031_cage_result_value_parameter: explicit float scalar copy/result conversion family; DIFFER 124/124, first difference37. Related pointer/reference and scalar copy variants grouped, not independent minimum strategies.

032_real_stlport_scalar_swap: enable existing STLport swap gate,124/124 same two-order residue. The standard swap body existed; no new model introduced.

033_scalar_destination_predecrement: legitimate scalar one-past pointer then access original actual float object; DIFFER 124/124; firstdiff37. Paired addressing variants grouped.

034_scalar_radius_predecrement: legitimate scalar one-past pointer then access original actual float object; DIFFER 124/124; firstdiff37. Paired addressing variants grouped.

035_scalar_destination_negative_index: legitimate scalar one-past pointer then access original actual float object; DIFFER 124/124; firstdiff37. Paired addressing variants grouped.

036_original_getter_member_pointer: genuine original getter through constant derived member pointer; DIFFER 124/124, firstdiff37.

037_original_average_member_pointer: genuine original rolling-average instantiation through constant member pointer; DIFFER 110/124, firstdiff1.

038_explicit_original_base_receiver: explicit original base qualification; same receiver family; DIFFER 124/124, firstdiff37.

039_length_store_both_timer_arms: real first-result member store lifetime across timer arms/commit point; DIFFER 136/124, firstdiff37. Related duplicated-store/commit variants grouped.

040_length_store_after_target_branch: real first-result member store lifetime across timer arms/commit point; DIFFER 124/124, firstdiff37. Related duplicated-store/commit variants grouped.

041_radius_select_before_length_commit: real first-result member store lifetime across timer arms/commit point; DIFFER 134/124, firstdiff37. Related duplicated-store/commit variants grouped.

042_reuse_original_scalar_working_value: original new_cage_radius variable first holds smoothed string value, then desired radius;124/124 same order. Genuine scalar lifetime/reuse family.

043_flag_EHsc_diagnostic: final compiler diagnostic /EHsc after genuine source shapes; DIFFER 124/124, firstdiff37. Not counted as a source strategy.

044_flag_EHa_diagnostic: final compiler diagnostic /EHa after genuine source shapes; DIFFER 124/124, firstdiff37. Not counted as a source strategy.

045_flag_GRminus_diagnostic: final compiler diagnostic /GR- after genuine source shapes; DIFFER 124/124, firstdiff37. Not counted as a source strategy.

046_flag_Oa_diagnostic: final compiler diagnostic /Oa after genuine source shapes; DIFFER 124/124, firstdiff37. Not counted as a source strategy.

047_flag_Os_diagnostic: final compiler diagnostic /Os after genuine source shapes; DIFFER 121/124, firstdiff0. Not counted as a source strategy.

048_grouped_original_average_call_1: natural grouped actual GetRollingAverage call result; DIFFER 124/124, firstdiff37. Three masks form one grouping family.

049_grouped_original_average_call_2: natural grouped actual GetRollingAverage call result; DIFFER 124/124, firstdiff37. Three masks form one grouping family.

050_grouped_original_average_call_3: natural grouped actual GetRollingAverage call result; DIFFER 124/124, firstdiff37. Three masks form one grouping family.

051_timer_reference_lifetime: true expansion timer field reference across first getter; DIFFER 124/124, firstdiff37.

052_timer_pointer_condition: timer pointer condition with original direct decrement; DIFFER 124/124, firstdiff37.

053_radius_value_before_getter: actual radius value lifetime beginning before original service call; DIFFER 136/124, firstdiff0.

054_final_exhausted_best_review: restored exact021 body, added required Grade comment.124/124 DIFFER; same10 unmasked bytes37..46 (one swapped instruction pair). All six relocated operands decoded again, four literal bits verified, both original getter calls bound to0084D080 with exact protected const symbol. Every local conditional branch and complete124-byte ret extent exact.6144 exact-body/template/frame-transition cases reran VC71/Od/W3/GR- and g++03 LP64 PASS. Source/test/headers frozen after this review.

Final54 grades are not54 strategies. Genuine families across parent/Sol work include original type/template/class rebase, accumulated versus returned-average lifetime, staged first-result/radius lifetime, complete authentic accessor visibility, branch-local radius assignment, comma-expression sequencing, return-reference lifetime, timer-value capture, conditional radius selection/decrement, lexical first-update scope, typed member-pointer destination, actual float field aliases, scalar representation copy/real STLport swap, genuine getter/function member-pointer receiver, duplicated member-store branch/commit placement, working scalar reuse, grouped call results and timer-reference/pointer lifetimes. Related pointer/ref, grouped-call masks, scalar-copy variants, flag reruns and final reviews are grouped rather than inflated. More than10 substantive source families were tested; no shrinking residue remains.

Compiler trace remains the measured explanation:3 scheduling DAGs, no81-tuple region cut; post-call integer reload priority43008 versus DesiredCageRadius fld35840. The full actual accessor chain eliminates both otherwise-unused pure calls, so it cannot reproduce this native body. New field alias/copy/receiver forms canonicalize to the same pair. This does not establish impossibility or justify altered code, a fake effectful getter, artificial dependency or extra masking. Parked with best authentic source and passing tests. Future useful work would need a newly evidenced original call/lifetime/alias shape that changes that graph while retaining both original getter calls.

Main proposed one last substantial original-model lever after054: replace the four opaque base composites with actual types. Work explicitly resumed for this bounded probe.

055_original_typed_base_composites: raw both-PDB types prove protected LookAt and private GoTo are NCameraModes::CPhysicsState, private PCameraModeDef is CDefPointer<const CCameraModeDef>, private SubComponents is vector<CCountedPointer<NCameraModes::CSubComponentBase>>. Added actual complete CPhysicsState (three real vectors, two real lists, CullPhysics; native48) and used existing genuine shared handles/containers. CBase remains0xA4 with a compile-time size check; source/boundaries remain124/124 and same two-instruction pair. No base extent or invented type/field.
056_final_typed_base_review: retains the more faithful narrowly gated typed base plus required Grade comment;124/124 DIFFER, same10 unmasked bytes37..46. Six bindings/constant bits identical to054 and audited;6144 exact-body/template/frame cases rerun PASS both compilers. Final in-process diagnostic reproduces normal RTM leaf and same3 DAGs.56 actual grades, no extra family inflation:055 is one substantial typed-model family,056 final review only. Source/test/headers frozen after056; no remaining shrinking residue.
