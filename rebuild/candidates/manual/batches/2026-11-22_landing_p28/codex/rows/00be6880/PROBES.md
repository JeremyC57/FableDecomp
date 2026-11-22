# Native VC7.1 source experiments

Received baseline DIFFER at 89 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_landscape_grid_switch_typed` | Replace the flank-selection condition ladder with an enum-like switch. | DIFFER 1772/1776 | 202 |
| `r_landscape_index_memcpy_typed` | Replace explicit index writes with a bounded memcpy. | DIFFER 1753/1776 | 197 |
| `r_landscape_reconstruction_typed` | Reconstruct and compile every vertex, edge-strip and index-buffer operation. | DIFFER 1753/1776 | 197 |
| `r_landscape_table_assignment_typed` | Initialize the index table via explicit stores to test the array initialization shape. | DIFFER 1753/1776 | 197 |
| `r_landscape_vertex_separate_typed` | Form the 3D position as independent fields instead of vector addition. | DIFFER 1753/1776 | 197 |

Selected `r_landscape_reconstruction_typed`. Reconstructed the complete default-section fan, four edge strips and index buffer. The evidence-backed if chain leaves x87, frame and store-order differences.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

All 1,776 bytes and all 416 instructions match. A named C3DPackedNormal in PackVertexNormal restored the stack-slot layout. Making the original SetTargetVertexNormal body visible resolved the final scheduling difference. That additional 150-byte body and its three literal operands match independently. The leaf has 26 verified operands. Three PDBs confirm the original GPEngineDef spelling, and the FableWin public and global identify its exact address in that build. The test executes the reconstructed section builder for even, odd and zero dimensions and checks signed normal unpacking, all vertices, all four edges, all twelve indices, buffer lifecycle and UV rounding.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_edge_all | 1772 | 298 | 0 / 0 |
| round2_edge_all_directuv | 1772 | 298 | 0 / 0 |
| round2_edge_all_double | 1772 | 294 | 0 / 0 |
| round2_edge_copycall | 1767 | 290 | 0 / 0 |
| round2_edge_fan | 1767 | 290 | 0 / 0 |
| round2_edge_funcscope | 1767 | 290 | 0 / 0 |
| round2_edge_spaces | 1772 | 298 | 0 / 0 |
| round2_edge_two_dim | 1767 | 290 | 0 / 0 |
| round2_land_authentic | 1751 | 207 | 0 / 0 |
| round2_land_directuv | 1751 | 207 | 0 / 0 |
| round2_land_scalaruv | 1751 | 210 | 0 / 0 |
| round2_land_single | 1751 | 210 | 0 / 0 |
| round2_land_world_first | 1751 | 207 | 0 / 0 |
| round2_landbits_flatall | 1776 | 415 | 0 / 0 |
| round2_landbits_flatnone | 1776 | 415 | 0 / 0 |
| round2_landbits_flatz | 1776 | 415 | 0 / 0 |
| round2_landbits_masklhsX | 1776 | 415 | 0 / 0 |
| round2_landbits_masklhsXY | 1776 | 415 | 0 / 0 |
| round2_landbits_masklhsXYZ | 1776 | 415 | 0 / 0 |
| round2_landbits_masklhsY | 1776 | 415 | 0 / 0 |
| round2_landbits_masklhsZ | 1776 | 415 | 0 / 0 |
| round2_landbits_revstepall | 1776 | 403 | 0 / 0 |
| round2_landbits_revstepnone | 1776 | 403 | 0 / 0 |
| round2_landbits_revstepz | 1776 | 403 | 0 / 0 |
| round2_landbits_rightall | 1776 | 415 | 0 / 0 |
| round2_landbits_rightnone | 1776 | 415 | 0 / 0 |
| round2_landbits_rightz | 1776 | 415 | 0 / 0 |
| round2_landbits_stepall | 1776 | 415 | 0 / 0 |
| round2_landbits_stepnone | 1776 | 415 | 0 / 0 |
| round2_landbits_stepz | 1776 | 415 | 0 / 0 |
| round2_landcallee_after0 | 1776 | 416 | 0 / 0 |
| round2_landcallee_after1 | 1776 | 416 | 0 / 0 |
| round2_landcallee_before0 | 1776 | 416 | 0 / 0 |
| round2_landcallee_before1 | 1776 | 416 | 0 / 0 |
| round2_landcallee_private | 1776 | 416 | 0 / 0 |
| round2_landissue_accum | 1776 | 415 | 0 / 0 |
| round2_landissue_argref | 1776 | 415 | 0 / 0 |
| round2_landissue_argvalue | 1776 | 415 | 0 / 0 |
| round2_landissue_compound | 1776 | 415 | 0 / 0 |
| round2_landissue_dbl | 1776 | 415 | 0 / 0 |
| round2_landissue_int | 1776 | 415 | 0 / 0 |
| round2_landissue_namedpacked | 1776 | 415 | 0 / 0 |
| round2_landissue_par | 1776 | 415 | 0 / 0 |
| round2_landissue_shift | 1776 | 415 | 0 / 0 |
| round2_landissue_subwords | 1776 | 415 | 0 / 0 |
| round2_landissue_vertices | 1776 | 415 | 0 / 0 |
| round2_landlast_append | 1776 | 415 | 0 / 0 |
| round2_landlast_double_normal | 1776 | 415 | 0 / 0 |
| round2_landlast_double_uv | 1776 | 411 | 0 / 0 |
| round2_landlast_mask | 1776 | 415 | 0 / 0 |
| round2_landlast_mixed | 1776 | 415 | 0 / 0 |
| round2_landlast_original_doubles | 1776 | 411 | 0 / 0 |
| round2_landlast_prodX | 1776 | 415 | 0 / 0 |
| round2_landlast_prodY | 1776 | 415 | 0 / 0 |
| round2_landlast_prodZ | 1776 | 415 | 0 / 0 |
| round2_landlast_ptr | 1776 | 415 | 0 / 0 |
| round2_landlast_ret | 1776 | 415 | 0 / 0 |
| round2_landlast_revX | 1776 | 415 | 0 / 0 |
| round2_landlast_revY | 1776 | 415 | 0 / 0 |
| round2_landlast_revZ | 1776 | 415 | 0 / 0 |
| round2_landlast_tree | 1776 | 403 | 0 / 0 |
| round2_landlast_zwalk | 1776 | 415 | 0 / 0 |
| round2_landpack_append | 1772 | 298 | 0 / 0 |
| round2_landpack_array | 1772 | 298 | 0 / 0 |
| round2_landpack_fanfields | 1772 | 298 | 0 / 0 |
| round2_landpack_mask | 1772 | 298 | 0 / 0 |
| round2_landpack_mixed | 1772 | 298 | 0 / 0 |
| round2_landpack_namednormal | 1776 | 415 | 0 / 0 |
| round2_landpack_postguard | 1772 | 298 | 0 / 0 |
| round2_landpack_tree | 1772 | 288 | 0 / 0 |
| round2_landpack_zwalk | 1772 | 298 | 0 / 0 |
| round2_landvec_ADD_NAMED_RESULT_ | 1776 | 415 | 0 / 0 |
| round2_landvec_ADD_NAMED_RESULT_LIBPK_IMPLICIT_COPY_PASS23 | 1910 | 276 | 0 / 0 |
| round2_landvec_ADD_NAMED_RESULT_ORIGINAL_COMPONENT_CTOR | 1776 | 415 | 0 / 0 |
| round2_landvec_ADD_NAMED_RESULT_ORIGINAL_COPY_FORMAL | 1776 | 415 | 0 / 0 |
| round2_landvec_INITIALISE_ADD_ | 1776 | 415 | 0 / 0 |
| round2_landvec_INITIALISE_ADD_LIBPK_IMPLICIT_COPY_PASS23 | 1910 | 276 | 0 / 0 |
| round2_landvec_INITIALISE_ADD_ORIGINAL_COMPONENT_CTOR | 1776 | 415 | 0 / 0 |
| round2_landvec_INITIALISE_ADD_ORIGINAL_COPY_FORMAL | 1776 | 415 | 0 / 0 |
| round2_landvec__LIBPK_IMPLICIT_COPY_PASS23 | 1853 | 278 | 0 / 0 |
| round2_landvec__ORIGINAL_COMPONENT_CTOR | 1776 | 415 | 0 / 0 |
| round2_landvec__ORIGINAL_COPY_FORMAL | 1776 | 415 | 0 / 0 |
| round2_landvec_copy | 1776 | 415 | 0 / 0 |
| round2_landvec_result | 1776 | 415 | 0 / 0 |
| round2_landvec_tupleget | 1776 | 415 | 0 / 0 |
| round2_landvec_tuplehigh | 1776 | 415 | 0 / 0 |
| round2_landvec_tuplelow | 1776 | 415 | 0 / 0 |
| round2_landvec_tuplepos | 1776 | 415 | 0 / 0 |
| round2_landvec_tupleset | 1776 | 415 | 0 / 0 |
| round2_landvec_tupleunsigned | 1776 | 415 | 0 / 0 |
| round2_landvec_tupleuv | 1776 | 415 | 0 / 0 |
| round2_map_namedadd | 1767 | 286 | 0 / 0 |
| round2_map_namedcopy | 1767 | 286 | 0 / 0 |
| round2_map_scalars | 1767 | 290 | 0 / 0 |
| round2_map_value | 1767 | 286 | 0 / 0 |
| round2_map_wordcasts | 1767 | 286 | 0 / 0 |
