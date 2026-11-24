# CEnginePrimitiveManagerRepeatedZSprites::RenderFromSortList at 00BAA870

The original FableWin procedure at 0x02ED3E30 and Ego_r procedure at 0x00A61800 name a private member accepting a repeated Z-sprite primitive by reference. The Ego_d records give the 38 locals and their source-line groups. FableWin confirms the inverse non-homogenous view transform of the initial back vector, the base and top control points, the separate front and back texture projections, and the normalized vertical and horizontal control point weights. The source reconstructs the documented sphere center, diagonal, radius, half-FOV, clip distance, viewport scale, near fade, and far fade calculations.

The original helper uses the in-place `CMatrix3x4::TransformPoint` at 0x024543E0 and the two-argument overload at 0x01D129F0. The common headers supply both. FableWin 0x02ED7600 proves that the matrix constructor calls the existing `CPreTransposedBoneMatrix::Set` member. The donor viewport constant at 0x04022F24 is 32.0f, and the angular scale at 0x04071768 is the float 6.2831855f promoted to double. The original `GetTransitionFade` callee emits the same 112 bytes and operand as the retail body at 0x00BAA460.

| Successful grade | Result | Shape under test |
| --- | --- | --- |
| archived_header_rebase | 4,600 bytes, 14 masked differences | Transported source. Three unused float locals, a spare counted pointer and four duplicate assertions lack original source evidence. |
| remove_unsupported_locals | 4,600 bytes, 41 hunks | Remove the unsupported locals and assertions. |
| donor_weight_normalisation | 4,600 bytes, 37 hunks | Restore the source-proven total weight and independent divisions. |
| spell02_original | 4,605 bytes, 162 hunks | Restore the inverse transform, bounding geometry, half FOV, viewport scale, fade locals, and the original helper calls. |
| spell03_retail_precision | 4,600 bytes, 34 hunks | Select the donor-proven left-associated in-place matrix helper and the float constants and reciprocal arithmetic found in retail. |
| spell04_vector_operators | 4,621 bytes, 109 hunks | Use alternate genuine C3DVector operator bodies that construct their result through Initialise. |
| spell05_index_accessors | 4,584 bytes, 31 hunks | Restore the FableWin index-buffer accessors and type the original enum field. |
| spell06_inverse_sum_order | 4,584 bytes, 31 hunks | Try the existing equivalent Z/Y/X in-place transform sums. |
| spell07_radius_division | 4,584 bytes, 31 hunks | Express the projected radius and alpha as the donor's divisions. |
| spell08_length_squares | 4,586 bytes, 31 hunks | Try the original squared-length grouping seen in another caller. One unreferenced helper owner differs. |

Ten successful grades preceded the owner’s authorization to continue. The baseline, archive1 and archive2 compilation failures are preserved. The index-accessor header experiment is preserved in its grade snapshots. The selected candidate retains the inherited index buffer field access code, which reads the same original enum at offset 0x0C.

The previous selection `spell03_retail_precision` uses the standard VC7.1 release flags /O2 /Oy /W3 and has 2,127 positional masked byte differences because the instruction offsets diverge through the leaf. All 155 linked operand identities and four Direct3D virtual slots agree by instruction and target. Seventy mapped local branch targets agree. A short jump to the aligned texture-loop entry replaces the end of the original six-byte NOP at 0x00BAB93A. The residual differences also include x87 term and spill scheduling, a hoisted shader manager receiver load and render-state bitmask register choices. All three mapped pick-any bodies agree.

Additional probes after authorization

| Grade | Bytes and normalized hunks | Source shape |
| --- | --- | --- |
| spell09_original_pretranslate_lhs | 4,600 bytes, 34 hunks | Use the original operand order in each of the three PreTranslate rows. |
| spell10_original_pretranslate_group | 4,600 bytes, 45 hunks | Parenthesize the left-associated X/Y pair in all three rows. |
| spell11_pretranslate_z_association | 4,600 bytes, 29 hunks | Try Z/Y-first arithmetic in the third row. Matches more retail instructions but changes the debug association. |
| spell12_pretranslate_original_products | 4,600 bytes, 38 hunks | Parenthesize each product in the third row. |
| spell13_pretranslate_z_chain | 4,600 bytes, 28 hunks | Try a Z/Y/X expression in the third row. |
| spell14_stock_accumulate | 4,600 bytes, 34 hunks | Use the shared PreTranslate compound-assignment option. |
| spell15_third_accumulate | 4,600 bytes, 34 hunks | Use the original operand order and compound assignment in the third row only. |
| spell16_third_left_group | 4,600 bytes, 31 hunks | Group the left-associated X/Y terms in the third row. |
| spell17_third_whole_group | 4,600 bytes, 31 hunks | Group the complete left-associated dot product in the third row. |
| spell18_third_z_first_group | 4,600 bytes, 29 hunks | Group Z/Y before X in the third row. Retained as an association probe. |
| spell19_third_original_ungrouped | 4,600 bytes, 38 hunks | Parenthesize the original individual products in the third row. |
| spell20_third_nested_term_products | 4,600 bytes, 37 hunks | Parenthesize each product within a nested Z/Y sum in the third row. |
| spell21_original_normalise_sqrs | 4,584 bytes, 29 hunks | Use the original GFSqr calls in Normalise. |
| spell22_original_normalise_grouped_sqrs | 4,584 bytes, 29 hunks | Use the shared grouped GFSqr variant in Normalise. |
| spell23_original_sphere_center | 4,600 bytes, 20 hunks | Use the original public Center member for PreTranslate. Fixes the complete PreTranslate and view matrix schedule. |
| spell24_original_posts_scale_divisions | 4,589 bytes, 89 hunks | Use the debug double divisions for PostScale after restoring the direct Center member. |
| spell25_original_center_left_group | 4,600 bytes, 27 hunks | Combine the original Center use with the third-row left grouping. |
| spell26_original_center_z_association | 4,600 bytes, 25 hunks | Combine the original Center use with the third-row Z/Y-first grouping. |
| spell27_original_backvec_sqr | 4,600 bytes, 16 hunks | Use original GFSqr calls with the original direct Center use. Normalise is exact through the first TransformPoint difference. |
| spell28_original_backvec_sqr_group | 4,600 bytes, 18 hunks | Group the three GFSqr calls after restoring the original Center use. |
| spell29_normalise_component_values | 4,600 bytes, 22 hunks | Read the Normalise components into the shared helper’s three named values. |
| spell30_normalise_named_squared | 4,600 bytes, 16 hunks | Use the shared named squared-length result and single grouped X/Y addition. |
| spell31_normalise_inline_sum | 4,586 bytes, 44 hunks | Use the shared inline Z/X/Y sum for Normalise. |
| spell32_normalise_grouped_components | 4,586 bytes, 45 hunks | Use three explicitly grouped component squares. |
| spell33_normalise_unparenthesized | 4,586 bytes, 44 hunks | Use the shared unparenthesized Normalise expression. |
| spell34_normalise_original_values_xyz | 4,600 bytes, 18 hunks | Read components into values and sum the squares in original X/Y/Z order. |

FableWin 0x02ED405E–0x02ED4074 performs unary minus on the bounding sphere’s direct Center member and passes the result to PreTranslate. The later local sphere_center_object_space uses the original GetCenter call. FableWin 0x01ACDFD0 identifies Normalise and its three GFSqr calls. Together those changes extend the original schedule through 0x00BAAE27. This intermediate leaf retains 4,600 bytes and 1,180 of 1,198 aligned instructions, and all three mapped helpers and GetTransitionFade agree. All 155 relocated operands, 70 mapped local transfers and the four device slots agree. The alternate texture-loop entry jump remains.

| Grade | Object bytes and instructions | Source shape |
| --- | --- | --- |
| spell35_transform_original_product_lhs | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use the donor multiplication operand order in every TransformPoint output. |
| spell36_transform_original_y_lhs | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use the donor multiplication operand order only for output Y. |
| spell37_transform_original_y_left_group | 4,574 bytes, 23 hunks, 3EQ0NEQ | Group the first two products of output Y. |
| spell38_transform_original_y_group | 4,600 bytes, 27 hunks, 3EQ0NEQ | Group the complete dot product of output Y. |
| spell39_transform_y_xz_group | 4,600 bytes, 30 hunks, 3EQ0NEQ | Group the X and Z products in output Y as seen in retail. |
| spell40_transform_y_parenthesized_products | 4,600 bytes, 25 hunks, 3EQ0NEQ | Group each product independently in output Y. |
| spell41_transform_y_runtime_accumulate | 4,584 bytes, 38 hunks, 3EQ0NEQ | Accumulate output Y in the destination. |
| spell42_magnitude_xy_then_z | 4,600 bytes, 15 hunks, 2EQ1NEQ | Select shared magnitude accumulation in X/Y/Z order. |
| spell43_magnitude_standard | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use the default shared magnitude. |
| spell44_original_magnitude_float_local | 4,600 bytes, 16 hunks, 3EQ0NEQ | Evaluate the original float local and single-precision root. |
| spell45_magnitude_gfroot_single_sum | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use GFRoot and the inline X/Y/Z sum. |
| spell46_magnitude_original_gfroot | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use GFRoot with the original float local. |
| spell47_magnitude_grouped_gfroot | 4,600 bytes, 15 hunks, 2EQ1NEQ | Group the first two squares before calling GFRoot. |
| spell48_magnitude_grouped_xz_gfroot | 4,600 bytes, 16 hunks, 2EQ1NEQ | Group the first and third squares before calling GFRoot. |
| spell49_radius_original_divisions | 4,600 bytes, 17 hunks, 3EQ0NEQ | Restore double divisions for the horizontal texture radius. |
| spell50_radius_original_float_divisions | 4,600 bytes, 16 hunks, 3EQ0NEQ | Restore float divisions for the horizontal texture radius. |
| spell51_original_window_extent_locals | 4,600 bytes, 16 hunks, 3EQ0NEQ | Use the debug GetWidth and GetHeight named difference locals. |
| spell52_original_window_extent_sums | 4,600 bytes, 16 hunks, 3EQ0NEQ | Simplify the viewport expression and make the offset single precision. |
| spell53_window_exact_double_divisions | 4,588 bytes, 25 hunks, 3EQ0NEQ | Use the donor double divisions in all four viewport components. |
| spell54_window_exact_float_divisions | 4,600 bytes, 10 hunks, 3EQ0NEQ | Use retail float divisions and the double offset. The tail matches. |
| spell55_window_ungrouped_products_double_offset | 4,600 bytes, 14 hunks, 3EQ0NEQ | Use mixed multiplication and the double offset without explicit grouping. |
| spell56_window_half_then_divide | 4,600 bytes, 12 hunks, 3EQ0NEQ | Apply the half before dividing by the viewport scale. |
| spell57_window_grouped_exact_divisions | 4,600 bytes, 18 hunks, 3EQ0NEQ | Use grouped double divisions with the original offset. |
| spell58_vector_scale_x_group | 4,600 bytes, 23 hunks, 3EQ0NEQ | Group the C3DVector scale operator’s X product. |
| spell59_vector_copy_original_formal | 4,600 bytes, 16 hunks, 3EQ0NEQ | Select the original C3DVector copy formal. |
| spell60_vector_add_group | 4,600 bytes, 24 hunks, 3EQ0NEQ | Group the C3DVector addition X/Y component expressions. |
| spell61_reviewed_window_divisions | 4,600 bytes, 10 hunks, 3EQ0NEQ | Label the inferred retail precision and format the viewport call. |
| spell62_donor_magnitude_gfroot | 4,600 bytes, 10 hunks, 3EQ0NEQ | Select the original GFRoot with the explicit float magnitude local. |
| spell63_div_x_arg_group | 4,600 bytes, 11 hunks, 3EQ0NEQ | Group the X reciprocal product in vector division. |
| spell64_div_y_arg_group | 4,600 bytes, 9 hunks, 3EQ0NEQ | Group the Y reciprocal product in vector division. |
| spell65_div_z_arg_group | 4,600 bytes, 7 hunks, 3EQ0NEQ | Group the Z reciprocal product in vector division. Moves the shader receiver load and two late register choices. |
| spell66_div_reciprocal_group | 4,600 bytes, 12 hunks, 3EQ0NEQ | Group the vector reciprocal. |
| spell67_window_qualified_object | 4,600 bytes, 10 hunks, 3EQ0NEQ | Spell the viewport receiver as an explicitly dereferenced object. |
| spell68_window_original_extent_named | 4,600 bytes, 12 hunks, 3EQ0NEQ | Restore named difference locals in the viewport extents. |
| spell69_window_group_half | 4,604 bytes, 18 hunks, 3EQ0NEQ | Group each viewport-extent halving. |
| spell70_transform_y_group_first_product | 4,600 bytes, 18 hunks, 3EQ0NEQ | Group the first product of the TransformPoint Y output. |
| spell71_transform_y_group_second_product | 4,584 bytes, 22 hunks, 3EQ0NEQ | Group the second product of the TransformPoint Y output. |
| spell72_transform_y_group_third_product | 4,600 bytes, 21 hunks, 3EQ0NEQ | Group the third product of the TransformPoint Y output. |
| spell73_transform_y_group_first_two_products | 4,584 bytes, 29 hunks, 3EQ0NEQ | Group each of the first two products in the TransformPoint Y output. |
| spell74_transform_y_group_last_two_products | 4,600 bytes, 25 hunks, 3EQ0NEQ | Group each of the last two products in the TransformPoint Y output. |
| spell75_transform_y_group_first_last_products | 4,600 bytes, 24 hunks, 3EQ0NEQ | Group each of the first and last products in the TransformPoint Y output. |
| spell76_transform_y_group_left_sum | 4,574 bytes, 21 hunks, 3EQ0NEQ | Group the left-associated partial sum in the TransformPoint Y output. |
| spell77_transform_y_group_complete_sum | 4,600 bytes, 25 hunks, 3EQ0NEQ | Group the complete sum in the TransformPoint Y output. |
| spell78_transform_y_original_product_rhs | 4,600 bytes, 18 hunks, 3EQ0NEQ | Reverse multiplication operand spelling while grouping the first product of TransformPoint Y. |
| spell79_portable_transition_owner | 4,600 bytes, 10 hunks, 3EQ0NEQ | Guard the transition helper’s MSVC attribute. Same leaf and helpers as spell61. |
| spell80_sphere_divisor_group | 4,600 bytes, 10 hunks, 3EQ0NEQ | Group the sphere radius expression. |
| spell81_sphere_vector_result_group | 4,600 bytes, 10 hunks, 3EQ0NEQ | Group the sphere-center division result. |
| spell82_sphere_vector_implicit_float | 4,600 bytes, 10 hunks, 3EQ0NEQ | Make the original float radius conversion explicit. |
| spell83_shader_window_xy_double_offset_group | 4,600 bytes, 16 hunks, 3EQ0NEQ | Group both double viewport offsets. |
| spell84_shader_window_only_x_double_offset_group | 4,600 bytes, 12 hunks, 3EQ0NEQ | Group only the horizontal double viewport offset. |
| spell85_shader_window_only_y_double_offset_group | 4,600 bytes, 12 hunks, 3EQ0NEQ | Group only the vertical double viewport offset. |
| spell86_shader_window_xy_halves_group | 4,604 bytes, 18 hunks, 3EQ0NEQ | Group both viewport half extents before dividing by scale. |
| spell87_shader_window_extent_both_float_scales | 4,600 bytes, 16 hunks, 3EQ0NEQ | Group the viewport extent half and scale into one divisor. |
| spell88_shader_window_group_extent_half | 4,600 bytes, 14 hunks, 3EQ0NEQ | Group the horizontal extent before dividing by scale. |
| spell89_shader_window_group_extent_negation | 4,600 bytes, 14 hunks, 3EQ0NEQ | Group the negative vertical extent. |
| spell90_shader_window_group_extent_half_negation | 4,600 bytes, 14 hunks, 3EQ0NEQ | Group the negative vertical half extent. |
| spell91_transform_x_first_product | 4,600 bytes, 16 hunks, 3EQ0NEQ | Group the first product in TransformPoint output X. |
| spell92_transform_x_second_product | 4,584 bytes, 28 hunks, 3EQ0NEQ | Group the second product in TransformPoint output X. |
| spell93_transform_x_third_product | 4,600 bytes, 23 hunks, 3EQ0NEQ | Group the third product in TransformPoint output X. |
| spell94_transform_x_left_partial_sum | 4,568 bytes, 26 hunks, 3EQ0NEQ | Group the partial left sum in TransformPoint output X. |
| spell95_transform_z_first_product | 4,600 bytes, 26 hunks, 3EQ0NEQ | Group the first product in TransformPoint output Z. |
| spell96_transform_z_second_product | 4,600 bytes, 25 hunks, 3EQ0NEQ | Group the second product in TransformPoint output Z. |
| spell97_transform_z_third_product | 4,600 bytes, 16 hunks, 3EQ0NEQ | Group the third product in TransformPoint output Z. |
| spell98_transform_z_left_partial_sum | 4,600 bytes, 18 hunks, 3EQ0NEQ | Group the partial left sum in TransformPoint output Z. |
| spell99_transform_x_z_matching_product_groups | 4,600 bytes, 23 hunks, 3EQ0NEQ | Group matching third products in TransformPoint outputs X and Z. |
| spell100_transform_all_third_product_groups | 4,600 bytes, 27 hunks, 3EQ0NEQ | Group the third product in all three TransformPoint outputs. |

