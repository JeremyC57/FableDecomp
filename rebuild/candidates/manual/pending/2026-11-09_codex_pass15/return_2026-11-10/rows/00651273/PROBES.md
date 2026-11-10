# 00651273 source-shape attempts

Current frozen selection:096_original_minigame_and_element_enums (1677/1681;601/602 instructions;90.44% loose share;0x214 frame). No match claimed. All histories are preserved; setup, flag, type/name audits and canonicalized spellings are separated from genuinely different mechanisms. Exact-body tests:1152 ordinary cases plus1 callback mutation; VC7.1/Od/W3 and g++03 PASS. All47 relocations audited. Alternative079 reaches native0x210 frame and90.77% loose share, but has different object homes.

| Trial | Bytes | Rationale/outcome |
|---|---|---|
| 001_first_complete_source | FAIL/1681 | Setup failure: missing existing persistent-primitive opt-in, not an attempt. |
| 001b_handle_dependency | 1615/1681 | First complete scalar-coordinate body; not matched. |
| 002_neighbour_size_flags | 1615/1681 | Compiler flag investigation, not a source-shape attempt. |
| 003_size_frame_optimisation | 1618/1681 | /Os with default/Oy restores the native biased frame-pointer form. Flag diagnostic only. |
| 004_box_corner_vectors | 1635/1681 | Construct rectangles using the original corner-vector constructor. |
| 005_box_extents_icon_subtraction | 1628/1681 | Use original extents constructor and vector subtraction; add the native unused fishing width query. |
| 006_direct_icon_argument | 1645/1681 | Build icon position directly in the sprite argument rather than a named vector. |
| 007_group_coordinate_expressions | 1645/1681 | Group complete coordinate expressions; no arithmetic change. |
| 008_real_offset_member | 1645/1681 | Correct the shared offset member identity; bytes unchanged, not another shape. |
| 009_box_vector_addition | 1663/1681 | Construct opposite corners using the original vector addition body;1663/1681. |
| 010_named_boxes_before_calls | 1658/1681 | Name complete boxes, and form the fill rectangle before color interpolation. |
| 011_icon_dimensions_object | 1658/1681 | Hold icon dimensions in the real vector class rather than independent floats. |
| 012_named_position_size_vectors | 1623/1681 | Use copied/modified positions and named size vectors, then four-scalar boxes (GOTCHAS chunk779). |
| 013_named_vectors_box_arguments | 1627/1681 | Keep named positions/sizes but pass box temporaries directly. |
| 014_named_icon_coordinate_values | 1636/1681 | Reference temporaries for first-icon coordinates restore its native store/clamp sequence; named final-icon coordinates evaluate Y first. |
| 015_reference_coordinate_lifetimes | 1671/1681 | Reference coordinate lifetimes combined with named rectangles;1671/1681 selected, still differs. |

Historical checkpoint:015_reference_coordinate_lifetimes. Its exact body passes the standalone behavior tests under both compilers; byte equality remains unresolved. No scheduling/packing device was introduced.

## Further native-guided geometry investigation

Current working source is069_reference_endpoints_direct_icon,1677/1681,601/602 instructions,90.44% loose alignment,0x214 frame. Alternative079_height_before_position_copy is preserved: same size/count,90.77% loose,0x210 native frame, but left/centre sprite homes exchange. Neither is matched.069 preserves native sprite object order with every object home shifted -4;079 fixes frame but does not fix every home. Older015 is retained exactly.

Native has five distinct84-byte sprite homes (four bars and a shared optional-icon home).016–019 scope experiments collapse those lifetimes and diverge (framesd4–1d0), proving the sibling questbar recipe cannot be applied wholesale.020–021 named vector endpoints were initially combined with named boxes and did not help.022–029 mix named and temporary rectangle lifetimes: named left/fill and temporary centre/edge improves frame and code alignment.030–034 inline height or change scalar-reference lifetimes; inline height removes4-byte frame pressure.035 reuse of one rectangle adds memory copying and diverges.036 visible genuine box copy body and037 reference-bound box temporary canonicalize.038 explicit coordinate sums removes genuine vector temporary operations and diverges;039–040 retain named bar-position vectors.

041 combines named-left/fill and inline-height and obtains native0x210 frame;042–045 study scalar endpoint and arrow evaluation order.043 is an undeclared-local compile failure, not a valid source attempt;044 independently tests the corrected bottom-coordinate form.046 authentic PDB local names alone changes no code.047 uses donor-proven scalar sprite width/height.048 primary position constructed in the argument retains computed coordinate lifetimes through UV argument setup;049 arrow counterpart differs.050–052 reconstruct donor-proven left_pos/middle_pos/inner_pos and named rectangles; all4 original rectangle objects are proven by PDB records, though native lifetime/coalescing differs from those donor builds.

