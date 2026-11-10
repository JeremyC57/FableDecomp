# SetupStaticBlocksNoMorph attempts

Selected final source:056_guarded_new_headers, verified324/324,112instructions,100% strict/loose,zero hunks. Previous018_combined_diffuse_condition (328/324) is retained historically. Original byte-closest001 (327/324) is preserved; its initial bump destination reload differs observably when a texture callback changes StaticBlocks. No match was claimed for those earlier candidates.

| Trial | Bytes | Strict hunks |
|---|---|---|
| 001_shared_managers | 327/324 | 1 |
| 002_cached_dest_after_material | 326/324 | 26 |
| 003_reference_dest_after_material | 326/324 | 26 |
| 004_scoped_first_block | 326/324 | 27 |
| 005_bump_member_reference | 326/324 | 26 |
| 006_loop_material_receiver | 323/324 | 30 |
| 007_named_primitive_pointer | 326/324 | 26 |
| 008_loop_reference_alias | 326/324 | 26 |
| 009_first_block_reference_in_loop | 296/324 | 21 |
| 010_complete_typed_shared_headers | 327/324 | 1 |
| 011_first_block_before_material | 326/324 | 26 |
| 012_reused_block_pointer | 326/324 | 26 |
| 013_named_initial_material_index | 326/324 | 26 |
| 014_while_loop_frequency | 326/324 | 26 |
| 015_guarded_do_loop | 346/324 | 16 |
| 016_explicit_countdown | 340/324 | 27 |
| 017_count_temporary_reference | 326/324 | 26 |
| 018_combined_diffuse_condition | 328/324 | 7 |
| 019_loop_material_reference | 326/324 | 26 |
| 020_first_material_reference | 326/324 | 26 |
| 021_conditional_diffuse_assignment | 327/324 | 2 |
| 022_reverse_inner_stage_test | 326/324 | 28 |
| 023_two_stage_guard_statements | 326/324 | 28 |
| 024_named_stage_availability | 326/324 | 26 |
| 025_void_conditional | 326/324 | 26 |
| 026_bool_conditional | 326/324 | 26 |
| 027_short_circuit | 326/324 | 26 |
| 028_inner_continue | 326/324 | 28 |
| 029_loop_parameter_alias | 326/324 | 26 |
| 030_loop_parameter_alias | 326/324 | 26 |
| 031_loop_parameter_alias | 326/324 | 26 |
| 032_final_reference | 326/324 | 26 |
| 033_aggregate_failure | 343/324 | 35 |
| 034_bitwise_failure | 337/324 | 27 |
| 035_guarded_failure_first | 328/324 | 7 |
| 036_stage_outer | 328/324 | 7 |

## Evidence and rationale

Native caches the initial block and material through both texture calls, then captures count. Each later iteration rereads primitive.StaticBlocks and mesh.Materials; current block/material remain fixed through that iteration. Flags/stages for bump are read after diffuse. Caching the entire array across calls (009) is invalid.

010 replaces local truncated models with complete shared layouts without changing baseline bytes. 011–017 preserve the first block using pointer lifetime, reference/count and loop-control alternatives; most emit326 bytes with allocation changes. 018 combines flag/stage tests and restores native parameter allocation, but duplicates flag handling. 019–020 material references canonicalize to011. 021 conditional destination assignment emits327 with only two hunks but introduces an unnecessary no-stage store and was not selected. 022–028 inverted, independent, conditional and short-circuit branches return to the same allocation or reverse CFG. 029–032 parameter/final-reference aliases canonicalize to011. 033–034 failure aggregation produces extra work. 035–036 invert the combined condition or move the stage condition outward; both328 bytes, no improvement.

Actual RTM allocator trace (unique diagnostic proxy; diagnostic leaf checked identical to normal RTM) shows primitive priority45/ref6 in010, versus25/ref5 with cached first block011; bank/level priorities26 then beat primitive. In018 bank/level priorities20 restore primitive EBP and bank EBX, while branch code differs. Trace files and CANDIDATE_VALIDATION.json remain in the corresponding histories. No compiler/priority modifications are used in row source.

Standalone exact function body test:5120 ordinary cases plus callback mutation scenario; g++03 and VC7.1/Od pass. It checks D/B order, initial destination/material stability, iteration array rereads, post-diffuse bump flags, snapshot count, output stores, failure and RecordedFrame.

## Delegated continuation: recovered original types

During037–055 the selected018 source/test remained frozen;056 is now selected after the complete audit and combined tests. Raw donor ORIGINAL_ACCESSOR_TYPES.json proves CStaticBlock texture fields are const CTexture*, GetMaterialIndex()const returns long, GetChangeFlags()const returns unsigned long, C3DMesh2 has original GetMaterial(unsigned long) const/nonconst pointer overloads, and C3DPrimitive2 has SetRecordedFrame(long) plus real CMovableResource base. ORIGINAL_FUNCTION_LOCALS.json proves first_block/block_info references and original parameter/local spellings. These corrections are retained in row-local scratch headers behind opt-ins, with full existing storage preserved. No invented helper or approximate external body supplied.

