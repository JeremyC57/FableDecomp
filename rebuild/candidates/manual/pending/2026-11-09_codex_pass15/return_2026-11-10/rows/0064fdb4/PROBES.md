# 0064fdb4 ongoing reconstruction

No match or final parking decision. Selected source096 (original enum names on the same081 body) has2593/2593bytes and898/898instructions, with frame0x164 against retail0x16c. This uses the supplied original shared floor helper; no new machine-code body was authored. Both standalone test builds pass on the exact method body. The pure-C++ rounding twin is used by behavior tests; dual-path helper execution now passes under the original compiler, with16385 eighth-step values and15 exceptional/boundary inputs; the portable-only native build also passes.

## Actual source work

001/001b setup failures are not source strategies.001c authored the complete method;001d corrected original64-bit UID alignment without new packing.072 tested the existing original floor routine after the C++/intrinsic diagnostics below.073 was a corner-constructor investigation with an inactive macro;073b uses the actual existing CORNERS option.074 extended the two edge-sprite lexical lifetimes, recovering retail0x16c frame (two84-byte objects no longer overlap).075 is an inline-budget flag experiment: retail expands the real Clear body; plain size mode called it.076 uses the success guard with the final zero return, restoring retail exit placement.077 names geometric boxes before calls;078 names endpoint vectors. Historical probes preserve their exact inputs.

## Rounding diagnostics and limitations

All eleven native rounding sites were investigated together with the same trial helper spelling:0064ff98,0064ffd3,006500c1,006500f3,006501d1,0065020c,0065024a,0065027c,00650359,0065043c,00650544. Each is a bare dword integer store with the half/one adjustment and bitwise integer-boundary correction of original GFFloatToLongFloor. Both PDB identity and matching native out-of-line routine0048d940 are documented in the supplied helper header.

002–031 are instruction-generation diagnostics, not selected implementations: their probe helper accidentally used the ordinary C convention, and021 failed on an ambiguous library overload.032–061 reran the30 spelled forms under the exact helper fastcall convention, also probing a larger inline budget after observing unwanted out-of-line calls. These are30 syntactic spellings, not30 independent mathematical algorithms: several canonicalize identically; plain casts/half casts do not implement general floor semantics and are rejected for delivery.062–071 are10 genuine conversion-intrinsic families (scalar/packed,single/double,nearest/truncating,MMX/SSE/SSE2) with a final floor adjustment. They emit SIMD conversion instructions or out-of-line helpers, not the native dword x87 sequence. No claim is made that these diagnostic functions are valid replacements at all exceptional inputs. The selected shared helper retains its supplied exact portable twin.

A method grade counts the full row; it is not an isolated conversion score. The helper-emission column inspects the helper COFF section where one exists. If no helper COMDAT exists, its casts were expanded into the row; row color/alpha casts also call the CRT, so aggregate row conversion counts would conflate unrelated operations.

