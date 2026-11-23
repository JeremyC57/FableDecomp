# Landing nf300 (Account A, 2026-11-23): T1 signature, NF297's continuation (38 rows, 1 header opt-in, 28 table lines)

Hand-back: `pending/2026-11-23_nf300_handback/`; helpers under `naming_batch/agent_tools/nf300/`. 71 of 292 entries reached: 25 done,
1 done-partial, 44 skipped-port, 1 rejected (entry 47, the VC10 Dinkumware list iterator: `done-equivalent` under the NF301 ruling).

- Binder signatures onto the PDB spellings in each row's local model (reference for pointer, const, access, enum parameter types, calling
  convention), zero line delta except five added lines (an `engine/EHeroAbility.h` / `EInputButtonEventType` include in 0x007BE0F9 /
  0x008EDDD0, the PASS26 define in 0x008EDDD0, and the new opt-in define in 0x00582944 / 0x0061A6A0). 0x00AE3A30's local STLport
  `allocator` is a `class` again, so the self defines the PDB V-allocator spelling (0x00AC3AF0 now resolves it); its Leaf is full mangled.
- Header: `engine/NHeroInformationScreens_CManager.h` gains the add-only `FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER_REFERENCE`
  (`RegisterScreen(const CBase&)`, both PDB name sets, the self 0x0066EB11 defines it) inside the existing `_REGISTER` block; equiv_check over
  57,146 rows: 0 differ; the planted control flags the 7 `_REGISTER` definers and none of the 3 other includers (`evidence/equiv_*`).
- Proof: objsec ALL_EQ (`.drectve` included, DEF sets equal, tables unchanged), links HEAD v new with compiled controls: no new duplicate,
  nothing HEAD resolved left unresolved. Staleness: all 38 orig/ copies and the header equal HEAD at landing (after nf299 / nf301).
- Table: 17 replaced, 11 deleted (binder moved onto an existing PDB line); entry 43 table-only (witness 0x00AE2D20 already bound ClingToMesh).
  0x006BA030 (now an NF300 row) still binds `?WriteData@CDataOutputStream@@QAEXPBXK@Z`, the line NF299's landing rewitnessed onto it.
- Queued: 23 T3 ports (entry 2 as a unit: binders onto `engine/CEngineSubPrimitiveBase.h` + the owner's pick-any scaffolding; entry 20's
  virtual destructor grades DIFFER), the CPersistTraits family (15 entries of 48-71) as one FAMILY UNIT; NAME_FIXES_QUEUE: 8 owner-name lines
  incl. the 0x00A03BF0 / 0x00A03C20 self-name swap (Pressed / Held). The six re-scope entries (5-7, 26, 31, 41) go to the continuation.
- Rulings (standing brief): Q1 an existing shared enum header included above the local model, the literal replaced by its enumerator, lands
  when objsec is ALL_EQ and the preprocessed diff shows only the enum / parameter / literal; Q2 a local-model re-scope of a nested or
  namespaced PDB type is in place (T1) when the TU emits no table / RTTI for it, none of the TU's own definitions change name and objsec is
  ALL_EQ, otherwise a port; Q4 the Dinkumware tracker equivalence stays on the ROADMAP; D1 also covers clang failures on MSVC-only syntax
  in an unchanged landed source (the `unsigned int(...)` functional cast), with `cl /Zs` as the precheck.

## Recheck
`logs/recheck.log` (landed tree): 38 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
