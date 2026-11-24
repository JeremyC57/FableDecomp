# NF390 hand-back (Account A naming agent, 2026-11-24)

Three jobs: (1) make NF387 b4 landable under ruling R2, (2) the remaining CArray binders of T3 nf383 0x009B7870 (+ T3 nf374
0x00762580), (3) ruling R3 forwarder reads (T3 nf383 0x0043E1C0). No repo writes. Work dir: $S/w22/agentNF390.

## Job 1: batches/b1 = NF387 b4 (009b7870 + 00adc0d0), ACCEPT under R2
- (a) Census by VC7.1 compile (tools/census390.py: the row's HEAD source through verify_and_land.vc71 + cl with its own flags, then
  every external DEF / UND of the watched spellings with the COMDAT selection read from the section aux record):
  evidence/census_b4_86.tsv: all 86 rows of NF387's clang census that clang could not compile compiled under VC7.1 (86 ok, 0 fail).
  None references (UND) ??4?$CArray@K@@QAEAAV0@ABV0@@Z; 3 define it (009b7650 009b8770 009b8c00), selection Any.
  evidence/census_b4_cldef.tsv: the 9 rows clang found naming it, recompiled with VC7.1: 009b7870 (HEAD) is the only UND;
  the other 8 define it, selection Any.
- Leaf reach of those 11 definers (advisor check): pk_all on each object (batches/b1/evidence/definers/): the copy is leafref n,
  reach n, address source b only (the table line this batch deletes) -- no definer's leaf calls its own copy, so no definer is a
  witness of the line. Today each of the 11 reads that copy NEQ against 0x0045BC09; after the delete it reads NOADDR.
- (b) No landed row at 0x009B5A60 (case-insensitive: build_candidates.ps1, self_symbols.tsv, auto-re-candidates.tsv, reloc_bindings.tsv targets: 0 each); every landed definer emits the forwarder pick-any (above). No
  NoDuplicates v Any duplicate; /FORCE link of the 2 rows + callee owners 0045bc09 0099efb0 0045c79e + 00adc120 + the 11 Any
  definers: 0 -> 0 duplicates; unresolved ??4SArray0C 2 -> 1; nothing HEAD resolved left unresolved (evidence/link_b1.txt).
- (c) 00adc0d0 cites Ego_r CVertexGroupBlock (ghidra_out/struct_layouts_egor.tsv: 32 B, +0 CArray<unsigned long> Groups,
  +16 map<unsigned long,long,...> GroupToIndexMap). Ego_r's containers are 16 B (its later STL); retail STLport's are 12, so the
  retail element is 24 B with GroupToIndexMap at +0x0C (retail 0x00ADC0FA `lea eax,[esi+0Ch]`). Members renamed to the PDB names
  (m_values -> Groups, m_tree -> GroupToIndexMap; no symbol change), stale Callees comment fixed. The element type and the self keep
  their landed names (Copy_SArrayTreePair18_00adc0d0 = STLport __copy over CVertexGroupBlock; respell queued, QUEUE_PROPOSALS q1).
  009b7870: comment fix only (CDefClassInfo is 24 B in Ego_r, 20 in retail).