| Trial | Result | Bytes | Strict hunks | Helper conversion evidence |
|---|---|---|---|---|
|001_complete_portable_rounding|COMPILE_FAIL|-/-|-||
|001b_complete_shared_chain|COMPILE_FAIL|-/-|-||
|001c_state_accessor|DIFFER|2098/2593|124|?X87FistpNear@fable_portable@@YAJN@Z|
|001d_proven_uid_alignment|DIFFER|2098/2593|124|?X87FistpNear@fable_portable@@YAJN@Z|
|002_cpp_rounding_long_cast|DIFFER|2041/2593|130|__ftol2|
|003_cpp_rounding_int_cast|DIFFER|2041/2593|130|__ftol2|
|004_cpp_rounding_short_cast|DIFFER|2068/2593|124|__ftol2|
|005_cpp_rounding_unsigned_cast|DIFFER|2041/2593|130|__ftol2|
|006_cpp_rounding_float_temp|DIFFER|2041/2593|130|__ftol2|
|007_cpp_rounding_double_temp|DIFFER|2037/2593|130|__ftol2|
|008_cpp_rounding_double_cast|DIFFER|2041/2593|130|__ftol2|
|009_cpp_rounding_wide_integer_cast|DIFFER|2041/2593|130|__ftol2|
|010_cpp_rounding_half_offset_cast|DIFFER|2091/2593|130|__ftol2|
|011_cpp_rounding_half_subtract_cast|DIFFER|2091/2593|130|__ftol2|
|012_cpp_rounding_floor_double|DIFFER|2116/2593|133|_floor; __ftol2|
|013_cpp_rounding_floor_float_result|DIFFER|2116/2593|133|_floor; __ftol2|
|014_cpp_rounding_floor_int_result|DIFFER|2116/2593|133|_floor; __ftol2|
|015_cpp_rounding_floor_unsigned_result|DIFFER|2116/2593|133|_floor; __ftol2|
|016_cpp_rounding_ceil_adjust|DIFFER|2116/2593|133|_ceil; __ftol2|
|017_cpp_rounding_trunc_correct|DIFFER|2098/2593|124|__ftol2|
|018_cpp_rounding_sign_trunc_correct|DIFFER|2098/2593|124|__ftol2|
|019_cpp_rounding_double_trunc_correct|DIFFER|2098/2593|124|__ftol2|
|020_cpp_rounding_split_modf|DIFFER|2116/2593|133|_modf; __ftol2|
|021_cpp_rounding_remainder_subtract|COMPILE_FAIL|-/-|-||
|022_cpp_rounding_negative_ceil|DIFFER|2116/2593|133|_ceil; __ftol2|
|023_cpp_rounding_positive_abs_branch|DIFFER|2116/2593|133|_ceil; __ftol2|
|024_cpp_rounding_ceil_minus_fraction|DIFFER|2116/2593|133|_ceil; __ftol2|
|025_cpp_rounding_double_floor_temp|DIFFER|2116/2593|133|_floor; __ftol2|
|026_cpp_rounding_half_round_fix|DIFFER|2098/2593|124|__ftol2|
|027_cpp_rounding_floor_abs_branch|DIFFER|2116/2593|133|_floor; __ftol2; _ceil|
|028_cpp_rounding_floor_ldexp|DIFFER|2116/2593|133|_ldexp; _floor; __ftol2|
|029_cpp_rounding_floor_frexp|DIFFER|2116/2593|133|_frexp; _ldexp; _floor; __ftol2|
|030_cpp_rounding_int_then_float_fix|DIFFER|2098/2593|124|__ftol2|
|031_cpp_rounding_wide_then_float_fix|DIFFER|2098/2593|124|__ftol2|
|032_cpp_inline_rounding_long_cast|DIFFER|2041/2593|130|__ftol2|
|033_cpp_inline_rounding_int_cast|DIFFER|2041/2593|130|__ftol2|
|034_cpp_inline_rounding_short_cast|DIFFER|2068/2593|124|__ftol2|
|035_cpp_inline_rounding_unsigned_cast|DIFFER|2041/2593|130|__ftol2|
|036_cpp_inline_rounding_float_temp|DIFFER|2041/2593|130|__ftol2|
|037_cpp_inline_rounding_double_temp|DIFFER|2037/2593|130|__ftol2|
|038_cpp_inline_rounding_double_cast|DIFFER|2041/2593|130|__ftol2|
|039_cpp_inline_rounding_wide_integer_cast|DIFFER|2041/2593|130|__ftol2|
|040_cpp_inline_rounding_half_offset_cast|DIFFER|2091/2593|130|__ftol2|
|041_cpp_inline_rounding_half_subtract_cast|DIFFER|2091/2593|130|__ftol2|
|042_cpp_inline_rounding_floor_double|DIFFER|2115/2593|135|_floor; __ftol2|
|043_cpp_inline_rounding_floor_float_result|DIFFER|2115/2593|135|_floor; __ftol2|
|044_cpp_inline_rounding_floor_int_result|DIFFER|2115/2593|135|_floor; __ftol2|
|045_cpp_inline_rounding_floor_unsigned_result|DIFFER|2115/2593|135|_floor; __ftol2|
|046_cpp_inline_rounding_ceil_adjust|DIFFER|2115/2593|135|_ceil; __ftol2|
|047_cpp_inline_rounding_trunc_correct|DIFFER|2095/2593|126|__ftol2|
|048_cpp_inline_rounding_sign_trunc_correct|DIFFER|2095/2593|126|__ftol2|
|049_cpp_inline_rounding_double_trunc_correct|DIFFER|2095/2593|126|__ftol2|
|050_cpp_inline_rounding_split_modf|DIFFER|2115/2593|135|_modf; __ftol2|
|051_cpp_inline_rounding_remainder_subtract|DIFFER|2095/2593|126|__CIfmod; __ftol2|
|052_cpp_inline_rounding_negative_ceil|DIFFER|2115/2593|135|_ceil; __ftol2|
|053_cpp_inline_rounding_positive_abs_branch|DIFFER|2115/2593|135|_ceil; __ftol2|
|054_cpp_inline_rounding_ceil_minus_fraction|DIFFER|2115/2593|135|_ceil; __ftol2|
|055_cpp_inline_rounding_double_floor_temp|DIFFER|2115/2593|135|_floor; __ftol2|
|056_cpp_inline_rounding_half_round_fix|DIFFER|2095/2593|126|__ftol2|
|057_cpp_inline_rounding_floor_abs_branch|DIFFER|2115/2593|135|_floor; __ftol2; _ceil|
|058_cpp_inline_rounding_floor_ldexp|DIFFER|2115/2593|135|_ldexp; _floor; __ftol2|
|059_cpp_inline_rounding_floor_frexp|DIFFER|2115/2593|135|_frexp; _ldexp; _floor; __ftol2|
|060_cpp_inline_rounding_int_then_float_fix|DIFFER|2095/2593|126|__ftol2|
|061_cpp_inline_rounding_wide_then_float_fix|DIFFER|2095/2593|126|__ftol2|
|062_intrinsic_rounding_sse_scalar_round|DIFFER|2095/2593|126|cvtss2si edx, xmm0|
|063_intrinsic_rounding_sse_scalar_truncate|DIFFER|2095/2593|126|cvttss2si edx, xmm0|
|064_intrinsic_rounding_sse_packed_round_mmx|DIFFER|2115/2593|135|cvtps2pi mm0, xmm0; emms |
|065_intrinsic_rounding_sse_packed_truncate_mmx|DIFFER|2115/2593|135|cvttps2pi mm0, xmm0; emms |
|066_intrinsic_rounding_sse2_scalar_double_round|DIFFER|2095/2593|126|cvtsd2si edx, xmm0|
|067_intrinsic_rounding_sse2_scalar_double_truncate|DIFFER|2095/2593|126|cvttsd2si edx, xmm0|
|068_intrinsic_rounding_sse2_packed_double_round|DIFFER|2095/2593|126|cvtpd2dq xmm0, xmm0|
|069_intrinsic_rounding_sse2_packed_double_truncate|DIFFER|2095/2593|126|cvttpd2dq xmm0, xmm0|
|070_intrinsic_rounding_sse2_packed_float_round|DIFFER|2095/2593|126|cvtps2dq xmm0, xmm0|
|071_intrinsic_rounding_sse2_packed_float_truncate|DIFFER|2095/2593|126|cvttps2dq xmm0, xmm0|
|072_shared_original_floor_helper|DIFFER|2457/2593|179|fistp dword ptr [ebp - 4]|
|073_vector_corner_boxes|DIFFER|2460/2593|192|fistp dword ptr [ebp - 4]|
|073b_existing_corner_optin|DIFFER|2511/2593|156|fistp dword ptr [ebp - 4]|
|074_function_scope_edge_sprites|DIFFER|2541/2593|144|fistp dword ptr [ebp - 4]|
|075_visible_clear_budget_S200|DIFFER|2573/2593|145|fistp dword ptr [ebp - 4]|
|076_success_guard_tail_zero|DIFFER|2582/2593|143|fistp dword ptr [ebp - 4]|
|077_named_box_before_arguments|DIFFER|2582/2593|166|fistp dword ptr [ebp - 4]|
|078_named_endpoint_vectors|DIFFER|2582/2593|149|fistp dword ptr [ebp - 4]|
|079_original_bitset_call|DIFFER|2596/2593|155|fistp dword ptr [ebp - 4]|
|080_real_alive_body|DIFFER|2598/2593|114|fistp dword ptr [ebp - 4]|
|081_actual_alive_call_budget|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|082_const_hero_pointer|DIFFER|2596/2593|155|fistp dword ptr [ebp - 4]|
|083_endpoint_const_references|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|084_size_const_references|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|085_hero_temporary_reference|DIFFER|2596/2593|155|fistp dword ptr [ebp - 4]|
|086_scalar_fill_coordinates|DIFFER|2593/2593|166|fistp dword ptr [ebp - 4]|
|087_converted_const_pointer_argument|DIFFER|2596/2593|155|fistp dword ptr [ebp - 4]|
|088_branch_box_lifetimes|DIFFER|2593/2593|165|fistp dword ptr [ebp - 4]|
|089_original_distinct_positions|DIFFER|2593/2593|130|fistp dword ptr [ebp - 4]|
|090_original_rectangles_and_width_accessor|DIFFER|2592/2593|159|fistp dword ptr [ebp - 4]|
|091_original_named_inner_box|DIFFER|2593/2593|107|fistp dword ptr [ebp - 4]|
|092_original_right_height_scope|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|093_const_original_middle_box|DIFFER|2592/2593|149|fistp dword ptr [ebp - 4]|
|094_named_middle_geometry_cached_width|DIFFER|2588/2593|149|fistp dword ptr [ebp - 4]|
|095_selected_metadata_verification|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|096_original_render_enumerators|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|097_complete_native_sprite_constructor|DIFFER|2898/2593|156|fistp dword ptr [ebp - 4]|
|098_visible_sprite_lower_inline_budget|DIFFER|2898/2593|156|fistp dword ptr [ebp - 4]|
|099_original_out_of_line_sprite|DIFFER|2593/2593|114|fistp dword ptr [ebp - 4]|
|100_original_named_boxes_visible_ctor|DIFFER|2588/2593|160|fistp dword ptr [ebp - 4]|

