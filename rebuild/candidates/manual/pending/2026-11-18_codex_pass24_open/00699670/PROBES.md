# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail emitted leaf diagnostic — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail pdb rekey — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure guard 1 named — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure guard 2 named — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure guard 3 named — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure guard 4 named — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure local fov assign — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure local groundClearance assign — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure local range assign — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure local vehicle assign — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure local waterClearance assign — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure loop 1 advance in body — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.
- tail structure loop 1 separate init — DIFFER, 759 v 761 bytes, 28 instruction hunks, first unequal byte at 0x00699694.

## Shared-header trials

| Trial | VC7.1 result |
| --- | --- |
| `tail_shared_01_camera_height` | COMPILE_FAIL |
| `tail_shared_02_camera_water_height` | 759/761 bytes, 28 strict hunks |
| `tail_shared_03_camera_named_cosine` | 759/761 bytes, 28 strict hunks |

Selected `tail_shared_03_camera_named_cosine` as the shared-header base. The emitted vector growth and water-height path agree in meaning. Retail saves the observed position in a different register, shifts the local frame homes, and schedules the ground comparison differently.

The merged include-tree regrade is `tail_shared_merged_receipt`. The standalone fixture builds and runs on both compilers against that tree.

The selected result is PARKED (759 object bytes, 761 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
