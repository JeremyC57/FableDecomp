# Native VC7.1 source experiments

Received baseline DIFFER at 2240 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_subtitle_area_argument` | Build the text-area box in the formatter call. | DIFFER 2274/2223 | 570 |
| `r_subtitle_extents_fields` | Write the measured extents to the two offset fields separately. | DIFFER 2271/2223 | 565 |
| `r_subtitle_line_block_unsigned` | Use signed variables with an unsigned size comparison throughout. | DIFFER 2274/2223 | 653 |
| `r_subtitle_new_lines_flag` | Replace the break loop with the donor newline flag. | DIFFER 2274/2223 | 585 |
| `r_subtitle_pushback_visibility` | Allow vector push_back to be expanded from its shared STLport body. | DIFFER 2255/2223 | 591 |
| `r_subtitle_second_count_guard` | Use a post-tested loop around the donor newline flag. | DIFFER 2260/2223 | 646 |
| `r_subtitle_shared` | Model through the shared classes with signed donor loop variables and accurate extents. | DIFFER 2276/2223 | 643 |
| `r_subtitle_wide_eq_decl` | Use the exact externally owned wide STLport string equality. | DIFFER 2260/2223 | 646 |

Selected `r_subtitle_wide_eq_decl`. Reconstructed private signature, original line-count flag and distinct width/height extents. Subtitle formatting temporary lifetimes and frame slots still differ.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

The full 2,223-byte function matched, all 726 instructions and all 150 relocated operands. The shared-vector destructor and insertion helpers accounted for five operand sites missing from the old binding table. The release erase at 004940AC calls the typed copy at 00493A5A then the typed destruction at 00440BA0, uses the 20-byte highlight and returns its first iterator. The push at 004942C3 calls the original embedded-highlight copy and 004941F2. The destructor at 00442870 walks highlights, destroys their wide strings, and frees storage. Five independently checked emitted copies match their own retail extents and all twelve of their relocs in addition to the eight the supplied checker recognized. The lexical lifetime of the text-area box, signed render-target width, typed STLport helpers, and grouping the top-edge conversion together reproduce the native instructions. The self-contained test executes the reconstructed algorithm with controlled wrapping and font metrics and passed on VC7.1.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_sub_component_push | 2236 | 660 | 5 / 3 |
| round2_sub_component_push_copy | 2236 | 660 | 5 / 3 |
| round2_sub_components | 2255 | 661 | 2 / 0 |
| round2_sub_components_copy | 2255 | 661 | 2 / 0 |
| round2_sub_push_decl_overflow | 2239 | 663 | 5 / 3 |
| round2_sub_push_decl_overflow_copy | 2239 | 663 | 5 / 3 |
| round2_sub_unsigned | 2258 | 656 | 2 / 0 |
| round2_sub_unsigned_copy | 2258 | 656 | 2 / 0 |
| round2_sub_visibility | 2239 | 663 | 5 / 3 |
| round2_sub_visibility_copy | 2239 | 663 | 5 / 3 |
| round2_subabi_concrete | 2223 | 724 | 6 / 2 |
| round2_subabi_concrete_default | 2223 | 724 | 8 / 0 |
| round2_subabi_concrete_fast | 2223 | 724 | 8 / 0 |
| round2_subabi_fast | 2223 | 724 | 6 / 2 |
| round2_subabi_visible_len | 2223 | 724 | 7 / 1 |
| round2_subabi_visible_ret | 2223 | 724 | 7 / 1 |
| round2_subabi_visible_rev | 2223 | 724 | 7 / 1 |
| round2_subbox_address0 | 2223 | 725 | 8 / 0 |
| round2_subbox_address1 | 2223 | 725 | 8 / 0 |
| round2_subbox_address2 | 2270 | 542 | 8 / 0 |
| round2_subbox_address3 | 2223 | 724 | 8 / 0 |
| round2_subbox_address4 | COMPILE_FAIL |  |  /  |
| round2_subbox_box_BODY | 2223 | 724 | 8 / 0 |
| round2_subbox_box_GROUP | 2223 | 717 | 8 / 0 |
| round2_subbox_box_INIT | 2223 | 724 | 8 / 0 |
| round2_subbox_box_REVERSE | 2223 | 718 | 8 / 0 |
| round2_subbox_box_YX | 2223 | 724 | 8 / 0 |
| round2_subbox_firstArg | 2223 | 724 | 8 / 0 |
| round2_subcs_both | 2223 | 726 | 8 / 0 |
| round2_subcs_callee_abi | 2223 | 726 | 8 / 0 |
| round2_subcs_copy | 2223 | 726 | 8 / 0 |
| round2_subcs_dtor_size | 2223 | 726 | 8 / 0 |
| round2_subcs_dtor_speed | 2223 | 726 | 8 / 0 |
| round2_subcs_fill | 2223 | 726 | 8 / 0 |
| round2_subcs_flare | 2223 | 726 | 8 / 0 |
| round2_subcs_specific | 2223 | 726 | 8 / 0 |
| round2_subcvt_bcast | 2229 | 706 | 8 / 0 |
| round2_subcvt_bnarrow | 2223 | 725 | 8 / 0 |
| round2_subcvt_bneg | 2223 | 725 | 8 / 0 |
| round2_subcvt_bnegcast | 2229 | 706 | 8 / 0 |
| round2_subcvt_brorder | 2223 | 724 | 8 / 0 |
| round2_subcvt_bwrap | 2225 | 724 | 8 / 0 |
| round2_subcvt_nameconstfloat | 2229 | 704 | 8 / 0 |
| round2_subcvt_nameconstfloatref | 2223 | 724 | 8 / 0 |
| round2_subcvt_namefloat | 2229 | 704 | 8 / 0 |
| round2_subcvt_tbcast | 2235 | 686 | 8 / 0 |
| round2_subcvt_tbnarrow | 2223 | 725 | 8 / 0 |
| round2_subcvt_tbneg | 2223 | 725 | 8 / 0 |
| round2_subcvt_tbnegcast | 2235 | 686 | 8 / 0 |
| round2_subcvt_tbwrap | 2227 | 724 | 8 / 0 |
| round2_subcvt_tcast | 2229 | 724 | 8 / 0 |
| round2_subcvt_tnarrow | 2223 | 725 | 8 / 0 |
| round2_subcvt_tneg | 2223 | 725 | 8 / 0 |
| round2_subcvt_tnegcast | 2229 | 724 | 8 / 0 |
| round2_subcvt_twrap | 2225 | 725 | 8 / 0 |
| round2_subdestroy_d_expand | 2223 | 720 | 6 / 0 |
| round2_subdestroy_decl | 2223 | 720 | 5 / 1 |
| round2_subdestroy_first_const | 2223 | 720 | 6 / 0 |
| round2_subdestroy_first_do | 2223 | 721 | 6 / 0 |
| round2_subdestroy_first_dos | 2223 | 721 | 5 / 1 |
| round2_subdestroy_speed_decl | 2223 | 720 | 6 / 0 |
| round2_subdestroy_speed_headers | 2223 | 720 | 5 / 1 |
| round2_subdestroy_v_expand | 2223 | 720 | 6 / 1 |
| round2_subdestroy_visible | 2223 | 720 | 5 / 2 |
| round2_subearly_concrete | COMPILE_FAIL |  |  /  |
| round2_subearly_concrete_default | COMPILE_FAIL |  |  /  |
| round2_subearly_concrete_fast | COMPILE_FAIL |  |  /  |
| round2_subearly_fast | COMPILE_FAIL |  |  /  |
| round2_subearly_visible_len | COMPILE_FAIL |  |  /  |
| round2_subearly_visible_ret | COMPILE_FAIL |  |  /  |
| round2_subearly_visible_rev | COMPILE_FAIL |  |  /  |
| round2_subfinal_both | 2242 | 671 | 6 / 0 |
| round2_subfinal_dtor | 2242 | 671 | 8 / 0 |
| round2_subfinal_overflow | 2242 | 671 | 6 / 0 |
| round2_subfinal_specboth | 2242 | 671 | 6 / 0 |
| round2_subfinal_specd | 2242 | 671 | 8 / 0 |
| round2_subfinal_specdo | 2242 | 671 | 6 / 0 |
| round2_subfinal_speco | 2242 | 671 | 6 / 0 |
| round2_subfloat_assignment | 2237 | 544 | 8 / 0 |
| round2_subfloat_cast_width | 2223 | 725 | 8 / 0 |
| round2_subfloat_extents | 2223 | 725 | 8 / 0 |
| round2_subfloat_local_const | 2223 | 725 | 8 / 0 |
| round2_subfloat_nondoubleright | 2223 | 725 | 8 / 0 |
| round2_subfloat_omit | 2223 | 725 | 8 / 0 |
| round2_subfloat_precisefirst | 2223 | 725 | 8 / 0 |
| round2_subfloat_ret_copy | 2223 | 725 | 8 / 0 |
| round2_subfloat_right | 2223 | 725 | 8 / 0 |
| round2_subfloat_roundtop | 2223 | 725 | 8 / 0 |
| round2_subfloat_spill | 2223 | 724 | 8 / 0 |
| round2_subfloat_top_store | 2237 | 544 | 8 / 0 |
| round2_subgrowth_both | 2223 | 726 | 10 / 0 |
| round2_subgrowth_calls | 2223 | 726 | 10 / 0 |
| round2_subgrowth_copy | 2223 | 726 | 9 / 0 |
| round2_subgrowth_fast | 2223 | 726 | 10 / 0 |
| round2_subgrowth_fill | 2223 | 726 | 9 / 0 |
| round2_subgrowth_flare | 2223 | 726 | 10 / 0 |
| round2_subindex_base | 2242 | 666 | 5 / 0 |
| round2_subindex_checked | 2238 | 665 | 5 / 0 |
| round2_subindex_clean | 2242 | 666 | 5 / 0 |
| round2_subindex_eq | 2242 | 666 | 5 / 1 |
| round2_subindex_eqout | 2242 | 666 | 5 / 1 |
| round2_subindex_guard | 2254 | 668 | 5 / 0 |
| round2_subindex_indexstart | 2240 | 646 | 5 / 0 |
| round2_subindex_origindex | 2242 | 666 | 5 / 0 |
| round2_subindex_ptr | 2239 | 651 | 5 / 0 |
| round2_subindex_ptrplus | 2239 | 651 | 5 / 0 |
| round2_subindex_ref | 2236 | 652 | 5 / 0 |
| round2_subindex_second | 2242 | 666 | 5 / 0 |
| round2_subnear_base | 2223 | 720 | 5 / 1 |
| round2_subnear_cache | 2220 | 705 | 5 / 1 |
| round2_subnear_checked | 2219 | 719 | 5 / 1 |
| round2_subnear_equal | 2223 | 720 | 5 / 2 |
| round2_subnear_first_base | 2223 | 720 | 5 / 1 |
| round2_subnear_first_guard | 2235 | 722 | 5 / 1 |
| round2_subnear_first_ptr | 2220 | 705 | 5 / 1 |
| round2_subnear_first_ref | 2217 | 706 | 5 / 1 |
| round2_subnear_index | 2221 | 697 | 5 / 1 |
| round2_subnear_second | 2223 | 720 | 5 / 1 |
| round2_subnear_store_lr | 2205 | 649 | 5 / 1 |
| round2_subnear_store_rl | 2205 | 660 | 5 / 1 |
| round2_subown_concrete | COMPILE_FAIL |  |  /  |
| round2_subown_concrete_default | COMPILE_FAIL |  |  /  |
| round2_subown_concrete_fast | COMPILE_FAIL |  |  /  |
| round2_subown_fast | 2223 | 724 | 6 / 2 |
| round2_subown_visible_len | COMPILE_FAIL |  |  /  |
| round2_subown_visible_ret | COMPILE_FAIL |  |  /  |
| round2_subown_visible_rev | COMPILE_FAIL |  |  /  |
| round2_subparen_bothheight | 2223 | 725 | 8 / 0 |
| round2_subparen_bothreturns | 2223 | 723 | 8 / 0 |
| round2_subparen_bottom | 2223 | 725 | 8 / 0 |
| round2_subparen_bottomret | 2223 | 724 | 8 / 0 |
| round2_subparen_height | 2223 | 725 | 8 / 0 |
| round2_subparen_inner | 2223 | 725 | 8 / 0 |
| round2_subparen_left | 2223 | 725 | 8 / 0 |
| round2_subparen_left_bothheight | 2223 | 725 | 8 / 0 |
| round2_subparen_left_bottom | 2223 | 725 | 8 / 0 |
| round2_subparen_left_height | 2223 | 725 | 8 / 0 |
| round2_subparen_left_leftarg | 2223 | 725 | 8 / 0 |
| round2_subparen_left_right | 2223 | 724 | 8 / 0 |
| round2_subparen_left_top | 2223 | 725 | 8 / 0 |
| round2_subparen_left_topret | 2223 | 724 | 8 / 0 |
| round2_subparen_leftarg | 2223 | 725 | 8 / 0 |
| round2_subparen_right | 2223 | 724 | 8 / 0 |
| round2_subparen_rightarg | 2223 | 725 | 8 / 0 |
| round2_subparen_rightsum | 2223 | 725 | 8 / 0 |
| round2_subparen_terms | 2223 | 724 | 8 / 0 |
| round2_subparen_top | 2223 | 726 | 8 / 0 |
| round2_subparen_toparg | 2223 | 725 | 8 / 0 |
| round2_subparen_topret | 2223 | 724 | 8 / 0 |
| round2_subreadonly_all_visible | 2164 | 531 | 4 / 4 |
| round2_subreadonly_box_arg | 2223 | 721 | 6 / 0 |
| round2_subreadonly_box_x_first | 2223 | 712 | 6 / 0 |
| round2_subreadonly_visible0 | 2223 | 724 | 6 / 2 |
| round2_subreadonly_visible1 | 2223 | 724 | 6 / 2 |
| round2_subreadonly_visible2 | 2223 | 724 | 6 / 2 |
| round2_subsched_ctorarg | 2223 | 725 | 8 / 0 |
| round2_subsched_init0 | 2223 | 725 | 8 / 0 |
| round2_subsched_init1 | 2223 | 724 | 8 / 0 |
| round2_subsched_init2 | 2223 | 724 | 8 / 0 |
| round2_subsched_init3 | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp0 | 2223 | 715 | 8 / 0 |
| round2_subsched_tmp0ref | 2223 | 715 | 8 / 0 |
| round2_subsched_tmp1 | 2223 | 714 | 8 / 0 |
| round2_subsched_tmp1ref | 2223 | 714 | 8 / 0 |
| round2_subsched_tmp2 | 2217 | 683 | 8 / 0 |
| round2_subsched_tmp2ref | 2217 | 683 | 8 / 0 |
| round2_subsched_tmp3 | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp32 | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp321 | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp3210 | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp3210ref | 2223 | 723 | 8 / 0 |
| round2_subsched_tmp321ref | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp32ref | 2223 | 724 | 8 / 0 |
| round2_subsched_tmp3ref | 2223 | 724 | 8 / 0 |
| round2_subscope_box_body | 2223 | 696 | 5 / 1 |
| round2_subscope_commute | 2223 | 696 | 5 / 1 |
| round2_subscope_declorder | 2226 | 691 | 5 / 1 |
| round2_subscope_narrow | 2223 | 720 | 5 / 1 |
| round2_subscope_narrow_order | 2223 | 720 | 5 / 1 |
| round2_subscope_outer_tmp | 2223 | 696 | 5 / 1 |
| round2_subscope_wide_decl | 2223 | 696 | 5 / 1 |
| round2_window_boxes | 2242 | 669 | 2 / 0 |
| round2_window_datum | 2239 | 661 | 2 / 0 |
| round2_window_datum_visible | 2220 | 688 | 5 / 3 |
| round2_window_double | 2242 | 670 | 2 / 0 |
| round2_window_iter_diagnostic | 2210 | 685 | 2 / 0 |
| round2_window_signed | 2242 | 669 | 2 / 0 |
| round2_window_visible | 2223 | 697 | 5 / 3 |
