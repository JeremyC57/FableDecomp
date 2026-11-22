# Native VC7.1 source experiments

Received baseline COMPILE_FAIL because the archived source requires opt-ins and headers absent from the package.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_mesh_for_finders_root_body` | Move the point finder into a for-loop initializer and increment. | DIFFER 3279/3051 | 461 |
| `r_mesh_generator_before_copies_root_body` | Separate the copy phase from the two sorts to test loop/lifetime scheduling. | DIFFER 3065/3051 | 647 |
| `r_mesh_named_generator_root_body` | Give the generated object name and generator their two donor locals. | DIFFER 3065/3051 | 659 |
| `r_mesh_pack_after_copies_root_body` | Explicitly qualify PackNames and scope only the packing and copy phase. | DIFFER 3065/3051 | 659 |
| `r_mesh_pdb_names_root_body` | Restore donor names and preserve the original loop scopes. | DIFFER 3065/3051 | 659 |
| `r_mesh_restore_headers_root_body` | Restore the previous header-gated draft on intact shared declarations. | DIFFER 3065/3051 | 659 |

Selected `r_mesh_named_generator_root_body`. Reinstated the missing mesh-file and preallocated-map headers. Counting loops and helper collection compile with the original named generator and object string. Register allocation and STLport expansion still differ.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

The selected 3,017-byte leaf is 34 bytes shorter than the 3,051-byte retail extent. All 27 mapped copies pass. Separate complete owner comparisons also prove the 101-byte GetRoot, 183-byte AllocateBuffers, 502-byte PackNames, and 218-byte vector<char> insertion, including their relocated operands. The debug file and native access agree on GetRoot converting the counted pointer before the static cast. The recovered bounds accessors and typed map allocations are present. Frame and access scheduling remain open.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_meshallocate_plain | 3017 | 666 | 27 / 0 |
| round2_meshallocate_site | 2974 | 574 | 27 / 0 |
| round2_meshchar_inline | 3017 | 666 | 26 / 1 |
| round2_meshchar_range | 3017 | 666 | 27 / 0 |
| round2_meshfaith_direct | 3017 | 666 | 27 / 0 |
| round2_meshfaith_getroot | 3017 | 666 | 27 / 0 |
| round2_meshfaith_original | 3017 | 666 | 27 / 0 |
| round2_meshfaith_original_scoped | 3017 | 666 | 27 / 0 |
| round2_meshowners_bytes | 3017 | 666 | 26 / 1 |
| round2_meshowners_char | 3017 | 666 | 26 / 1 |
| round2_meshowners_rb | 3017 | 666 | 26 / 1 |
| round2_meshroot_budget110 | 3431 | 394 | 23 / 4 |
| round2_meshroot_budget35 | 2169 | 172 | 14 / 13 |
| round2_meshroot_budget50 | 2765 | 331 | 20 / 7 |
| round2_meshroot_budget65 | 3052 | 484 | 24 / 3 |
| round2_meshroot_budget75 | 3195 | 529 | 25 / 2 |
| round2_meshroot_budget90 | 3282 | 499 | 23 / 4 |
| round2_meshroot_decl | 3025 | 671 | 25 / 2 |
| round2_meshroot_site0 | 3017 | 666 | 25 / 2 |
| round2_meshroot_site1 | 3065 | 659 | 25 / 2 |
| round2_meshroot_site2 | 3065 | 659 | 25 / 2 |
| round2_meshsource_cast | COMPILE_FAIL |  |  /  |
| round2_meshsource_release | 3017 | 666 | 27 / 0 |
| round2_meshsource_source | 3017 | 666 | 27 / 0 |
