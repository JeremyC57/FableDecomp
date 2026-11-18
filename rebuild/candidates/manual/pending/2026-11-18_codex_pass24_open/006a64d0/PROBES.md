# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail emitted leaf diagnostic — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail measured focal position named bound — DIFFER, 895 v 865 bytes, 56 instruction hunks, first unequal byte at 0x006A64D2.
- tail measured name copy from counts — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail measured name early focal init — COMPILE_FAIL.
- tail measured named generator — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail measured status string order — DIFFER, 827 v 865 bytes, 48 instruction hunks, first unequal byte at 0x006A64D2.
- tail pdb rekey — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure calculation focal Z 1 separate result — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure calculation focal Z 2 separate result — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure calculation text 3 separate result — COMPILE_FAIL.
- tail structure calculation text 4 separate result — COMPILE_FAIL.
- tail structure calculation text 5 separate result — COMPILE_FAIL.
- tail structure calculation text 6 separate result — COMPILE_FAIL.
- tail structure guard 1 named — DIFFER, 823 v 865 bytes, 53 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure guard 2 named — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure guard 3 named — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.
- tail structure guard 4 named — DIFFER, 839 v 865 bytes, 54 instruction hunks, first unequal byte at 0x006A64D2.

## Shared-header trials

| Trial | VC7.1 result |
| --- | --- |
| `tail_shared_01_thing_debugdraw` | COMPILE_FAIL |
| `tail_shared_02_thing_debug_symbols` | COMPILE_FAIL |
| `tail_shared_03_thing_debug_primitive_refs` | 855/865 bytes, 45 strict hunks |
| `tail_shared_04_thing_debug_focal_lifetime` | 873/865 bytes, 57 strict hunks |
| `tail_shared_05_thing_primitive_full_expression` | 865/865 bytes, 23 strict hunks |
| `tail_shared_06_thing_colour_expression` | 865/865 bytes, 23 strict hunks |

Selected `tail_shared_06_thing_colour_expression` as the shared-header base. The full-expression render text lifetime restores the 865-byte extent. The frame size, renderer call dispatch temporary, and choice of registers in component lookup and string concatenation still differ.

The merged include-tree regrade is `tail_shared_merged_receipt`. The standalone fixture builds and runs on both compilers against that tree.

The selected result is PARKED (865 object bytes, 865 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
