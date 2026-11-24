# NF230 notes (2026-11-22, round 10): the nf229 follow-ups (q12620-q12624)

Base: the repo tree with nf229 landed. Locks NF230 (pre-claimed by the coordinator for the entry rows; the 12 vtable-slot test rows were
`released` and are now NF230). No AUTH19 lock (0x00B4D370 0x00BD3070 0x00502E90), Codex pass 27 row or 0x0042BEC0 touched. Tools: NF229's
(re-pathed, tools/; every `rm -rf` replaced by the guarded tools/clean.py) + work/t3/gen_obs.py / gen_mgr.py (test generators).
hdr/: three add-only opt-ins (hdr/README.md). Flags lines: none. EH funclets: none. Descriptive names added: none (every new spelling is a
PDB one; 0x0042A8E0's is in both PDBs).

## Entry 1 (q12620): 0x0042A8E0 -> the pair's ??_G -- done
- Census: callers.py (E8/E9) and absrefs.py (absolute) find NO landed binder of 0x0042A8E0; 0x00429E16's binders are 0x0042A8E0
  (@CAssoc_dtor@4) and 0x0042B8FC (??1pair), plus the unlanded 0x0042B462 (work/callers_e1.txt, absrefs_e1.txt empty).
- Name: Ego_r.pdb AND FableWin.pdb S_GPROC32 `std::pair<CCharString const ,CCountedPointer<NUISystem::CComponent> >::`scalar deleting
  destructor'` = ??_G?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAEPAXI@Z, both in ui_manager.obj (the
  CManager TU); the retail body calls 0x00429E16 (landed ~pair) and operator delete. STL instance: the name is the PDB's.
- Row: the explicit instantiation `template void std::_Destroy(pair<...>*)` under 0x0042B8FC's defines / pragma; instantiating the pair's
  destructor makes VC7.1 emit the pair's ??_G COMDAT with it (as in 0x0042B8FC's object). RELOCATION_MATCH on the first form. The leaf is a
  compiler-emitted COMDAT, not the instantiated function itself (Decision 1). Its call to 0x00429E16 now binds ??1pair: 0x00429E16 2 -> 1
  spelling; 0x0042A8E0 1 spelling (it had only the self leaf). Table: the @CAssoc_dtor@4 line goes (TABLE_CHANGES).
