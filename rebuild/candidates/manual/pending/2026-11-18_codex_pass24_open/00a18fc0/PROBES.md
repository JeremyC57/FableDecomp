# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail emitted leaf diagnostic — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail pdb rekey — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure calculation delta 1 separate result — COMPILE_FAIL.
- tail structure calculation delta 2 separate result — COMPILE_FAIL.
- tail structure guard 1 named — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure guard 2 named — DIFFER, 1111 v 1125 bytes, 61 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure guard 3 named — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure guard 4 named — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure local delta assign — DIFFER, 1146 v 1125 bytes, 83 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure local distanceSquared assign — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure local next assign — DIFFER, 1147 v 1125 bytes, 56 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure local rangeSquared assign — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.
- tail structure local range assign — DIFFER, 1123 v 1125 bytes, 60 instruction hunks, first unequal byte at 0x00A18FC2.

- main_final_c3dvector_normalise_locals — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_c3dvector_normalise_locals_yzx — DIFFER, 1131/1125 bytes, 63 strict instruction hunks.
- main_final_c3dvector_normalise_named_squared — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_c3dvector_normalise_squared_zxy — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_c3dvector_normalise_unparen — DIFFER, 1103/1125 bytes, 67 strict instruction hunks.
- main_final_c3dvector_normalise_zxy — DIFFER, 1103/1125 bytes, 67 strict instruction hunks.
- main_final_donor_locals — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_inline_flat — DIFFER, 1131/1125 bytes, 61 strict instruction hunks.
- main_final_merge_arrival — DIFFER, 1134/1125 bytes, 77 strict instruction hunks.
- main_final_path_assignment — DIFFER, 1155/1125 bytes, 63 strict instruction hunks.
- main_final_port01 — DIFFER, 1123/1125 bytes, 60 strict instruction hunks.
- main_final_reverse_next — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_scalar_temp — DIFFER, 1131/1125 bytes, 60 strict instruction hunks.
- main_final_speed_after — DIFFER, 1127/1125 bytes, 61 strict instruction hunks.

- main_selected_ports — DIFFER, 1127/1125 bytes, 61 strict instruction hunks.

- main_final_port01 — DIFFER, 1123/1125 bytes, 60 strict instruction hunks.

The selected result is PARKED (1127 object bytes, 1125 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
