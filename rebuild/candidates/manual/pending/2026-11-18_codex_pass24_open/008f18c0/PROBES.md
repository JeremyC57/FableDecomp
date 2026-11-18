# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 772 v 770 bytes, 32 instruction hunks, first unequal byte at 0x008F195C.
- tail emitted leaf diagnostic — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.
- tail measured highlighted read before clear — DIFFER, 772 v 770 bytes, 32 instruction hunks, first unequal byte at 0x008F195C.
- tail measured named card and extent — DIFFER, 772 v 770 bytes, 36 instruction hunks, first unequal byte at 0x008F18FF.
- tail pdb rekey — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.
- tail structure guard 1 named — DIFFER, 772 v 770 bytes, 32 instruction hunks, first unequal byte at 0x008F195C.
- tail structure guard 2 named — DIFFER, 772 v 770 bytes, 32 instruction hunks, first unequal byte at 0x008F195C.
- tail structure guard 3 named — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F195C.
- tail structure guard 4 named — DIFFER, 772 v 770 bytes, 32 instruction hunks, first unequal byte at 0x008F195C.
- tail structure local halfHeight assign — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.
- tail structure local halfWidth assign — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.
- tail structure loop 1 advance in body — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.
- tail structure loop 1 separate init — DIFFER, 772 v 770 bytes, 34 instruction hunks, first unequal byte at 0x008F1925.

- main_final_cached_axes_component — DIFFER, 741/770 bytes, 41 strict instruction hunks.
- main_final_cached_axes_condition — DIFFER, 754/770 bytes, 39 strict instruction hunks.
- main_final_cached_axes_default — DIFFER, 741/770 bytes, 39 strict instruction hunks.
- main_final_cached_axes_iterator — DIFFER, 741/770 bytes, 39 strict instruction hunks.
- main_final_cross_component — DIFFER, 795/770 bytes, 22 strict instruction hunks.
- main_final_cross_condition — DIFFER, 808/770 bytes, 22 strict instruction hunks.
- main_final_cross_default — DIFFER, 795/770 bytes, 22 strict instruction hunks.
- main_final_cross_iterator — DIFFER, 795/770 bytes, 22 strict instruction hunks.
- main_final_left_component — DIFFER, 795/770 bytes, 22 strict instruction hunks.
- main_final_left_condition — DIFFER, 808/770 bytes, 21 strict instruction hunks.
- main_final_left_iterator — DIFFER, 795/770 bytes, 21 strict instruction hunks.
- main_final_reverse_component — COMPILE_FAIL.
- main_final_reverse_condition — COMPILE_FAIL.
- main_final_reverse_default — COMPILE_FAIL.
- main_final_reverse_iterator — COMPILE_FAIL.

- main_shared_try01 — COMPILE_FAIL.
- main_shared_try02 — DIFFER, 795/770 bytes, 21 strict instruction hunks.

- main_selected_ports — DIFFER, 741/770 bytes, 39 strict instruction hunks.

- main_card_final — DIFFER, 795/770 bytes, 21 strict instruction hunks.

The selected result is PARKED (795 object bytes, 770 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
