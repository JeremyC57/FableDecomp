# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail emitted leaf diagnostic — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail pdb rekey — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure guard 1 named — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure guard 2 named — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure guard 3 named — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure guard 4 named — DIFFER, 555 v 529 bytes, 12 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure local duration assign — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure local found assign — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure local hi assign — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure local index assign — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.
- tail structure local lo assign — DIFFER, 535 v 529 bytes, 11 instruction hunks, first unequal byte at 0x006E38C5.

## Shared-header trials

| Trial | VC7.1 result |
| --- | --- |
| `tail_shared_01_typed_speech` | COMPILE_FAIL |
| `tail_shared_02_emote_primitive_layout` | 584/529 bytes, 12 strict hunks |
| `tail_shared_03_speech_donor_names` | 584/529 bytes, 12 strict hunks |
| `tail_shared_04_speech_inline_release` | 546/529 bytes, 11 strict hunks |
| `tail_shared_05_speech_donor_return` | 547/529 bytes, 27 strict hunks |
| `tail_shared_06_speech_size_inliner` | 457/529 bytes, 37 strict hunks |

Selected `tail_shared_04_speech_inline_release` as the shared-header base. Inline intrusive pointer release removes the excess destructor relocation. Group lookup is still scheduled after the emote arm and the pending-return paths retain different instruction sequences.

The merged include-tree regrade is `tail_shared_merged_receipt`. The standalone fixture builds and runs on both compilers against that tree.

The selected result is PARKED (546 object bytes, 529 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
