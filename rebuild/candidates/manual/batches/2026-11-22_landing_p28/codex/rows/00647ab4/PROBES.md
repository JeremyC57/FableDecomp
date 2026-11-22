# Native VC7.1 source experiments

Received baseline DIFFER at 87 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_health_decl_bitset` | Ablate the existing LowerBound owner option while keeping the bitset declaration-only. | DIFFER 1852/1910 | 443 |
| `r_health_effect_generic_complete` | Lead with the effect identity before testing fade state. | DIFFER 1852/1910 | 441 |
| `r_health_hero_guard_complete` | Place the body under a positive alive test. | DIFFER 1852/1910 | 443 |
| `r_health_inventory_guard_order_complete` | Use the definition lookup result to select the potion count before the count check. | DIFFER 1856/1910 | 442 |
| `r_health_position_box_complete` | Construct the middle box from its two corner vectors. | DIFFER 1866/1910 | 432 |
| `r_health_reconstruction_complete` | Reconstruct the complete effect, item count, and sprite handling. | DIFFER 1852/1910 | 443 |
| `r_health_speed_copy_owners` | Use shared declaration-only bitset and existing speed-mode LowerBound owner. | DIFFER 1852/1910 | 443 |

Selected `r_health_speed_copy_owners`. Reconstructed alive guard, heal effects, inventory counter and five sprite submissions. Frame slots, colour copying and primitive-setup ordering remain different.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

All 1,910 bytes, 663 instructions and 57 leaf operands match. Eight mapped copies match and three independently recovered owners match, including both GUI overload/copy sites. The raw checker also reports the previously documented, unreferenced folded destructor alias. All 2,048 fixture scenarios run the submitted Draw body.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_healthassign_implicit | 1910 | 658 | 5 / 2 |
| round2_healthassign_member | 1916 | 347 | 5 / 2 |
| round2_healthassign_named | 1916 | 651 | 5 / 2 |
| round2_healthassign_owner | 1897 | 645 | 7 / 1 |
| round2_healthassign_temp | 1910 | 653 | 5 / 2 |
| round2_healthassign_typed | 1910 | 648 | 5 / 2 |
| round2_healthbox_combined | 1916 | 346 | 5 / 2 |
| round2_healthbox_copy | 1916 | 347 | 4 / 3 |
| round2_healthbox_copyvec | 1916 | 347 | 4 / 3 |
| round2_healthbox_font | 1916 | 347 | 4 / 3 |
| round2_healthbox_refs | 1916 | 347 | 4 / 3 |
| round2_healthbox_size | 1916 | 347 | 5 / 2 |
| round2_healthbox_sum | 1916 | 346 | 4 / 3 |
| round2_healthcalls_bitown | 1914 | 459 | 6 / 2 |
| round2_healthcalls_branches | 1913 | 454 | 4 / 3 |
| round2_healthcalls_col | 1927 | 468 | 4 / 3 |
| round2_healthcalls_font | COMPILE_FAIL |  |  /  |
| round2_healthcalls_stats | 1927 | 462 | 4 / 3 |
| round2_healthcalls_vec | 1927 | 468 | 4 / 3 |
| round2_healthgeom_dword | 1913 | 455 | 4 / 3 |
| round2_healthgeom_extents | 1913 | 455 | 4 / 3 |
| round2_healthgeom_plain | 1913 | 456 | 4 / 3 |
| round2_healthgeom_real | 1913 | 455 | 4 / 3 |
| round2_healthgeom_rgba | 1927 | 466 | 4 / 3 |
| round2_healthlast_both | 1910 | 656 | 5 / 2 |
| round2_healthlast_copy0 | 1911 | 640 | 5 / 2 |
| round2_healthlast_copy2 | 1914 | 656 | 5 / 2 |
| round2_healthlast_firstscalar | 1911 | 643 | 5 / 2 |
| round2_healthlast_flip0 | 1910 | 655 | 5 / 2 |
| round2_healthlast_flip2 | 1910 | 659 | 5 / 2 |
| round2_healthlast_force | 1910 | 658 | 7 / 1 |
| round2_healthlast_forceExpanded | 1910 | 658 | 7 / 1 |
| round2_healthlast_potion | 1911 | 653 | 5 / 2 |
| round2_healthlast_scalars | 1912 | 640 | 5 / 2 |
| round2_healthown_bit | 1897 | 431 | 4 / 3 |
| round2_healthown_cursor | 1924 | 386 | 3 / 3 |
| round2_healthown_full | 1910 | 438 | 3 / 3 |
| round2_healthown_rgb | 1852 | 443 | 3 / 2 |
| round2_healthown_screen | 1910 | 442 | 3 / 3 |
| round2_healthown_stats | 1852 | 445 | 3 / 2 |
| round2_healthsealprobe_both | 1910 | 656 | 7 / 1 |
| round2_healthsealprobe_ctor | 1910 | 659 | 7 / 1 |
| round2_healthsealprobe_def | 1910 | 659 | 7 / 1 |
| round2_healthsealprobe_lhs | 1910 | 659 | 7 / 1 |
| round2_healthsealprobe_pflip | 1910 | 655 | 7 / 1 |
| round2_healthsealprobe_plain | 1910 | 659 | 7 / 1 |
| round2_healthsealprobe_rhs | 1910 | 656 | 7 / 1 |
| round2_healthsealprobe_sumrhs | 1910 | 658 | 7 / 1 |
| round2_healthsealprobe_typed | 1910 | 653 | 7 / 1 |
| round2_healthshape_choices | 1910 | 658 | 7 / 1 |
| round2_healthshape_def | 1910 | 658 | 7 / 1 |
| round2_healthshape_faithful | 1910 | 658 | 7 / 1 |
| round2_healthshape_init | 1914 | 657 | 7 / 1 |
| round2_healthshape_initall | 1914 | 657 | 7 / 1 |
| round2_healthshape_lex | 1910 | 658 | 7 / 1 |
| round2_healthshape_locals | 1910 | 658 | 7 / 1 |
| round2_healthshape_named | 1916 | 651 | 7 / 1 |
| round2_healthshape_ref | 1910 | 656 | 7 / 1 |
| round2_healthshape_type | 1910 | 648 | 7 / 1 |
| round2_healthshape_vec | 1910 | 658 | 7 / 1 |
| round2_healthshared_parenraw | 1910 | 649 | 8 / 1 |
| round2_healthshared_parentyped | 1910 | 648 | 8 / 1 |
| round2_healthshared_plainraw | 1910 | 658 | 8 / 1 |
| round2_healthshared_plaintyped | 1910 | 653 | 8 / 1 |
| round2_healthshared_readrhsraw | 1910 | 660 | 8 / 1 |
| round2_healthshared_readrhstyped | 1910 | 655 | 8 / 1 |
| round2_healthshared_reverseXraw | 1910 | 660 | 8 / 1 |
| round2_healthshared_reverseXtyped | 1910 | 653 | 8 / 1 |
| round2_healthshared_reverseYraw | 1910 | 658 | 8 / 1 |
| round2_healthshared_reverseYtyped | 1910 | 653 | 8 / 1 |
| round2_healthshared_site0explicit | 1910 | 660 | 8 / 1 |
| round2_healthshared_site0fromoffset | 1911 | 648 | 8 / 1 |
| round2_healthshared_site0leftref | 1910 | 658 | 8 / 1 |
| round2_healthshared_site0members | 1910 | 655 | 8 / 1 |
| round2_healthshared_site0members2 | 1911 | 645 | 8 / 1 |
| round2_healthshared_site0members3 | 1910 | 660 | 8 / 1 |
| round2_healthshared_site0paren | 1910 | 660 | 8 / 1 |
| round2_healthshared_site0ptr | 1910 | 655 | 8 / 1 |
| round2_healthshared_site0refs | 1910 | 655 | 8 / 1 |
| round2_healthshared_site0refs2 | 1910 | 653 | 8 / 1 |
| round2_healthshared_site0returnref | 1910 | 660 | 8 / 1 |
| round2_healthshared_site0rightref | 1910 | 655 | 8 / 1 |
| round2_healthshared_site0self | 1926 | 637 | 8 / 1 |
| round2_healthshared_site0tmp | 1910 | 655 | 8 / 1 |
| round2_healthshared_site2explicit | 1910 | 660 | 8 / 1 |
| round2_healthshared_site2fromoffset | 1914 | 657 | 8 / 1 |
| round2_healthshared_site2leftref | 1910 | 658 | 8 / 1 |
| round2_healthshared_site2members | 1910 | 658 | 8 / 1 |
| round2_healthshared_site2members2 | 1911 | 655 | 8 / 1 |
| round2_healthshared_site2members3 | 1910 | 653 | 8 / 1 |
| round2_healthshared_site2paren | 1910 | 660 | 8 / 1 |
| round2_healthshared_site2ptr | 1910 | 655 | 8 / 1 |
| round2_healthshared_site2refs | 1910 | 655 | 8 / 1 |
| round2_healthshared_site2refs2 | 1910 | 655 | 8 / 1 |
| round2_healthshared_site2returnref | 1910 | 660 | 8 / 1 |
| round2_healthshared_site2rightref | 1910 | 655 | 8 / 1 |
| round2_healthshared_site2self | 1926 | 656 | 8 / 1 |
| round2_healthshared_site2tmp | 1910 | 663 | 8 / 1 |
| round2_healthshared_v2hptr | 1910 | 653 | 8 / 1 |
| round2_healthshared_v2ibox | 1910 | 660 | 8 / 1 |
| round2_healthshared_v2lhs | 1910 | 651 | 8 / 1 |
| round2_healthshared_v2lhsrhs | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2pcopy | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2pcref | 1910 | 660 | 8 / 1 |
| round2_healthshared_v2pflip | 1910 | 661 | 8 / 1 |
| round2_healthshared_v2poti | 1914 | 661 | 8 / 1 |
| round2_healthshared_v2preceiver | 1914 | 658 | 8 / 1 |
| round2_healthshared_v2pscalar | 1911 | 655 | 8 / 1 |
| round2_healthshared_v2reverseboth | 1910 | 660 | 8 / 1 |
| round2_healthshared_v2rgb | 1896 | 643 | 8 / 1 |
| round2_healthshared_v2rhs_my_yref | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2rhs_my_yvalue | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2rhs_p | 1910 | 653 | 8 / 1 |
| round2_healthshared_v2rhs_r_xy | 1910 | 660 | 8 / 1 |
| round2_healthshared_v2rhs_r_y | 1910 | 660 | 8 / 1 |
| round2_healthshared_v2rhs_yref | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2rhs_yvalue | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2x_ref | 1910 | 655 | 8 / 1 |
| round2_healthshared_v2y_ref | 1910 | 655 | 8 / 1 |
| round2_healthshared_valueraw | 1900 | 540 | 8 / 1 |
| round2_healthshared_valuetyped | 1900 | 540 | 8 / 1 |
| round2_healthshared_xyraw | 1910 | 658 | 8 / 1 |
| round2_healthshared_xytyped | 1910 | 653 | 8 / 1 |
| round2_healthshared_yxraw | 1910 | 658 | 8 / 1 |
| round2_healthshared_yxtyped | 1910 | 653 | 8 / 1 |
| round2_healthtyped_0copy | 1910 | 653 | 7 / 1 |
| round2_healthtyped_0init | 1910 | 658 | 7 / 1 |
| round2_healthtyped_0member | 1910 | 658 | 7 / 1 |
| round2_healthtyped_0paren | 1910 | 658 | 7 / 1 |
| round2_healthtyped_0rhs | 1911 | 646 | 7 / 1 |
| round2_healthtyped_0sum | 1911 | 643 | 7 / 1 |
| round2_healthtyped_2copy | 1910 | 653 | 7 / 1 |
| round2_healthtyped_2init | 1910 | 658 | 7 / 1 |
| round2_healthtyped_2member | 1910 | 658 | 7 / 1 |
| round2_healthtyped_2paren | 1910 | 658 | 7 / 1 |
| round2_healthtyped_2rhs | 1914 | 652 | 7 / 1 |
| round2_healthtyped_2sum | 1914 | 659 | 7 / 1 |
| round2_healthtyped_all | 1910 | 653 | 7 / 1 |
| round2_healthtyped_haste | 1910 | 648 | 7 / 1 |
| round2_healthtyped_invent | 1910 | 653 | 7 / 1 |
