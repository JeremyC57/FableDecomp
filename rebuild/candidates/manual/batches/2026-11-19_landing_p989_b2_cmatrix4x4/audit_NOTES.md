# p989 b2 audit notes (verifier)
- oracle bytes: all 5 equal Fable.exe; a9e1/bd2d equal catalog oracle.
- grades with b2/hdr overlay: all 5 RELOCATION_MATCH (own harness aq.py, own work dir).
- expanders (landed text, overlay): a5cb90(/Zi) a9d480 a9d580 a9d750 aa0fa0 aa2110 a9e1e0 bd2d90 all RELOCATION_MATCH.
- preprocess compare (g++ -E, overlay vs repo) on all 8 landed srcs + tests including CMatrix4x4 retail: only bd2d90 differs = PMT inline respelled (outside the EGOD opt-in).
- hdrcheck2: all same (vacuous for nested branches). pin_asserts: 0 to pin. #error pins fire (g++).
- Ego_d: SetupSimulation 7-arg record IS complete (len 280): dest, anim, anim_manager, bone_morphs, mesh, alloc, context; no_bones, bone_transforms; {c_bone}. Row spells `sim` -> FAIL; `dest` variant RELOCATION_MATCH.
- a9e1e0 alternatives: plain N DIFFER, t1n1 RM, both-plain DIFFER. `// Fitted:` marker should be `// Inferred:` (EXTRA_RULES 2026-11-19).
- purity: all 5 rows "genuine"; landed a9e1e0 "retail_asm" (spells AlignedSetMultiply); purity misses helpers reached via Transposed/PMT inlines.
- reloc_bindings: 0x00AE1320 / 0x00AE29D0 bound with non-PDB manglings from callers 00ae2d20 / 00b82100 (QAEXPAU / QAE_NPAV) -> NAME_FIXES missing.
- 0x013D2880: reloc_bindings has BOTH GEnableSIMD (00bf7bc0) and g_SSEAvailable (00a9d480).

## Final (fixed_b2, today's tree; the two CMatrix4x4 headers are unchanged in the repo since 17:56)
- fixed_b2/hdr/engine/CMatrix4x4.h: PMT respelling nested under ..._ALIGNEDSETMULTIPLY_EGOD, old text in #else. It is byte-neutral: 1320 and bd2d90 RM with the old PMT text (ov_sseonly).
- ppcmp fixed vs repo: all 113 landed srcs/tests mentioning CMatrix4x4 are the same. fixed vs b2 on 5 rows + 5 tests: the same. pin_asserts 0.
- grade_final.txt: 5 fixed rows + 8 landed expanders RELOCATION_MATCH.
- tests_fixed.txt: all PASS; author mutants all FAIL; my mutants all FAIL except 1320 simd_inverted (equivalent mutant).
- portable: tests/portable/test_CMatrix4x4AlignedSSE retail+FABLE_PORTABLE PASS (VC7.1); adefc0 test with FABLE_PORTABLE PASS. g++ -m32 is not available (no 32-bit headers).
- payload.json matches b2 src/test exactly; the re-port targets are the landed paths. fixed_b2 needs a payload regeneration.
- callers: 00ae2d20 now spells ClingToMesh right (its binding line is stale); 00b82100 local class spells SetupSimulation/SimulationStep with pointers, non-const (QAE_NPAV..). Naming item for A.
