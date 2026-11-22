# Native VC7.1 source experiments

Received baseline DIFFER at 2432 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_trail_blend_table_poly` | Share the reverse-subtract and additive arm through a conditional mode. | DIFFER 2547/2464 | 433 |
| `r_trail_bookmark_guard_poly` | Use the existing early-return form of the unchanged-state guard. | DIFFER 2432/2464 | 649 |
| `r_trail_inline_budget_ablation_poly` | Use plain inline for both private shader selection helpers. | DIFFER 2423/2464 | 625 |
| `r_trail_local_snapshot_poly` | Use the existing desired-state and bookmark snapshots to test operand ordering. | DIFFER 2432/2464 | 649 |
| `r_trail_shared_poly` | Use shared production classes and explicit donor UV locals. | DIFFER 2432/2464 | 649 |
| `r_trail_donor_constant_blend` | Recover the original constant displacement blend helper calls from the donor body. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_donor_direct_pixel` | Use the direct donor pixel-shader call on both displacement branches. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_donor_point_arg` | Clamp the original point-light argument in place after the donor comparison. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_final_donor_accessor` | Use the original GetPrimitiveManager accessor and particle group enum. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_final_plain_shader` | Use plain inline for both shader routines. | DIFFER 2438/2464 | 637 |
| `r_trail_final_plain_light` | Use plain inline for the point-light routine alone. | DIFFER 2438/2464 | 637 |
| `r_trail_final_plain_blend` | Use plain inline for both blend helpers. | DIFFER 1550/2464 | 292 |
| `r_trail_final_all_plain` | Use plain inline for every reconstructed helper. | DIFFER 1502/2464 | 284 |
| `r_trail_final_inline_t150` | Build plain helpers with the per-row `/d2inlT150` setting. | DIFFER 1550/2464 | 292 |
| `r_trail_final_inline_s150` | Build plain helpers with the per-row `/d2inlS150` setting. | DIFFER 1704/2464 | 334 |
| `r_trail_donor_signed_light_count` | Restore the donor signed light-count local and use a standard downcast expression. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_plain_displacement_only` | Use ordinary inline for displacement shader selection alone. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_plain_vertex_api` | Use ordinary inline for the vertex-shader forwarding routine alone. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_plain_displacement_and_vertex` | Use ordinary inline for both of the small vertex-shader helpers. | MASKED_EQUAL 2464/2464 | 682 |
| `r_trail_final_combined_match` | Rebuild the selected source and test against the combined render header tree. | MASKED_EQUAL 2464/2464 | 682 |

Selected `r_trail_final_combined_match`. The displacement branch folds constant arguments through the same blend helper calls used on the ordinary pass. The donor identifies those calls directly. Restoring them changes backend ordering without introducing a new render-state implementation.

The plain-inline and per-row flag ablations justify the remaining three forced helpers. The two small shader helpers use ordinary inline in the selected source.
