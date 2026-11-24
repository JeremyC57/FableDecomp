# NF300 notes (2026-11-23): T1 signature entries 1-71 (NF297's reopened 24, then NF297 81..129)

Base: the repo working tree as of today. I never wrote to the repo and ran no git. Every orig/ file (38 rows, 1 header) equals the repo at
hand-back (cmp, rerun after the NF298 / NF299 landings wrote to rebuild/src around 03:30; none of my rows was touched). STATUS / APPLIED use my ENTRIES numbering (1-292): lines 1-24 are NF297's 50, 51, 49, 52-57, 63-66, 70, 71, 79, 80,
59, 61, 62, 68, 74-76; lines 25.. are NF297's 81 on.

## Result
- **71 entries reached (1-71):** 25 done, 1 done-partial (69), 44 skipped-port (T3 or a decision), 1 rejected (47). 72-292 not started (`open`).
- **38 rows changed** (rows.txt == the 38 locks I still hold; every control lock released). Zero line delta except five added lines:
  0x007BE0F9 (+1 `#include "engine/EHeroAbility.h"`), 0x008EDDD0 (+2: `#define FABLE_RETAIL_COMBAT_INPUT_PASS26` + the
  EInputButtonEventType include), 0x00582944 / 0x0061A6A0 (+1 each: the new header opt-in define).
- **objsec HEAD v new:** ALL_EQ for all 38 (evidence/proof_g1..g4; proof_g4 also re-proves 0x00AE3A30 after its Leaf edit): every non-debug section's bytes and relocations equal under the
  renames, `.drectve` included, DEF sets equal; every emitted table unchanged.