## New authored investigations079–094

079 restores the native bitset::test call using the existing header option.080 provides the faithful null/dead predicate body; it removes the extra capture spill but gets expanded.081 retains the native predicate call with the current owner inline ladder, while allowing the actual Clear body to expand.082/085/087 test const-pointer/reference call forms without the visible body; they canonicalize back to079.083/084 reference endpoints/sizes are identical to081.086 splits fill coordinates into scalars;088 names the conditional geometry boxes; neither improves.089 follows original donor distinct right/inner position locals.090 reconstructs all donor named rectangles plus GetWidth;091 isolates the named inner rectangle;092 restores the donor right-height scope;093/094 use a persistent middle box with repeated or cached width. These are measured non-matches; donor debug/release locals are preserved as evidence and their frame layouts are not asserted identical to the native PC binary.095 changes the heading only and verifies selected081 again.096 uses proven original enum names without changing bytes.097/098 expose the complete actual sprite constructor, which gets expanded at both inline budgets.099 preserves its native out-of-line calls and is byte-identical to096.100 combines the full constructor with donor-named boxes and is worse. The selected source remains096.

## Behavior tests

576 rendering scenarios cover both optional graphics, all three scales/GUI alpha values, four signed fill amounts, target highlighting, render-call order and literal fields. Five early exits cover missing/dead/in-limbo things and missing/dead hero.81 quarter-step floor inputs check the copied portable twin. Tests preserve the native second-icon quirk: its height comes from the first icon height. Both compilers pass. This is behavior validation, not a byte-match claim: stack offsets and one final-fill instruction placement region remain different.


