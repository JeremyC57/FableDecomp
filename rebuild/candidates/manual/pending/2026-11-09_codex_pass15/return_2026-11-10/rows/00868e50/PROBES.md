# Armour-weight reconstruction attempts
Selected:041_final_original_component_lookup,456/456bytes,163/163instructions. VERIFIED_RELOCATION_MATCH after all10bindings and standalone VC7.1/sanitized C++03 tests.
The old local models were replaced with shared headers, true class hierarchy, correct enum parameter and actual callee bodies. No invented callee models or new packing.
Existing prior attempts read in full;646 differs from481 only in notes,895 equals646. All prior approaches listed there retained as inherited history.
| Trial | Result | Note |
|---|---|---|
|001_shared_headers|COMPILE_FAIL |No completed grade: compiler/Leaf setup repair.|
|001b_shared_accessors|COMPILE_FAIL |No completed grade: compiler/Leaf setup repair.|
|001c_shared_accessors|STARTED |No completed grade: compiler/Leaf setup repair.|
|001d_exact_leaf|STARTED |No completed grade: compiler/Leaf setup repair.|
|001e_verified_anon_leaf|DIFFER 531|Strict hunks 42|
|002_visible_real_callees|COMPILE_FAIL |No completed grade: compiler/Leaf setup repair.|
|002b_visible_real_callees|DIFFER 456|Strict hunks 2|
|003_dereferenced_definition_reference|DIFFER 456|Strict hunks 2|
|004_shared_weight_reference|DIFFER 456|Strict hunks 2|
|005_named_armour_index|DIFFER 456|Strict hunks 2|
|006_narrow_lookup_scope|DIFFER 452|Strict hunks 27|
|007_iterator_postincrement|DIFFER 456|Strict hunks 2|
|008_map_pair_value|DIFFER 452|Strict hunks 3|
|009_pointer_def_manager|DIFFER 456|Strict hunks 2|
|010_weight_before_lookup|DIFFER 456|Strict hunks 2|
|011_const_weight_value|DIFFER 467|Strict hunks 27|
|012_accumulator_local|DIFFER 456|Strict hunks 2|
|013_negative_subtraction|DIFFER 458|Strict hunks 2|
|014_named_increment|DIFFER 456|Strict hunks 2|
|015_float_total_cast|DIFFER 456|Strict hunks 2|
|016_nested_typed_sum|DIFFER 456|Strict hunks 2|

Tests:256 scenarios under VC7.1/Od/W3 and g++ C++03 PASS. Exact selected function body pasted. External game lookups are fixture stubs and host STL containers supply storage; the test does not validate the complete production STLport balancing implementation. Covers absent component, duplicates, negative armour index, signed dyadic weights, retained pre-existing entry, lookup counts and reference-count balance.
Not all compiler trials are distinct meaningful strategies. Alias/cast variants canonicalized identically; narrowed lookup scope changes destruction timing and is rejected; cached weight changes read timing and is not selected. No padding or target-byte edits.
Next: inspect C2 operand selection/candidate IDs at the remaining fadd. Ordinary regrouping/aliases did not change it.

## Further evidence-guided retry (017–037)

All21 new official grades are preserved. Baseline002b remains selected; no new best is claimed. The field-offset naming explanation was corrected: both preceding vectors shrink4 bytes, not one vector plus an alignment change.

Actual C2 baseline trace captures27 DAGs and checks the diagnostic leaf against normal RTM bytes. DAG16 already contains fld(weight),mov(total pointer),fadd(total),fstp before scheduling. Thus this residue is operand lowering, not the list scheduler. Trial017 total-read grouping produces exactly the same27 DAGs;018/019 weight/whole-sum grouping is also inert. The trace driver needed an exact emitted anonymous hash from the object; unique trace_armour_retry.py handles that without changing the shared tracing tool.