- **Grades:** every HEAD and new object RELOCATION_MATCH / MATCH (evidence/grades.log; the log also holds the three held variants).
- **vland `--recheck-landed`:** 38 / 38 RELOCATION_MATCH + test PASS, 0 RELOC_CONFLICT (recheck_final.log = rc1 + rc2, run with
  FABLE_HDR_OVERLAY = a full copy of today's include tree plus my header, + rc3 without overlay). RELOC_NAME warnings touching a moved spelling:
  `SetAsKeyboardKeyHeld -> 0x00A03C20 labelled SetAsKeyboardKeyPressed` (the swapped self names, NAME_FIXES) and a pre-existing
  `??2CLandscapeBackgroundPatch` label on 0x00BF6F80. The others are pre-existing names of other callees. SHARED_HEADER_AVAILABLE warnings
  are pre-existing local models (unchanged by these edits).
- **Links** (evidence/link_g1..g4; /FORCE; HEAD set v new set, both with compiled controls): no new duplicate (the duplicates listed are
  identical on both sides: CalculateSegmentation in 0x00A6C850 / 0x00A6DA40, GetSubDef<CCreatureDef> in the inventory screens), nothing
  HEAD resolved is left unresolved; each moved spelling resolves against its self wherever the self defines the PDB spelling. Left
  unresolved on both sides where the self is a stand-in: SetAsKeyboardKeyHeld (0x00A03C20), Write UAE J (0x00996CC0 defines QAE),
  CPersistTraits<short> (0x004CBC70), CPersistTraits<CDefString>::TransferIn (0x007E3820), Reset (0x00A6E9A0). All in NAME_FIXES.
- **Hand-back counts:** RENAMES 1 self rename (0x00AE3A30, an address outside ENTRIES, entry 44) + 39 caller lines; TABLE_CHANGES 28 lines
  (one a stale-witness replace, entry 43); NAME_FIXES 9 lines; REJECTED 1.

## The header opt-in (entry 30)
- engine/NHeroInformationScreens_CManager.h gains FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER_REFERENCE, add-only, inside the
  existing _REGISTER block: `#ifdef` (comment + `void RegisterScreen(const CBase& screen);`) / `#else` / the unchanged pointer line /
  `#endif`. Declaration only. Both PDB name sets give ?RegisterScreen@CManager@NHeroInformationScreens@@QAEXABVCBase@2@@Z (Ego_r 0x006A104A,
  FableWin 0x02114C57); the self 0x0066EB11 (control) defines it. The header has no FABLE_STATIC_ASSERT / __LINE__ / ASSERT_ARM users.
- Users: 0x00582944 and 0x0061A6A0 (one added `#define` line each, `RegisterScreen(*this)`). The five other rows that define _REGISTER
  (0x00619FD0, 0x0061A000, 0x0061A010, 0x00624DA0, 0x00624DB0) do not call it and are untouched.
- **equiv_check:** old = today's include tree, new = + the opt-in, all 57,146 landed rows: 0 rows differ (evidence/equiv_hdr.log / .tsv).
- **Planted control:** a real token on the `#else` pointer line (`int NF300_PLANTED_CONTROL;`), restricted to the seven _REGISTER definers
  plus three includers that do not define it (0x00621C80, 0x00621720, 0x00622A00): it flags all seven and none of the three
  (evidence/equiv_planted_control.tsv, planted_addrs.txt).
- The existing opt-in's comment says the pointer form is the PDB spelling; it is not (both PDBs: reference). I left that comment alone
  (no in-place edits of shared lines) and said so in the new comment.

## Method
- Triage per entry: exe census -> tools/show.py (now handles operator names and has a SHOW_WIT=1 witness-only mode) -> open each binder.
  sigdiff's STANDIN flag was wrong twice (entry 16: CThingBuilding; entry 17: CLandscapeBackgroundTreeNode are real classes). For every
  done entry the full census was read (show.py without SHOW_WIT: that found the untabled binders 0x00514050, 0x00AC3E00, 0x00816000);
  for skipped entries the wrong-spelling witness binders were opened (entries 27 and 50 have 147 / 30 census rows: witnesses only).
  Each status line cites the binder lines read.
- Every self of a done entry was compiled as a control and its DEF read (bind / syms). That found: 0x00A03C20 / 0x00A03BF0 swapped self names, the
  non-virtual Write self 0x00996CC0, the stand-in Reset self 0x00A6E9A0, and a stale table witness (0x00AE2D20 already binds the PDB
  ClingToMesh: entry 43 is table-only).
- Pick-any copies (entries 1 / 2): tools/pickany.py (NF298's) compares the object's COMDAT with Fable.exe at the owner address
  (relocation fields zeroed; each relocation's retail target checked against the table) (evidence/pickany_e1.txt).
- Edit classes used: pointer -> reference (`*arg` / `&` dropped at the call), pointer -> pointer-to-const, `const` / access labels on
  the declaration's own line, `__fastcall` dropped on a no-argument const member, `unsigned long` -> `long` fed unsigned values,
  a const return where the receiver already held a const pointer (0x00A6DA40) or was retyped to one (0x00A6C850: CCountedPointer<const T>,
  which also moves its Reset binding to the PDB spelling), a local STLport `allocator` struct -> class (EXTRA_RULES class key),
  an existing shared enum header included above the local class + the literal replaced by its enumerator (entries 18 and 21).

## Decisions / questions
0. **Entry 57 note:** 0x004C84B0 / 0x004CA390 already define CPersistTraits<short>::TransferBinaryIn strongly under the PDB spelling
   (pre-existing same-TU copies); the moved binder 0x004CC3F0 can resolve to one of them in a full link. Pre-existing debt, flagged.
1. **Q1 (entries 18 / 21):** NF297 classed "the row declares no such enum" T3 (its 22, 29, 32, 42, 46). Where a shared header for the
   PDB enum exists (engine/EHeroAbility.h, engine/EInputButtonEventType.h) I included it just above the row's local class and replaced
   the literal with the enumerator of the same value (HERO_ABILITY_FORCE_PUSH = 1 in the force-push ball's OnCollision;
   INPUT_BUTTON_TYPE_HELD = 2, FableWin TPI 0x1a894). 0x008EDDD0 also needs `#define FABLE_RETAIL_COMBAT_INPUT_PASS26` because the enum
   sits under that macro. objsec ALL_EQ for both. evidence/pp_diff_enum_rows.txt: gcc -E of HEAD v new differs only by the enum
   definition, the parameter type and the literal (0x008EDDD0: the macro reaches nothing else; 0x007BE0F9: EHeroAbility is now defined
   earlier in the TU than its later CTCSpecialAbilities.h include, a symbol-order change the bytes do not show). Accept, or hold them
   as T3 for consistency?
2. **Q2 (entries 5-7, 26, 31, 41):** the binder models the PDB's nested / namespaced parameter type at global scope (or forward-declares
   ::CIEngine inside a namespace). Nesting / moving the declaration in the local model would probably leave the bytes unchanged. Is a
   local-model re-scope T1 (in place) or T3 (port)? I classed them skipped-port pending the ruling.
3. **Q3 (entry 2):** the binders' CEngineSubPrimitiveParticleGroup copy equals the owner 0x00BBBDB0, but their `__declspec(novtable)`
   base model also emits a 28-byte CEngineSubPrimitiveBase ctor copy without the vtable store; under the PDB name it would join the equal
   copies of 0x00BBC130 / 0x00BBBDB0 in one pick-any group, and the ParticleGroup copy would meet the owner's strong definition. I held
   both (variants in work/held/). Port the two binders onto engine/CEngineSubPrimitiveBase.h and scaffold the owner, as one unit?
4. **Q4 (entry 47):** rejected: the tracker's pdb_spelling for STL-typed members comes from the VC10 donor PDBs (Dinkumware
   `_List_iterator<_List_val<...> >`); retail is STLport, and the binder already spells the STLport iterator. Should naming_status.py
   skip (or map) STL-instance parameter types so such entries stop appearing?
5. **Entry 20:** `virtual ~CFileDataInputStream()` in 0x009B08C0 grades DIFFER (506v506): not in place. Hand to a port.
6. **CPersistTraits family (entries 48-71):** 9 of the 24 (56, 57, 61, 63, 65, 66, 68, 71 done; 69 partial) were declaration fixes in
   local template models; the rest
   bind the CMemoryDataInputStreamReader / CDataOutputStreamWriter stand-ins or define strong `template <>` specializations under the
   wrong spelling (NF281 D2): a family unit for the T3 queue.

## Rule slips
- None that ran a compile without a lock. Clang prechecks ran in the foreground under `timeout 120` (pre_list / pre_tests); VC7.1 grades,
  `cl /Zs`, links and vland ran in the background with `timeout 900` on their own launch line; equiv_check (gcc cpp, not a compile) ran
  in the background under `timeout 3000` / `900`.
- D1 `cl /Zs` (evidence/pre_zs.log) covered: the landed tests of 0x00445E90, 0x00A86CD0, 0x00522160, 0x0061A6A0, 0x0082B7E0,
  0x0066EB11, 0x00AE3920, 0x006B91A0, 0x009E81E0 (clang failed only in VC7.1 headers, or on the MSVC functional cast `unsigned int(...)`
  in the test's own text), and the HEAD sources of controls 0x00445E90 / 0x0066EB11 (the same functional cast; strictly outside D1's
  wording, which names only toolchain headers: the VC7.1 syntax check is the evidence I used).
- Reading list: the standing brief, my prefix, GOTCHAS (all), EXTRA_RULES (all), NF297's NOTES / LESSONS / STATUS / NAME_FIXES. Not read
  this pass: the older landing READMEs (nf281-285) and docs/COORDINATION.md / ASM_RULES.md (no asm or coordination question arose).
- No sleeps, no `rm` (clean.py only), no retail or donor bytes in hand-back files (the vland oracle files stay in work/; include-tree
  copies deleted).