## Independent lifetime and geometry investigation103–133

Selected096 remains unchanged. These experiments use separate runtime sources; no diagnostic object replaces the normal compiler result. The 106/107/123 actual frame traces validate byte-for-byte against the ordinary RTM objects.

103 restores donor scalar scaled-height/width locals in native right-to-left floor order and canonicalizes to096.104 directly constructs the primary endpoint;105 gives its rectangle a name; neither improves.106 restores four independent early position snapshots from the original position. This produces the native0x16c frame and exact three sprite homes, but adds two real early copy instructions, changes other slot priorities, and remains a non-match.107 copying inner from bar permits right/inner storage merging and returns to0x164.108 initializes only the inner Y snapshot before later assigning X (all read components are defined); it is byte-identical to106.109 moves that inner copy before the optional right-icon call and still coalesces.

110–113 use genuine vector translation/addition for the fill or bar steps. They can increase use weights enough to place the definition/inner position at their native homes, but also introduce nonnative operations.114–116 use the existing proven C2DBoxF point/extents constructor instead of corner vectors; instruction counts diverge.117/118 use the real C2DVector::Initialise operation and canonicalize to their respective096/106 inputs.119/120 reconstruct the translated fill point; the extra stores remain.121–123 delay the separate right-position assignment or construction into its actual guard, retaining the original early inner snapshot.123 has898 instructions and0x164 frame: its right position shares the icon/size slot, while the inner position remains a separate eight-byte slot of weight12. Thus merely giving the two positions distinct source names does not produce the native frame.

