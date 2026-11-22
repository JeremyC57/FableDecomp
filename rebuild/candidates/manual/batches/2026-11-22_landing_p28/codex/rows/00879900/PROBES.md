# Probe history

The chronological records below distinguish source changes, compile failures, and repeat checks. The instruction count is a diagnostic alignment. A zero masked-byte residue and an independent operand audit determine a match.

| Probe | Outcome | Compiled bytes | Aligned instructions | Experiment |
|---|---|---:|---:|---|
| baseline | COMPILE_FAIL |  |  | Compile the supplied draft on the unmodified package headers. |
| recovery | COMPILE_FAIL |  |  | recovery |
| profile | COMPILE_FAIL |  |  | profile |
| recovered | DIFFER | 3374 | 486 | recovered (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |
| ncameramodes_cbase_cylinder_input | DIFFER | 3374 | 490 | ncameramodes cbase cylinder input |
| ncameramodes_cbase_setposition_inline | DIFFER | 3409 | 555 | ncameramodes cbase setposition inline |
| projectile_setfov_pass28 | DIFFER | 3402 | 538 | projectile setfov pass28 |
| projectile_pressure_pass28 | DIFFER | 3400 | 603 | Expose the original projectile lock-pressure getter on the shared attack component. |
| nominal_fov | DIFFER | 3400 | 603 | nominal fov |
| sway_shape | DIFFER | 3402 | 579 | sway shape |
| debug_zoom_division | DIFFER | 3400 | 597 | debug zoom division |
| debug_base_camera_pos | DIFFER | 3413 | 531 | debug base camera pos |
| debug_lock_bounds | DIFFER | 3392 | 535 | debug lock bounds |
| debug_view_box | DIFFER | 3383 | 537 | debug view box |
| debug_pop_out | DIFFER | 3381 | 536 | debug pop out |
| camera_debug_zoom_pair | DIFFER | 3416 | 572 | Use the named zoom-length parameter from the donor source scopes. |
| camera_line_end_argument | DIFFER | 3416 | 572 | camera line end argument |
| camera_magnitude | COMPILE_FAIL |  |  | camera magnitude |
| camera_named_amplitude | COMPILE_FAIL |  |  | camera named amplitude |
| camera_donor_base | COMPILE_FAIL |  |  | camera donor base |
| camera_out_copy | COMPILE_FAIL |  |  | camera out copy |
| camera_magnitude_typed | DIFFER | 3416 | 572 | camera magnitude typed |
| camera_named_amplitude_typed | COMPILE_FAIL |  |  | camera named amplitude typed |
| camera_donor_base_typed | COMPILE_FAIL |  |  | camera donor base typed |
| camera_out_copy_typed | COMPILE_FAIL |  |  | camera out copy typed |
| camera_named_amplitude_typed_abi | DIFFER | 3416 | 571 | camera named amplitude typed abi |
| camera_donor_base_typed_abi | DIFFER | 3429 | 533 | camera donor base typed abi |
| camera_out_copy_typed_abi | DIFFER | 3427 | 533 | camera out copy typed abi |
| camera_settled_locals | DIFFER | 3409 | 529 | Use PDB local names and the original 3D base camera position. |
| camera_look_then_base | DIFFER | 3413 | 531 | camera look then base |
| camera_up_projection | DIFFER | 3413 | 531 | camera up projection |
| handback_d558d19cdc_19bfecb | DIFFER | 3409 | 529 | Independent verification of a selected source. (same token sequence and flags as a previous probe; headers, leaf selection or verification context may differ). |

## Continued main-chat grades

The selected body compiles to 3,430 versus 3,415 bytes. The original creature filter and InvertY are now identified. Eight mapped bodies match. The 384-case behavior test passes. Three extra 12-byte vector slots remain the leading frame difference.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_cameraown_all | COMPILE_FAIL |  |  /  |
| round2_cameraown_both | 3438 | 636 | 6 / 1 |
| round2_cameraown_box | 3427 | 535 | 7 / 1 |
| round2_cameraown_full | 3436 | 630 | 6 / 1 |
| round2_cameraown_fullsearch | COMPILE_FAIL |  |  /  |
| round2_cameraown_later | 3411 | 534 | 7 / 1 |
| round2_cameraown_los | 3415 | 547 | 7 / 1 |
| round2_cameraown_search | COMPILE_FAIL |  |  /  |
| round2_cameraown_sensitivity | 3409 | 529 | 7 / 1 |
| round2_cameraown_vecpush | 3409 | 529 | 7 / 1 |
| round2_cameraown_vectors | 3388 | 533 | 6 / 1 |
| round2_camframe_double | 3442 | 650 | 8 / 0 |
| round2_camframe_early | 3442 | 649 | 8 / 0 |
| round2_camframe_end | 3430 | 661 | 8 / 0 |
| round2_camframe_enddouble | 3430 | 662 | 8 / 0 |
| round2_camframe_full | 3388 | 590 | 8 / 0 |
| round2_camframe_los | 3406 | 602 | 8 / 0 |
| round2_camframe_noalias | 3442 | 649 | 8 / 0 |
| round2_camframe_params | 3403 | 597 | 8 / 0 |
| round2_camframe_relative | 3442 | 649 | 8 / 0 |
| round2_camframe_roi | 3433 | 603 | 8 / 0 |
| round2_camframe_scopedend | 3388 | 590 | 8 / 0 |
| round2_camframe_union | 3397 | 620 | 8 / 0 |
| round2_camframe_vectors | 3403 | 597 | 8 / 0 |
| round2_cammath_adddirect | 3485 | 564 | 8 / 0 |
| round2_cammath_addref | 3494 | 614 | 8 / 0 |
| round2_cammath_alldirect | 3534 | 525 | 8 / 0 |
| round2_cammath_allref | 3546 | 515 | 8 / 0 |
| round2_cammath_groupdirect | 3442 | 650 | 8 / 0 |
| round2_cammath_groupref | 3430 | 662 | 8 / 0 |
| round2_cammath_nameddirect | 3534 | 525 | 8 / 0 |
| round2_cammath_namedref | 3546 | 515 | 8 / 0 |
| round2_cammath_scaledirect | 3456 | 589 | 8 / 0 |
| round2_cammath_scaleref | 3459 | 581 | 8 / 0 |
| round2_cammath_subdirect | 3468 | 552 | 8 / 0 |
| round2_cammath_subref | 3468 | 587 | 8 / 0 |
| round2_cammath_sumdirect | 3496 | 562 | 8 / 0 |
| round2_cammath_sumref | 3520 | 587 | 8 / 0 |
| round2_camowners2_all | COMPILE_FAIL |  |  /  |
| round2_camowners2_allhelpers | COMPILE_FAIL |  |  /  |
| round2_camowners2_both | COMPILE_FAIL |  |  /  |
| round2_camowners2_bothhelpers | COMPILE_FAIL |  |  /  |
| round2_camowners2_look | COMPILE_FAIL |  |  /  |
| round2_camowners2_pos | 3430 | 662 | 8 / 2 |
| round2_camowners2_scoped | COMPILE_FAIL |  |  /  |
| round2_camowners2_sphere | 3430 | 662 | 9 / 0 |
| round2_camowners3_all | 3433 | 650 | 10 / 2 |
| round2_camowners3_allhelpers | 3433 | 650 | 10 / 2 |
| round2_camowners3_both | 3433 | 650 | 9 / 2 |
| round2_camowners3_bothhelpers | 3433 | 650 | 9 / 2 |
| round2_camowners3_look | 3433 | 650 | 9 / 0 |
| round2_camowners3_pos | 3430 | 662 | 8 / 2 |
| round2_camowners3_scoped | 3391 | 622 | 10 / 2 |
| round2_camowners3_sphere | 3430 | 662 | 9 / 0 |
| round2_camquery_alive | COMPILE_FAIL |  |  /  |
| round2_camquery_alivebod | COMPILE_FAIL |  |  /  |
| round2_camquery_cols | 3438 | 636 | 6 / 1 |
| round2_camquery_direct | 3430 | 639 | 6 / 1 |
| round2_camquery_expr | 3432 | 647 | 6 / 1 |
| round2_camquery_good | 3436 | 630 | 6 / 1 |
| round2_camquery_havehelper | 3436 | 630 | 6 / 1 |
| round2_camquery_latehelper | 3436 | 630 | 6 / 1 |
| round2_camquery_orig | 3409 | 529 | 7 / 1 |
| round2_camquery_vis | 3415 | 547 | 7 / 1 |
| round2_camregion_00d | 3552 | 609 | 8 / 0 |
| round2_camregion_00r | 3552 | 628 | 8 / 0 |
| round2_camregion_01d | 3460 | 632 | 8 / 0 |
| round2_camregion_01r | 3466 | 642 | 8 / 0 |
| round2_camregion_02d | 3433 | 604 | 8 / 0 |
| round2_camregion_02r | 3430 | 662 | 8 / 0 |
| round2_camregion_03d | 3433 | 604 | 8 / 0 |
| round2_camregion_03r | 3430 | 662 | 8 / 0 |
| round2_camregion_10d | 3552 | 609 | 8 / 0 |
| round2_camregion_10r | 3552 | 628 | 8 / 0 |
| round2_camregion_11d | 3460 | 632 | 8 / 0 |
| round2_camregion_11r | 3466 | 642 | 8 / 0 |
| round2_camregion_12d | 3433 | 604 | 8 / 0 |
| round2_camregion_12r | 3430 | 662 | 8 / 0 |
| round2_camregion_13d | 3433 | 604 | 8 / 0 |
| round2_camregion_13r | 3430 | 662 | 8 / 0 |
| round2_camregion_21d | 3481 | 643 | 8 / 0 |
| round2_camregion_21r | 3490 | 679 | 8 / 0 |
| round2_camregion_22d | 3439 | 651 | 8 / 0 |
| round2_camregion_22r | 3427 | 640 | 8 / 0 |
| round2_camregion_23d | 3439 | 651 | 8 / 0 |
| round2_camregion_23r | 3427 | 640 | 8 / 0 |
| round2_camtrue_alive | 3442 | 649 | 7 / 1 |
| round2_camtrue_bound | 3442 | 647 | 7 / 0 |
| round2_camtrue_boxdecl | 3442 | 647 | 6 / 1 |
| round2_camtrue_combo | 3442 | 649 | 8 / 0 |
| round2_camtrue_final | 3442 | 647 | 6 / 1 |
| round2_camtrue_helpers | 3442 | 647 | 6 / 1 |
| round2_camtrue_late | 3442 | 647 | 6 / 1 |
| round2_camtrue_names | 3432 | 647 | 6 / 1 |
| round2_camtrue_vector | 3442 | 647 | 6 / 1 |
| round2_camtrue_weights | 3432 | 651 | 6 / 1 |
