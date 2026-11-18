# Compiler probes

- package baseline — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail deliverable — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail pdb rekey — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure guard 1 named — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure guard 2 named — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure guard 3 named — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure guard 4 named — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure local dataSize assign — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure local entryExists assign — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail structure local index assign — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail target compare bytes named — DIFFER, 927 v 920 bytes, 51 instruction hunks, first unequal byte at 0x00A3A426.
- tail target compare length named — DIFFER, 920 v 920 bytes, 33 instruction hunks, first unequal byte at 0x00A3A47D.
- tail target compare length reversed — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.
- tail target compare mtime named — DIFFER, 918 v 920 bytes, 34 instruction hunks, first unequal byte at 0x00A3A47D.


The native 0x00A3A59F call binds to CDateAndTime::operator!= at 0x009E9B20, as confirmed by the landed callee's negated comparison and both PDB name sets. Its zero-result branch at 0x00A3A5A6 goes to final teardown at 0x00A3A76F. Existing equal-size entries therefore exit when timestamps match and compare bytes when timestamps differ. The selected source now uses this comparison, and its copied-body fixture passes both compilers. The corrected 920-byte body still differs in 33 strict instruction hunks.

## Shared-header trials

| Trial | VC7.1 result |
| --- | --- |
| `tail_shared_01_wad_model` | PREPROCESS_FAIL |
| `tail_shared_02_wad_model_complete` | COMPILE_FAIL |
| `tail_shared_03_wad_typed_set` | COMPILE_FAIL |
| `tail_shared_04_wad_complete_bank_base` | 945/920 bytes, 56 strict hunks |
| `tail_shared_05_wad_counted_base_ctor` | 945/920 bytes, 56 strict hunks |
| `tail_shared_06_wad_charstring_ctor_scope` | COMPILE_FAIL |
| `tail_shared_07_wad_single_teardown` | 938/920 bytes, 48 strict hunks |
| `tail_shared_08_wad_explicit_empty_string` | 942/920 bytes, 41 strict hunks |
| `tail_shared_09_wad_donor_allocation_order` | 940/920 bytes, 52 strict hunks |
| `tail_shared_10_wad_original_counted_type` | 942/920 bytes, 41 strict hunks |

Selected `tail_shared_10_wad_original_counted_type` as the shared-header base. The typed bank and stream hierarchy emit their original members. Counted allocation, creation and destruction order, and common exit scheduling still differ. The native inequality call and equal-timestamp skip are preserved.

The merged include-tree regrade is `tail_shared_merged_receipt`. The standalone fixture builds and runs on both compilers against that tree.

The selected result is PARKED (942 object bytes, 920 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