124/125 expose the complete proven native box-sprite constructor as an inline COMDAT or an in-class body while preserving native out-of-line calls with the existing owner inline ladder. Both reproduce096 exactly; visible noncapture semantics alone do not alter this frame. These differ from the earlier ordinary out-of-class definition in099, but produce the same measured result.126/127 combine independent positions with the named fill rectangle.126 has the native frame and98.89% loose agreement with only eight loose hunks; it still has2598 bytes/900 instructions versus2593/898, wrong scalar homes, two extra early copies, and final-fill argument scheduling differences. This diagnostic is not selected.128/129 use the proven scalar-corner rectangle constructor; they canonicalize to096/106.130–133 accumulate the origin into the size or accumulate the extents into a copied endpoint using the genuine vector addition-assignment operator; all canonicalize to the096/106 input stream. Canonicalized spellings and constructor setup/visibility checks are not counted as new successful source-shape strategies.

|History|Bytes|Instructions|Loose agreement|
|---|---:|---:|---:|
|103_original_scaled_icon_scalar_types|2593|898|97.77%|
|104_scaled_icon_direct_endpoint|2590|898|96.77%|
|105_scaled_icon_original_named_box|2593|898|96.99%|
|106_original_four_position_lifetimes|2598|900|98.11%|
|107_inner_position_copy_from_bar|2593|898|97.77%|
|108_defined_inner_component_lifetimes|2598|900|98.11%|
|109_inner_position_before_right_call|2593|898|97.77%|
|110_fill_vector_horizontal_translation|2599|900|97.55%|
|111_four_lifetimes_vector_translation|2604|902|99.00%|
|112_fill_vector_addition_assignment|2611|904|97.67%|
|113_bar_vector_horizontal_steps|2605|902|97.11%|
|114_fill_real_extents_constructor|2585|895|96.82%|
|115_fill_extents_from_vector_dimensions|2589|897|96.94%|
|116_real_extents_for_independent_rectangles|2551|881|89.49%|
|117_fill_real_point_initialise|2593|898|97.77%|
|118_four_lifetimes_point_initialise|2598|900|98.11%|
|119_four_lifetimes_point_reconstruction|2616|906|99.11%|
|120_fill_point_reconstruction|2611|904|97.67%|
|121_defined_late_right_assignment|2605|902|97.67%|
|122_right_assignment_after_available_floor|2605|902|97.78%|
|123_right_position_construction_in_guard|2593|898|98.22%|
|124_faithful_inline_comdat_ctor|2593|898|97.77%|
|125_faithful_inclass_ctor|2593|898|97.77%|
|126_four_positions_named_fill_box|2598|900|98.89%|
|127_guarded_right_named_fill_box|2593|898|97.44%|
|128_fill_scalar_corner_constructor|2593|898|97.77%|
|129_four_positions_scalar_fill_corners|2598|900|98.11%|
|130_fill_size_accumulates_origin|2593|898|97.77%|
|131_four_positions_size_accumulates_origin|2598|900|98.11%|
|132_fill_endpoint_accumulates_extents|2593|898|97.77%|
|133_four_positions_endpoint_accumulates_extents|2598|900|98.11%|

