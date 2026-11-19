# fixed_b2 (verifier, p989 b2): changes vs b2 (README / payload.json / oracle.tsv must be regenerated from this tree)
- hdr/engine/CMatrix4x4.h: the AlignedPreMultiplyTransposed respelling is now nested under FABLE_RETAIL_CMATRIX4X4_ALIGNEDSETMULTIPLY_EGOD, with today's text in the #else. It is byte-neutral (1320 and bd2d90 RELOCATION_MATCH with the old text). All 113 landed users preprocess the same as the repo.
- hdr/engine/CMatrix4x4AlignedSSE.h: unchanged from b2.
- src/..._00ae29d0.cpp: param `sim` -> `dest`. The Ego_d 7-arg record is complete (debug length 280, file offset 0x7FCD2BC); the Locals note is rewritten and no longer says Inferred.
- src/..._00a9e1e0.cpp: `// Fitted:` -> `// Inferred:` (EXTRA_RULES 2026-11-19), with the measured alternatives; the Shape comment now describes the Transposed call.
- src/..._00adefc0.cpp: the Name line cites FableWin + Ego_d (no Ego_r record found in ghidra_out).
- test/..._00ae29d0_test.cpp + mutants: model param `dest`; the stubs record the anim / bone_morphs / skin-input pointers.
- test/..._00ae1320_test.cpp: sim.Size != InitialSimulation.Size; strides and the anim / bone_morphs pointers are checked.
- NAME_FIXES.tsv: b2's two lines plus 5 stale caller bindings that the new rows' callees will conflict with at landing.