The earlier selection spell79_portable_transition_owner follows the original sequential divisions at the viewport upload. The two offset operands have the exact double width and contents at 0x00BAB33C and 0x00BAB355. The texture-loop entry and all 70 local transfers agree. That draft has 86 positional masked byte differences. All three mapped helpers and the 112-byte GetTransitionFade callee agree.

| Grades | Best result | Source change |
| --- | --- | --- |
| spell101 through spell107 | 4,600 bytes, 25 hunks | Group individual scalar products in C3DVector multiplication and combine them with the debug GFRoot body. |
| spell108 through spell115 | 4,600 bytes, 10 hunks | Test the debug PostScale division and sign placement, the original float GFRoot, and grouping at the upper and rear control point calculations. |
| spell116 through spell120 | 4,600 bytes, 7 hunks | Group the independent products of vector division. Grouping only Z and grouping Z with the reciprocal both improve the sphere-center and viewport schedules. |
| spell121 through spell130 | 4,600 bytes, 7 hunks | Pair the Z product with direct initialization, radius division, the original half-FOV expression, and alternate viewport expressions. spell128 uses only the Z product grouping and has 63 positional masked differences. |
| spell131 through spell137 | 4,600 bytes, 7 hunks | The original state helper formals preserve 63 differences. The bookmark assertion alone moves several unrelated regions. Five setup failures record the capacity constants missing from the active header branch. |
| spell138 through spell139 | 4,600 bytes, 7 hunks | The original reciprocal-square-root union with its seed square grouped restores another aligned instruction. Both grades have 61 positional masked differences. |
| spell140 through spell148 | 4,600 bytes, 7 hunks | Restore the source-proven capacity constants and exercise the original assertions individually and together. The assertion grades change the extent to 4,584 or 4,588 bytes. The original declaration order without the dead arms retains 61 differences. |
| spell149 through spell160 | 4,600 bytes, 7 hunks | Explicit shader and texture-filter receivers, scalar argument grouping, the explicit division operator call and control-point radius grouping retain 61 differences. Grouping the complete Normalise square sum gives 12 hunks. |
| spell161 through spell172 | 4,600 bytes, 7 hunks | Compare double and mixed-width Newton-step constants and the original nested product groupings. Grouping just the product `r * r * n` ties the selected draft. The debug double literals and the GFSqr product grouping produce wider instruction drift. |
| spell173 through spell182 | 4,600 bytes, 7 hunks | Test the donor's direct vector subtraction constructor, its `coord` formal, and the real component and copy constructors. Direct subtraction changes 36 hunks. The original component constructor alone retains 61 differences. |
| spell183 through spell196 | 4,600 bytes, 7 hunks | Restore the scoped capture manager checks and the stack capacity and bookmark depth checks. The original store sequence and the scoped destructor check tie the prior draft independently. Other combinations move the geometry schedule. |
| spell197 through spell206 | 4,600 bytes, 7 hunks | Restore the original `CMatrix3x4` data union and the `scale` and `one_on_scale` names in vector division. The original float reciprocal retains 61 differences with the union. The double reciprocal leaves 69 differences. |
| spell207 through spell217 | 4,600 bytes, 7 hunks | Restore the original typed index enum, the original index, vertex and device accessors, and the signed texture stage local. The accessors preserve the prior grade. Release-dead shader state and Direct3D checks leave 67 differences. |
| spell218 through spell226 | 4,600 bytes, 7 hunks | Test the original bookmark check and update using explicit assignment and natural grouping around the texture filter lookup. All preserve the 61 remaining differences. The FableWin global-symbol records independently confirm `D3DTextureFilterModeTable` is an array of three unsigned long values. |

FableWin 0x01DB57B0 confirms the three vector components multiplied by one float reciprocal. Grouping the Z product in the shared operator fixes the sphere-center divide and viewport receiver order. Keeping the existing parenthesis around the reciprocal-square-root seed square from 0x0066C900 also matches one more TransformPoint instruction. FableWin uses double constants 3.0 and 0.5 at 0x01ACE0E4 and 0x01ACE0ED. The selected source follows the float operands in retail at 0x00BAAC90 and 0x00BAAC9A. The selected spell217_reviewed_original_draw_accessors has 4,600 bytes, 1,190 of 1,198 aligned instructions, and 61 positional masked byte differences across three sites.

FableWin 0x02DBF4D0, 0x02DCFDF0 and 0x02E77CC0 and the raw Ego_d records identify the draw helper formals, `c_stage` and the vertex, index and device accessors. The selected draft uses the original `EPrimitiveType` storage and both index accessors. FableWin 0x019072B0 constructs the vector subtraction result directly. The inherited Initialise-form release profile gives the closest leaf. The direct constructor and original formal survive as spell174. The state checks in other diagnostic forms come from FableWin 0x018BF760, 0x018BFB30, 0x018B1C40, 0x018B1DF0 and 0x018B1FA0.

- At 0x00BAAE27, the native TransformPoint expansion for the back control point's Y coordinate brings the X term into the sum with Z before adding Y. The selected helper keeps the original donor's left-associated row sum. The compiled x87 order adds Y and Z before adding X.
- At 0x00BAB21F, retail loads the shader receiver before setting up constant index 4. The object puts the same load at 0x00BAB24D. The audit validates that both loads feed `ecx` into the same `SetUserVertexShaderConstant` call at 0x00BAB253 with index 4 and no intervening `ecx` write.
- At 0x00BAB758, retail tests and combines the bookmark masks using the opposite operand order in the minimum texture filter state update. The stored mask is the same.

All 155 relocated operands, 70 local targets, four Direct3D slots, three pick-any bodies and the 112-byte transition helper agree for spell217.

The native /Od /W3 /GR- test inlines the complete selected source bodies at fixture generation time. It exercises 3,072 configurations of unloaded, loading, available and cached mesh paths, the blocking flag, billboard generation, occlusion, alpha test, three near-fade clamp cases and four combinations of UV bounds. It checks the mesh lifetime, billboard arguments, shader and state scope ordering, complete constant uploads, draw indices and counts, texture stages, and both occlusion operations. GPU and resource calls use deterministic adapters.

The matched optimized Ego_r executable gives a separate check on the three residual sites. The back-control-point Y sum at 0x00A61EF0 combines the Y and X terms before adding Z and translation, consistent with the FableWin source grouping. The constant-4 receiver loads at 0x00A623A1 and reaches the named call at 0x00A623E4 without an intervening receiver write. The minimum texture filter at 0x00A629D0 and 0x00A629D4 tests the stored mask against the current mask and combines them into the current-mask register. `BINDING_EVIDENCE.json` records the three checked paths and their optimized donor identity.

The intermediate `normalize_host_fallback` rebuilds the same native leaf and the exact 112-byte transition helper. The raw `GetTransitionFade` donor calls the fade constructor, resolves its two bounds, then performs the nested comparisons itself. Ego_d identifies `fade`, `min_mesh_dist`, `max_mesh_dist` and `clamped_dist`. The selected helper follows those statements and names. Its only linked operand is the original primitive renderer global at 0x01436E80. The three alternate comparison spellings retain the leaf and differ in the helper. The portable host branch uses the existing width-safe reciprocal-root helper.

The expanded native test checks 6,144 scenarios. The second camera orientation exercises nonzero X and Y terms in the rear transform, and the test independently computes the projection with scalar doubles. The complete selected body also passes the GCC C++03 LP64 AddressSanitizer and UndefinedBehaviorSanitizer run.

The FableWin bodies at 0x02D78DD0 and 0x01D129F0 start every pre-translation and two-argument transform product with the source coordinate. Correcting both operand orders in the selected leaf's headers reproduces its native object. FableWin 0x02D86EF0 calls `CIVCountedPointer::Get` on both LOD pointers. That typed variant matches the full opening region through 0x00BAAB1A and the minimum-filter bookmark sequence, with later code movement. The zero-condition loop scope guards in the two original LOD and texture-stage helpers produce the same optimized code when reconstructed as `if (false) { } else for` or `if (0) ; else for`.

`coordinate_source_matching_y_native_division_clip` independently matches all three previously isolated instruction groups in one 4,600-byte object with the exact mapped helpers. That profile moves several adjacent native schedules. `coordinate_source_selected_native_division_clip` also has the exact mapped helpers and matches 1,191 of 1,198 instructions. It matches the shader receiver and bookmark groups, with a new receiver move at 0x00BAB383 and two instructions exchanged in the fade computation at 0x00BAB39A. The selected object retains fewer positional masked byte differences.

The selected `geometry_reviewed_original_compilands` adopts the donor-proven matrix operand order and the original component and copy constructors. Its leaf, named relocations and mapped helper inventory are byte-identical to `normalize_host_fallback`. The additive matrix header patch affects only the row opt-in. The revised 6,144-case native fixture and the GCC C++03 LP64 sanitizer run both pass. The previous selection and independently verified 1,191-instruction and three-group alternatives remain in the work history.

| Build tags | Native result | Source fact tested |
| --- | --- | --- |
| `bounds_owner_*` and `geometry_budget_*` | 4,600 bytes and 1,190 aligned instructions for the selected tests of bound-return and inline ownership | FableWin 0x0263EED0 returns the typed bounding box by copy. GetCenter at 0x025276C0 and GetDiagonal at 0x02E34510 use vector arithmetic and the half scale. The explicit copy spelling leaves the selected object identical. |
| `actual_state_checks_*` | 4,600 bytes and 1,190 aligned instructions when the assertions expand to empty release statements. Dead-arm variants change code across the leaf. | Actual debug assertion strings at 0x018BF7EF, 0x018BF90F and 0x018BFBD5 name the stack bounds, bookmark mask and dirty-list bounds. Their order and the debug initialisation store were checked against the original owners. |
| `viewport_extent_*`, `viewport_region_*` and `native_window_*` | The unnamed group and conversion shapes leave either a viewport difference or nearby bookmark differences. | The viewport extent helpers each have one source line and no named local. The debug caller divides all four viewport values by double 2.0, then by the float `sts`. The advancing shader receiver has a measured four-tuple difference at the next scheduler boundary. |
| `uv_precision_*` | Selected radius in float divisions is byte-identical. Double clamp arms widen the stores and move the geometry and shader code. | FableWin 0x02ED44EF, 0x02ED4507 and 0x02ED4548 confirm double constants in the debug radius and clamping expressions. Native retail uses single-width clamp stores and scales. |

Native compiler instrumentation verifies the source of the 32-record allocations used by the back point. The selected profile allocates temporary-record blocks at 992 and 1024 while expanding the bounding-box operations and assigns identifier 1058 to the back point's X field clone. The typed LOD variant moves that clone to 1028 and computes the X, Z and Y terms in retail order. In that variant the separate ESI load comes later. The matrix Z grouping produces the exact ESI placement too, while neighboring X/Z schedules still differ. Every trace build reproduces the full native function bytes and named relocations of its separate uninstrumented object.

The selected `primitive_conversion_reviewed_types` restores the original typed DrawIndexedPrimitive argument. FableWin 0x02DCFF0A calls `GFPrimitiveTypeToD3DPT` at 0x02DD0060. That helper checks `prim_type != PT_NULL` in the debug build and returns the original enum value as the Direct3D enum. The shared SDK declaration preserves the verified 0x148 slot. Native code and all helper bindings are byte-identical to the prior selection. The native and portable fixtures also exercise all six legal enum mappings.

`transform_esi_top_height_group_early_receiver` reaches 1,181 aligned instructions with the three originally investigated regions exact and the full prefix through 0x00BAACEB matching. `residual_point_transform_z_fourth_group` reaches the end of the back-Y calculation before its first difference and 1,186 aligned instructions. It retains other adjacent projection, viewport and bookmark differences. Both remain diagnostic builds.

The selected `original_magnitude_selected_ordinary` uses the original return expression in `C3DVector::GetMagnitude`. FableWin 0x01B52910 and its source and local records establish the X, Y and Z products and the call to `GFRoot`. The ordinary expression reproduces all 33 bytes at the native GetMagnitude target. The complete sprite leaf and all named relocations match the previous selection. The existing shared header body supplies it without a new patch.

The original Ego_r and FableWin compile records identify compiler 16.0.40219.1 for both donor objects. Their speed switches were tested as diagnostics with the native VC7.1 grader in `original_compiland_options_*`. Matching VC7.1 speed and inline switches leave the three tested profiles identical. VC7.1 /Op enlarges the leaf and changes its arithmetic schedules.

FableWin 0x01A664E0 and its `lib_box_f.hpp` line 21 record identify the viewport copy constructor's `box` formal and the ordered `TLX`, `TLY`, `BRX`, `BRY` stores. The 3D box and sphere copies at 0x01B408F0 and 0x01B409B0 have the compiler-generated `__that` formal and no source-line records. The selected source retains those implicit copies. The actual viewport copy formal, its two equivalent member-copy spellings and alternate typed addresses into `view_matrix.Data` leave the three measured profiles identical, including the named relocations. The established packed vector profile also leaves those bytes unchanged.

