# 006d8830 attempts

Selected031_group_variation_result; final frozen source/header snapshot035_final_metadata_and_validation.321/321 exact119 instructions; all9 relocations PASS; self-contained exact-body tests PASS native64 g++03 and VC7.1/Od/W3.

| Trial | Result | Bytes | Strict hunks |
|---|---|---|---|
| 001_shared_typed_members | COMPILE_FAIL | -/- | - |
| 001b_complete_thing | DIFFER | 321/321 | 2 |
| 002_abs_input_double_cast | DIFFER | 321/321 | 2 |
| 003_abs_input_parenthesis | DIFFER | 321/321 | 2 |
| 004_reassociate_multiplier | DIFFER | 321/321 | 2 |
| 005_reference_absolute_value | DIFFER | 330/321 | 5 |
| 006_scale_strength_in_place | DIFFER | 321/321 | 2 |
| 007_named_scaled_strength | DIFFER | 321/321 | 2 |
| 008_named_variation_before_abs | DIFFER | 319/321 | 4 |
| 009_reuse_damage_local | DIFFER | 321/321 | 2 |
| 010_float_abs_overload | DIFFER | 321/321 | 2 |
| 011_double_abs_named_result | DIFFER | 321/321 | 2 |
| 012_accumulate_explicit | DIFFER | 321/321 | 2 |
| 013_split_product_store | DIFFER | 321/321 | 2 |
| 014_implicit_abs_conversion | DIFFER | 321/321 | 2 |
| 015_functional_abs_conversion | DIFFER | 321/321 | 2 |
| 016_group_variation_product | DIFFER | 321/321 | 2 |
| 017_explicit_float_product | DIFFER | 321/321 | 2 |
| 018_bool_named_update | DIFFER | 321/321 | 2 |
| 019_local_ref_accumulator | DIFFER | 321/321 | 2 |
| 020_block_strength_lifetime | DIFFER | 321/321 | 2 |
| 021_scaled_before_accumulation | DIFFER | 319/321 | 3 |
| 022_guard_missing_multiplier | DIFFER | 321/321 | 2 |
| 023_cache_damage_map | DIFFER | 321/321 | 2 |
| 024_post_increment_iterator | DIFFER | 321/321 | 2 |
| 025_group_entire_added_amount | DIFFER | 321/321 | 2 |
| 026_abs_after_variation_block | DIFFER | 321/321 | 2 |
| 027_named_damage_pair | DIFFER | 321/321 | 2 |
| 028_comma_update_expression | DIFFER | 321/321 | 2 |
| 029_comma_sequenced_statements | DIFFER | 321/321 | 2 |
| 030_explicit_global_fabs | DIFFER | 321/321 | 2 |
| 031_group_variation_result | MASKED_MATCH_PENDING_AUDIT | 321/321 | 0 |
| 032_group_absolute_result | DIFFER | 321/321 | 2 |
| 033_group_strength_operand | DIFFER | 321/321 | 2 |
| 034_group_combination_multiplier | DIFFER | 321/321 | 2 |
| 035_final_metadata_and_validation | MASKED_MATCH_PENDING_AUDIT | 321/321 | 0 |

Every prior source and log remains. The compile-repair001 and final metadata/audit035 are not distinct source strategies.

The measured winning hypothesis targets integer issue slots, not floating arithmetic: baseline push is independently ready after the variation call and issues cycle2 alongside fabs, before the float store. Removing two no-code PAREN nodes after fabs(trial002) cannot fix that; comma sequencing028 gives the exact same DAG as baseline.

Trial031 groups the floating call result: `(GetAttackVariationMultiplier()) * strength * found->second`. It creates a no-code0x269 node directly after the variation call. Actual C2 trace DAG7: cycle1 call+fld; cycle2 fabs+newPAREN; cycle3 fst+existingPAREN; cycle4 existingPAREN; cycle5 existingPAREN+fmulp; cycle6 push1. Thus the new grouping occupies the previously free integer issue slot atcycle2, and the existing grouping chain keeps it busy throughcycle5. Retail push after fmulp is reproduced exactly. The arithmetic and behavior are unchanged.

Trials032–034 separately group fabs result, strength and found multiplier; they retain the old321/321 residue. The placement of the natural grouping, not parenthesis count alone, is decisive.031 schedule diagnostics are separate objects and SCHEDULE_VALIDATION.json proves the diagnostic leaf is byte-identical to normal unmodified RTM output. Final035 uses the same body with the required metadata. No compiler control, padding or object patch is used.

## Main complete-enum revalidation

FINAL_002_complete_original_enums restores the complete original enum definitions from both raw donor records. Fixed-value source spellings now use original enumerators where applicable. Target bytes, full Leaf and all relocation identities remain unchanged. Fresh VC7.1 /GR- and g++03 /O2 -fstrict-enums ASan/UBSan tests pass. This corrects type fidelity and portable-test validity; it is not a new matching attempt. Main verification and fresh logs are retained in FINAL_002 history.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
