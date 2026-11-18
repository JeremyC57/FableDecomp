# Compiler probes

- package baseline — LEAF_REVIEW_REQUIRED.
- tail deliverable — DIFFER, 1358 v 1471 bytes, 90 instruction hunks, first unequal byte at 0x0068DD02.
- tail emitted leaf diagnostic — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail pdb rekey — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure guard 1 named — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure guard 2 named — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure guard 3 named — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure guard 4 named — DIFFER, 1358 v 1471 bytes, 90 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure local buttons assign — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure local enemy assign — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure local hero assign — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure local item assign — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.
- tail structure local thing assign — DIFFER, 1469 v 1471 bytes, 89 instruction hunks, first unequal byte at 0x0068DD02.

The previous local-model grade was 1469/1471 under `tail_pdb_rekey`. The selected shared-header grade `tail_use_environment_shared_def` emits 1617/1471 with 84 strict instruction hunks. The shared input, event and component interfaces preserve the native SpellMode and UseInfoDisplay gates. The compiler still differs in component lookups, event lifetimes and the protected input-body exits. The standalone fixture executes the selected method body with controlled service stubs and passes both compilers.

The selected result is PARKED (1617 object bytes, 1471 retail bytes). The final test passed with VC7.1 and GNU C++03. GRADE_HISTORY.tsv includes all recorded compiler runs, including baseline and verification repeats.