|Trial|Bytes|Strict hunks|
|---|---|---|
|037_original_references_getters|323/324|30|
|038_reference_first_accessors|326/324|26|
|039_original_recorded_frame_setter|326/324|26|
|040_real_movable_base|326/324|26|
|041_getter_pointer_cursor|326/324|26|
|042_original_declaration_order|323/324|30|
|043_material_pointer_cursor|326/324|26|
|044_named_diffuse_change|326/324|26|
|045_diffuse_switch|328/324|5|
|046_diffuse_zero_case|326/324|26|
|047_diffuse_boolean_switch|328/324|5|
|048_diffuse_stage_switch|326/324|26|

037 uses original material-before-block references and real getters;038 binds block references first.039 adds genuine SetRecordedFrame setter.040 restores CMovableResource inheritance, original destructor/RelocateData virtual declarations and existing shared base virtual order.041 getter pointer cursor,042 original local declaration order and043 material pointer cursor canonicalize.044 named diffuse-change bool canonicalizes.045/047 switches on the nonzero diffuse bit restore native primitive allocation but introduce integer-mask/decrement work.046 zero-case switch and048 stage switch collapse back to038.039 actual allocator replay is byte-identical to normal RTM and still shows primitive priority25/ref5 versus bank/level26, unlike010 priority45/ref6.

Compilation paused for parent-reported ENOSPC after040; no current evidence deleted. Parent freed only an obsolete archive copy and confirmed2GB free before041. Read-only investigation during the pause found no complete local GetDiffuseTexture/GetBumpTexture source; no capture-only substitute was authored.

049 explicit masked-bit equality,050 assigning the proven stage failure value and051 complete shared CTexture/CGraphicDataBank argument types all emit326/324 with26hunks. All51 normal candidate compiler calls retained. Full texture-helper native phase/model plan is retries/texture_helpers_full/PLAN.md; no helper body compiled before reconstruction is complete.

## Complete real helpers and final verification

|Trial|Result|Bytes|Strict hunks|
|---|---|---|---|
|052_complete_texture_helpers|DIFFER; historical clock-store omission discovered later|608/324|32|
|053_inline_real_get_texture|Apparent masked match; rejected by helper audit|324/324|0|
|054_real_manager_hierarchy|Apparent masked match; rejected by helper audit|324/324|0|
|055_native_touch_clocks|MASKED_MATCH with corrected complete helper behavior|324/324|0|
|056_guarded_new_headers|RELOCATION_MATCH; selected|324/324|0|

56 normal candidate compiler calls total, all preserved.052 supplied complete intended native helper phases, but CGraphicInfoFrame::GetTexture stayed outlined and the smaller texture wrappers expanded into Setup.053's portable FABLE_FORCEINLINE on that original header helper restores the actual native inline/call split.054 restores all real manager bases, class keys, original own fields and virtual order. A separate native audit of CResource::SetAsMRU80bytes caught the omitted LastUsedFrame stores: SetAsMRU only relinks. The earlier fixture boundary had incorrectly supplied the stores.053/054 were never selected or counted as accepted.055 fixes both genuine clock stores and the fixture;056 explicitly guards all four new dependent headers behind the helper opt-in.

The final exact RTM allocator trace validates primitive priority28/ref5 versus bank/level20, while039 had primitive25/ref5 losing to bank/level26. failed spill benefit changes from−10 to−4. The genuine Bump helper leaves failed unchanged; Diffuse writes only on final failure. The diagnostic object is byte-identical to normal RTM. No priorities, scheduler settings or compiler binary were altered.

All4 assigned call operands resolve D/B/D/B to original GetDiffuseTexture00ba9360/GetBumpTexture00ba95d0. All9 external identities in the provided helper closure are proven; helper own extents are not claimed exact.34 default/prior header token checks pass. CGraphicFrameData's27-byte packing is proven by native new0x1b and Sizeof0x1b and uses the existing unchanged CGraphicFrame header packing boundary, with no added directive.

The final standalone fixture contains the exact selected Setup body and all7 exact helper/inline bodies.5120 ordinary cases plus callback mutation and18 helper/resource cases pass VC7.1/Od/W3 and64-bit g++ C++03/O2 ASan+UBSan; live frame/managed resource counters balance. Cases cover blank/composed/mutated index, loaded/unloaded/forced/asynchronous paths, output stability on failure, flag4/8/16 behavior, resource/frame clocks, repeated touches, signed sentinel and constructor-time pointer replacement. Invalid nonzero bump indices are deliberately not guarded because native dereferences the missing frame. An optional reportAllClassLayout diagnostic was rejected by VC7.1 C1007; it is recorded as unsupported diagnostic, not a normal candidate failure. Normal builds validate all owning-model size assertions.

## Main independent verification

057 adds only the required address/name heading and explicit empty extra-flags line. Recompiled with RTM:324/324bytes,112instructions,zero hunks. Independently checked allfour native call bindings and exact full Leaf against both donor name sets. Fresh VC71 /GR- and sanitized g++03 tests pass using the returned standalone test; exact assigned-row and seven helper bodies are present. MAIN_VERIFICATION.json is in057 history.

## Final handoff authority

Current status: RELOCATION_MATCH. Final integrated grade: INTEGRATION_001_shared_header_revalidation. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