134–137 initialize the real public rectangle fields explicitly, first storing the top-left snapshot and then either the named endpoint or direct endpoint sums. They test a different dependency graph after the actual126 scheduler trace showed the named rectangle moves UV setup across endpoint stores. The four forms emit2590/2595/2582/2587 bytes and897/899/894/896 instructions; none restores the required native argument scheduling. All rectangle fields are assigned before the sprite consumes them. No source with partially initialized consumed geometry is used.

|History|Bytes|Instructions|Loose agreement|
|---|---:|---:|---:|
|134_fill_explicit_rectangle_fields|2590|897|96.49%|
|135_fill_explicit_rectangle_fields|2595|899|97.94%|
|136_fill_direct_rectangle_fields|2582|894|96.09%|
|137_fill_direct_rectangle_fields|2587|896|97.55%|

Checkpoint: all103–137 histories retain their source, copied headers, normal RTM object, disassemblies, relocations and comparison results. No new candidate has replaced selected096, so its previously passing exact-body tests and relocation evidence remain applicable. These experiments establish failures of the tested source lifetime and geometry forms; they do not prove that a matching portable original form is impossible.

138–140 commute the genuine vector-plus operands for the fill endpoint on096,106 and126. All three canonicalize exactly to their respective inputs. Original vector return qualification was checked directly rather than guessed: FableWin supplies `??HC2DVector@@QBE?AV0@ABV0@@Z` and `??YC2DVector@@QAEABV0@ABV0@@Z`, confirming non-const by-value plus and const-reference addition-assignment, exactly as the shared header declares. Ego_r has no corresponding public symbol records in this query. Extracted textual evidence is in ORIGINAL_VECTOR_PLUS_SYMBOLS.json. No speculative const-return header was authored.


## Branch-result dataflow investigation141–147

The successful CODA two-arm-store mechanism suggested a specific follow-up rather than more constructor spelling probes.141 uses natural scalar `if(max_graphic_height<new_height) max_graphic_height=new_height` updates instead of the reference-returning std::max body;142 uses scalar conditional results. Both canonicalize together, emitting2592 bytes/893 instructions and retaining0x164 frame. This removes five native instructions and changes many slot/use relationships, so it is not an improvement.143 moves the highlight dimensions/layer into complete assignments in each existing target/dead branch, with no extra condition or artificial operation. All dimension values are defined before use. It emits2587/896, still frame0x164: it removes the native pre-target copies/default-layer assignment instead of recovering the missing slot.144 combines these two mechanisms and is worse.

145/146 separately define optional left dimensions or right width in both existing graphic branches, instead of preinitializing zero before the query guard.147 combines those result assignments. They emit2595/899,2595/899 and2597/900, respectively, and retain0x164 frame. The zero stores move to absent-graphic arms and the guard CFG adds instructions; native needs the early zero stores. Original recovered donor names/types are used for all named persistent values in these runtime candidates. Canonicalized scalar conditional form142 and combination checks144/147 are not counted as additional independent mechanisms.