- R2 allowance, measured: objsec by COMDAT symbol under the rename (evidence/objsec/*.comdat.txt): every HEAD section paired EQ;
  one NEW_ONLY section ??4?$CArray@K@@QAEAAV0@ABV0@@Z (19 B, selection Any) = the generated CArray<unsigned long>::operator= of the
  shared engine/CArray.h; exact PDB name (ghidra_out/fablewin_pdb_names.tsv 02945ea0, FableWin edit_gui_bone_dialog.obj);
  pk_all EQ at retail 0x009B5A60 -- through --addrs only (evidence/pk/addrs_forwarder.tsv, TA_*.pkall.tsv): no table line can bind
  it (nothing calls 0x009B5A60, so no relocation witness); plain pk_all on the shipped table reads NOADDR (T_*). Ruling R4 below.
  009b7870's .drectve gains /DEFAULTLIB:"libcp" (stlport/fable_containers.h, NF318 Q1).
- Gates: vland HEAD + new at one path (work/wd_b1{h,n}): RELOCATION_MATCH PASS x2 each side; pk_all + pickany_vs_retail HEAD v new:
  only the forwarder copy differs (NEQ v the deleted line; NOADDR with it applied, pvr empty: evidence/pk/T_*); reach flips 0;
  apply_table --dry TABLE_CHANGES + HAND_TABLE_CHANGES together: delete 1, rewitness 1, 0 misses, multi-target 1 -> 1, none new.
- Table: TABLE_CHANGES delete ??4?$CArray@K@@ -> 0045bc09 (witness 009b7870, a run row); HAND_TABLE_CHANGES rewitness ??4SArray0C ->
  00adc120 (not a run row; VC7.1 census_c3 + xr: its relocation resolves to 0x0045BC09; only 00adc0d0 / 00adc120 name SArray0C).

## Job 3: ruling R3 reads (DECIDED.tsv)
- tools/r3read.py (own PE reader, independent of NF387's): 19 bytes at each forwarder, E8 rel32 decoded, bytes compared with the
  pilot 0x009B5A60 with the 4 rel32 bytes masked. Pilot bytes `8b 44 24 04 56 50 8b f1 e8 <rel32> 8b c6 5e c2 04 00`.
  Controls 0x009B5A60 / 0x00709A30 / 0x00709BB0 / 0x0075F6A0 reproduce (evidence/r3_forwarders_full.tsv also has prev byte = cc and
  16-alignment for every forwarder).
- All 10 candidates decide vector: 12 forwarders, each rel32 lands exactly on its self and each equals the pilot apart from the
  relocation. A whole-image scan (evidence/r3_scan.txt) finds no further forwarder to any of the 14 selves; 0043e1c0 006f15e0
  009f6a70 have none, 00515ad0's 0x0051A1B0 lies inside a 129 B catalog function (stays unknown).
- Self spellings (evidence/r3_self_symbols.txt + the row // Leaf lines): the 10 decided selves are all std::vector members
  (??0 / ??4 ?$vector@...), so no respell. Disagreement with "all 14 vector-spelled": 0043e1c0 is
  ?ReleaseData@?$CCountedPointer@VCQuestInfoElement@... and 006f15e0 is ??$swap@VCProng@CDefendingCombatantInfo@@@std@@...; neither
  is a vector member (both stay unknown, no forwarder). 009f6a70 (~vector<CInputEvent>) and 00515ad0 (vector<CMapInfo>(n)) are.
- Table lines that bind CArray / stand-in spellings at the 10 decided vector addresses (evidence/r3_table_nonvector_lines.tsv,
  15 lines): the same shape as b1's delete (each binder's model calls the CArray member where retail calls the vector body).
  Queued per address (QUEUE_PROPOSALS q2); not changed here.

## Process notes
- One read-only `git log -1` ran early by mistake (no write); no other git. No repo writes (apply_table only --dry on the repo;
  the applied-table pk runs used a scratch root work/troot with its own reloc_bindings.tsv copy).
- Locks: 009b7870 / 00adc0d0 were NF387's (handed back, not released); taken over as NF390 on the coordinator's assignment.

## Job 2: the remaining CArray binders (budget-limited; ~140 of ~150 compiles used overall)
Common finding: the binder ports onto the shared headers all reach RELOCATION_MATCH in one probe, and the binder relocations then
bind the vector members at 0x005BBDF1 / 0x0045BC09 / 0x006F0810. What holds them is the set of extra pick-any COMDATs VC7.1 emits
from the shared headers' inline special members (and, for two rows, a CArray table line witnessed by a row not in this batch).
Probe = one VC7.1 compile of the candidate source, graded at its // Leaf, every external symbol listed with its selection
(tools/probe390.py; held/evidence/*.syms.txt).

| row | form | grade | verdict | blocker |
|---|---|---|---|---|
| 00709ce0 | binder (self unchanged): engine/CEngineTextureMorph.h (FABLE_RETAIL_CENGINETEXTUREMORPHSET) + NGraphicAppearance_CMeshInfo.h + STLport pair / FABLE_STLP_OUTOFLINE_UNINIT_FILL_N + VECTOR_COPY_DECL_ONLY | vland H/N RELOCATION_MATCH PASS (b2) | HOLD_RULING | 3 extras all EQ at retail (--addrs: CArray copy = 0x00709A30, the two dtor thunks = 0x005BC0F6), but a NEW NEQ: the CArray copy v table line ??0?$CArray@VCEngineTextureMorph@@ -> 005bbdf1 (witness 00728d10, not ported) -- R5 |
| 00728a40 | binder + self respell to the table's ??$__uninitialized_fill_n@PAVCMeshMorphSetParts...@std@@YI...@Z (witness 00728d10); engine/CTCAppearanceMorphBase.h | vland H/N RELOCATION_MATCH PASS (b2) | HOLD | the same NEW NEQ, and a 29 B ??$_Construct<CMeshMorphSetParts> extra with no retail copy (findwrap: none) |
| 00709c90 | binder + self respell (global pair -> std::pair, FABLE_STLP_OUTOFLINE_UNINIT_COPY) | probe RELOCATION_MATCH | held/ (not run) | self spelling has no PDB source: the (I,O) form ??$__uninitialized_copy@PAU?$pair...@std@@PAU12@@std@@YI... (STLport text; 278 table lines, e.g. 0x00CB9B00) v the T-form ??$__uninitialized_copy@U?$pair...@std@@@std@@YI...PBU10@0PAU10@... that the table binds at 0x00709C90 (witness 00709d60): ruling R6; plus the same 3 extras + NEQ as 00709ce0 |
| 00b217d0 | binder: global CArray<unsigned long> + VECTOR_ASSIGN; GFInterpolateArray as the NParticleEngine template (Ego_r + FableWin ??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z; landed 0x00B22310 precedent) | probe RELOCATION_MATCH | held/ | 14 DEF in the new object (HEAD not compiled; the CArray / vector / _Vector_base / allocator dtor chain, the ??4?$CArray@K@@ forwarder, 1-byte helpers), most without a unique retail address; callee respell also changes the 0x00B22F30 line (witness 00b217d0) |
| 00762440 | binder + self respell to ??0CBasicShape@NShape@@QAE@AAVCTCShapeManager@@ABV?$vector@VC3DVector@@...@Z (FableWin.pdb 0x025D8210 + table witness 007624e0: two sources) on engine/NShape_CBasicShape.h + FABLE_RETAIL_NSHAPE_CONSTRUCTOR_BODIES_PASS26 | probe RELOCATION_MATCH | held/ | 49 DEF in the new object (HEAD not compiled): the STLport vector / allocator / CCountedPointer special-member chain |
| 00728d10 | not attempted | - | lock released | full local namespace std model with same-TU stand-in helper bodies; a port is a re-author of _M_insert_overflow on stlport/fable_containers.h. FABLE_RETAIL_CENGINETEXTUREMORPHSET already gives the out-of-line ~vector<CEngineTextureMorph> 0x005BC0D3 (NF387's "needs a ~CArray decl-only opt-in" is solved). It is the witness that holds b2 (its ??0?$CArray@VCEngineTextureMorph@@ line) |
| 00bd8b30 | not attempted | - | released | local std tree model (fable_map.h port first), NF387 |
| 00709a50 | not attempted | - | released | self respell to the FableWin ctor (vector<CEngineTextureMorph> + set<CCharString> params; table binds it at 0x00709A50) needs CHiddenMaterialSet = std::set<CCharString> with an out-of-line copy (0x00514400) and the base CEnginePrimitiveAnimatingMesh model |
| 00762d80 + 00762580 | not attempted | - | released | unit (T3 nf374): AddNewShape stand-in ?AddNewShape@CTCShapeManager@@QAEXUCShapePointer@@@Z -> 00762580 (witness 00762d80) -> PDB ?AddNewShape@CTCShapeManager@@QAEXV?$CCountedPointer@VCShape@NShape@@@@@Z; expect the same special-member extras as 00762440 |
| 009b77b0 | not retried | - | (released by NF387) | ~10 extras like 00b217d0; R2 does not cover them |

Side findings (no change made):
- Retail keeps the 19 B CArray copy forwarders unreferenced (0x00709A30 / 0x00709BB0 for CEngineTextureMorph) and standalone 5 B
  `jmp ~vector` dtor thunks in the /Os vector TU (0x005BC0F6 right after ~vector<CEngineTextureMorph>'s `ret`; 0x005BC1AB /
  0x005BC272 are further `jmp 0x005BC0D3` sites). FableWin.pdb names both ??1?$CArray@VCEngineTextureMorph@@@@QAE@XZ and
  ??1CEngineTextureMorphSet@@QAE@XZ.
- 0x00709CE0 has two names today: the self ??$__uninitialized_fill_n@PAU?$pair...@std@@IU12@@std@@YI... and the table line
  ??$__uninitialized_fill_n@U?$pair...@std@@@std@@YI... (witness 00709d60, census_c_b2 + xr). Same (I,O) v T-form question as
  00709c90 (R6). NAME_FIXES n1.
- FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22 (user-written CArray copy / dtor) does not stop the dtor COMDATs
  (held/evidence/00709ce0_p3.syms.txt = p2's set).

## Rulings needed
- R4 (b1, informational): the R2 forwarder copy is EQ at 0x009B5A60 only through pk_all --addrs (no relocation witness can ever
  bind an unreferenced function); plain pk_all on the shipped table reads it NOADDR. Accept --addrs as the R2 "EQ at a retail
  address" evidence (b1 marked ACCEPT on that basis), or name another record.
- R5 (b2, 00709ce0): a binder port whose new pick-any CArray copy is EQ at its retail forwarder but reads NEQ against an existing
  CArray-at-vector table line witnessed by another (unported) row. Accept the NEQ as a known-wrong line (same shape as b1's delete), or
  hold until 00728d10 is ported and the line deleted. With R5 yes 00709ce0 still needs its link gate (not run: held).
- R6 (00709c90, and the 0x00709CE0 / 0x00709C90 table lines): STLport (I,O) template-argument spelling v the T-form spelling for the
  __uninitialized_copy / __uninitialized_fill_n instances at 0x00709C90 / 0x00709CE0; neither spelling is in ghidra_out/fablewin_pdb_names.tsv, fablewin_obj_functions.tsv, egor_pdb_names.tsv or egor_obj_functions.tsv (no __uninitialized_* names at all there); the other ghidra_out hits are repo label passes, not PDB.
- R7 (general, the rest of nf383 0x009B7870): shared-header ports emit unreferenced pick-any special-member chains (leafref n,
  reach n, mostly NOADDR) that have no unique retail address (00b217d0 14 DEF / 00762440 49 DEF in the new objects, HEAD not compiled; 009b77b0 ~10 per NF387). Either allow
  unreferenced NOADDR pick-any copies from shared headers as an objsec exception, or these binders stay on their local models.
- A scratch `cat > /tmp/x` slipped into one command (removed in the same command); nothing else was written outside the work dir.
- Compiles: ~140 VC7.1 TUs (99 census of b1, 3 + 2 + 4 census/owner, 12 vland pair, 9 probes + 2 HEAD symbol compiles). The b2 link
  gate ran (3 owner compiles).
