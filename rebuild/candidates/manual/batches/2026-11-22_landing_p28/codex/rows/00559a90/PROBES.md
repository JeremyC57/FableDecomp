# Native VC7.1 source experiments

Received baseline DIFFER at 2144 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_scroll_decl_only_map` | Use the existing declaration-only map-index opt-in to avoid replacing its owner. | DIFFER 2144/2163 | 571 |
| `r_scroll_delta_components` | Accumulate the mouse delta per component. | DIFFER 2144/2163 | 568 |
| `r_scroll_diverge` | Split zero and opposing-direction accumulation tests into two branches. | DIFFER 2176/2163 | 564 |
| `r_scroll_down_subtract_after` | Group the discarded motion and accumulated step length in the down arm. | DIFFER 2142/2163 | 568 |
| `r_scroll_gesture` | Guard non-movement events and forward to CClickable early. | DIFFER 2173/2163 | 540 |
| `r_scroll_pdb` | Restore donor names and remove the extra named downward limit and drag magnitude. | DIFFER 2144/2163 | 571 |
| `r_scroll_scale_compound` | Treat X and Y scaling as one sequential expression. | DIFFER 2144/2163 | 571 |
| `r_scroll_set_position_inline` | Construct the clamped position directly at its call site. | DIFFER 2145/2163 | 536 |
| `r_scroll_donor_getter` | Recover the donor switch, both clamping expressions, the untouched delta_size local, the up-residue addition order and CList accessor. | DIFFER 2163/2163 | 582 |
| `r_scroll_donor_release_pointee` | Use the counted-pointee release path for definition temporaries. | DIFFER 2163/2163 | 582 |
| `r_scroll_donor_release_selfdeleting_pointee` | Use the retail self-deleting counted-pointee release variant. | DIFFER 2033/2163 | 480 |
| `r_scroll_v_add_sub_mul_copy` | Recover the named return locals for all vector operators and direct component copy. | DIFFER 2179/2163 | 580 |
| `r_scroll_m_original` | Make the original STLport body visible to preserve a nonescaping key. The leaf matches and its weak map copy differs. | Leaf masked equal, 3 mapped copies differ 2163/2163 | 646 |
| `r_scroll_m_low_t44` | Lower the inliner threshold with the original map body visible. The mapped copy still differs. | Leaf masked equal, 3 mapped copies differ 2163/2163 | 646 |
| `r_scroll_extern_map_member` | Reuse the existing map owner with a VC7.1 extern-template declaration. The key now escapes compiler analysis. | DIFFER 2163/2163 | 582 |
| `r_scroll_copy_local_default` | Try size pragmas on the regular template definitions and a named default CUIState. | Leaf masked equal, 4 mapped copies differ 2163/2163 | 646 |
| `r_scroll_spec_both` | Instantiate the three visible specializations with the same local size and frame settings. | Leaf masked equal, 2 mapped copies differ 2163/2163 | 646 |
| `r_scroll_spec_tree_size` | Apply size optimization to the lower-bound walk and pair constructor while retaining their original omitted frames. | Leaf masked equal, 0 mapped copies differ 2163/2163 | 646 |
| `r_scroll_final_combined` | Move the matching specializations into the opt-in shared STLport map header and regrade the combined tree. | RELOCATION_MATCH 2163/2163 | 646 |
| `r_scroll_inline_all_plain` | Use ordinary inline on all three owner definitions. | Leaf masked equal, 1 leaf-referenced mapped copy differs 2163/2163 | 646 |
| `r_scroll_inline_plain_tree` | Use ordinary inline on the tree lower-bound walk. | Leaf masked equal, 1 leaf-referenced mapped copy differs 2163/2163 | 646 |
| `r_scroll_inline_plain_pair` | Keep the pair constructor ordinary inline. | Leaf masked equal, 0 mapped copies differ 2163/2163 | 646 |
| `r_scroll_inline_plain_map` | Keep the map index ordinary inline. | Leaf masked equal, 0 mapped copies differ 2163/2163 | 646 |
| `r_scroll_final_minimized` | Keep only the lower-bound walk out of line and regrade against the combined header tree. | RELOCATION_MATCH 2163/2163 | 646 |

Selected `r_scroll_final_minimized`. All 64 displacement and frame allocation differences in the declaration-only version disappear with the complete read-only key path. The resulting leaf and all five mapped copies match with independently verified operands.