The direct donor subtraction at 0x019072B0 and both plain and forced inline forms were checked in `original_subtraction_rvo_*` and `arithmetic_inline_ownership_*`. With the exact back-Y and early shader receiver, direct subtraction gives 1,181 aligned instructions and 87 masked byte differences, with all three isolated regions exact and differences in adjacent schedules. Using ordinary inline for `Normalise`, `PreTranslate` or the two-argument `TransformPoint` leaves calls where retail expands the helpers. Copying the original geometry owners onto the combined header tree in `original_geometry_viewport_join_old_spacing` reproduces the complete 1,186-instruction alternative byte-for-byte. Its double viewport divides and original signed PostScale divisions were checked separately.

FableWin 0x01A66530 and 0x01A66560 both store their return expressions through an unnamed float scratch, and both have only the `this` local. Optimized Ego_r computes `BRY-TLY` at 0x00A62567 and stores it through the float scratch at 0x00A6256B. It computes `BRX-TLX` at 0x00A62589 and stores it through the same scratch at 0x00A6258D. Both feed constant 0xB at 0x00A625A4. Its different read instructions follow the same float rounding in each axis. VC7.1 retail retains x87 values from the subtractions through the scale until the argument stores at 0x00BAB367 and 0x00BAB37C.

The `materialisation_symmetric_reviewed_owners` probe uses an unnamed C++03 float temporary for both getters and reaches 1,184 aligned instructions and 42 positional masked differences. The `materialisation_owner_join_selected_ordinary` probe applies the same spelling to one axis and reaches 1,185 aligned instructions and 36 positional masked differences. The one-axis width and height versions emit identical bytes. Both alternates retain the exact mapped helpers and all operand identities, and pass the 6,144-case native and C++03 sanitizer fixtures. The raw donor supports symmetric source. The selected source retains its original one-expression returns and 1,190 aligned instructions.

The remaining schedules depend on front-end choices that the available donor machine code and line records do not fully encode. The most direct source input for resolving them would be the preprocessed VC7.1 PC retail translation unit for `Engine_Primitive_Manager_Repeated_ZSprites.cpp`, or that release source together with its `lib_matrix.hpp`, `lib_coord.hpp` and render-state inline header versions and build defines.

Twenty-four joint grades combined the symmetric viewport float materializations with the original source-coordinate-first matrix rows. Grouping X/Y sums, the first three terms, the Z term or the complete transformed coordinate, with both original reciprocal forms, did not improve the selected source. All 24 objects retained three equal mapped helper copies.

# Compiler attempts for 00baa870

Each line records a compiler invocation. Regrading an inherited file establishes a baseline. Historical rechecks and compiler failures do not establish a new successful source shape.

