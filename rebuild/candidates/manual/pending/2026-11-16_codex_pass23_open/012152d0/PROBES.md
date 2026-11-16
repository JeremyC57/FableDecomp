# ADPCM parent probes

The assigned18-byte range is compiler-generated cleanup+handler for parent00c425e0/217. It is not authored separately. Current selected baseline is221 bytes and is NOT a match. No acceptance or complete operand audit is claimed.

Old hand438 source was read completely. It had the same221-byte residue but used forbidden local address-suffixed models. This pass uses the existing SDK shared header without any header change. Its old /Gr, pointer-local and QFE experiments are not credited to this pass.

| Build | Parent bytes | First difference | Strategy |
|---|---:|---:|---|
| main_01_shared_sdk_baseline | 221 | 74 | Shared SDK baseline; correct original class/enum/format types and canonical descriptive parent. |
| main_02_real_empty_constructor | 203 | 2 | Genuine native+SDK empty constructor body: compiler removes native call; reject. |
| main_03_constructor_retained_Ob1 | 203 | 2 | Ob1 attempt to retain genuine constructor; still removes it; same family. |
| main_04_constructor_retained_Ob0 | 203 | 2 | Ob0 diagnostic also removes empty constructor; same family. |
| main_05_real_constructor_noinline | 203 | 2 | Guarded noinline diagnostic still removes empty constructor; rejected, not retained. |
| main_06_positive_nested_success | 225 | 62 | Positive nested success exits: different return-block survivor, worse225. |
| main_07_format_first_lifetime | 221 | 74 | Declare format before codec; same221 bytes. |
| main_08_genuine_format_body | 280 | 62 | Genuine CreateImaAdpcmFormat body visible: expanded call,280 bytes. |
| main_09_format_body_Ob1 | 221 | 74 | Ob1 keeps real format helper called;221bytes unchanged; same family. |
| main_10_original_Initialize_body | 221 | 74 | Original SDK Initialize body visible under Ob1;221bytes unchanged. |
| main_11_failure_disjunction | 225 | 62 | Disjunction failure guard; same225bytes as nested success, not new family. |
| main_12_initialization_result_reference | 221 | 74 | const-reference initialization result local; unchanged. |
| main_13_receiver_after_guard | 221 | 74 | Codec receiver pointer introduced only after guard; unchanged. |
| main_14_format_narrow_scope | 221 | 74 | Narrow address-taken format lifetime in lexical block; unchanged. |
| main_15_single_result_lifetime | 209 | 62 | Single result variable, destruction after branches;209bytes worse. |
| main_16_commutative_size_operands | 221 | 74 | Reversed commutative size operands; unchanged221. |
| main_17_inplace_size_and_named_result | 221 | 27 | Compound size calculation and named BOOL;221 but earlier coloring differs. |
| main_18_static_call_through_object | 221 | 74 | Static format method called through codec object; unchanged221. |
| main_19_sdk_lifetimes_after_parent | 122 | 0 | Genuine empty constructor/destructor below parent: compiler removes lifetime and EH122bytes; reject. |

19 builds,14 conservatively grouped source-shape families, no claiming flags as distinct source strategies. Selected simplest shared-header baseline. Extra LEA and three scratch-register/address-load placements remain; the first74 bytes match (excluding relocated operands). Parent ctor/dtor are genuine empty SDK bodies but making ctor visible removes its required call even under Ob0; therefore keep external. Same-TU format/Initialize bodies did not improve the selected parent and are not retained.

Self-contained native /Od /W3 without /EHsc:702 normal branch/boundary cases PASS. GCC64 C++03 noRTTI: same702 plus3exception-cleanup cases PASS. Channels0 is outside the native division precondition; no invented behavior added.

No new packing, class models, pragmas or shared-header edits. Remaining candidate IN_PROGRESS, not declared done merely because minimum attempts was met.

## Main continuation after 76-row acceptance

20 `main_20_single_element_codec_array`: one actual SDK codec in a one-element automatic array, accessed by reference. 294/217, introduces array lifetime machinery; rejected.
21 `main_21_named_bool_initialize`: normalize the Initialize BOOL result into a named bool before the error guard. 226/217, worsens the branch sequence; related to prior result-local strategy, not counted as an independent family.
22 `main_22_explicit_codec_receiver_after_initialize`: retain the BOOL result, then define the real codec receiver before testing and use it for Convert. 221/217 unchanged; related receiver-local control, no new family.
23 `main_23_complete_sdk_convert_body`: the genuine complete Microsoft SDK Convert implementation is visible before the wrapper, including all four modes and real array lifetimes. Wrapper remains221/217, same first difference74. Context Convert itself is1244/1244 but337 masked bytes differ, so neither helper nor parent is claimed matched. Its source was read in full and native entry/mode-dispatch corroborated; no fabricated helper or no-op was introduced. All source/COFF/headers/results retained.

