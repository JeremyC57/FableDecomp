# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail emitted leaf diagnostic — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail measured named candidate distances — DIFFER, 1034 v 1023 bytes, 61 instruction hunks, first unequal byte at 0x00877842.
- tail measured write no hit early — DIFFER, 1011 v 1023 bytes, 71 instruction hunks, first unequal byte at 0x00877842.
- tail pdb rekey — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure guard 1 named — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure guard 2 named — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure guard 2 nested and — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure guard 3 named — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure guard 4 named — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure local bestValue assign — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure local hit assign — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.
- tail structure local value assign — DIFFER, 1021 v 1023 bytes, 27 instruction hunks, first unequal byte at 0x008778C8.

Main review used the actual shared camera, thing, vector and STL types and the four Ego_d inner scopes. It reduced the strict diff to the final hit conditional. Eight graded control-flow combinations compared a positive hit exit, a shared boolean result, nested distance guard, and early failure guard. The positive hit exit followed by return true, with the distance failure guard early, matched the complete 1023-byte extent.

- main_final_port01 — DIFFER, 1021/1023 bytes, 2 strict instruction hunks.

- main_los_assigned_result0 — DIFFER, 1006/1023 bytes, 67 strict instruction hunks.
- main_los_assigned_result1 — DIFFER, 1010/1023 bytes, 66 strict instruction hunks.
- main_los_inverted_result0 — DIFFER, 1011/1023 bytes, 68 strict instruction hunks.
- main_los_inverted_result1 — DIFFER, 1015/1023 bytes, 67 strict instruction hunks.
- main_los_neg_reversed0 — DIFFER, 1021/1023 bytes, 2 strict instruction hunks.
- main_los_neg_reversed1 — DIFFER, 1025/1023 bytes, 4 strict instruction hunks.
- main_los_positive0 — MASKED_MATCH, 1023/1023 bytes, 0 strict instruction hunks.
- main_los_positive1 — DIFFER, 1027/1023 bytes, 2 strict instruction hunks.
- main_los_verified — MASKED_MATCH, 1023/1023 bytes, 0 strict instruction hunks.

- main_final_port01 — DIFFER, 1021/1023 bytes, 2 strict instruction hunks.

The selected result is RELOCATION_MATCH (1023 object bytes, 1023 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
