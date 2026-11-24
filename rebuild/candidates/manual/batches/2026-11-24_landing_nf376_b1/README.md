# Landing nf376_b1 (2026-11-24): the first option S unit, 0x00B80AD0 / 0x00B80F90 / 0x00B80FE0 graded from one translation unit

Owner question 2, ruled by the advisor (owner delegated): option S for rows whose blocker is a second strong definition, or a needed
same-TU body, of a function with its own retail address, when both PDBs put the functions in one .obj and the in-TU copy is EQ at its
own address under one flag set. Rows whose residue is only which pick-any copy the linker keeps stay on B's object-order build mode.
Pilot by agent NF376 (`pending/2026-11-24_nf376_handback/`: NOTES.md, STATUS.tsv, tool_diffs/, the measurement tools).

The unit (all three in Engine_Simulation_Manager.obj in Ego_r.pdb and FableWin.pdb, default flags):
- 0x00B80AD0 CEngineClothSimulationMeshBase::ConvertVectorToLocal: the OWNER. Its .cpp holds all three bodies in retail address order, one
  each, on the shared headers CEngineClothSimulationMeshBase.h / CMatrix3x4.h. Its self moves to the PDB spelling IBE (was QBE).
- 0x00B80F90 GetGravityVector and 0x00B80FE0 GetWindVector: THIN rows (form T: their own `// Leaf:`, test and catalog entry, and
  `#include "../src/compiled/00/b8/<owner>.cpp"`, the EH-funclet shape). Each previously carried its own strong ConvertVectorToLocal copy
  (one IBE, one QBE). FE0 now calls 0x00B80B70 by its PDB name GetWindVectorNoise and reads g_pEngineSimulationManager.
Grades (landed tree): 0x00B80AD0 MATCH, 0x00B80F90 and 0x00B80FE0 RELOCATION_MATCH, tests PASS, each at its own leaf (checked by hand:
land_scripted skips the symbol check for unit members). funclet_includes_check sees the two new includers: 1364 checked, 0 broken.
NF376 also measured: pk_all / pickany_vs_retail every addressed copy EQ, 0 reach flips; /FORCE link of the owner once with 13 census
objects: duplicates 5 -> 4 (the QBE copy gone), unresolved set a subset of HEAD's; form C (two catalog entries on one Source) mis-grades
the absorbed addresses (one `// Leaf:` per file), so form T is the S shape. Table: delete the QBE ConvertVectorToLocal line, replace
GetWindAt -> GetWindVectorNoise at 0x00B80B70, delete g_pSimulationEnvironment (0x01436E68 keeps its one landed name); the coordinator
checked no other landed source binds the two old names (0x00B80B70 defines the PDB name; GetWindAt is only in its comment).

Rulings (advisor decision, owner delegated):
- Q1: the shared-header `CMatrix3x4::GetTranslation` form lands; its unreferenced pick-any copy is the NF371 R2 shape.
- `ScaleInto` (a descriptive TU-local helper) stays with an `// Inferred:` note: FableWin's debug GetWindVector calls GetTranslation,
  GetWindVectorNoise, C3DVector::operator*(float) and ConvertVectorToLocal, but twelve helper-free spellings (four operator* header
  forms x three statement shapes, `pending/2026-11-24_nf376_handback/spelling_grid/`) all grade 0x00B80FE0 DIFFER(112v112 / 122v112).
- Q2: form T is the S catalog shape. Tool diffs before a second S unit: (a) land_scripted strict member symbols with (d)
  LANDVERIFY_STRICT_LEAF first, then (c) owner_source / includers_of / recheck --with-includers wired into landing, (e) s_unit_check.py
  as a baseline; (b) the build_candidates cache diff waits for a Windows build test.
- 0x0055FB80 stays held: its unit needs Account B's 0x00560BB0 (queued as nf376).
T3: nf283 and nf373 lines on 0x00B80AD0 closed (`T3_CLOSED.tsv`). Queued: FE0's test retext (names only), the 0x0055FB80 joint unit.
Rule text (GOTCHAS, BRIEF_NF_BATCH): one object per TU in link and census sets; edits go to the owner.