Selected production and its 702-case tests remain unchanged. These four builds add two conservatively distinct families (array lifetime; full Convert visibility), for16 families and23 build probes. No new matched-byte credit.

24 `main_24_named_format_pointer` gives both format consumers one named actual SDK descriptor pointer;25 `main_25_named_destination_pointer` captures the output pointer before codec construction. Both canonicalize to the same221-byte candidate, first difference74. These are two additional pointer/dataflow controls within the prior receiver/local family, not two new strategy families. All25 build probes retained; selected baseline/test unchanged.

## Sol supplementary private retry

The selected wrapper, parent and self-contained fixture are unchanged. Thirteen fresh grade invocations comprise twelve source/control probes and one final preserved-baseline verification. The final baseline is not credited as a source strategy. All previous25 builds and16 grouped families remain preserved. Three conservatively grouped new families cover explicit shared failure labels/dispatch, scalar value initialization, and combined genuine SDK body visibility; arithmetic/local controls and equivalent branch spellings are not separate families. Total38 compiler invocations /19 families. Zero matched-byte credit.

| Grade | Bytes | First difference |
|---|---:|---:|
| sol_retry_adpcm_both_after_wrapper_01 | 294 | 62 |
| sol_retry_adpcm_combined_sdk_visibility_01 | 294 | 62 |
| sol_retry_adpcm_common_failure_01 | 221 | 63 |
| sol_retry_adpcm_conditional_return_01 | 224 | 74 |
| sol_retry_adpcm_consume_input_count_01 | 221 | 74 |
| sol_retry_adpcm_convert_after_wrapper_01 | 221 | 74 |
| sol_retry_adpcm_explicit_else_control_01 | 221 | 74 |
| sol_retry_adpcm_explicit_false_control_01 | 221 | 74 |
| sol_retry_adpcm_final_preserved_01 | 221 | 74 |
| sol_retry_adpcm_initialize_after_wrapper_01 | 294 | 62 |
| sol_retry_adpcm_sample_byte_count_01 | 223 | 52 |
| sol_retry_adpcm_switch_failure_01 | 225 | 62 |
| sol_retry_adpcm_value_initialization_01 | 221 | 74 |

The shared failure label moves return blocks but remains221; switch dispatch gives225. Scalar value initialization, explicit FALSE comparisons, explicit else and consuming the input byte-count variable canonicalize to the old221-byte object. Sample-count/sizeof(short) size calculation gives223. A conditional final return gives224. Combined actual SDK Initialize+Convert bodies before the wrapper, and Initialize alone or both bodies after it, give294 because Initialize expands. Moving only the complete Convert body after the wrapper remains221. No new flags, invented helpers, assembly, packing or headers were used. The complete1532-line SDK implementation and original153-line interface were read for this retry; complete main probe23 and all current probe histories were consulted. Context helper bodies are not claimed matched.

Fresh final grade sol_retry_adpcm_final_preserved_01 reproduces221 vs217, first difference74. Native sol_native_retry_adpcm_preserved_01 passes702 cases on RTM13.10.3077 /Od /W3 WITHOUT /EHsc, zero warnings. GCC64 GNU C++03 passes the same702 plus3 cleanup throws, zero warnings. Both consume the immutable final grade include tree. No full operand acceptance or complete root match is claimed; REGISTER_RESIDUE.json describes the extra receiver LEA and remaining scratch-register differences.

Historical-source gap: earlier main notes report reading old hand438. This retry could not independently locate that original file and does not infer or credit its experiments. The current25 main immutable histories, SDK sources and native evidence remain authoritative.

## Main continuation39–41
Signed decoded-result local alone, then signed block count plus decoded-result local (arithmetic still unsigned before assignment), both221/same first difference74. Standard lifetime extension of a genuine codec temporary through const reference plus mutable reference to the originally nonconst object also221/same. No invented class or helper; neither changes selected source/headers. Three more compiler probes, two broad strategy families (scalar-result type/dataflow and genuine temporary lifetime),41totalinvocations/21families; no new credit. Exact parent and additional native/relocation verification remain required.

## Sol nearmisses continuation42–66

Read the original SDK implementation/header in full, actual saved parent source shapes, current full PROBES/NAMING/ROW, and main39–41 before new probes. Recovered and read old hand438 source9492f92d7de7b667bb0a1d54f1892b320c89d774c3c78ca25b9ae448fdde6193 plus its NOTES/LESSONS/STATUS; this resolves the earlier historical-source gap. Its address-suffixed local models remain historical evidence only. No old undocumented experiments added to counts.