053–056 compute real vector endpoints before rendering arguments, then build the rectangle in the argument. This preserves geometry-before-UV computation while delaying rectangle stores.057/O1 and058/Ob1 are inline-policy diagnostics, identical to050, not new source strategies.059 reference-bound endpoint temporaries improve UV/store scheduling.060–062 all-endpoint and icon-argument combinations;063–068 copy/in-place vector arithmetic and position forms canonicalize.069 combines reference endpoints with direct primary icon argument and reaches90.44% loose.070–072 right endpoints/bottom scalar/named-box references regress.073–078 both icon argument forms, const/reference position lifetimes and scalar coordinate captures do not improve.079 swaps adjacent readonly height/position initializations after bank queries and gets native frame with90.77% loose.080–083 height/dimension/reference aliases canonicalize.084 amount materialization canonicalizes;085–086 percentage copy reference changes x87 lifetime and regresses;087 restores named fill box and regresses.

Original local evidence lives in rows/0064fdb4/ORIGINAL_DRAW_LOCALS.json (both Ego_r and FableWin). Names alone do not solve stack order; no invented layouts or padding introduced. Actual RTM allocator trace for069 was captured using an independent diagnostic proxy, and its leaf is byte-identical to normal RTM (CANDIDATE_VALIDATION.json). Selected069 exact-body test passes VC7.1/Od/W3 and g++03,864 combinations; all47 native relocation target multiplicities and literal bytes pass audit in RELOCATION_AUDIT.json. Full match remains unresolved.

088_original_complete_render_enums is the frozen current selection: exact069 leaf bytes, with complete original layer/blend/interpolation enums from both raw donor PDBs and exact enumerator names. This is a type/name audit regrade, not a source-shape attempt. Both exact-body standalone tests rerun PASS with valid complete enum definitions. Further experiments will use separate runtime source files so this tested selection stays stable.

089–090 complete real position-sprite constructor visibility (native151 bytes,00493ac2..00493b58) does not improve selected or original-named geometry.091–095 complete real box constructor plain/guarded plus both bodies: plain stays out of line in this row; body visibility reduces frame16 bytes on088 and48 on original all-named geometry050, past native0x210. No visibility candidate selected.096 is the new frozen type/name audit selection, byte-identical088: complete original EMiniGameType and nested EElementType recovered independently from both raw donors, using original fishing/pickpocket/picklock/steal checks. Standalone tests now cover all8 valid mini-game types (1152 ordinary cases) plus renderer callback mutation; VC/g++ PASS.

## Final follow-up evidence

097–100 target the middle region (the least aligned region in an independent per-render-call comparison): real extents constructor, explicit top/height dependencies, named size vector. No gain.101–105 commute real vector addition; most canonicalize exactly, while the right rectangle regresses.106–109 read subsequent geometry from original named box fields with the complete faithful visible box constructor; no gain.110 reproduces donor-proven nested graphic2/fishing scopes; both leaf bytes and complete frame packing are identical to096.

111–118 measure natural coordinate/product grouping with actual C2 traces.111 adds precisely one0x269 PAREN after primary-Y subtraction (633→634 total nodes), retains20 scheduling regions and regresses.112–114 restore the original named primary position and group its coordinates, no gain.115–116 canonicalize in the scheduler trace;117 retains one extra0x269 PAREN but emits identical leaf bytes;118 adds a middle-height grouping tuple and regresses. An ENOSPC interruption occurred before112 history creation; after root recovered space it was retried under its intended fresh tag. No completed attempt was removed or overwritten.

119–120 select the existing proven CRGBColour Initialise-based copy body: no gain.121–124 use the actual original GetR/G/B/A methods in render arguments or interpolation inputs; canonicalize. CRGBColour::Interpolate was investigated without inventing a body: raw FableWin has a79-byte function with by-value col and float interp, and native0066dfa0..0066e00a plus the same-TU inlined call0066ce67 show one interp*256 weight conversion and a call to the MMX GFGetInterpolatedColour. This differs from the bars four-ftol channel blends, so no such helper was authored.125–126 test existing proven CRGBFloatColour arithmetic/Interpolate/conversion bodies via a narrow additive pure-math opt-in in a unique runtime overlay. The1/255 and255 operations survive, producing1785/1796 bytes and635/639 instructions; rejected.

The independent frame tracer uses documented RTM packer hooks and verifies its diagnostic selected leaf against the normal object.096:64nodes,20slots,packed0x214. Presort/final slot memberships, sizes, IL weights, conflicts and symbol offsets are captured. The named right_box gets a separate16-byte exposed slot, while middle/fill compiler-generated boxes share. Making the real constructor visible removes16 bytes, whereas native needs4 removed, proving further geometry node/lifetime differences remain. Both the frame trace and exact-target relocation audit establish measured limits; no compiler modification, padding or forced allocation is part of production source.

