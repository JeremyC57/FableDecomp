# Compiler probes

- tail deliverable — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail emitted leaf diagnostic — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail pdb rekey — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure guard 1 named — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure guard 2 named — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure guard 3 named — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure guard 4 named — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure local allDead assign — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure local effect assign — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure local thing assign — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure local trail assign — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure loop 1 advance in body — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure loop 1 separate init — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure loop 2 advance in body — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.
- tail structure loop 2 separate init — DIFFER, 1065 v 1067 bytes, 24 instruction hunks, first unequal byte at 0x00766F5F.

The previous local-model grade was 1065/1067 under `tail_pdb_rekey`. The selected shared-header grade `tail_aug_shared_outline` emits 1161/1067 with 19 strict instruction hunks. The shared owner and STLport containers retain the native predicate calls and component names. Vector allocation, period scheduling, and iterator code generation still differ. The standalone fixture executes the selected method body with controlled service stubs and passes both compilers.

The selected result is PARKED (1161 object bytes, 1067 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