24 new controls/refinements42–65 plus final selected-baseline verification66:66total compiler invocations, conservative21families unchanged. No count inflation for equivalent pointer, branch, scalar type, array or flags spellings. All source/COFF/header snapshots retained; selected production/test/headers unchanged and no owned header deltas.

| Build | Bytes | First difference |
|---|---:|---:|
| sol_adpcm_42_uint_block_count | 221 | 74 |
| sol_adpcm_43_uint_all_counts | 221 | 74 |
| sol_adpcm_44_initialize_call_result_parenthesis | 221 | 74 |
| sol_adpcm_45_convert_call_result_parenthesis | 221 | 74 |
| sol_adpcm_46_destination_constref_after_initialize | 221 | 74 |
| sol_adpcm_47_source_constref_after_initialize | 221 | 74 |
| sol_adpcm_48_processor_G5 | 221 | 74 |
| sol_adpcm_49_processor_G7 | 222 | 33 |
| sol_adpcm_50_format_POD_array | 221 | 74 |
| sol_adpcm_51_receiver_reference_before_format | 221 | 74 |
| sol_adpcm_52_byte_pointer_arguments | 221 | 74 |
| sol_adpcm_53_single_iteration_breaks | 221 | 63 |
| sol_adpcm_54_scoped_success_returns | 221 | 74 |
| sol_adpcm_55_one_shot_while_success | 225 | 62 |
| sol_adpcm_56_common_success_label | 225 | 62 |
| sol_adpcm_57_const_integer_locals | 221 | 74 |
| sol_adpcm_58_parenthesized_receiver | 221 | 74 |
| sol_adpcm_59_size_optimization_control | 157 | 0 |
| sol_adpcm_60_reused_BOOL_status | 221 | 74 |
| sol_adpcm_61_status_outer_lifetime | 221 | 74 |
| sol_adpcm_62_format_reference_from_pointer | 221 | 74 |
| sol_adpcm_63_successful_return_comma | 224 | 74 |
| sol_adpcm_64_one_result_branch_status | 207 | 47 |
| sol_adpcm_65_nonconst_BOOL_initialize_only | 221 | 74 |
| sol_adpcm_66_frozen_selected_baseline | 221 | 74 |

UINT count-domain, scalar call-result parentheses, reference receiver/input/output, POD format array, typed byte pointers, reused/narrow/outer BOOL lifetimes and equivalent success/failure returns did not improve the selected221/217 residue. One-shot loop/common-success controls produce221/225 with earlier differences, conditional comma224, single-result207. /G5 stays221; /G7 gives222; /Os157 is rejected. None is a match.

Final66 exactly reproduces the selected source:221bytes/72instructions versus217/71, first difference74,11instruction hunks. Fresh native sol_native_adpcm_final66_702 passes702cases /Od/W3 WITHOUT /EHsc, zero warnings. GCC64 GNU03/O0/noRTTI passes the same702 plus3cleanup throws, zero warnings, using the identical immutable final grade include tree. Full acceptance operand audit is not claimed while the parent differs. Selected generated cleanup is never hand-authored. Qualified parked near miss,0credit; continued genuine source/context ideas remain welcome.

## Main67–69
Capacity-success scope containing the real codec lifetime remains221/217, first74. A complete Initialize&&Convert conditional result gives207, wrong return layout; a switch whose default owns the whole success path gives225. These are refinements of existing control/lifetime families, not three new independent families.69total normal grade invocations,21conservativefamilies. Selected221-byte source and702native/GCC+3throw fixture unchanged; no match credit.

## Main70–75
70lifetime-bound const-reference to the actual codec pointer and71to the SDK format pointer both221/same.72restores the four genuine inline SDK interface definitions under a private new opt-in; the setup failed because FABLE_FASTCALL belongs to rebuild_abi.h rather than fable_compiler.h.73fixes that include,221/same; original interface was reread fully, and source/header snapshots remain private.74uses the block product independently in capacity check and return:221but earlier coloring differs at27.75conditional initialized codec pointer gives227 with new branchless neg/sbb/and sequence, worse. No helper/context/header accepted; selected source/test remainunchanged.75total compiler invocations including retained setup failures;21conservative sourcefamilies unchanged (related alias/context/control refinements, not six new mechanisms).

## Main76 and owner wrap-up
Inverting only the final Convert guard into a successful return followed by failure produces225/217,firstdifference62; no improvement. Related return-layout control, not a new broad family.76normal invocations including prior setup/control/review snapshots;21conservativefamilies retained. Selected221-byte source and702-case fixture unchanged. Work frozen at owner request; no further retries,0credit.