Distinct strategy families tested: grouping at different value nodes; genuine standard addition functor; comma-sequenced insertion/accumulation; a typed modifier pointer; two named read values; subtracting negative total; unary-positive rvalue conversion; missing-component guard; negative-index continue; persistent loop accumulator; pre-insert total/weight captures; direct/scoped pair construction preserving destructor order. These are12 source hypotheses, not21 claimed independent strategies. Many canonicalize; each result is logged below.

Trial024 forces the desired fld(total) direction but emits fchs/fsubr instead of the one fadd(458 bytes); it is diagnostic evidence only. Floating no-code grouping and unary-positive casts do not survive operand lowering. Precapture/loop accumulation adds memory/frame work and changes read timing, so it is not selected. Scoped named pairs preserve native lifetime but remain byte-identical to the baseline. This matches VC71_CODEGEN_RESEARCH R1/R5: the+60 indirect field outranks the+0 indirect total, and affine pointer aliases fold before operand sort. A new useful lever must survive that lowering without introducing an extra instruction; scheduler-only changes cannot swap the already-selected fld/fadd memory operands.

| Trial | Result | Bytes | Strict hunks |
|---|---|---|---|
| 017_group_total_read | DIFFER | 456/456 | 2 |
| 018_group_weight_read | DIFFER | 456/456 | 2 |
| 019_group_entire_sum | DIFFER | 456/456 | 2 |
| 020_standard_addition_functor | DIFFER | 456/456 | 2 |
| 021_comma_insert_and_accumulate | DIFFER | 452/456 | 3 |
| 022_current_modifier_pointer | DIFFER | 456/456 | 2 |
| 023_named_total_then_weight | DIFFER | 456/456 | 2 |
| 024_subtract_negative_total | DIFFER | 458/456 | 1 |
| 025_unary_positive_total | DIFFER | 456/456 | 2 |
| 026_unary_positive_weight | DIFFER | 456/456 | 2 |
| 027_group_positive_total | DIFFER | 456/456 | 2 |
| 028_reversed_positive_total | DIFFER | 456/456 | 2 |
| 029_absent_component_guard | DIFFER | 456/456 | 2 |
| 030_negative_index_continue | DIFFER | 470/456 | 36 |
| 031_loop_accumulator_with_output | DIFFER | 482/456 | 29 |
| 032_total_value_before_insert | DIFFER | 468/456 | 26 |
| 033_mutable_total_before_insert | DIFFER | 468/456 | 26 |
| 034_capture_pair_and_total | DIFFER | 473/456 | 26 |
| 035_direct_pair_argument | DIFFER | 456/456 | 2 |
| 036_scoped_constructed_pair | DIFFER | 456/456 | 2 |
| 037_scoped_factory_pair | DIFFER | 456/456 | 2 |

## Original donor locals resolved the final residue038–041

Both raw PDB procedure records prove def_man is const CGameDefinitionManager&, ptc_armour_clothes is a const component pointer, and the clothing index is a named long.038 restores these source types/local forms and remains456bytes/2hunks.039 repeats the earlier016 double-sum hypothesis on these corrected types; it is not a new strategy and remains identical.040 uses the original shared boolean PeekTC(id,&ptc_armour_clothes) out-parameter form instead of separate HasTC/GetTC calls. This produces456/456bytes,163/163instructions and zero hunks. The apparently isolated floating operand choice was affected by the earlier lookup lowering; the previous statement that it was an irreducible lowering gap was premature.041 records proven APPEARANCE_CLOTHING and TCI_HERO_ATTACHABLE_APPEARANCE_MODIFIERS names and valid enum definitions, with unchanged match.

Original method/local evidence is ORIGINAL_FUNCTION_LOCALS.json. ORIGINAL_APPEARANCE_ENUM.json proves all four enum values in both donors. The final source retains actual shared GetDef/GetSubDef/STLport bodies, not artificial no-capture helpers. MAIN_VERIFICATION.json and logs in history041 record final10native bindings, full anonymous-namespace signature, exact standalone body and256testcases. Only the compiler TU hash in the anonymous namespace is normalized.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
