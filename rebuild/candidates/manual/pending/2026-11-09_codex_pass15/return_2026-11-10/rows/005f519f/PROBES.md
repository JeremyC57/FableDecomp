# 005f519f probes

Every history directory is retained. 005-008 are fake capture-body diagnostics and are excluded from selection and genuine reconstruction attempts. 024 was also rejected before selection because a wide literal was wrong; its apparent byte grade applied only to the screen and did not validate the supporting helper. 020 and026 are selected-source verification runs.

| History | Lever | Compiler / row grade | Strict / loose | Selection |
|---|---|---|---|---|
| 001_full_screen_shared | First complete shared-header screen; copied sibling frame-pointer pragma. | DIFFER 2108v2088 | 67.5953% / 81.0850%; 199 / 107 hunks | superseded / rejected |
| 002_size_biased_frame | Removed frame-pointer-off pragma for the observed size-biased retail frame. | DIFFER 2086v2088 | 79.5604% / 95.8242%; 124 / 33 hunks | superseded / rejected |
| 003_named_description_text | Named description CText pointers before SetValue. | DIFFER 2086v2088 | 79.5604% / 96.4103%; 124 / 27 hunks | superseded / rejected |
| 004_manager_reference | Bound UI manager by reference rather than pointer. | DIFFER 2086v2088 | 79.5604% / 96.4103%; 124 / 27 hunks | superseded / rejected |
| 005_DIAGNOSTIC_visible_count_bodies | INVALID throwaway capture diagnostic with two constant-output fake bodies. | DIFFER 2268v2088 | 51.6631% / 85.9165%; 119 / 85 hunks | superseded / rejected |
| 006_DIAGNOSTIC_nocapture_noexpand | INVALID throwaway capture diagnostic with no-expansion fake bodies; apparent match is NOT a valid match. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | superseded / rejected |
| 007_DIAGNOSTIC_magic_nocapture | INVALID throwaway capture diagnostic with only a fake magic body. | DIFFER 2086v2088 | 88.6447% / 98.3150%; 77 / 14 hunks | superseded / rejected |
| 008_DIAGNOSTIC_expression_nocapture | INVALID throwaway capture diagnostic with only a fake expressions body. | DIFFER 2088v2088 | 87.4085% / 98.0966%; 86 / 13 hunks | superseded / rejected |
| 009_real_expression_body | Added the authentic expressions-list helper on shared headers; magic helper remains declaration-only. | DIFFER 2088v2088 | 87.4085% / 98.0966%; 86 / 13 hunks | superseded / rejected |
| 010_scoped_count_outputs | Scoped both address-taken output counts and transferred values; kept real callee calls. | DIFFER 2100v2088 | 92.4088% / 96.3504%; 44 / 17 hunks | superseded / rejected |
| 011_paired_counts | Grouped count output storage as the real STLport pair with reference aliases. | DIFFER 2086v2088 | 82.4908% / 96.4103%; 113 / 27 hunks | superseded / rejected |
| 012_array_counts | Grouped count output storage as a two-element array with reference aliases. | DIFFER 2086v2088 | 82.4908% / 96.4103%; 113 / 27 hunks | superseded / rejected |
| 013_named_magic_result | Named raw magic getter result before constructing its counted pointer. | DIFFER 2088v2088 | 87.4085% / 98.0966%; 86 / 13 hunks | superseded / rejected |
| 014_common_menu_entry_call | Rewrote each menu-entry region with one shared CreateMenuEntry call and a conditional template name. | DIFFER 1896v2088 | 74.9808% / 87.6059%; 108 / 35 hunks | superseded / rejected |
| 015_arithmetic_item_count | Computed available-entry count with two conditional values rather than guarded increments. | DIFFER 2092v2088 | 86.2372% / 97.3646%; 86 / 15 hunks | superseded / rejected |
| 016_scoped_raw_results | Narrow each real output-count scope; immediately construct its counted pointer from an outside raw result before calling the next getter. | DIFFER 2088v2088 | 99.5608% / 99.5608%; 4 / 4 hunks | superseded / rejected |
| 017_early_scoped_magic_output | Move only the magic-output scope before UI creation; keep the authentic expressions body. | DIFFER 2088v2088 | 87.4085% / 98.0966%; 86 / 13 hunks | superseded / rejected |
| 018_seed_scoped_outputs | Initialise narrow outputs from the earlier zero-valued screen counts. | DIFFER 2088v2088 | 99.5608% / 99.5608%; 4 / 4 hunks | superseded / rejected |
| 019_mixed_output_scopes | Combine early magic scope with late narrow expressions output. | DIFFER 2088v2088 | 87.2621% / 97.9502%; 87 / 15 hunks | superseded / rejected |
| 020_verify_selected_016_annotations | Verification only: selected016 after comments and exact-body test update; not another source-shape attempt. | DIFFER 2088v2088 | 99.5608% / 99.5608%; 4 / 4 hunks | superseded / rejected |
| 021_condition_output_scopes | Scalar condition-clause initialisers around real getter outputs; frame regressed. | DIFFER 2088v2088 | 87.2621% / 97.9502%; 87 / 15 hunks | superseded / rejected |
| 022_loop_initializer_outputs | Single-pass loop initialisers around outputs; frame/code regressed. | DIFFER 2094v2088 | 81.1404% / 97.5146%; 81 / 17 hunks | superseded / rejected |
| 023_complete_magic_body | First complete magic-helper authoring on full shared types; compile failed on unavailable swap and GetTC overload. | COMPILE_FAIL ?v? | n/a | superseded / rejected |
| 024_complete_magic_build_fix | Explicit native swaps and existing expanded GetTC; apparent match rejected during literal audit because wide level prefix was wrong. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | superseded / rejected |
| 025_magic_wide_level_prefix | Corrected native UTF-16 level prefix to space-dash-space; complete helper, exact screen match. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | superseded / rejected |
| 026_verify_final_complete_magic | Verification after source comments/header promotion; no new source strategy. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | superseded / rejected |
| 027_portable_category_enum_bound | Temporary enum ABI-bound spelling after empty-enum UBSan failure; superseded by original raw-PDB enumerators. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | superseded / rejected |
| 028_original_pdb_category_enum | Original enumerator names/values from both raw PDBs and original unsigned-long category counters; final selected exact screen. | MASKED_MATCH_PENDING_AUDIT 2088v2088 | 100.0000% / 100.0000%; 0 / 0 hunks | final |