|History|Bytes|Instructions|Loose agreement|
|---|---:|---:|---:|
|141_scalar_maximum_result_branches|2592|893|93.91%|
|142_scalar_conditional_maxima|2592|893|93.91%|
|143_complete_highlight_branch_results|2587|896|97.66%|
|144_highlight_and_maximum_result_branches|2586|891|92.90%|
|145_optional_left_dimension_branch_results|2595|899|97.27%|
|146_optional_right_width_branch_result|2595|899|97.61%|
|147_both_optional_dimension_results|2597|900|97.11%|

Measured unresolved constraints remain:096 has all898 native instructions/2593 bytes but frame0x164 rather than0x16c and incorrect scalar homes; its final fill geometry/UV pointer pushes are interleaved differently. Four independent early position lifetimes106/126 restore native0x16c and three sprite homes, but necessarily retain two extra early copies in those tested forms. Moving the separate right-position construction into its guard123 eliminates those extra instructions, yet C2 merges its home with earlier icon/size storage and returns to0x164. Complete result assignments141–147 do not break that conflict/use-weight tradeoff. No candidate in this batch improves096, and its source/test remain unchanged.

## Main follow-up148–150

148 restores a function-scope original bar_middle_box, constructs its endpoint directly without a named middle-size vector, and uses its GetWidth twice.149 makes that box const. Both canonicalize the earlier middle-box mechanism:2592bytes,899instructions,frame174 (native16c).150 exposes the same complete genuine sprite constructor already preserved in099;2588bytes,897instructions,frame164. The expected8-byte reduction did not occur: full visibility removes16. None selected;096/tests unchanged.


## Fresh original-type and lifetime investigation151–159

Selected096 source, selected headers and standalone test remain unchanged. Native initial geometry has three X/Y snapshots; the final inner Y load uses bar_tl_pos.Y, while the earlier right-icon Y load uses right_sprite_pos.Y. The original release donor likewise homes right_sprite_pos and inner_bar_tl_pos together. This supports testing a late copy from bar_tl_pos, rather than assuming an independently initialized fourth early snapshot is original. It does not establish a matching stack layout.

Fresh complete raw type extracts are in ORIGINAL_GEOMETRY_COPY_TYPES.json and ORIGINAL_PRIMITIVE_LIFETIME_TYPES.json. Both donors retain the genuine C2DBoxF::Initialise corner overload and authored box/vector copy constructors. Neither sprite, primitive base nor theme descriptor has an authored destructor; no empty destructor or additional member was added. The box Initialiser body supplied in151 copies all four actual fields, not a capture-only stand-in. The vector copy in154 delegates to the genuine complete Initialise operation.

|History|Specific evidence/source lever|Bytes|Instructions|Frame|Result|
|---|---|---:|---:|---|---|
|151_original_fill_box_initialise|Real named inner box with original Initialise(corners)|2593|898|164|Identical091|
|152_late_inner_copy_original_fill_box|Late inner copy from bar position plus original named inner box|2593|898|164|111strict/20loose hunks|
|153_original_box_copy_visible_sprite|Enable proven authored box copy in150's complete sprite closure|2588|897|164|Identical150|
|154_original_vector_copy_initialise|Genuine vector copy delegates to original Initialise(float,float)|2593|898|164|Identical096|
|155_middle_box_inline_comdat_sprite_ctor|Actual full sprite body as inline COMDAT with persistent middle box|2588|897|164|Identical150|
|156_original_floor_local_declarations|Original floor local names/types and debug-home declaration/initializer order|2593|898|164|Identical096|
|157_bar_advance_from_constructed_endpoint|Reuse genuine already constructed left endpoint after callback|2575|890|178|Regresses; removes8native instructions|
|158_original_left_box_endpoint_reuse|Original named left box/GetBRX with complete inline sprite body|2575|890|178|Regresses; removes8native instructions|
|159_original_sprite_enum_member_types|Restore both proven enum field types in existing retail branch|2593|898|164|Identical096; header correction available separately|