- `baseline`  COMPILE_FAIL  —/4600
- `archive1`  COMPILE_FAIL  —/4600
- `archive2`  COMPILE_FAIL  —/4600
- `archived_header_rebase`  DIFFER  4600/4600  5 differing instruction groups.
- `remove_unsupported_locals`  DIFFER  4600/4600  41 differing instruction groups.
- `donor_weight_normalisation`  DIFFER  4600/4600  37 differing instruction groups.
- `spell02_original`  DIFFER  4605/4600  162 differing instruction groups.
- `spell03_retail_precision`  DIFFER  4600/4600  34 differing instruction groups.
- `spell04_vector_operators`  DIFFER  4621/4600  109 differing instruction groups.
- `spell05_index_accessors`  DIFFER  4584/4600  31 differing instruction groups.
- `spell06_inverse_sum_order`  DIFFER  4584/4600  31 differing instruction groups.
- `spell07_radius_division`  DIFFER  4584/4600  31 differing instruction groups.
- `spell08_length_squares`  DIFFER  4586/4600  31 differing instruction groups.
- `spell09_original_pretranslate_lhs`  DIFFER  4600/4600  34 differing instruction groups.
- `spell10_original_pretranslate_group`  DIFFER  4600/4600  45 differing instruction groups.
- `spell11_pretranslate_z_association`  DIFFER  4600/4600  29 differing instruction groups.
- `spell12_pretranslate_original_products`  DIFFER  4600/4600  38 differing instruction groups.
- `spell13_pretranslate_z_chain`  DIFFER  4600/4600  28 differing instruction groups.
- `spell14_stock_accumulate`  DIFFER  4600/4600  34 differing instruction groups.
- `spell15_third_accumulate`  DIFFER  4600/4600  34 differing instruction groups.
- `spell16_third_left_group`  DIFFER  4600/4600  31 differing instruction groups.
- `spell17_third_whole_group`  DIFFER  4600/4600  31 differing instruction groups.
- `spell18_third_z_first_group`  DIFFER  4600/4600  29 differing instruction groups.
- `spell19_third_original_ungrouped`  DIFFER  4600/4600  38 differing instruction groups.
- `spell20_third_nested_term_products`  DIFFER  4600/4600  37 differing instruction groups.
- `spell21_original_normalise_sqrs`  DIFFER  4584/4600  29 differing instruction groups.
- `spell22_original_normalise_grouped_sqrs`  DIFFER  4584/4600  29 differing instruction groups.
- `spell23_original_sphere_center`  DIFFER  4600/4600  20 differing instruction groups.
- `spell24_original_posts_scale_divisions`  DIFFER  4589/4600  89 differing instruction groups.
- `spell25_original_center_left_group`  DIFFER  4600/4600  27 differing instruction groups.
- `spell26_original_center_z_association`  DIFFER  4600/4600  25 differing instruction groups.
- `spell27_original_backvec_sqr`  DIFFER  4600/4600  16 differing instruction groups.
- `spell28_original_backvec_sqr_group`  DIFFER  4600/4600  18 differing instruction groups.
- `spell29_normalise_component_values`  DIFFER  4600/4600  22 differing instruction groups.
- `spell30_normalise_named_squared`  DIFFER  4600/4600  16 differing instruction groups.
- `spell31_normalise_inline_sum`  DIFFER  4586/4600  44 differing instruction groups.
- `spell32_normalise_grouped_components`  DIFFER  4586/4600  45 differing instruction groups.
- `spell33_normalise_unparenthesized`  DIFFER  4586/4600  44 differing instruction groups.
- `spell34_normalise_original_values_xyz`  DIFFER  4600/4600  18 differing instruction groups.
- `spell35_transform_original_product_lhs`  DIFFER  4600/4600  16 differing instruction groups.
- `spell36_transform_original_y_lhs`  DIFFER  4600/4600  16 differing instruction groups.
- `spell37_transform_original_y_left_group`  DIFFER  4574/4600  23 differing instruction groups.
- `spell38_transform_original_y_group`  DIFFER  4600/4600  27 differing instruction groups.
- `spell39_transform_y_xz_group`  DIFFER  4600/4600  30 differing instruction groups.
- `spell40_transform_y_parenthesized_products`  DIFFER  4600/4600  25 differing instruction groups.
- `spell41_transform_y_runtime_accumulate`  DIFFER  4584/4600  38 differing instruction groups.
- `spell42_magnitude_xy_then_z`  DIFFER  4600/4600  15 differing instruction groups.
- `spell43_magnitude_standard`  DIFFER  4600/4600  16 differing instruction groups.
- `spell44_original_magnitude_float_local`  DIFFER  4600/4600  16 differing instruction groups.
- `spell45_magnitude_gfroot_single_sum`  DIFFER  4600/4600  16 differing instruction groups.
- `spell46_magnitude_original_gfroot`  DIFFER  4600/4600  16 differing instruction groups.
- `spell47_magnitude_grouped_gfroot`  DIFFER  4600/4600  15 differing instruction groups.
- `spell48_magnitude_grouped_xz_gfroot`  DIFFER  4600/4600  16 differing instruction groups.
- `spell49_radius_original_divisions`  DIFFER  4600/4600  17 differing instruction groups.
- `spell50_radius_original_float_divisions`  DIFFER  4600/4600  16 differing instruction groups.
- `spell51_original_window_extent_locals`  DIFFER  4600/4600  16 differing instruction groups.
- `spell52_original_window_extent_sums`  DIFFER  4600/4600  16 differing instruction groups.
- `spell53_window_exact_double_divisions`  DIFFER  4588/4600  25 differing instruction groups.
- `spell54_window_exact_float_divisions`  DIFFER  4600/4600  10 differing instruction groups.
- `spell55_window_ungrouped_products_double_offset`  DIFFER  4600/4600  14 differing instruction groups.
- `spell56_window_half_then_divide`  DIFFER  4600/4600  12 differing instruction groups.
- `spell57_window_grouped_exact_divisions`  DIFFER  4600/4600  18 differing instruction groups.
- `spell58_vector_scale_x_group`  DIFFER  4600/4600  23 differing instruction groups.
- `spell59_vector_copy_original_formal`  DIFFER  4600/4600  16 differing instruction groups.
- `spell60_vector_add_group`  DIFFER  4600/4600  24 differing instruction groups.
- `spell61_reviewed_window_divisions`  DIFFER  4600/4600  10 differing instruction groups.
- `spell62_donor_magnitude_gfroot`  DIFFER  4600/4600  10 differing instruction groups.
- `spell63_div_x_arg_group`  DIFFER  4600/4600  11 differing instruction groups.
- `spell64_div_y_arg_group`  DIFFER  4600/4600  9 differing instruction groups.
- `spell65_div_z_arg_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell66_div_reciprocal_group`  DIFFER  4600/4600  12 differing instruction groups.
- `spell67_window_qualified_object`  DIFFER  4600/4600  10 differing instruction groups.
- `spell68_window_original_extent_named`  DIFFER  4600/4600  12 differing instruction groups.
- `spell69_window_group_half`  DIFFER  4604/4600  18 differing instruction groups.
- `spell70_transform_y_group_first_product`  DIFFER  4600/4600  18 differing instruction groups.
- `spell71_transform_y_group_second_product`  DIFFER  4584/4600  22 differing instruction groups.
- `spell72_transform_y_group_third_product`  DIFFER  4600/4600  21 differing instruction groups.
- `spell73_transform_y_group_first_two_products`  DIFFER  4584/4600  29 differing instruction groups.
- `spell74_transform_y_group_last_two_products`  DIFFER  4600/4600  25 differing instruction groups.
- `spell75_transform_y_group_first_last_products`  DIFFER  4600/4600  24 differing instruction groups.
- `spell76_transform_y_group_left_sum`  DIFFER  4574/4600  21 differing instruction groups.
- `spell77_transform_y_group_complete_sum`  DIFFER  4600/4600  25 differing instruction groups.
- `spell78_transform_y_original_product_rhs`  DIFFER  4600/4600  18 differing instruction groups.
- `spell79_portable_transition_owner`  DIFFER  4600/4600  10 differing instruction groups.
- `spell80_sphere_divisor_group`  DIFFER  4600/4600  10 differing instruction groups.
- `spell81_sphere_vector_result_group`  DIFFER  4600/4600  10 differing instruction groups.
- `spell82_sphere_vector_implicit_float`  DIFFER  4600/4600  10 differing instruction groups.
- `spell83_shader_window_xy_double_offset_group`  DIFFER  4600/4600  16 differing instruction groups.
- `spell84_shader_window_only_x_double_offset_group`  DIFFER  4600/4600  12 differing instruction groups.
- `spell85_shader_window_only_y_double_offset_group`  DIFFER  4600/4600  12 differing instruction groups.
- `spell86_shader_window_xy_halves_group`  DIFFER  4604/4600  18 differing instruction groups.
- `spell87_shader_window_extent_both_float_scales`  DIFFER  4600/4600  16 differing instruction groups.
- `spell88_shader_window_group_extent_half`  DIFFER  4600/4600  14 differing instruction groups.
- `spell89_shader_window_group_extent_negation`  DIFFER  4600/4600  14 differing instruction groups.
- `spell90_shader_window_group_extent_half_negation`  DIFFER  4600/4600  14 differing instruction groups.
- `spell91_transform_x_first_product`  DIFFER  4600/4600  16 differing instruction groups.
- `spell92_transform_x_second_product`  DIFFER  4584/4600  28 differing instruction groups.
- `spell93_transform_x_third_product`  DIFFER  4600/4600  23 differing instruction groups.
- `spell94_transform_x_left_partial_sum`  DIFFER  4568/4600  26 differing instruction groups.
- `spell95_transform_z_first_product`  DIFFER  4600/4600  26 differing instruction groups.
- `spell96_transform_z_second_product`  DIFFER  4600/4600  25 differing instruction groups.
- `spell97_transform_z_third_product`  DIFFER  4600/4600  16 differing instruction groups.
- `spell98_transform_z_left_partial_sum`  DIFFER  4600/4600  18 differing instruction groups.
- `spell99_transform_x_z_matching_product_groups`  DIFFER  4600/4600  23 differing instruction groups.
- `spell100_transform_all_third_product_groups`  DIFFER  4600/4600  27 differing instruction groups.
- `spell101_scale_y_component_group`  DIFFER  4600/4600  25 differing instruction groups.
- `spell102_scale_z_component_group`  DIFFER  4600/4600  25 differing instruction groups.
- `spell103_scale_y_z_component_group`  DIFFER  4600/4600  34 differing instruction groups.
- `spell104_scale_total_components_group`  DIFFER  4586/4600  67 differing instruction groups.
- `spell105_original_magnitude_and_scale_y_group`  DIFFER  4600/4600  25 differing instruction groups.
- `spell106_original_magnitude_and_scale_z_group`  DIFFER  4600/4600  25 differing instruction groups.
- `spell107_original_magnitude_and_scale_grouped_yz`  DIFFER  4600/4600  34 differing instruction groups.
- `spell108_posts_signed_single_divisions`  DIFFER  4600/4600  22 differing instruction groups.
- `spell109_posts_multiply_negative_reciprocal`  DIFFER  4600/4600  22 differing instruction groups.
- `spell110_posts_negate_per_axis`  DIFFER  4600/4600  164 differing instruction groups.
- `spell111_posts_reciprocal_divisions`  DIFFER  4600/4600  20 differing instruction groups.
- `spell112_posts_divisors_original_gfroot`  DIFFER  4600/4600  22 differing instruction groups.
- `spell113_group_upper_control_point_height`  DIFFER  4600/4600  12 differing instruction groups.
- `spell114_group_negative_back_vector`  DIFFER  4600/4600  10 differing instruction groups.
- `spell115_original_height_and_z_scale_group`  DIFFER  4600/4600  25 differing instruction groups.
- `spell116_division_yz_groups`  DIFFER  4600/4600  10 differing instruction groups.
- `spell117_division_xz_groups`  DIFFER  4600/4600  12 differing instruction groups.
- `spell118_division_xy_groups`  DIFFER  4600/4600  12 differing instruction groups.
- `spell119_division_xyz_groups`  DIFFER  4600/4600  15 differing instruction groups.
- `spell120_division_z_and_reciprocal_groups`  DIFFER  4600/4600  7 differing instruction groups.
- `spell121_division_z_and_direct_sphere_center`  DIFFER  4600/4600  7 differing instruction groups.
- `spell122_division_z_and_direct_control_points`  DIFFER  4600/4600  7 differing instruction groups.
- `spell123_division_z_and_float_radius_divisions`  DIFFER  4600/4600  7 differing instruction groups.
- `spell124_division_z_and_grouped_half_fov`  DIFFER  4600/4600  7 differing instruction groups.
- `spell125_division_z_and_grouped_viewport_x_offset`  DIFFER  4600/4600  7 differing instruction groups.
- `spell126_division_z_and_grouped_viewport_y_offset`  DIFFER  4600/4600  7 differing instruction groups.
- `spell127_division_z_and_named_window_extents`  DIFFER  4600/4600  11 differing instruction groups.
- `spell128_division_z_owner_original_ungrouped_calls`  DIFFER  4600/4600  7 differing instruction groups.
- `spell129_direct_sphere_center_initialisation`  DIFFER  4600/4600  10 differing instruction groups.
- `spell130_direct_control_point_initialisation`  DIFFER  4600/4600  10 differing instruction groups.
- `spell131_original_renderstate_formals`  DIFFER  4600/4600  7 differing instruction groups.
- `spell132_renderstate_original_assert_arms`  COMPILE_FAIL  —/4600
- `spell133_renderstate_original_assert_wrappers`  COMPILE_FAIL  —/4600
- `spell134_renderstate_entry_assert_arms`  COMPILE_FAIL  —/4600
- `spell135_renderstate_update_assert_arm`  COMPILE_FAIL  —/4600
- `spell136_renderstate_stack_assert_arm`  COMPILE_FAIL  —/4600
- `spell137_renderstate_mask_assert_arm`  DIFFER  4588/4600  48 differing instruction groups.
- `spell138_division_and_original_union_square`  DIFFER  4600/4600  7 differing instruction groups.
- `spell139_union_square_and_original_state_formals`  DIFFER  4600/4600  7 differing instruction groups.
- `spell140_all_state_assert_arms`  DIFFER  4584/4600  53 differing instruction groups.
- `spell141_all_state_assert_wrappers`  DIFFER  4584/4600  53 differing instruction groups.
- `spell142_entry_state_assert_arms`  DIFFER  4584/4600  47 differing instruction groups.
- `spell143_update_state_assert_arm`  DIFFER  4588/4600  53 differing instruction groups.
- `spell144_stack_state_assert_arm`  DIFFER  4588/4600  53 differing instruction groups.
- `spell145_mask_state_assert_arm`  DIFFER  4588/4600  48 differing instruction groups.
- `spell146_recovered_state_declaration_order`  DIFFER  4600/4600  7 differing instruction groups.
- `spell147_recovered_state_order_assert_arms`  DIFFER  4584/4600  53 differing instruction groups.
- `spell148_recovered_state_order_assert_wrappers`  DIFFER  4584/4600  53 differing instruction groups.
- `spell149_explicit_shader_receiver`  DIFFER  4600/4600  7 differing instruction groups.
- `spell150_group_shader_index_four_vector`  DIFFER  4600/4600  7 differing instruction groups.
- `spell151_group_shader_index_four_radius`  DIFFER  4600/4600  7 differing instruction groups.
- `spell152_group_shader_index_four_fourth_component`  DIFFER  4600/4600  7 differing instruction groups.
- `spell153_explicit_shader_vector_division`  DIFFER  4600/4600  7 differing instruction groups.
- `spell154_explicit_filter_receiver`  DIFFER  4600/4600  7 differing instruction groups.
- `spell155_group_min_filter_stage`  DIFFER  4600/4600  7 differing instruction groups.
- `spell156_group_min_filter_enum`  DIFFER  4600/4600  7 differing instruction groups.
- `spell157_group_negative_front_control_vector`  DIFFER  4600/4600  7 differing instruction groups.
- `spell158_group_back_control_point_radius`  DIFFER  4600/4600  7 differing instruction groups.
- `spell159_group_front_control_point_radius`  DIFFER  4600/4600  7 differing instruction groups.
- `spell160_group_normalise_square_calls`  DIFFER  4600/4600  12 differing instruction groups.
- `spell161_reciprocal_root_debug_double_literals`  DIFFER  4600/4600  13 differing instruction groups.
- `spell162_reciprocal_root_single_half`  DIFFER  4600/4600  13 differing instruction groups.
- `spell163_reciprocal_root_single_step`  DIFFER  4600/4600  12 differing instruction groups.
- `spell164_reciprocal_root_original_product_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell165_reciprocal_root_original_nested_product_group`  DIFFER  4600/4600  12 differing instruction groups.
- `spell166_reciprocal_root_original_newton_iteration_group`  DIFFER  4600/4600  12 differing instruction groups.
- `spell167_reciprocal_root_original_full_newton_group`  DIFFER  4600/4600  16 differing instruction groups.
- `spell168_group_original_gfsqr_product`  DIFFER  4600/4600  16 differing instruction groups.
- `spell169_group_original_gfsqr_and_unbracketed_seed`  DIFFER  4600/4600  12 differing instruction groups.
- `spell170_group_original_gfsqr_and_newton_product`  DIFFER  4600/4600  18 differing instruction groups.
- `spell171_group_original_gfsqr_and_newton_step`  DIFFER  4600/4600  18 differing instruction groups.
- `spell172_reciprocal_root_double_newton_product`  DIFFER  4600/4600  14 differing instruction groups.
- `spell173_original_vector_subtraction_constructor`  DIFFER  4600/4600  36 differing instruction groups.
- `spell174_original_vector_subtraction_constructor_formal`  DIFFER  4600/4600  36 differing instruction groups.
- `spell175_original_subtraction_and_component_constructor`  DIFFER  4600/4600  36 differing instruction groups.
- `spell176_original_subtraction_and_copy_constructor`  DIFFER  4600/4600  36 differing instruction groups.
- `spell177_original_subtraction_and_both_constructors`  DIFFER  4600/4600  36 differing instruction groups.
- `spell178_original_component_constructor`  DIFFER  4600/4600  7 differing instruction groups.
- `spell179_original_direct_subtraction_x_group`  DIFFER  4600/4600  36 differing instruction groups.
- `spell180_original_direct_subtraction_xy_group`  DIFFER  4600/4600  36 differing instruction groups.
- `spell181_original_direct_subtraction_y_group`  DIFFER  4600/4600  36 differing instruction groups.
- `spell182_original_direct_subtraction_z_group`  DIFFER  4600/4600  36 differing instruction groups.
- `spell183_original_capture_manager_asserts`  DIFFER  4586/4600  47 differing instruction groups.
- `spell184_original_capture_manager_assert_wrappers`  DIFFER  4586/4600  53 differing instruction groups.
- `spell185_original_state_stack_asserts`  DIFFER  4600/4600  15 differing instruction groups.
- `spell186_original_scoped_capture_asserts`  DIFFER  4600/4600  15 differing instruction groups.
- `spell187_original_scoped_capture_constructor_assert`  DIFFER  4600/4600  11 differing instruction groups.
- `spell188_original_scoped_capture_destructor_assert`  DIFFER  4600/4600  7 differing instruction groups.
- `spell189_original_bookmark_capture_store_sequence`  DIFFER  4600/4600  7 differing instruction groups.
- `spell190_original_capture_asserts_and_vector_subtraction`  DIFFER  4590/4600  39 differing instruction groups.
- `spell191_original_start_asserts_and_vector_subtraction`  DIFFER  4600/4600  36 differing instruction groups.
- `spell192_original_scope_asserts_and_vector_subtraction`  DIFFER  4600/4600  36 differing instruction groups.
- `spell193_original_capture_asserts_with_postincrement`  DIFFER  4586/4600  47 differing instruction groups.
- `spell194_original_capture_asserts_with_subtraction_and_postincrement`  DIFFER  4590/4600  39 differing instruction groups.
- `spell195_original_capture_ctor_and_bookmark_depth`  DIFFER  4600/4600  17 differing instruction groups.
- `spell196_original_capture_ctor_and_stack_capacity`  DIFFER  4600/4600  17 differing instruction groups.
- `spell197_original_matrix_data_union`  DIFFER  4600/4600  7 differing instruction groups.
- `spell198_original_union_and_vector_subtraction`  DIFFER  4600/4600  36 differing instruction groups.
- `spell199_original_matrix_trailing_data_union`  DIFFER  4600/4600  7 differing instruction groups.
- `spell200_original_trailing_union_and_vector_subtraction`  DIFFER  4600/4600  36 differing instruction groups.
- `spell201_original_pretranslate_and_data_union`  DIFFER  4584/4600  106 differing instruction groups.
- `spell202_original_pretranslate_and_vector_subtraction`  DIFFER  4584/4600  106 differing instruction groups.
- `spell203_original_division_formals`  DIFFER  4600/4600  7 differing instruction groups.
- `spell204_original_division_formals_and_double_reciprocal`  DIFFER  4600/4600  8 differing instruction groups.
- `spell205_original_union_and_division_formals`  DIFFER  4600/4600  7 differing instruction groups.
- `spell206_original_data_union_pretranslate_division_formals`  DIFFER  4584/4600  106 differing instruction groups.
- `spell207_original_index_buffer_accessors`  DIFFER  4600/4600  7 differing instruction groups.
- `spell208_typed_index_buffer_enum`  DIFFER  4600/4600  7 differing instruction groups.
- `spell209_original_index_and_vertex_buffer_accessors`  DIFFER  4600/4600  7 differing instruction groups.
- `spell210_original_all_draw_buffer_accessors`  DIFFER  4600/4600  7 differing instruction groups.
- `spell211_original_named_draw_buffer_parameters`  DIFFER  4600/4600  7 differing instruction groups.
- `spell212_original_draw_buffer_assertions_and_accessors`  DIFFER  4600/4600  11 differing instruction groups.
- `spell213_original_named_and_asserted_draw_buffer_helpers`  DIFFER  4600/4600  11 differing instruction groups.
- `spell214_original_index_and_vertex_accessors_typed_enum`  DIFFER  4600/4600  7 differing instruction groups.
- `spell215_original_draw_buffer_formals_only`  DIFFER  4600/4600  7 differing instruction groups.
- `spell216_original_shader_and_render_callers`  DIFFER  4600/4600  7 differing instruction groups.
- `spell217_reviewed_original_draw_accessors`  DIFFER  4600/4600  7 differing instruction groups.
- `spell218_original_state_explicit_bitmap_assignment`  DIFFER  4600/4600  7 differing instruction groups.
- `spell219_original_state_bitmap_guard`  DIFFER  4600/4600  7 differing instruction groups.
- `spell220_original_state_bitmap_assignment_and_guard`  DIFFER  4600/4600  7 differing instruction groups.
- `spell221_original_state_bitmap_guard_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell222_original_state_bitmap_update_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell223_original_minimum_filter_mode_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell224_original_minimum_filter_stage_group`  DIFFER  4600/4600  7 differing instruction groups.
- `spell225_original_minimum_filter_mask_scope`  DIFFER  4600/4600  7 differing instruction groups.
- `spell226_original_state_explicit_bitmap_assignment_original_order`  DIFFER  4600/4600  7 differing instruction groups.
- `partial_tree_efee91b366bb_24e36883`  DIFFER  4600/4600  7 differing instruction groups.
- `partial_tree_2de865f160dd_24e36883`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_01_back_constructor_direct`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_02_front_constructor_direct`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_03_back_constructor_expression`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_04_front_constructor_expression`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_05_center_result`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_06_top_control_height`  DIFFER  4600/4600  12 differing instruction groups.
- `followup_07_fourth_upload_natural_return_group`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_08_fifth_upload_natural_return_group`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_09_third_upload_natural_return_group`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_10_half_fov_conversion_group`  DIFFER  4600/4600  14 differing instruction groups.
- `followup_11_clip_cosine_operand_group`  DIFFER  4600/4600  11 differing instruction groups.
- `followup_12_math_header_first`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_13_state_header_first`  DIFFER  4600/4600  7 differing instruction groups.
- `partial_tree_6a20af7d676e_24e36883`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_14_mesh_header_first`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_15_camera_header_first`  COMPILE_FAIL  —/4600
- `followup_16_original_draw_enums_first`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_17_row_owner_first`  COMPILE_FAIL  —/4600
- `followup_18_donor_transform_product_order`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_19_donor_transform_y_product_order`  DIFFER  4600/4600  7 differing instruction groups.
- `followup_20_donor_transform_left_addend_group`  DIFFER  4556/4600  30 differing instruction groups.
- `followup_21_donor_transform_rightmost_addend_group`  DIFFER  4600/4600  28 differing instruction groups.
- `followup_22_donor_transform_fourth_operand_group`  DIFFER  4600/4600  11 differing instruction groups.
- `receiver_region_cast_call_normal`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_coefficient_float_clip_factors`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_coefficient_float_literal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_coefficient_float_normal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_coefficient_named_normal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_donor_divisions_literal`  DIFFER  4600/4600  8 differing instruction groups.
- `receiver_region_donor_divisions_normal`  DIFFER  4600/4600  8 differing instruction groups.
- `receiver_region_donor_divisor_group_literal`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_donor_divisor_group_normal`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_native_coefficient_clip_factors`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_native_coefficient_literal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_native_coefficient_normal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_native_divisor_group_normal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_no_cast_literal`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_no_cast_normal`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_normal_clip_cast`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_normal_clip_factors`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_region_normal_literal`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_normal_literal_reverse`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_normal_operand_cast`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_region_normal_product_group`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp00`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp01`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp10`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp11`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp00_original_locals`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp01_original_locals`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp10_original_locals`  DIFFER  4600/4600  7 differing instruction groups.
- `transition_original_cmp11_original_locals`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_constant_two_scale`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_constant_two_double`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_selected_constant_two_factor_group`  DIFFER  4600/4600  11 differing instruction groups.
- `receiver_joint_selected_constant_two_inverse_group`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_constant_two_byte_size`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_constant_three_integer_zero`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_constant_four_integer_zero`  DIFFER  4600/4600  7 differing instruction groups.
- `receiver_joint_selected_viewport_extent_original`  DIFFER  4600/4600  24 differing instruction groups.
- `receiver_joint_selected_viewport_offset_x_group`  DIFFER  4600/4600  11 differing instruction groups.
- `receiver_joint_selected_viewport_offset_y_group`  DIFFER  4600/4600  11 differing instruction groups.
- `receiver_joint_selected_viewport_full_y_group`  DIFFER  4600/4600  13 differing instruction groups.
- `receiver_joint_receiver_fade_distance_group`  DIFFER  4600/4600  11 differing instruction groups.
- `receiver_joint_receiver_constant_two_scale`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_receiver_constant_two_double`  DIFFER  4600/4600  17 differing instruction groups.
- `receiver_joint_receiver_constant_two_factor_group`  DIFFER  4600/4600  16 differing instruction groups.
- `receiver_joint_receiver_constant_two_inverse_group`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_receiver_constant_two_byte_size`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_receiver_constant_three_integer_zero`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_receiver_constant_four_integer_zero`  DIFFER  4600/4600  12 differing instruction groups.
- `receiver_joint_receiver_viewport_extent_original`  DIFFER  4600/4600  14 differing instruction groups.
- `receiver_joint_receiver_viewport_offset_x_group`  DIFFER  4600/4600  16 differing instruction groups.
- `receiver_joint_receiver_viewport_offset_y_group`  DIFFER  4600/4600  16 differing instruction groups.
- `receiver_joint_receiver_viewport_full_y_group`  DIFFER  4600/4600  14 differing instruction groups.
- `normalize_host_fallback`  DIFFER  4600/4600  7 differing instruction groups.
- `normalize_portable_native`  DIFFER  4600/4600  18 differing instruction groups.
- `normalize_portable_native_forced`  DIFFER  4600/4600  18 differing instruction groups.
- `lod_original_get_calls`  COMPILE_FAIL  —/4600
- `lod_original_get_first`  COMPILE_FAIL  —/4600
- `lod_original_get_nonnull`  COMPILE_FAIL  —/4600
- `lod_original_get_guard`  COMPILE_FAIL  —/4600
- `lod_original_get_nested_scopes`  COMPILE_FAIL  —/4600
- `lod_original_get_body_wrapper`  COMPILE_FAIL  —/4600
- `normalize_seed_separate`  DIFFER  4600/4600  17 differing instruction groups.
- `normalize_seed_shared`  DIFFER  4600/4600  17 differing instruction groups.
- `normalize_seed_shared_reciprocal_product_group`  DIFFER  4600/4600  18 differing instruction groups.
- `normalize_seed_shared_direct_return`  DIFFER  4600/4600  17 differing instruction groups.
- `normalize_seed_shared_native_width_unwrapped`  DIFFER  4600/4600  17 differing instruction groups.
- `lod_original_bound_get_calls`  COMPILE_FAIL  —/4600
- `lod_original_bound_get_first`  COMPILE_FAIL  —/4600
- `lod_original_bound_get_nonnull`  COMPILE_FAIL  —/4600
- `lod_original_bound_get_guard`  COMPILE_FAIL  —/4600
- `lod_original_bound_get_nested_scopes`  COMPILE_FAIL  —/4600
- `lod_original_bound_get_body_wrapper`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_calls`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_first`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_nonnull`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_guard`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_nested_scopes`  COMPILE_FAIL  —/4600
- `lod_original_complete_get_body_wrapper`  COMPILE_FAIL  —/4600
- `partial_tree_6a20af7d676e_a0b20d54`  DIFFER  4600/4600  7 differing instruction groups.
- `state_owner_preprocessor`  DIFFER  4600/4600  7 differing instruction groups.
- `state_owner_dead_statement`  DIFFER  4600/4600  7 differing instruction groups.
- `state_owner_dead_block`  DIFFER  4600/4600  7 differing instruction groups.
- `state_owner_dead_wrapper`  DIFFER  4600/4600  11 differing instruction groups.
- `state_owner_dead_assert_block`  DIFFER  4584/4600  57 differing instruction groups.
- `state_owner_dead_assert_separate`  DIFFER  4600/4600  11 differing instruction groups.
- `lod_original_typed_get_calls`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_original_typed_get_first`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_original_typed_get_nonnull`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_original_typed_get_guard`  DIFFER  4568/4600  51 differing instruction groups.
- `lod_original_typed_get_nested_scopes`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_original_typed_get_body_wrapper`  DIFFER  4600/4600  23 differing instruction groups.
- `matrix_owner_pretranslate_named_donor`  DIFFER  4600/4600  7 differing instruction groups.
- `matrix_owner_pretranslate_operands_donor`  DIFFER  4600/4600  7 differing instruction groups.
- `matrix_owner_pretranslate_group_xy`  DIFFER  4600/4600  32 differing instruction groups.
- `matrix_owner_pretranslate_group_x_product`  DIFFER  4600/4600  19 differing instruction groups.
- `matrix_owner_pretranslate_group_y_product`  DIFFER  4600/4600  18 differing instruction groups.
- `matrix_owner_pretranslate_group_z_product`  DIFFER  4600/4600  16 differing instruction groups.
- `matrix_owner_postscale_explicit`  DIFFER  4600/4600  7 differing instruction groups.
- `matrix_owner_postscale_y_group`  DIFFER  4600/4600  12 differing instruction groups.
- `matrix_owner_postscale_translation_group`  DIFFER  4600/4600  20 differing instruction groups.
- `fifth_constant_radius_paren_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_radius_cast_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_radius_cast_y`  DIFFER  4600/4600  16 differing instruction groups.
- `fifth_constant_radius_cast_x_and_y`  DIFFER  4600/4600  16 differing instruction groups.
- `fifth_constant_radius_unary_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_full_paren_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_full_paren_y`  DIFFER  4600/4600  16 differing instruction groups.
- `fifth_constant_full_paren_x_and_y`  DIFFER  4600/4600  16 differing instruction groups.
- `fifth_constant_explicit_member_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_vector_reference_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_coefficient_cast_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_ordinary_unchanged`  DIFFER  4600/4600  12 differing instruction groups.
- `fifth_constant_ordinary_y`  DIFFER  4600/4600  16 differing instruction groups.
- `fifth_constant_ordinary_x_and_y`  DIFFER  4600/4600  16 differing instruction groups.
- `lod_geometry_orig_loop_early_clip_swap`  DIFFER  4600/4600  18 differing instruction groups.
- `lod_geometry_orig_loop_early_ordinary`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_geometry_only_meshdata_get_clip_swap`  DIFFER  4600/4600  12 differing instruction groups.
- `lod_geometry_only_meshdata_get_ordinary`  DIFFER  4600/4600  7 differing instruction groups.
- `lod_geometry_only_lod_get_clip_swap`  DIFFER  4600/4600  12 differing instruction groups.
- `lod_geometry_only_lod_get_ordinary`  DIFFER  4600/4600  7 differing instruction groups.
- `lod_geometry_complete_type_after_counted_ordinary`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_geometry_complete_type_before_renderer_ordinary`  COMPILE_FAIL  —/4600
- `lod_geometry_original_required_owner_counts_ordinary`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_geometry_matrix_donor_order_ordinary`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_geometry_matrix_original_x_group_ordinary`  DIFFER  4600/4600  19 differing instruction groups.
- `lod_geometry_matrix_original_y_group_ordinary`  DIFFER  4600/4600  18 differing instruction groups.
- `lod_geometry_matrix_original_z_group_ordinary`  DIFFER  4600/4600  17 differing instruction groups.
- `lod_geometry_matrix_original_post_scale_ordinary`  DIFFER  4590/4600  23 differing instruction groups.
- `lod_geometry_matrix_original_post_y_group_ordinary`  DIFFER  4600/4600  23 differing instruction groups.
- `lod_geometry_root_original_portable_ordinary`  DIFFER  4590/4600  29 differing instruction groups.
- `scope_owner_current_draw_empty_body`  DIFFER  4600/4600  7 differing instruction groups.
- `scope_owner_current_draw_empty_statement`  DIFFER  4600/4600  7 differing instruction groups.
- `scope_owner_typed_draw_empty_body`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_typed_draw_empty_statement`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_typed_lod_empty_body`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_typed_lod_empty_statement`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_typed_both_empty_body`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_typed_both_empty_statement`  DIFFER  4600/4600  23 differing instruction groups.
- `scope_owner_transform_draw_empty_body`  DIFFER  4600/4600  17 differing instruction groups.
- `scope_owner_transform_draw_empty_statement`  DIFFER  4600/4600  17 differing instruction groups.
- `scope_owner_transform_lod_empty_body`  DIFFER  4600/4600  17 differing instruction groups.
- `scope_owner_transform_lod_empty_statement`  DIFFER  4600/4600  17 differing instruction groups.
- `scope_owner_transform_both_empty_body`  DIFFER  4600/4600  17 differing instruction groups.
- `scope_owner_transform_both_empty_statement`  DIFFER  4600/4600  17 differing instruction groups.
- `coordinate_source_selected_donor_rows`  DIFFER  4600/4600  7 differing instruction groups.
- `coordinate_source_selected_original_pretranslate`  DIFFER  4600/4600  7 differing instruction groups.
- `coordinate_source_selected_y_first_term`  DIFFER  4600/4600  19 differing instruction groups.
- `coordinate_source_selected_third_terms`  DIFFER  4600/4600  22 differing instruction groups.
- `coordinate_source_selected_native_division_clip`  DIFFER  4600/4600  8 differing instruction groups.
- `coordinate_source_typed_donor_rows`  DIFFER  4590/4600  23 differing instruction groups.
- `coordinate_source_typed_original_pretranslate`  DIFFER  4590/4600  23 differing instruction groups.
- `coordinate_source_typed_y_first_term`  DIFFER  4600/4600  25 differing instruction groups.
- `coordinate_source_typed_third_terms`  DIFFER  4590/4600  36 differing instruction groups.
- `coordinate_source_typed_native_division_clip`  DIFFER  4600/4600  18 differing instruction groups.
- `coordinate_source_matching_y_donor_rows`  DIFFER  4600/4600  17 differing instruction groups.
- `coordinate_source_matching_y_original_pretranslate`  DIFFER  4590/4600  23 differing instruction groups.
- `coordinate_source_matching_y_x_first_term`  DIFFER  4600/4600  25 differing instruction groups.
- `coordinate_source_matching_y_x_third_term`  DIFFER  4600/4600  27 differing instruction groups.
- `coordinate_source_matching_y_y_first_term`  DIFFER  4600/4600  23 differing instruction groups.
- `coordinate_source_matching_y_y_third_term`  DIFFER  4600/4600  25 differing instruction groups.
- `coordinate_source_matching_y_z_third_term`  DIFFER  4600/4600  23 differing instruction groups.
- `coordinate_source_matching_y_third_terms`  DIFFER  4600/4600  32 differing instruction groups.
- `coordinate_source_matching_y_original_postscale`  DIFFER  4600/4600  17 differing instruction groups.
- `coordinate_source_matching_y_clip_reciprocal`  DIFFER  4600/4600  19 differing instruction groups.
- `coordinate_source_matching_y_native_division`  DIFFER  4600/4600  25 differing instruction groups.
- `coordinate_source_matching_y_native_division_clip`  DIFFER  4600/4600  20 differing instruction groups.
- `coordinate_source_matching_y_stage_comparison`  DIFFER  4600/4600  17 differing instruction groups.
- `viewport_fade_boundary_original_doubles_ordinary_ordinary`  DIFFER  4588/4600  21 differing instruction groups.
- `viewport_fade_boundary_explicit_float_convert_ordinary_ordinary`  DIFFER  4600/4600  16 differing instruction groups.
- `viewport_fade_boundary_grouped_offsets_ordinary_ordinary`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_fade_boundary_grouped_offsets_y_ordinary_ordinary`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_fade_boundary_grouped_division_x_ordinary_ordinary`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_fade_boundary_grouped_double_y_ordinary_ordinary`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_fade_boundary_grouped_extent_ordinary_ordinary`  DIFFER  4604/4600  11 differing instruction groups.
- `viewport_fade_boundary_widths_double_ordinary_ordinary`  DIFFER  4588/4600  17 differing instruction groups.
- `viewport_fade_boundary_center_double_ordinary_ordinary`  DIFFER  4600/4600  16 differing instruction groups.
- `viewport_fade_boundary_ordinary_all_divisions_ordinary`  DIFFER  4584/4600  13 differing instruction groups.
- `viewport_fade_boundary_ordinary_near_division_ordinary`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_fade_boundary_ordinary_far_divisions_ordinary`  DIFFER  4584/4600  13 differing instruction groups.
- `viewport_fade_boundary_ordinary_far_factor_group_ordinary`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_fade_boundary_ordinary_far_factor_right_ordinary`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_fade_boundary_original_doubles_all_divisions_ordinary`  DIFFER  4584/4600  23 differing instruction groups.
- `viewport_fade_boundary_original_doubles_near_division_ordinary`  DIFFER  4588/4600  21 differing instruction groups.
- `viewport_fade_boundary_original_doubles_far_divisions_ordinary`  DIFFER  4584/4600  23 differing instruction groups.
- `viewport_fade_boundary_original_doubles_far_factor_group_ordinary`  DIFFER  4588/4600  21 differing instruction groups.
- `viewport_fade_boundary_original_doubles_far_factor_right_ordinary`  DIFFER  4588/4600  21 differing instruction groups.
- `viewport_fade_boundary_ordinary_ordinary_source_order`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_fade_boundary_ordinary_ordinary_source_group`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_fade_boundary_ordinary_ordinary_real_extent`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_fade_boundary_original_doubles_ordinary_source_order`  DIFFER  4588/4600  25 differing instruction groups.
- `viewport_fade_boundary_original_doubles_ordinary_source_group`  DIFFER  4588/4600  20 differing instruction groups.
- `viewport_fade_boundary_original_doubles_ordinary_real_extent`  DIFFER  4588/4600  20 differing instruction groups.
- `viewport_region_offset_x`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_region_offset_y`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_region_uv_width`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_uv_constant`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_extent_width`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_extent_height`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_centre_x`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_region_centre_y`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_region_uv_width_uv_constant`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_uv_width_uv_constant_extent_width`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_uv_width_uv_constant_extent_width_extent_height`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_region_uv_width_uv_constant_centre_x_centre_y`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_region_uv_width_uv_constant_offset_x_offset_y`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_region_extent_width_extent_height_centre_x_centre_y`  DIFFER  4600/4600  11 differing instruction groups.
- `viewport_region_extent_width_extent_height_offset_x_offset_y`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_region_uv_width_uv_constant_extent_width_extent_height_centre_x_centre_y`  DIFFER  4600/4600  11 differing instruction groups.
- `source_geometry_eager_inplace_raw`  DIFFER  4600/4600  8 differing instruction groups.
- `source_geometry_eager_inplace_raw_group`  DIFFER  4600/4600  12 differing instruction groups.
- `source_geometry_eager_inplace_raw_component_ctor`  DIFFER  4600/4600  8 differing instruction groups.
- `source_geometry_eager_copy_component_ctors`  DIFFER  4600/4600  8 differing instruction groups.
- `source_geometry_eager_scale_name_x`  DIFFER  4600/4600  23 differing instruction groups.
- `source_geometry_eager_scale_name_y`  DIFFER  4600/4600  23 differing instruction groups.
- `source_geometry_eager_scale_name_z`  DIFFER  4600/4600  23 differing instruction groups.
- `source_geometry_eager_scale_name_xyz`  DIFFER  4586/4600  63 differing instruction groups.
- `source_geometry_eager_scale_name_grouped_z`  DIFFER  4600/4600  23 differing instruction groups.
- `source_geometry_eager_post_raw_double`  DIFFER  4584/4600  79 differing instruction groups.
- `source_geometry_eager_post_raw_float`  DIFFER  4600/4600  10 differing instruction groups.
- `source_geometry_eager_post_raw_x_double`  DIFFER  4616/4600  166 differing instruction groups.
- `source_geometry_eager_post_raw_y_double`  DIFFER  4616/4600  166 differing instruction groups.
- `source_geometry_eager_pretranslate_all_product_groups`  DIFFER  4600/4600  35 differing instruction groups.
- `source_geometry_eager_pretranslate_partial_sums`  DIFFER  4600/4600  35 differing instruction groups.
- `source_geometry_eager_pretranslate_compound`  DIFFER  4600/4600  8 differing instruction groups.
- `source_geometry_eager_pretranslate_scope`  DIFFER  4600/4600  8 differing instruction groups.
- `source_geometry_exact_y_inplace_raw`  DIFFER  4600/4600  20 differing instruction groups.
- `source_geometry_exact_y_inplace_raw_group`  DIFFER  4600/4600  20 differing instruction groups.
- `source_geometry_exact_y_inplace_raw_component_ctor`  DIFFER  4600/4600  20 differing instruction groups.
- `source_geometry_exact_y_copy_component_ctors`  DIFFER  4600/4600  20 differing instruction groups.
- `source_geometry_exact_y_scale_name_xyz`  DIFFER  4600/4600  39 differing instruction groups.
- `source_geometry_exact_y_scale_name_grouped_z`  DIFFER  4600/4600  30 differing instruction groups.
- `source_geometry_exact_y_post_raw_double`  DIFFER  4584/4600  84 differing instruction groups.
- `source_geometry_exact_y_post_raw_float`  DIFFER  4600/4600  24 differing instruction groups.
- `source_geometry_exact_y_pretranslate_all_product_groups`  DIFFER  4600/4600  31 differing instruction groups.
- `source_geometry_exact_y_pretranslate_partial_sums`  DIFFER  4600/4600  38 differing instruction groups.
- `source_geometry_exact_y_pretranslate_compound`  DIFFER  4600/4600  18 differing instruction groups.
- `viewport_extent_selected_group_both`  DIFFER  4600/4600  9 differing instruction groups.
- `viewport_extent_selected_explicit_float_both`  DIFFER  4600/4600  9 differing instruction groups.
- `viewport_extent_selected_cast_float_both`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_selected_functional_float_both`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_selected_nested_float_both`  DIFFER  4600/4600  9 differing instruction groups.
- `viewport_extent_eager_group_width`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_group_height`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_group_both`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_explicit_float_width`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_explicit_float_height`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_explicit_float_both`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_cast_float_width`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_cast_float_height`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_cast_float_both`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_functional_float_width`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_functional_float_height`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_functional_float_both`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_extent_eager_nested_float_width`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_nested_float_height`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_extent_eager_nested_float_both`  DIFFER  4600/4600  7 differing instruction groups.
- `native_window_selected_direct_products`  DIFFER  4600/4600  14 differing instruction groups.
- `native_window_selected_direct_direct`  DIFFER  4600/4600  12 differing instruction groups.
- `native_window_selected_direct_both_with_offsets`  DIFFER  4600/4600  14 differing instruction groups.
- `native_window_selected_direct_double_width`  DIFFER  4600/4600  16 differing instruction groups.
- `native_window_selected_scaled_declared_products`  DIFFER  4600/4600  15 differing instruction groups.
- `native_window_selected_scaled_declared_direct`  DIFFER  4600/4600  11 differing instruction groups.
- `native_window_selected_scaled_declared_both_with_offsets`  DIFFER  4600/4600  15 differing instruction groups.
- `native_window_selected_scaled_declared_double_width`  DIFFER  4600/4600  17 differing instruction groups.
- `native_window_selected_combined_divisor_products`  DIFFER  4600/4600  15 differing instruction groups.
- `native_window_selected_combined_divisor_direct`  DIFFER  4600/4600  13 differing instruction groups.
- `native_window_selected_combined_divisor_both_with_offsets`  DIFFER  4600/4600  15 differing instruction groups.
- `native_window_selected_combined_divisor_double_width`  DIFFER  4600/4600  17 differing instruction groups.
- `native_window_eager_direct_products`  DIFFER  4600/4600  14 differing instruction groups.
- `native_window_eager_direct_direct`  DIFFER  4600/4600  17 differing instruction groups.
- `native_window_eager_direct_both_with_offsets`  DIFFER  4600/4600  14 differing instruction groups.
- `native_window_eager_direct_double_width`  DIFFER  4600/4600  16 differing instruction groups.
- `native_window_eager_scaled_declared_products`  DIFFER  4600/4600  11 differing instruction groups.
- `native_window_eager_scaled_declared_direct`  DIFFER  4600/4600  12 differing instruction groups.
- `native_window_eager_scaled_declared_both_with_offsets`  DIFFER  4600/4600  13 differing instruction groups.
- `matrix_product_slice_pretranslate_11`  DIFFER  4600/4600  22 differing instruction groups.
- `native_window_eager_scaled_declared_double_width`  DIFFER  4600/4600  13 differing instruction groups.
- `matrix_product_slice_pretranslate_21`  DIFFER  4600/4600  23 differing instruction groups.
- `native_window_eager_combined_divisor_products`  DIFFER  4600/4600  13 differing instruction groups.
- `matrix_product_slice_pretranslate_31`  DIFFER  4600/4600  22 differing instruction groups.
- `native_window_eager_combined_divisor_direct`  DIFFER  4600/4600  14 differing instruction groups.
- `matrix_product_slice_pretranslate_12`  DIFFER  4600/4600  22 differing instruction groups.
- `native_window_eager_combined_divisor_both_with_offsets`  DIFFER  4600/4600  13 differing instruction groups.
- `matrix_product_slice_pretranslate_22`  DIFFER  4600/4600  21 differing instruction groups.
- `native_window_eager_combined_divisor_double_width`  DIFFER  4600/4600  15 differing instruction groups.
- `matrix_product_slice_pretranslate_32`  DIFFER  4600/4600  19 differing instruction groups.
- `matrix_product_slice_pretranslate_13`  DIFFER  4590/4600  17 differing instruction groups.
- `matrix_product_slice_pretranslate_23`  DIFFER  4600/4600  22 differing instruction groups.
- `matrix_product_slice_pretranslate_33`  DIFFER  4600/4600  19 differing instruction groups.
- `matrix_product_slice_transform_y_9`  DIFFER  4574/4600  21 differing instruction groups.
- `matrix_product_slice_transform_y_10`  DIFFER  4600/4600  23 differing instruction groups.
- `matrix_product_slice_transform_y_11`  DIFFER  4584/4600  20 differing instruction groups.
- `matrix_product_slice_transform_y_12`  DIFFER  4600/4600  19 differing instruction groups.
- `matrix_product_slice_lod_bool_conversion`  DIFFER  4600/4600  8 differing instruction groups.
- `matrix_product_slice_lod_bool_eq`  DIFFER  4600/4600  8 differing instruction groups.
- `matrix_product_slice_lod_bool_group`  DIFFER  4600/4600  8 differing instruction groups.
- `matrix_product_slice_lod_integer_bound`  DIFFER  4600/4600  14 differing instruction groups.
- `matrix_product_slice_lod_fixed_count`  DIFFER  4600/4600  8 differing instruction groups.
- `geometry_reviewed_original_compilands`  DIFFER  4600/4600  7 differing instruction groups.
- `clip_literal_coefficient_group_ordinary_cos_left`  DIFFER  4600/4600  16 differing instruction groups.
- `clip_literal_inverse_half_ordinary_cos_left`  DIFFER  4600/4600  16 differing instruction groups.
- `clip_literal_divisions_ordinary_cos_left`  DIFFER  4600/4600  13 differing instruction groups.
- `clip_literal_grouped_divisions_ordinary_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_divisor_group_ordinary_cos_left`  DIFFER  4600/4600  17 differing instruction groups.
- `clip_literal_single_divisor_group_ordinary_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_constant_ordinary_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_expanded_half_ordinary_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_ordinary_double_donor_cos_left`  DIFFER  4600/4600  15 differing instruction groups.
- `clip_literal_ordinary_qword_literal_cos_left`  DIFFER  4600/4600  15 differing instruction groups.
- `clip_literal_ordinary_float_product_left_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_ordinary_float_scale_group_cos_left`  DIFFER  4600/4600  11 differing instruction groups.
- `clip_literal_ordinary_ordinary_cos_left_group`  DIFFER  4600/4600  11 differing instruction groups.
- `clip_literal_ordinary_ordinary_cos_left_static`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_ordinary_ordinary_cos_left_signed`  DIFFER  4600/4600  16 differing instruction groups.
- `clip_literal_ordinary_ordinary_cos_left_neg_difference`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_ordinary_ordinary_cos_right_signed`  DIFFER  4600/4600  11 differing instruction groups.
- `clip_literal_divisions_double_donor_cos_left`  DIFFER  4600/4600  16 differing instruction groups.
- `clip_literal_divisions_float_product_left_cos_left`  DIFFER  4600/4600  13 differing instruction groups.
- `clip_literal_expanded_half_double_donor_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `clip_literal_expanded_half_float_product_left_cos_left`  DIFFER  4600/4600  12 differing instruction groups.
- `original_formals_reviewed_matrix_const_factors`  DIFFER  4600/4600  7 differing instruction groups.
- `original_formals_reviewed_matrix_translate_const`  DIFFER  4600/4600  7 differing instruction groups.
- `original_formals_reviewed_matrix_const_both`  DIFFER  4600/4600  7 differing instruction groups.
- `original_formals_reviewed_matrix_compound_z`  DIFFER  4600/4600  16 differing instruction groups.
- `original_formals_reviewed_normalise_product_group_x`  DIFFER  4600/4600  12 differing instruction groups.
- `original_formals_reviewed_normalise_product_group_y`  DIFFER  4600/4600  17 differing instruction groups.
- `original_formals_reviewed_normalise_original_const_forms`  DIFFER  4600/4600  7 differing instruction groups.
- `original_formals_eager_matrix_const_factors`  DIFFER  4600/4600  8 differing instruction groups.
- `original_formals_eager_matrix_translate_const`  DIFFER  4600/4600  8 differing instruction groups.
- `original_formals_eager_matrix_const_both`  DIFFER  4600/4600  8 differing instruction groups.
- `partial_tree_1d4aa74860b1_a0b20d54`  DIFFER  4600/4600  7 differing instruction groups.
- `point_neighbor_front_negative_group_ordinary`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_front_product_group_ordinary`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_front_numeric_rhs_ordinary`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_front_direct_init_ordinary`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_back_direct_init_ordinary`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_top_z_group_ordinary`  DIFFER  4600/4600  26 differing instruction groups.
- `point_neighbor_ordinary_compound_pretranslate_z`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_ordinary_group_postscale_z`  DIFFER  4600/4600  26 differing instruction groups.
- `point_neighbor_ordinary_group_transform_z_1`  DIFFER  4602/4600  26 differing instruction groups.
- `point_neighbor_ordinary_group_transform_z_2`  DIFFER  4602/4600  26 differing instruction groups.
- `point_neighbor_ordinary_group_transform_z_sum`  DIFFER  4600/4600  22 differing instruction groups.
- `point_neighbor_ordinary_original_magnitude`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_ordinary_direct_subtraction`  DIFFER  4600/4600  19 differing instruction groups.
- `point_neighbor_ordinary_preserve_original_ld`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_ordinary_pretranslate_one_less_group`  DIFFER  4600/4600  18 differing instruction groups.
- `point_neighbor_front_negative_group_group_transform_z_1`  DIFFER  4602/4600  26 differing instruction groups.
- `point_neighbor_front_negative_group_group_transform_z_2`  DIFFER  4602/4600  26 differing instruction groups.
- `point_neighbor_front_negative_group_original_magnitude`  DIFFER  4600/4600  20 differing instruction groups.
- `point_neighbor_front_negative_group_group_postscale_z`  DIFFER  4600/4600  26 differing instruction groups.
- `partial_tree_a6c63044ee0e_b73f6436`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_explicit_bounds_result`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_grouped_bounds_result`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_original_copy_ctor`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_grouped_centre`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_grouped_diagonal`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_selected_direct_subtraction`  DIFFER  4588/4600  36 differing instruction groups.
- `bounds_owner_selected_subtraction_and_return`  DIFFER  4588/4600  36 differing instruction groups.
- `bounds_owner_selected_both_owner_constructors`  DIFFER  4600/4600  7 differing instruction groups.
- `bounds_owner_matching_y_explicit_bounds_result`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_grouped_bounds_result`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_original_copy_ctor`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_direct_centre`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_grouped_centre`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_grouped_diagonal`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_grouped_result_both`  DIFFER  4600/4600  20 differing instruction groups.
- `bounds_owner_matching_y_direct_subtraction`  DIFFER  4600/4600  19 differing instruction groups.
- `bounds_owner_matching_y_subtraction_and_return`  DIFFER  4600/4600  19 differing instruction groups.
- `bounds_owner_matching_y_subtraction_and_copy`  DIFFER  4600/4600  19 differing instruction groups.
- `bounds_owner_matching_y_both_owner_constructors`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_selected_lod_select`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_lod_has`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_mesh_box`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_box_centre`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_box_diagonal`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_lod_both`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_selected_box_chain`  DIFFER  4600/4600  7 differing instruction groups.
- `geometry_budget_eager_lod_both`  DIFFER  4600/4600  8 differing instruction groups.
- `geometry_budget_eager_box_chain`  DIFFER  4600/4600  8 differing instruction groups.
- `geometry_budget_matching_y_lod_select`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_lod_has`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_mesh_box`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_box_centre`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_box_diagonal`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_lod_both`  DIFFER  4600/4600  20 differing instruction groups.
- `geometry_budget_matching_y_box_chain`  DIFFER  4600/4600  20 differing instruction groups.
- `actual_state_checks_selected_both_set`  DIFFER  4584/4600  54 differing instruction groups.
- `actual_state_checks_selected_dirty`  DIFFER  4584/4600  57 differing instruction groups.
- `actual_state_checks_selected_all`  DIFFER  4584/4600  57 differing instruction groups.
- `actual_state_checks_selected_all_block`  DIFFER  4584/4600  57 differing instruction groups.
- `actual_state_checks_selected_all_plain_release`  DIFFER  4600/4600  7 differing instruction groups.
- `actual_state_checks_selected_all_capture`  DIFFER  4584/4600  57 differing instruction groups.
- `actual_state_checks_eager_both_set`  DIFFER  4584/4600  50 differing instruction groups.
- `actual_state_checks_eager_dirty`  DIFFER  4588/4600  53 differing instruction groups.
- `actual_state_checks_eager_all`  DIFFER  4584/4600  56 differing instruction groups.
- `actual_state_checks_eager_all_block`  DIFFER  4584/4600  56 differing instruction groups.
- `actual_state_checks_eager_all_plain_release`  DIFFER  4600/4600  8 differing instruction groups.
- `actual_state_checks_eager_all_capture`  DIFFER  4584/4600  56 differing instruction groups.
- `actual_state_checks_exact_y_all`  DIFFER  4584/4600  60 differing instruction groups.
- `actual_state_checks_exact_y_all_block`  DIFFER  4584/4600  60 differing instruction groups.
- `actual_state_checks_exact_y_all_capture`  DIFFER  4584/4600  60 differing instruction groups.
- `uv_precision_ordinary_left_canonical`  DIFFER  4600/4600  157 differing instruction groups.
- `uv_precision_ordinary_right_canonical`  DIFFER  4619/4600  141 differing instruction groups.
- `uv_precision_ordinary_both_canonical`  DIFFER  4620/4600  98 differing instruction groups.
- `uv_precision_ordinary_neither_original_double`  DIFFER  4600/4600  8 differing instruction groups.
- `uv_precision_ordinary_both_original_double`  DIFFER  4620/4600  98 differing instruction groups.
- `uv_precision_ordinary_neither_original_float`  DIFFER  4600/4600  7 differing instruction groups.
- `uv_precision_ordinary_both_original_float`  DIFFER  4620/4600  98 differing instruction groups.
- `uv_precision_ordinary_both_grouped_division`  DIFFER  4620/4600  98 differing instruction groups.
- `uv_precision_eager_left_canonical`  DIFFER  4600/4600  163 differing instruction groups.
- `uv_precision_eager_right_canonical`  DIFFER  4619/4600  145 differing instruction groups.
- `uv_precision_eager_both_canonical`  DIFFER  4620/4600  95 differing instruction groups.
- `uv_precision_eager_neither_original_double`  DIFFER  4600/4600  11 differing instruction groups.
- `uv_precision_eager_both_original_double`  DIFFER  4620/4600  95 differing instruction groups.
- `uv_precision_eager_neither_original_float`  DIFFER  4600/4600  8 differing instruction groups.
- `uv_precision_eager_both_original_float`  DIFFER  4620/4600  95 differing instruction groups.
- `uv_precision_eager_both_grouped_division`  DIFFER  4620/4600  95 differing instruction groups.
- `uv_precision_exact_y_both_canonical`  DIFFER  4622/4600  96 differing instruction groups.
- `uv_precision_exact_y_neither_original_double`  DIFFER  4600/4600  21 differing instruction groups.
- `uv_precision_exact_y_both_original_double`  DIFFER  4622/4600  96 differing instruction groups.
- `uv_precision_exact_y_neither_original_float`  DIFFER  4600/4600  24 differing instruction groups.
- `uv_precision_exact_y_both_original_float`  DIFFER  4622/4600  96 differing instruction groups.
- `uv_precision_exact_y_both_grouped_division`  DIFFER  4622/4600  96 differing instruction groups.
- `fade_argument_near_one_double`  DIFFER  4600/4600  25 differing instruction groups.
- `fade_argument_near_one_group`  DIFFER  4600/4600  12 differing instruction groups.
- `fade_argument_near_two_quotient`  DIFFER  4600/4600  8 differing instruction groups.
- `fade_argument_near_two_product_negation`  DIFFER  4600/4600  10 differing instruction groups.
- `fade_argument_far_one_signed_quotient_float`  DIFFER  4600/4600  8 differing instruction groups.
- `fade_argument_far_one_signed_quotient_double`  DIFFER  4600/4600  13 differing instruction groups.
- `fade_argument_far_one_negative_reciprocal`  DIFFER  4590/4600  10 differing instruction groups.
- `fade_argument_far_one_negative_reciprocal_group`  DIFFER  4588/4600  13 differing instruction groups.
- `fade_argument_far_one_multiply_reversed`  DIFFER  4600/4600  8 differing instruction groups.
- `fade_argument_far_two_direct_float`  DIFFER  4600/4600  8 differing instruction groups.
- `fade_argument_far_two_direct_double`  DIFFER  4600/4600  12 differing instruction groups.
- `fade_argument_far_two_group_product`  DIFFER  4600/4600  14 differing instruction groups.
- `fade_argument_far_two_group_double`  DIFFER  4600/4600  16 differing instruction groups.
- `fade_argument_far_two_reverse_terms`  DIFFER  4600/4600  8 differing instruction groups.
- `transform_esi_top_height_group_grouped_division`  DIFFER  4600/4600  23 differing instruction groups.
- `transform_esi_top_height_group_early_receiver`  DIFFER  4600/4600  16 differing instruction groups.
- `transform_esi_normalise_sum_group_grouped_division`  DIFFER  4600/4600  23 differing instruction groups.
- `transform_esi_normalise_sum_group_early_receiver`  DIFFER  4600/4600  16 differing instruction groups.
- `transform_esi_postscale_y_group_grouped_division`  DIFFER  4600/4600  23 differing instruction groups.
- `transform_esi_postscale_y_group_early_receiver`  DIFFER  4600/4600  16 differing instruction groups.
- `transform_esi_normalise_store_x_group_grouped_division`  DIFFER  4600/4600  23 differing instruction groups.
- `transform_esi_normalise_store_x_group_early_receiver`  DIFFER  4600/4600  16 differing instruction groups.
- `transform_esi_newton_original_group_grouped_division`  DIFFER  4600/4600  23 differing instruction groups.
- `transform_esi_newton_original_group_early_receiver`  DIFFER  4600/4600  16 differing instruction groups.
- `transform_esi_newton_original_group_no_seed_grouped_division`  DIFFER  4590/4600  23 differing instruction groups.
- `transform_esi_newton_original_group_no_seed_early_receiver`  DIFFER  4600/4600  22 differing instruction groups.
- `primitive_conversion_selected_original`  DIFFER  4600/4600  7 differing instruction groups.
- `primitive_conversion_selected_original_assert`  DIFFER  4600/4600  7 differing instruction groups.
- `primitive_conversion_selected_typed_device`  DIFFER  4600/4600  7 differing instruction groups.
- `primitive_conversion_selected_typed_assert`  DIFFER  4600/4600  7 differing instruction groups.
- `primitive_conversion_selected_typed_all_checks`  DIFFER  4600/4600  28 differing instruction groups.
- `primitive_conversion_selected_typed_all_bare`  DIFFER  4600/4600  28 differing instruction groups.
- `primitive_conversion_selected_typed_returns`  DIFFER  4600/4600  20 differing instruction groups.
- `primitive_conversion_eager_original`  DIFFER  4600/4600  8 differing instruction groups.
- `primitive_conversion_eager_original_assert`  DIFFER  4600/4600  8 differing instruction groups.
- `primitive_conversion_eager_typed_device`  DIFFER  4600/4600  8 differing instruction groups.
- `primitive_conversion_eager_typed_assert`  DIFFER  4600/4600  8 differing instruction groups.
- `primitive_conversion_eager_typed_all_checks`  DIFFER  4600/4600  16 differing instruction groups.
- `primitive_conversion_eager_typed_all_bare`  DIFFER  4600/4600  16 differing instruction groups.
- `primitive_conversion_eager_typed_returns`  DIFFER  4600/4600  8 differing instruction groups.
- `primitive_conversion_exact_y_original`  DIFFER  4600/4600  20 differing instruction groups.
- `primitive_conversion_exact_y_original_assert`  DIFFER  4600/4600  17 differing instruction groups.
- `primitive_conversion_exact_y_typed_assert`  DIFFER  4600/4600  17 differing instruction groups.
- `primitive_conversion_exact_y_typed_all_checks`  DIFFER  4590/4600  30 differing instruction groups.
- `primitive_conversion_reviewed_types`  DIFFER  4600/4600  7 differing instruction groups.
- `residual_point_scale_x`  DIFFER  4600/4600  27 differing instruction groups.
- `residual_point_scale_y`  DIFFER  4600/4600  27 differing instruction groups.
- `residual_point_scale_z`  DIFFER  4600/4600  26 differing instruction groups.
- `residual_point_scale_xy`  DIFFER  4600/4600  28 differing instruction groups.
- `residual_point_scale_yz`  DIFFER  4600/4600  29 differing instruction groups.
- `residual_point_scale_all`  DIFFER  4600/4600  35 differing instruction groups.
- `residual_point_correct_subtraction`  DIFFER  4600/4600  15 differing instruction groups.
- `residual_point_correct_subtraction_with_y`  DIFFER  4600/4600  16 differing instruction groups.
- `residual_point_front_negative`  DIFFER  4600/4600  16 differing instruction groups.
- `residual_point_transform_z_third_group`  DIFFER  4600/4600  18 differing instruction groups.
- `residual_point_transform_z_fourth_group`  DIFFER  4600/4600  13 differing instruction groups.
- `residual_point_transform_y_fourth_group`  DIFFER  4600/4600  13 differing instruction groups.
- `front_point_actual_difference`  DIFFER  4600/4600  15 differing instruction groups.
- `front_point_neg_x`  DIFFER  4600/4600  23 differing instruction groups.
- `front_point_neg_y`  DIFFER  4600/4600  23 differing instruction groups.
- `front_point_neg_z`  DIFFER  4600/4600  23 differing instruction groups.
- `front_point_neg_xy`  DIFFER  4600/4600  26 differing instruction groups.
- `front_point_neg_xyz`  DIFFER  4600/4600  27 differing instruction groups.
- `front_point_translation_x_addend`  DIFFER  4600/4600  13 differing instruction groups.
- `front_point_translation_y_addend`  DIFFER  4600/4600  13 differing instruction groups.
- `front_point_translation_xy_addends`  DIFFER  4600/4600  17 differing instruction groups.
- `front_point_actual_term_order`  DIFFER  4600/4600  13 differing instruction groups.
- `front_point_actual_difference_term_order`  DIFFER  4600/4600  15 differing instruction groups.
- `author_merged_ecee975d2a37_5bbafa42`  DIFFER  4600/4600  7 differing instruction groups.
- `mesh_pointer_check_selected_inline_release_assert`  DIFFER  4600/4600  19 differing instruction groups.
- `mesh_pointer_check_selected_bare_release_assert`  DIFFER  4600/4600  19 differing instruction groups.
- `mesh_pointer_check_selected_ordinary_release`  DIFFER  4600/4600  7 differing instruction groups.
- `mesh_pointer_check_selected_paired_count_check`  DIFFER  4600/4600  11 differing instruction groups.
- `mesh_pointer_check_selected_both_original_guards`  DIFFER  4600/4600  19 differing instruction groups.
- `mesh_pointer_check_joined_inline_release_assert`  DIFFER  4590/4600  26 differing instruction groups.
- `mesh_pointer_check_joined_bare_release_assert`  DIFFER  4590/4600  26 differing instruction groups.
- `mesh_pointer_check_joined_ordinary_release`  DIFFER  4600/4600  16 differing instruction groups.
- `mesh_pointer_check_joined_paired_count_check`  DIFFER  4600/4600  16 differing instruction groups.
- `mesh_pointer_check_joined_both_original_guards`  DIFFER  4590/4600  26 differing instruction groups.
- `mesh_pointer_check_joined_c_style_static_conversion`  DIFFER  4600/4600  16 differing instruction groups.
- `mesh_pointer_check_near_bare_release_assert`  DIFFER  4590/4600  26 differing instruction groups.
- `mesh_pointer_check_near_paired_count_check`  DIFFER  4600/4600  13 differing instruction groups.
- `mesh_pointer_check_near_both_original_guards`  DIFFER  4590/4600  26 differing instruction groups.
- `mesh_pointer_check_near_c_style_static_conversion`  DIFFER  4600/4600  13 differing instruction groups.
- `actual_subtraction_join_field_product_y`  DIFFER  4600/4600  21 differing instruction groups.
- `actual_subtraction_join_field_product_x`  DIFFER  4600/4600  25 differing instruction groups.
- `actual_subtraction_join_normalise_group`  DIFFER  4600/4600  21 differing instruction groups.
- `actual_subtraction_join_normalise_product_x`  DIFFER  4600/4600  21 differing instruction groups.
- `actual_subtraction_join_normalise_product_y`  DIFFER  4600/4600  23 differing instruction groups.
- `actual_subtraction_join_normalise_group_and_x`  DIFFER  4600/4600  21 differing instruction groups.
- `actual_subtraction_join_inplace_original`  DIFFER  4600/4600  15 differing instruction groups.
- `actual_subtraction_join_component_copy_original`  DIFFER  4600/4600  15 differing instruction groups.
- `actual_subtraction_join_original_ctor_and_matrix`  DIFFER  4600/4600  15 differing instruction groups.
- `actual_subtraction_join_centre_divisor`  DIFFER  4600/4600  15 differing instruction groups.
- `actual_subtraction_join_explicit_magnitude`  DIFFER  4600/4600  15 differing instruction groups.
- `partial_tree_ecee975d2a37_5bbafa42`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_copy_eager_copy_direct`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_copy_eager_copy_group`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_copy_eager_float_offset_type`  DIFFER  4600/4600  19 differing instruction groups.
- `viewport_copy_eager_integer_half`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_copy_eager_center_half_group`  DIFFER  4600/4600  12 differing instruction groups.
- `viewport_copy_eager_extent_half_group`  DIFFER  4600/4600  14 differing instruction groups.
- `viewport_copy_near_copy_direct`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_copy_group`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_scale_double`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_scale_integer`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_scale_direct`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_float_offset_type`  DIFFER  4600/4600  28 differing instruction groups.
- `viewport_copy_near_integer_half`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_center_half_group`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_extent_half_group`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_viewport_index_converted`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_copy_near_viewport_explicit_typed_call`  DIFFER  4600/4600  13 differing instruction groups.
- `original_compiland_options_selected_speed`  DIFFER  4600/4600  7 differing instruction groups.
- `original_compiland_options_selected_speed_inline`  DIFFER  4600/4600  7 differing instruction groups.
- `original_compiland_options_selected_speed_calling_convention`  DIFFER  4600/4600  7 differing instruction groups.
- `original_compiland_options_selected_speed_thread_local`  DIFFER  4600/4600  7 differing instruction groups.
- `original_compiland_options_selected_strict_fp_mapping`  DIFFER  5430/4600  146 differing instruction groups.
- `original_compiland_options_eager_speed`  DIFFER  4600/4600  8 differing instruction groups.
- `original_compiland_options_eager_speed_inline`  DIFFER  4600/4600  8 differing instruction groups.
- `original_compiland_options_eager_speed_calling_convention`  DIFFER  4600/4600  8 differing instruction groups.
- `original_compiland_options_eager_strict_fp_mapping`  DIFFER  5405/4600  151 differing instruction groups.
- `original_compiland_options_near_speed`  DIFFER  4600/4600  13 differing instruction groups.
- `original_compiland_options_near_speed_inline`  DIFFER  4600/4600  13 differing instruction groups.
- `original_compiland_options_near_speed_calling_convention`  DIFFER  4600/4600  13 differing instruction groups.
- `original_compiland_options_near_strict_fp_mapping`  DIFFER  5405/4600  151 differing instruction groups.
- `original_magnitude_selected_ordinary`  DIFFER  4600/4600  7 differing instruction groups.
- `original_magnitude_selected_x`  DIFFER  4600/4600  7 differing instruction groups.
- `original_magnitude_selected_y`  DIFFER  4600/4600  10 differing instruction groups.
- `original_magnitude_selected_z`  DIFFER  4600/4600  11 differing instruction groups.
- `original_magnitude_selected_xy`  DIFFER  4600/4600  9 differing instruction groups.
- `original_magnitude_selected_xz`  DIFFER  4600/4600  7 differing instruction groups.
- `original_magnitude_selected_yz`  DIFFER  4600/4600  11 differing instruction groups.
- `original_magnitude_selected_xyz`  DIFFER  4600/4600  7 differing instruction groups.
- `original_magnitude_selected_explicit_float_sum`  DIFFER  4600/4600  7 differing instruction groups.
- `original_magnitude_eager_ordinary`  DIFFER  4600/4600  8 differing instruction groups.
- `original_magnitude_eager_y`  DIFFER  4600/4600  13 differing instruction groups.
- `original_magnitude_eager_z`  DIFFER  4600/4600  14 differing instruction groups.
- `original_magnitude_eager_xz`  DIFFER  4600/4600  10 differing instruction groups.
- `original_magnitude_eager_xyz`  DIFFER  4600/4600  10 differing instruction groups.
- `original_magnitude_eager_explicit_float_sum`  DIFFER  4600/4600  8 differing instruction groups.
- `original_magnitude_near_ordinary`  DIFFER  4600/4600  13 differing instruction groups.
- `original_magnitude_near_x`  DIFFER  4600/4600  13 differing instruction groups.
- `original_magnitude_near_y`  DIFFER  4600/4600  16 differing instruction groups.
- `original_magnitude_near_z`  DIFFER  4600/4600  17 differing instruction groups.
- `original_magnitude_near_xy`  DIFFER  4600/4600  15 differing instruction groups.
- `original_magnitude_near_xz`  DIFFER  4600/4600  13 differing instruction groups.
- `original_magnitude_near_yz`  DIFFER  4600/4600  17 differing instruction groups.
- `original_magnitude_near_xyz`  DIFFER  4600/4600  13 differing instruction groups.
- `original_magnitude_near_explicit_float_sum`  DIFFER  4600/4600  13 differing instruction groups.
- `original_subtraction_rvo_selected_group_x`  DIFFER  4588/4600  36 differing instruction groups.
- `original_subtraction_rvo_selected_group_y`  DIFFER  4588/4600  36 differing instruction groups.
- `original_subtraction_rvo_selected_group_z`  DIFFER  4588/4600  37 differing instruction groups.
- `original_subtraction_rvo_selected_group_xyz`  DIFFER  4588/4600  37 differing instruction groups.
- `original_subtraction_rvo_selected_cast_xyz`  DIFFER  4588/4600  36 differing instruction groups.
- `original_subtraction_rvo_selected_ordinary`  DIFFER  4588/4600  36 differing instruction groups.
- `original_subtraction_rvo_near_group_x`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_near_group_y`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_near_group_z`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_near_group_xy`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_near_group_xz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_near_group_yz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_near_group_xyz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_near_cast_xyz`  DIFFER  4600/4600  15 differing instruction groups.
- `original_subtraction_rvo_near_ordinary`  DIFFER  4600/4600  15 differing instruction groups.
- `original_subtraction_rvo_joined_group_x`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_joined_group_y`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_joined_group_z`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_joined_group_xy`  DIFFER  4600/4600  19 differing instruction groups.
- `original_subtraction_rvo_joined_group_xz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_joined_group_yz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_joined_group_xyz`  DIFFER  4600/4600  21 differing instruction groups.
- `original_subtraction_rvo_joined_cast_xyz`  DIFFER  4600/4600  15 differing instruction groups.
- `original_subtraction_rvo_joined_ordinary`  DIFFER  4600/4600  15 differing instruction groups.
- `author_merged_ecee975d2a37_be5e19f5`  DIFFER  4600/4600  7 differing instruction groups.
- `native_union_representation_selected_input`  DIFFER  4600/4600  15 differing instruction groups.
- `native_union_representation_selected_output`  DIFFER  4600/4600  13 differing instruction groups.
- `native_union_representation_selected_both_fields`  DIFFER  4600/4600  17 differing instruction groups.
- `native_union_representation_selected_both_object`  DIFFER  4600/4600  17 differing instruction groups.
- `native_union_representation_selected_both_grouped_product`  DIFFER  4600/4600  18 differing instruction groups.
- `native_union_representation_near_input`  DIFFER  4600/4600  17 differing instruction groups.
- `native_union_representation_near_output`  DIFFER  4600/4600  17 differing instruction groups.
- `native_union_representation_near_both_fields`  DIFFER  4600/4600  19 differing instruction groups.
- `native_union_representation_near_both_object`  DIFFER  4600/4600  19 differing instruction groups.
- `native_union_representation_near_both_grouped_product`  DIFFER  4600/4600  23 differing instruction groups.
- `partial_tree_17db647dc01f_be5e19f5`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_copy_body`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_copy_init_formal`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_matrix_row`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_matrix_first_element`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_matrix_address`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_matrix_row_copy_body`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_selected_matrix_address_copy_body`  DIFFER  4600/4600  7 differing instruction groups.
- `viewport_original_objects_receiver_copy_body`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_copy_init_formal`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_matrix_row`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_matrix_first_element`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_matrix_address`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_matrix_row_copy_body`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_receiver_matrix_address_copy_body`  DIFFER  4600/4600  8 differing instruction groups.
- `viewport_original_objects_early_copy_body`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_copy_init_formal`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_matrix_row`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_matrix_first_element`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_matrix_address`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_matrix_row_copy_body`  DIFFER  4600/4600  13 differing instruction groups.
- `viewport_original_objects_early_matrix_address_copy_body`  DIFFER  4600/4600  13 differing instruction groups.
- `arithmetic_inline_ownership_selected_subtract`  DIFFER  4588/4600  36 differing instruction groups.
- `arithmetic_inline_ownership_selected_multiply`  DIFFER  4600/4600  7 differing instruction groups.
- `arithmetic_inline_ownership_selected_add`  DIFFER  4600/4600  7 differing instruction groups.
- `arithmetic_inline_ownership_selected_negate`  DIFFER  4600/4600  7 differing instruction groups.
- `arithmetic_inline_ownership_selected_normalise`  DIFFER  4504/4600  141 differing instruction groups.
- `arithmetic_inline_ownership_selected_post_scale`  DIFFER  4507/4600  118 differing instruction groups.
- `arithmetic_inline_ownership_selected_pre_translate`  DIFFER  4584/4600  106 differing instruction groups.
- `arithmetic_inline_ownership_selected_transform_destination`  DIFFER  4415/4600  151 differing instruction groups.
- `arithmetic_inline_ownership_selected_vector_operators`  DIFFER  4588/4600  36 differing instruction groups.
- `arithmetic_inline_ownership_selected_exact_direct_subtraction`  DIFFER  4588/4600  36 differing instruction groups.
- `arithmetic_inline_ownership_near_subtract`  DIFFER  4600/4600  15 differing instruction groups.
- `arithmetic_inline_ownership_near_multiply`  DIFFER  4600/4600  13 differing instruction groups.
- `arithmetic_inline_ownership_near_add`  DIFFER  4600/4600  13 differing instruction groups.
- `arithmetic_inline_ownership_near_negate`  DIFFER  4600/4600  13 differing instruction groups.
- `arithmetic_inline_ownership_near_normalise`  DIFFER  4504/4600  158 differing instruction groups.
- `arithmetic_inline_ownership_near_post_scale`  DIFFER  4507/4600  131 differing instruction groups.
- `arithmetic_inline_ownership_near_pre_translate`  DIFFER  4584/4600  109 differing instruction groups.
- `arithmetic_inline_ownership_near_transform_destination`  DIFFER  4417/4600  146 differing instruction groups.
- `arithmetic_inline_ownership_near_vector_operators`  DIFFER  4600/4600  15 differing instruction groups.
- `arithmetic_inline_ownership_near_exact_direct_subtraction`  DIFFER  4600/4600  15 differing instruction groups.
- `original_geometry_viewport_join_old_spacing`  DIFFER  4600/4600  13 differing instruction groups.
- `original_geometry_viewport_join_legacy_source`  DIFFER  4600/4600  28 differing instruction groups.
- `original_geometry_viewport_join_actual_doubles`  DIFFER  4588/4600  22 differing instruction groups.
- `original_geometry_viewport_join_explicit_float_widths`  DIFFER  4588/4600  18 differing instruction groups.
- `original_geometry_viewport_join_explicit_float_centers`  DIFFER  4600/4600  17 differing instruction groups.
- `original_geometry_viewport_join_shader_layout_matrix_data`  DIFFER  4600/4600  13 differing instruction groups.
- `original_geometry_viewport_join_grouped_reciprocal_original`  DIFFER  4600/4600  25 differing instruction groups.
- `original_geometry_viewport_join_ungrouped_geometry`  DIFFER  4600/4600  18 differing instruction groups.
- `original_geometry_viewport_join_original_grouped_geometry`  DIFFER  4600/4600  23 differing instruction groups.
- `original_geometry_viewport_join_typed_mesh_exact_source`  DIFFER  4600/4600  31 differing instruction groups.
- `original_geometry_viewport_join_exact_scalar_sign`  DIFFER  4600/4600  13 differing instruction groups.
- `original_vector_alignment_selected_packed`  DIFFER  4600/4600  7 differing instruction groups.
- `original_vector_alignment_selected_packed_and_direct_difference`  DIFFER  4588/4600  36 differing instruction groups.
- `original_vector_alignment_receiver_packed`  DIFFER  4600/4600  8 differing instruction groups.
- `original_vector_alignment_receiver_packed_and_direct_difference`  DIFFER  4600/4600  42 differing instruction groups.
- `original_vector_alignment_early_packed`  DIFFER  4600/4600  13 differing instruction groups.
- `original_vector_alignment_early_packed_and_direct_difference`  DIFFER  4600/4600  15 differing instruction groups.
- `original_vector_alignment_joined_packed`  DIFFER  4600/4600  16 differing instruction groups.
- `original_vector_alignment_joined_packed_and_direct_difference`  DIFFER  4600/4600  15 differing instruction groups.
- `window_extent_materialisation_selected_both`  DIFFER  4600/4600  24 differing instruction groups.
- `window_extent_materialisation_selected_width`  DIFFER  4600/4600  11 differing instruction groups.
- `window_extent_materialisation_selected_height`  DIFFER  4600/4600  11 differing instruction groups.
- `window_extent_materialisation_receiver_both`  DIFFER  4600/4600  14 differing instruction groups.
- `window_extent_materialisation_receiver_width`  DIFFER  4600/4600  12 differing instruction groups.
- `window_extent_materialisation_receiver_height`  DIFFER  4600/4600  12 differing instruction groups.
- `window_extent_materialisation_early_both`  DIFFER  4584/4600  22 differing instruction groups.
- `window_extent_materialisation_early_width`  DIFFER  4600/4600  23 differing instruction groups.
- `window_extent_materialisation_early_height`  DIFFER  4600/4600  23 differing instruction groups.
- `materialisation_owner_join_selected_ordinary`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_owner_join_selected_top_height`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_owner_join_selected_postscale`  DIFFER  4600/4600  30 differing instruction groups.
- `materialisation_owner_join_selected_weight`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_owner_join_selected_camera_half_fov`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_owner_join_receiver_ordinary`  DIFFER  4600/4600  7 differing instruction groups.
- `materialisation_owner_join_receiver_top_height`  DIFFER  4600/4600  7 differing instruction groups.
- `materialisation_owner_join_receiver_postscale`  DIFFER  4600/4600  25 differing instruction groups.
- `materialisation_owner_join_receiver_weight`  DIFFER  4600/4600  7 differing instruction groups.
- `materialisation_owner_join_receiver_camera_half_fov`  DIFFER  4600/4600  7 differing instruction groups.
- `materialisation_owner_join_all_groups_ordinary`  DIFFER  4600/4600  18 differing instruction groups.
- `materialisation_owner_join_all_groups_top_height`  DIFFER  4600/4600  18 differing instruction groups.
- `materialisation_owner_join_all_groups_postscale`  DIFFER  4600/4600  28 differing instruction groups.
- `materialisation_owner_join_all_groups_weight`  DIFFER  4590/4600  23 differing instruction groups.
- `materialisation_owner_join_all_groups_camera_half_fov`  DIFFER  4590/4600  23 differing instruction groups.
- `materialisation_owner_join_early_ordinary`  DIFFER  4600/4600  23 differing instruction groups.
- `materialisation_owner_join_early_top_height`  DIFFER  4600/4600  23 differing instruction groups.
- `materialisation_owner_join_early_postscale`  DIFFER  4600/4600  31 differing instruction groups.
- `materialisation_owner_join_early_weight`  DIFFER  4584/4600  22 differing instruction groups.
- `materialisation_owner_join_early_camera_half_fov`  DIFFER  4584/4600  22 differing instruction groups.
- `materialisation_receiver_cut_direct_double`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_float_promotion`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_original_order`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_grouped_double`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_half_float`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_half_cast`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_half_divided`  DIFFER  4600/4600  12 differing instruction groups.
- `materialisation_receiver_cut_multiply_group`  DIFFER  4600/4600  15 differing instruction groups.
- `materialisation_receiver_cut_cos_first_group`  DIFFER  4600/4600  15 differing instruction groups.
- `materialisation_symmetric_reviewed_owners`  DIFFER  4600/4600  14 differing instruction groups.
- `symmetric_extent_clip_radius_field_of_view_multiply`  DIFFER  4600/4600  29 differing instruction groups.
- `symmetric_extent_clip_radius_field_of_view`  DIFFER  4600/4600  18 differing instruction groups.
- `symmetric_extent_clip_radius_multiply`  DIFFER  4600/4600  28 differing instruction groups.
- `symmetric_extent_clip_radius`  DIFFER  4600/4600  17 differing instruction groups.
- `symmetric_extent_clip_field_of_view_multiply`  DIFFER  4600/4600  25 differing instruction groups.
- `symmetric_extent_clip_field_of_view`  DIFFER  4600/4600  15 differing instruction groups.
- `symmetric_extent_clip_multiply`  DIFFER  4600/4600  24 differing instruction groups.
- `alternate_replay_17db647dc01f_asymmetric`  DIFFER  4600/4600  12 differing instruction groups.
- `alternate_replay_17db647dc01f_symmetric`  DIFFER  4600/4600  14 differing instruction groups.
- `partial_tree_5bf00538c146_be5e19f5`  DIFFER  4600/4600  7 differing instruction groups.
- `alternate_replay_5bf00538c146_asymmetric`  DIFFER  4600/4600  12 differing instruction groups.
- `alternate_replay_5bf00538c146_symmetric`  DIFFER  4600/4600  14 differing instruction groups.
- `sprite_symmetric_transform_sumxy_all_selected`  DIFFER  4556/4600  35 differing instruction groups.
- `sprite_symmetric_transform_sumxy_all_plain`  DIFFER  4556/4600  30 differing instruction groups.
- `sprite_symmetric_transform_sumxy_Y_selected`  DIFFER  4574/4600  25 differing instruction groups.
- `sprite_symmetric_transform_sumxy_Y_plain`  DIFFER  4574/4600  20 differing instruction groups.
- `sprite_symmetric_transform_sumxy_YZ_selected`  DIFFER  4574/4600  28 differing instruction groups.
- `sprite_symmetric_transform_sumxy_YZ_plain`  DIFFER  4574/4600  23 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_all_selected`  DIFFER  4600/4600  35 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_all_plain`  DIFFER  4600/4600  28 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_Y_selected`  DIFFER  4600/4600  27 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_Y_plain`  DIFFER  4600/4600  22 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_YZ_selected`  DIFFER  4600/4600  29 differing instruction groups.
- `sprite_symmetric_transform_sumxyz_YZ_plain`  DIFFER  4600/4600  24 differing instruction groups.
- `sprite_symmetric_transform_zterm_all_selected`  DIFFER  4600/4600  27 differing instruction groups.
- `sprite_symmetric_transform_zterm_all_plain`  DIFFER  4600/4600  22 differing instruction groups.
- `sprite_symmetric_transform_zterm_Y_selected`  DIFFER  4600/4600  23 differing instruction groups.
- `sprite_symmetric_transform_zterm_Y_plain`  DIFFER  4600/4600  18 differing instruction groups.
- `sprite_symmetric_transform_zterm_YZ_selected`  DIFFER  4600/4600  25 differing instruction groups.
- `sprite_symmetric_transform_zterm_YZ_plain`  DIFFER  4600/4600  20 differing instruction groups.
- `sprite_symmetric_transform_translation_all_selected`  DIFFER  4600/4600  27 differing instruction groups.
- `sprite_symmetric_transform_translation_all_plain`  DIFFER  4600/4600  20 differing instruction groups.
- `sprite_symmetric_transform_translation_Y_selected`  DIFFER  4600/4600  23 differing instruction groups.
- `sprite_symmetric_transform_translation_Y_plain`  DIFFER  4600/4600  18 differing instruction groups.
- `sprite_symmetric_transform_translation_YZ_selected`  DIFFER  4600/4600  25 differing instruction groups.
- `sprite_symmetric_transform_translation_YZ_plain`  DIFFER  4600/4600  20 differing instruction groups.
