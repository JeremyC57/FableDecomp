# 00a55f90 source attempts

Read all four supplied earlier drafts, hand-back notes and all six neighbours in full before authoring. Prior 20 probes are preserved separately and not counted as new work.

## a01_original_trig_helpers
Restore genuine GFCos/GFSin and original local identities. First compile exposed missing pre-existing FABLE_FASTCALL definition; dependency fixed in a01b, not counted twice.
Result: COMPILE_FAIL; dependency compile failure resolved in a01b.

## a02_original_matrix_data_union
Use genuine Data[4][3] union and indexed element stores instead of flattened E members.
Result: DIFFER; 259/259 bytes; 0.922 instruction ratio

## a03_reference_alias_original_trig
Bind one const C3DVector reference for all component reads; tests whether alias identity changes x87 ordering.
Result: DIFFER; 259/259 bytes; 0.922 instruction ratio

## a04_grouped_rodrigues_terms
Natural grouping of diagonal sum/product and off-diagonal difference/product changes scheduler regions, but worsens ordering.
Result: DIFFER; 259/259 bytes; 0.911 instruction ratio

## a05_reference_component_x
Name only X as a const float reference, leaving Y/Z member reads; canonicalized identically.
Result: DIFFER; 259/259 bytes; 0.922 instruction ratio

## a06_named_vector_copy
Copy the entire original vector before computing; changes live values/loads and emits263bytes.
Result: DIFFER; 263/259 bytes; 0.333 instruction ratio

## a07_value_component_x
Load only X into a named value while leaving Y/Z accessed through input;251bytes, larger instruction residue.
Result: DIFFER; 251/259 bytes; 0.422 instruction ratio

## a08_update_mixed_terms_before_stores
Update each mixed product by subtracting its cosine-scaled value before matrix stores;271bytes and larger residue.
Result: DIFFER; 271/259 bytes; 0.627 instruction ratio

## a09_interleave_products_and_symmetric_stores
Compute each mixed product next to its symmetric matrix pair;196bytes and changes alias behavior if input overlaps output, so rejected.
Result: DIFFER; 196/259 bytes; 0.257 instruction ratio

## a10_original_data_translation_loop
Zero genuine Data row3 with a 3-element loop, unrolled by VC71;259bytes, same pair/load and terminal schedule gaps.
Result: DIFFER; 259/259 bytes; 0.922 instruction ratio

Selected a01b: closest instruction ratio ties several alternatives; choose original donor helper structure and simplest member spelling. No match claimed. Compiler count is11 for10 proposed source strategies; reference-alias subcases are explicitly distinguished, not represented as successful progress.