156 preserves every existing shared floor instruction, changing only C++ declaration/initializer layout and proven original local names. ORIGINAL_FLOOR_LOCALS.json gives FableWin's float f, float half, long i, float one and float i_plus_one; no Ego_r procedure record was found by the same query. Debug homes are not asserted equal to the optimized PC helper. The new instruction block was independently checked equal to the supplied block after renaming identifiers; the portable twin remains unchanged. This evidence-based source check supplies no frame improvement and is not selected.

159 fixes actual owning-header member types: both donor sprite records say EEngine2DLayer Layer at1c and EEnginePrimitiveBlendMode BlendMode at30, where the existing header uses long. The separate retry header uses the existing ENUM_CTORS branch, retaining the original default field declarations byte-for-byte. Its entire normal leaf equals096. All remaining sprite fields/names/types and84-byte size agree. The shared primitive base's three data members are protected in both donors, while its current shared declarations are public; that existing access debt was recorded, not changed in this batch.

All163 normal grade histories are preserved. These nine measured checks include canonicalized spellings and combinations, not nine independent successful mechanisms. No better source or byte match is claimed. RESUME_BATCH_151_159.json records hashes, frames and prior identical streams.

Independent resumed tests verify the exact selected DrawAt body in the fixture, then pass VC7.1/Od/W3/GR- and g++ C++03/O2 with strict enums and address/undefined sanitizers:576 controlled rendering combinations,5 early exits and81 portable floor inputs. Separate RESUME_TEST_* logs retain the new runs. Existing dual-path floor evidence remains applicable because selected helper/test are unchanged. These tests do not establish a byte match or coverage of every possible game callback/configuration.

BOUNDARY_COVERAGE.md records the exact selected rendering/floor boundaries, unexercised middle-width/epsilon/invalid-component paths and the PC-address trace for native right/inner X reuse with bar-position Y. These limits and the new native finding must survive the Claude handoff.


## Authorized middle-boundary fixture extension (no production grade)

After batch151–159, the parent requested closing the known middle-width coverage gap. The previous fixture/logs/metadata are preserved under test_history/001_before_middle_boundaries. The exact selected096 function body and production source are unchanged. The fixture now accepts a definition-width input and conditions the expected middle-piece draw on its signed width. It seeds handle3's Clears field, asserts its index/state stay unchanged, and checks handle3 is absent from rendering calls when skipped. The right edge still uses signed middle width, and fill remains signed rather than normalized.

25 new cases cover all four icon pairs at exact middle widths−1/0/+1 with fill absent/present (24), plus a backwards fill box at available7, middle−9 and fill width−0.25 (1). Counts are8 zero,9 negative and8 adjacent positive middle widths. VC7.1/Od/W3/GR- and optimized g++ C++03 with strict enums and address/undefined sanitizers both pass. Current TEST_RESULTS.json reports601 rendering cases (576 prior+25 new),5 early exits and81 floor checks. No production source/header edit or new grade was needed. The native inner-Y finding and remaining coverage limits stay in BOUNDARY_COVERAGE.md. New fixture SHA256:5f8704c18f545f49a25b0725c57d8c8668ef68e98190aa3d1016c5bc3820f20a.

Final handoff: selected body retained, with required line-2 DIFFER annotation. FINAL_003_handoff_metadata recompiles this comment-only change using the integrated physical include tree. It is a verification grade, not another source-shape attempt.

Final authority: CURRENT_VERIFICATION.json and root STATUS.tsv supersede historical selected/pending paragraphs. Final compile FINAL_003_handoff_metadata preserves the chosen body; later timer037/038 do not improve it.