Ready files:00651273.cpp,00651273_test.cpp,FOLLOWUP_TEST_RESULTS.json,RELOCATION_AUDIT.json,ORIGINAL_MINIGAME_LOCALS.json,ORIGINAL_RENDER_ENUMS.json,ORIGINAL_QUEST_ENUMS.json,NAMING_EVIDENCE.md. Frozen096 source/header snapshot remains in history; runtime candidates after096 leave the frozen production/test files unchanged.

## Main follow-up127–133

127 uses original named sprite_pos initialized from draw_pos and compound coordinate updates;128 applies it to both icons. Both1664bytes, worse, and repeat the broader mechanism of historical051 with the newer geometry.132–133 default-construct sprite_pos and assign its fields separately; they canonicalize to127–128. These are not four independent optimizer mechanisms.130–131 use sprite copy initialization at six sites, with baseline or the earlier079 height-before-position lifetime: bytes exactly reproduce096 or079, so not a new improvement.129 tests the original operator+ signature implemented through a temporary copy plus original +=;1755bytes and worse. This is an unselected alternate-source hypothesis, not recovered original source. Raw donor local records for vector/box constructors and arithmetic are now retained in ORIGINAL_GEOMETRY_LOCALS.json; they expose no named result in operator+. Frozen096 and its passing tests remain unchanged.


## Local-color and frame-packer follow-up134–141

134/135 implement the specific new hypothesis that blending reads the copied interp_colour channel on the RHS instead of the unchanged member Colour channel, on096 and079 respectively. No other operation or expression order changes. Both emit1676 bytes/601 instructions, regress loose agreement to86.78/87.12%, and retain their0x214/0x210 frames. Actual135 frame tracing validates against the normal RTM object: the color/left-height shared six-byte slot grows from weight17 to20 (color12→15; height remains5), density2833→3333, but stays at+0x60. The hottest eight-byte slot retains weight32/density4000 and the next highest home. Native explicitly reloads member Colour at offsets26,25,24,27 during blending;135 instead reads the saved local channels (first R remains in CL), explaining why the surviving local-use change is not a native improvement.

136/137 default-construct interp_colour and then use the language-generated copy assignment, on096/079. These emit1659 bytes/595 instructions, with compressed dword copying instead of the native initial channel moves. Actual137 trace still gives a six-byte color slot, now shared weight14 and moved down to+0x58. So assignment changes use weights but does not provide a matching storage form.138/139 assign complete color results in each Type branch, using aggregate assignment or explicit channels in the nonblend branch;1660/596 and1674/601, both regress. All consumed channels are initialized on every path. No assignment helper body was invented: ORIGINAL_COLOUR_ASSIGNMENT_SYMBOLS.json records no matching public assignment symbol in either donor; FableWin does supply the original Initialise symbol. Existing119/120 tested the separate proven Initialise-based copy-constructor body, so134–139 do not repeat that body change.

140/141 copy the original left_pos from draw_pos then update its Y coordinate instead of constructing from scalar components. Both canonicalize byte-for-byte to096/079. This isolated position-copy form does not reduce the weight32 hot-slot bucket; no extra source strategy is claimed for canonicalization.

|History|Bytes|Instructions|Loose agreement|
|---|---:|---:|---:|
|134_local_colour_rhs|1676|601|86.78%|
|135_native_frame_local_colour_rhs|1676|601|87.12%|
|136_colour_assignment_after_default|1659|595|87.55%|
|137_native_frame_colour_assignment|1659|595|87.89%|
|138_complete_colour_branch_assignment|1660|596|87.15%|
|139_complete_colour_branch_channels|1674|601|87.45%|
|140_left_position_copy_then_vertical_update|1677|601|90.44%|
|141_native_frame_left_position_copy|1677|601|90.77%|

Selected096, its header overlay,1153-case exact-body tests and relocation evidence remain untouched. These measurements reject the local-color RHS/copy-result variants tested here; they do not establish impossibility of another genuine source lifetime form.

Final handoff: selected body retained, with required line-2 DIFFER annotation. FINAL_003_handoff_metadata recompiles this comment-only change using the integrated physical include tree. It is a verification grade, not another source-shape attempt.

## Final handoff authority

Current status: PARKED. Final integrated grade: FINAL_003_handoff_metadata. Final source/test hashes and fresh test results: CURRENT_VERIFICATION.json. Earlier pending-review statements above are historical and are superseded by this record. Compiler trials include flags/setup/audit repeats; they are not independent source strategies.
