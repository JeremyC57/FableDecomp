# 00972f30 SetupArena — active work

Current disposition: VERIFIED exact1026bytes/306instructions;720native/GCCcases pass. Earlier entries below preserve the actual near-miss history. Compiler: VC7.1 RTM13.10.3077, base /O2 /Oy /W3 plus /GS. All previous sources and headers remain in immutable history snapshots.

## Identity and layout

Both original PDBs prove protected virtual SetupArena, char markerName[25], and the reused pMarkerThing local. Original donor calls HasScriptName, not the historical HasTrackName model. Shared pointer alignment was checked against both PDB CTCOwnedEntity layouts: POwner+13 and OwnerUID+24 prove byte-aligned pointer class in a naturally aligned owner. Therefore no alignment override was introduced.

## Recorded experiments

### main_01_supplied_baseline

Supplied local-model baseline only; corrected the historical Leaf selection so the existing object could be measured. Not returnable.

Result: DIFFER; 1026/1026 bytes; strict hunks 57; loose instruction ratio 1.0.

### main_02_shared_original_source

Rebase onto actual shared Jack base/stage2, HasScriptName, camera and definition classes; restore the PDB markerName[25], function-scope pMarkerThing and reused final search output.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_03_stl_original_calling_conventions

Use original separate fastcall STL uninitialized-copy/fill implementations; fixes helper calling convention while retaining real bodies.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_04_original_coordinate_copy

Restore original C2DCoordI user copy constructor, as evidenced by PDB and debug donor.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_05_radius_direct_initializers

Select original C2DBoxI midpoint/radius direct-initializer constructor; 73-byte donor confirms its shape.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_06_external_definition_template

Use external GetDef template instantiation instead of visible shared definition body; check capture/slot effects.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_07_original_box_copy

Restore original C2DBoxI copy constructor (62-byte debug donor).

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_08_implicit_pointer_conversions

Use implicit intelligent-pointer conversions at push_back and assignment sites instead of explicit temporaries.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

### main_09_original_inclass_overflow_scope

Select original in-class STLport overflow member scope, matching original _vector.h declaration placement.

Result: DIFFER; 1026/1026 bytes; strict hunks 60; loose instruction ratio 1.0.

The matching instruction topology does not establish byte equivalence: stack displacements differ, and a vector-zero store is scheduled differently. Helper bodies and relocation targets still require final audit.

## Follow-up source families

main_10_explicit_search_loop_exit: explicit infinite loop with post-search break, same1026bytes/60slot hunks.

main_11_original_pointer_copy_chain: original GetPointer/SetPointer chain and mutable pointer setter, same1026bytes/60slot hunks. No alignment override: bothPDBs confirm byte-aligned intelligent-pointer fields. No source is marked parked or verified; behavior test and complete closure audit pending. All immutable sources/headers retained.

## main_12_genuine_allthings_body — exact bytes, validation pending
Select existing shared FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA, exposing the genuine GetAllThingsInRoughArea template already present in the package. No fake body or new inline policy. This removes every one of the 60 stack-home/scheduling hunks:1026/1026 bytes,306/306instructions,zero differences. Source12 selected; prior selection saved BEFORE_EXACT_BODY. Complete relocation/name review and self-contained behavior fixture remain pending; zero provisional byte credit.

## Final verification
main_13 checks the package baseline with only the two Jack headers, still exact; its tag says current but it is a baseline compatibility control. main_14 failed while creating a header snapshot (inode exhaustion), before compilation, so it is not an attempt. main_15 verifies the current combined overlay, still exact. Final all_verified_headers_38:1026/1026bytes,306instructions,66operand identities,2virtual sites.12 genuine source attempts; two later header compatibility controls.720 native /Od/W3/noEH and720GCC64C++03 cases pass. All83 accepted rows pass combined170-header-patch regression. Native fixture has one C4355 real constructor-this warning and link4078 shared section-attribute warning; GCC clean. Earlier incomplete-test/near-miss statements above are historical.
