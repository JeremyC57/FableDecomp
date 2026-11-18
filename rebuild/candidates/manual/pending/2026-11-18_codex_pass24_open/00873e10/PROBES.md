# Compiler probes

- package baseline — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail deliverable — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail pdb rekey — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure calculation sensitivity 1 separate result — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure calculation sensitivity x 2 separate result — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure guard 1 named — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure guard 2 named — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure guard 3 named — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure guard 4 named — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure local dolly target assign — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure local head pos assign — DIFFER, 2400 v 2386 bytes, 39 instruction hunks, first unequal byte at 0x00873E58.
- tail structure local sensitivity x assign — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure local sensitivity y assign — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure local thing pos assign — DIFFER, 2388 v 2386 bytes, 15 instruction hunks, first unequal byte at 0x00873EEE.
- tail structure loop 1 advance in body — DIFFER, 2296 v 2386 bytes, 56 instruction hunks, first unequal byte at 0x00873E77.
- tail structure loop 1 separate init — DIFFER, 2384 v 2386 bytes, 13 instruction hunks, first unequal byte at 0x00873EEE.

## Shared-header trials

| Trial | VC7.1 result |
| --- | --- |
| `tail_shared_01_pcmouselook` | 2380/2386 bytes, 16 strict hunks |
| `tail_shared_02_mouse_donor_locals` | 2380/2386 bytes, 16 strict hunks |
| `tail_shared_03_mouse_zfirst_dot` | 2380/2386 bytes, 16 strict hunks |
| `tail_shared_04_mouse_ordered_dot` | 2384/2386 bytes, 15 strict hunks |

Selected `tail_shared_04_mouse_ordered_dot` as the shared-header base. The shared Z-Y-X dot product improves the grade to 2384 of 2386 bytes. The first difference is in input sensitivity stack placement, followed by current DollyLength and rotation scheduling.

The merged include-tree regrade is `tail_shared_merged_receipt`. The standalone fixture builds and runs on both compilers against that tree.

The selected result is PARKED (2384 object bytes, 2386 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