- pick-any (work/pk_e1.tsv): ReleaseData / ~CCountedPointer / ~pair EQ 0x004291DE / 0x00429C53 / 0x00429E16; _Destroy (9 B, `mov ecx,[esp+4] ;
  jmp ~pair`) NO-OWNER (0x0042B8FC's object defines the same copy). Symbol diff HEAD v new (work/sc_e1.log): only the leaf and those COMDATs.
- Link (work/link_e1.txt): with 0x00429E16 0x0042B8FC 0x004291DE 0x00429C53 0x0042C02F: 0 duplicates (HEAD 0).
- Test: new header test (engine/NUISystem_CManager.h): `delete` of a heap pair (the test object's own call goes to ??_G: checked with
  objdump), key destroyed once, storage freed once with its own address; null delete does nothing. It keeps the ps1 PassPattern
  `ULTRA2_0042a8e0_CASSOC_SDD PASS`, so the build entry needs no change (the pattern text still says CASSOC; rename it if wanted).

## Entry 2 (q12621-q12623): 0x013B871C -- the stand-ins cleared but one
- Census: absrefs.py on today's tree: 45 references in the same 28 landed rows as nf229 (work/absrefs_e2.txt). Compiled census with this
  hand-back (bindat.sh USEROWS=1, overlay on; work/bind_013b871c.tsv): 27 rows bind ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A
  (0x0041CFB2 through its pick-any Get copy), 0x0042EBB6 binds the ?g_ stand-in. ?System_013b871c@@3PAUSystem@@A and the public-static
  @@2PAV1@A: 0 emitters (no table line names either). 0x013B871C: 4 -> 2 spellings (1 if the 0x0042EBB6 draft lands).
- Ported, each on its first form, RELOCATION_MATCH, the 0x0041CFB2 / 0x00446720 pattern (include the header after the row's local
  string / bank models; only the stand-in expressions change; local models of other classes are HEAD debt, unchanged):
  0x00541F20 (x1) and 0x00542140 (x3) `Get()->GetTextBank()`; 0x00597BF2 (x1) `Get()->ResetAttractModeTimer()` (opt-in); 0x0059A238 (x4)
  Get / GetTextBank (x2) / StartGame (the landed STARTGAME opt-in for 0x0F's slot 4 + m_bCreateGame) / ResetAttractModeTimer / SetQuit
  (opt-in, 0x128's slot 4); 0x0041D0A8 (x1) `Get()->GetPSoundBank()` (opt-in). 0x0059A238's truncated Leaf line is HEAD (unchanged).
- pick-any (work/pk_e2.tsv, pk_e2h.tsv): the new copies Get / GetTextBank / CCountedPointer<CDataBank>::operator* / StartGame /
  GetPSoundBank / CCountedPointer<CASoundBank>::Get have no landed owner (NF229 measured the Get copy equal to the unlanded out-of-line Get
  at 0x00405FD0, whose absolute reference absrefs shows at 0x00405FD1).
- Static's spelling: ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A is in Ego_d.pdb (carved names; LF_STMEMBER private
  in the class field list) and is what the header declares and 19+ landed rows already bound (nf228 / nf229); FableWin.pdb has the public
  inline accessor Get. GetPSoundBank non-const: its LF_MFUNCTION `this` is 0x57857 (-> the class 0x57818) where the const GetTextBank's is
  0x57869 (-> the const modifier 0x57858). ResetAttractModeTimer: Ego_d attrs = public. NEQ copies, all identical at HEAD (existing debt): 0x00597BF2's
  ??8 _Rb_tree_base_iterator (v 0x0041C820), map::insert (v 0x0059B559), map::operator[] (v 0x0059B5D7); 0x0059A238's map::operator[]
  (17 v 69, its same-TU noinline stand-in).
- Link (work/link_check.txt): the 6 changed rows + co-definers 0x00429E16 0x0042B8FC 0x004291DE 0x00429C53 0x0042C02F 0x0041CFB2
  0x00446720 0x00558170 0x00599D5C 0x00599E3F 0x0042DBEE 0x00430340 (18 objects, overlay on): 2 LNK4006, the same 2 with the 6 HEAD
  objects (work/link_check_head.txt): ~vector<CWideString> (0x00599E3F v 0x00599D5C) and 0x0059A238's map::operator[] stand-in. None new.
- SetQuit (0x0059A238): CGameComponent::SetQuit is protected (?SetQuit@CGameComponent@@MAEXXZ); CNewFrontendGameComponent overrides it
  public: FableWin.pdb ?SetQuit@CNewFrontendGameComponent@@UAEXXZ, Ego_d.pdb's field list (public virtual), and the retail RTTI vtable slot 4
  = 0x00430340 (not 0x00415930). The opt-in exposes the class's own public override, not the base's protected one (Decision 2).
- GetPSoundBank (0x0041D0A8): nf229 found no accessor in Ego_r / FableWin; Ego_d.pdb's CNewFrontendGameComponent field list has
  LF_ONEMETHOD GetPSoundBank (public, non-const, CASoundBank*). One PDB + the retail read (+0x50) (Decision 3). No friend in the class.
- **0x0042EBB6 Shutdown: blocked, needs header claim** (what is missing, exactly):
  1. FileNames / FileTimes typed: Ego_r and Ego_d type them `vector<CWideString>` / `vector<CDateAndTime>` (retail 12-byte STLport vectors at
     +0x6C / +0x78); the header's RUN branch has `unsigned char[0xc]` arrays. A new opt-in would type them (stlport vector +
     fable_string.h CWideString + engine/CDateAndTime.h), and the /Os TU must then emit erase(first, last) out of line as 0x00412450 /
     0x00411B00 (pk to check).
  2. m_pFrontEndDef: Ego_r and Ego_d both type it CDefPointer<CFrontEndDef const> (header right; the row's non-const local model is the
     defect). `m_pFrontEndDef = 0` maps onto the landed FABLE_RETAIL_CDEFPOINTER_RESET Reset(0) (FableWin
     ?Reset@?$CDefPointer@$$CBVCFrontEndDef@@@@QAEXPBVCFrontEndDef@@@Z; its comment gives retail's release + second null store) with
     engine/CFrontEndDef.h: no new claim needed there.
  3. The row-local CWideString, CDateAndTime, CRGBColour, CDisplayManager, CFrontEndDef models move to their headers (all exist), and the
     two header-less stand-ins (CFrontendGlobal_013b838c for PFontManager +0x38, SetActiveFrontend_004336b0) need real names. That is a
     re-authoring of the row, not a respell.
  A cheap alternative is ready and graded: work/e3/0042ebb6_inplace.cpp adds `static CNewFrontendGameComponent* m_pNewFrontendGameComponent;`
  (private) to the row's EXISTING local model and writes it in place of the ?g_ extern: RELOCATION_MATCH, the only symbol change is UND
  ?g_ -> UND ?m_p...@@0PAV1@A (no new DEF). It meets the in-place rule's conditions (no new class token; nothing defined naming the class
  beyond the leaf) but keeps a local model of a class that has a header (Decision 4). With it, 0x013B871C ends on one spelling and the ?g_
  table line goes.

## Entry 3 (q12624): tests -- 11 of 14
- New self-contained header tests: 0x0055CA40 0x0055C9C0 0x0055CAE0 0x0055CA90 0x0055CA00 0x0055C950 (engine/NUISystem_CObservable.h: the
  row body under its defines; a test subclass reads the protected lists; the other CObservable virtuals and CObserver's out-of-line
  ctor / dtor / virtuals stubbed; a test observer counts IgnoreAllEvents) and 0x0042D4F0 0x0042D5AA 0x0042D592 0x0041E3B2 0x0041DED2
  (engine/NUISystem_CManager.h: the row body called non-virtually on raw storage; a never-constructed test subclass names the protected
  members as pointers-to-member; only the touched members are placement-built). One case per condition. Each keeps its ps1 PassPattern.
- Test-only defines (tests only, per the nf226-nf228 rulings): 0x0041E3B2 drops FABLE_RETAIL_STLP_TREE_MFIND_DECL and 0x0041DED2 drops
  FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE (declaration-only bodies the rows call out of line), and both add FABLE_RETAIL_STLP_MAP +
  _TREE_INSERT_UNIQUE_HINT_BODY + _MAP_ORIGINAL_INSERT_BODY (the map operator[] body; GetLayer's own fallback calls it) and stub the STLport
  rebalance; 0x0041DED2 adds FABLE_RETAIL_CCHARSTRING with stubbed CCharString ctors / dtor / operator< (the 0x0042B8FC test pattern).
- Open (context): 0x0041D98A GetIconsDef (89-line row, 532-line HEAD test), 0x00446720, 0x00599D5C. Untried idea for 0x00599D5C: define the
  private static in the test (`CNewFrontendGameComponent* CNewFrontendGameComponent::m_pNewFrontendGameComponent = ...;`, its initializer is
  in class scope) instead of the INIT stub, so the default-branch accessor opt-in can be used.

## Decisions for the coordinator
1. 0x0042A8E0's leaf is the ??_G COMDAT VC7.1 emits beside the pair's destructor, which the row reaches through an explicit instantiation of
   the owner-less std::_Destroy (9 B pick-any copy, also in 0x0042B8FC's object). OK as a row shape?
2. SetQuit opt-in: declares CNewFrontendGameComponent's own public virtual override (FableWin + Ego_d + retail vtable slot 4 = 0x00430340).
   The landed self row 0x00430340 still spells ?SetQuit@CNewFrontendGameComponent@@QAEXXZ (NAME_FIXES; no relocation binder).
3. GetPSoundBank opt-in rests on Ego_d.pdb (name, access, signature) + the retail read (nf229 rule: one PDB + the body).
4. 0x0042EBB6: land the in-place draft (work/e3/0042ebb6_inplace.cpp: ?g_ goes, 0x013B871C one spelling), or keep it blocked for the header
   port above?

5. The HEAD tests of 0x00542140, 0x0059A238 and 0x0041D0A8 are host models that keep their own CNewFrontendGameComponent / `System`
   stand-in and the old static spelling (test only; files unchanged): land them with the HEAD tests and queue the rewrite (nf229 ruling 2)?
6. The 0x0042EBB6 in-place draft has a q.py grade only (RELOCATION_MATCH), not a vland run with its HEAD test.

## Self-check
- recheck_final.log: one vland --recheck-landed run, overlay on, over the 6 changed rows + the 11 test-only rows: 17/17 PASS, 4 MATCH +
  13 RELOCATION_MATCH, no RELOC_CONFLICT (warnings: SHARED_HEADER_AVAILABLE / RELOC_NAME = HEAD debt of the unchanged local models and table
  labels; 00542140 `vfunc_143` leaf = HEAD catalog).
- Spellings per address after this hand-back: 0x0042A8E0 1; 0x00429E16 1; 0x013B871C 2 (?m_p@@0 + ?g_ on 0x0042EBB6). Vtable slots: absrefs.py
  finds no absolute reference (vtable slot or data) to 0x0042A8E0 or 0x00429E16; 0x013B871C is data, its 45 absolute references are the
  28 rows of the compiled census.
- Header proof: pin_asserts 0 pinned; assert_names_check 0 mismatches; equiv_check over the 29 includers 0 differing.