Attempt accounting: 28 total compiler-grade calls. This includes4 excluded fake-body diagnostics,2 verification-only runs, and1 initial complete-helper compile failure. All source trials and failures remain preserved; this is a compiler-trial count, not a claim of28 distinct source mechanisms. The empty-enum native portability test was a separate non-RTM check and is not added to that count.

Final selected:028_original_pdb_category_enum. RTM13.10.3077 emits the exact2088-byte screen section modulo its relocations;683/683 instructions,100% strict/loose,zero hunks. AUDIT_FINAL.json records zero relocation hard errors,137 non-string operands,32 string operands and4 float operands checked. Status: RELOCATION_MATCH.

The row TU contains the complete authentic magic and expressions helper reconstructions so c2 sees the count references do not escape. No supporting-helper byte-match claim is made. All magic-helper phases and data types were checked against native code and both raw donor PDBs. ORIGINAL_CATEGORY_ENUMS.json and ORIGINAL_MAGIC_LOCALS.json retain the recovered evidence.

The required standalone test embeds the exact final ConstructScreen and magic-helper function bodies. Six screen cases and six helper cases pass under VC7.1 /Od /W3 and 64-bit g++ -std=c++03 -O2 -fstrict-enums with AddressSanitizer/UndefinedBehaviorSanitizer. Explicit component/action/parameter/control-block counters return to zero. The empty-enum spelling failed UBSan and was discarded; final source uses original PDB-proven enumerators.

The four-hunk scoped-output source016 remains in history as an honest intermediate. It restored frame0x94 and every slot/dtor but moved three zero-initialisation instructions later. Full faithful magic-body visibility recovered the original early zero stores and exact screen bytes.

No fake/partial capture stand-in is selected, and no input package, repository, other row, compiler or source history was mutated.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
