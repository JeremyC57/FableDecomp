# NF232 notes (2026-11-22, round 10): the nf231 follow-ups (q12629-q12634)

Base: the repo tree with nf231 landed. Locks NF232 (pre-claimed for the 12 entry rows; 0x00412B70 claimed; 0x00495780 claimed then
released, owner file reads `released`). No AUTH21 lock (0x00B4D370 0x00BD3070 0x00502E90), `$S/w22/auth21/`, `$S/w22/c2t1/`, Codex pass 27
row or 0x0042BEC0 touched. Tools: NF231's (tools/, re-pathed). hdr/: no header change (every row uses landed opt-ins). Flags lines: none.
EH funclets: none. Descriptive names added: none.

## Entry 1 (q12629-q12631): the three self rows -- done
- **0x004336B0** `@OperatorNew@4` -> `?Set@CStartupDefinitionManager@@SIXPAV1@@Z` (Ego_r 0x00441A90 + FableWin 0x01B31560): the row includes
  engine/CStartupDefinitionManager.h (landed SET + PCLASS) and engine/CCountedPointer.h (landed RESET_CAMERA_DECL, Reset declared only);
  body `PClass.Reset(manager)`. RELOCATION_MATCH first form. Census: binders 0x0042F5A9 / 0x0042EBB6 bind ?Set (work/e1/hb_0042ebb6.tsv):
  2 -> 1 spelling. Callee 0x00433D3A: `?Alloc@Pool@@QAEPAXI@Z` -> FableWin `?Reset@?$CCountedPointer@VCStartupDefinitionManager@@@@QAEXPAVCStartupDefinitionManager@@@Z`
  (the PDB spelling replaces a stand-in; 0x00433D3A stays at 2: its self row keeps the CPolymorphicData_00433D3A stand-in, NAME_FIXES).
  Data 0x013B8760: the g_pool stand-in -> PClass, which 0x004085F0 / 0x00408820 / 0x0043368D bind (work/e1/bind_013b8760.tsv): 3 -> 2
  (0x01228AF1's g_AutoTinyThunkObject stand-in left, NAME_FIXES). Catalog CEngineSubPrimitiveBase::operator_new -> CStartupDefinitionManager::Set.
- **0x00411B00** stand-in `vector_push_back_copy` -> `?erase@?$vector@VCDateAndTime@@V?$allocator@...@@QAEPAVCDateAndTime@@PAV3@0@Z`
  (ruling 2): explicit instantiation over stlport/fable_containers.h with the landed FABLE_RETAIL_STLP_COPY_NOINLINE. Form 1 kept HEAD's
  size pragma: DIFFER(37v38), frame pointer; form 2 /O2: RELOCATION_MATCH. Callee 0x0040E350 Vector_CopyImpl -> the
  `??$__copy@PAVCDateAndTime@@...` spelling 0x0059AEAF and 0x004091C0 bind (work/e1/bind_0040e350.tsv); its pick-any copy EQ owner
  0x0040E350 (72 B). Binders 0x0047FE83 / 0x004861F7 / 0x00597DF7 / 0x0042EBB6 already STLport: 2 -> 1 spelling.
- **0x00412450** CRetailVector stand-in -> the STLport spelling (ruling 2), explicit instantiation over stlport/fable_containers.h +
  fable_string.h, no opt-in: RELOCATION_MATCH first form; callees unchanged; pick-any __copy<CWideString*> EQ owner 0x0040E290 (53 B).
  Census (NF231 bind_00412450.tsv + hb_0042ebb6.tsv): 13 + 0x0042EBB6 = 14 STLport, 0x00412B70 CRetailVector, 0x00495780 allocator-less.
  - **0x00412B70** (resize, its own leaf the CRetailVector stand-in) ported too: explicit instantiation of the STLport resize(n) with the
    landed ERASE_RANGE_OUTOFLINE; RELOCATION_MATCH first form; self leaf -> the STLport spelling all 3 binders emit (0x0047FE83, 0x00501D30,
    0x00597DF7: work/e1/bind_00412b70.tsv; 0x00501D30's DIFFER in that bindat run is the lane-flag artifact: vland grades it RELOCATION_MATCH,
    recheck_e1.log); callees erase 0x00412450 and _M_fill_insert 0x004126E0 onto the spellings every other binder emits
    (bind_004126e0.tsv: 0x0040BCA0, 0x0040C790, 0x00412B20). The pick-any resize(n, x) copy EQ owner 0x00412B20 (66 B).
  - **0x00495780** held: the in-place respell (local std::vector<T> -> <T, A = allocator<T> >, allocator declared only) grades
    RELOCATION_MATCH but the object defines begin / end / clear under the STLport class name that the shared header defines inline (in-place
    rule condition 2). It needs a header port (CFrontendGameComponent.h members). So 0x00412450 ends at 2 spellings (was 3): an existing split.
- Recheck (work/e1/recheck_e1.log, HEAD tests): 4/4 RELOCATION_MATCH + PASS (+ control 0x00501D30). Link (work/e1/link_check.txt, the 4
  rows + 0x0059AEAF 0x004091C0 0x0040E350 0x0040E290 0x00412B20 0x0042EBB6 0x0042F5A9 0x00433D3A 0x0047FE83 0x00597DF7): 0 duplicates.
  Symbol diffs HEAD v new (work/sc_e1.log): the leaves, the callee UNDs above, shared-header inlines / pick-any COMDATs only.

- Table line kept (no change): `?erase@?$vector@VCWideString@@@std@@QAEPAVCWideString@@PAV3@0@Z 00412450 00495780`, still emitted by
  the held 0x00495780.

## Entry 2 (q12633): the CNewFrontendGameComponent vtable split -- blocked: needs header claim (census + gap list; no form graded)
- Retail RTTI (COL / class hierarchy at 0x01230CA0 and 0x01230C94): CNewFrontendGameComponent : CGameComponent : CBaseClassNonCopyable :
  CBaseClass, with CDeviceResetCallback at +4. So retail's vtable names are (INFERRED from that hierarchy and VC7.1's naming; the
  landed 0x00418B7A, whose local CGameComponent has the same two bases, emits ??_7CGameComponent@@6BCBaseClassNonCopyable@@@ /
  6BCDeviceResetCallback@@@ for the base: no header build of this class was compiled) `??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@`
  (0x01230CA0, the table line, witness 0x0042F50B) and `...@@6BCDeviceResetCallback@@@` (0x01230C94; the table line says
  6BCGameComponentInterface, witness 0x0042F50B, a stand-in). Slots: 0x01230CA0 = ??_E 0x004330D2, Init 0x0042F75E, Run 0x0042EC7C,
  ChangeTextureColourDepth 0x0042DC5C, SetQuit 0x00430340 (5); 0x01230C94 = OnPreDeviceReset 0x0042DC5F (`ret`), OnPostDeviceReset 0x0042DC60
  (`mov al,1 ; ret`) (2).
- Landed emitters today (NF231 work/vt_e1.txt): 0x0042EA8F `6BCBase@@@` + `6BCGameComponentInterface@@@`, 0x0042F50B
  `6BCBaseClassNonCopyable@@@` + `6BCGameComponentInterface@@@`, one slot each (truncated local models). absrefs: only these two rows
  reference either vtable.
- Gaps a header port must close (none is in the header today):
  1. Slot 1: `Init` is declared only in the non-RUN branch (FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT is the #else of RUN), so a RUN
     build binds ?Init@CGameComponent@@UAEXXZ; slots 2-4 need RUN and SETQUIT.
  2. Second vtable: engine/CDeviceResetCallback.h has the pure OnPreDeviceReset / OnPostDeviceReset; CNewFrontendGameComponent's overrides
     (0x0042DC5F / 0x0042DC60, catalog names) are not declared, so a header build binds _purecall.
  3. No ctor / dtor declaration. PDBs (Ego_r + FableWin): `??0CNewFrontendGameComponent@@QAE@AAVCGame@@ABVCNewFrontendGameComponentInit@@@Z`,
     `??1CNewFrontendGameComponent@@UAE@XZ`. Ctor census (work/e2/bind_0042ea8f.tsv): 0x00496CF0 / 0x0049DCC0 bind the PDB spelling,
     0x00412F90 binds `...@@QAE@PAVCGame@@ABVCNewFrontendGameComponentInit@@@Z`, the self row emits `(CGame*, CNewFrontendGameComponentInit*)`:
     3 spellings; the port moves the self leaf (two sources) and 0x00412F90 with it.
  4. Base ctor 0x0051D8E0: the header says CGameComponent(CGame&) = PDB `??0CGameComponent@@QAE@AAVCGame@@@Z`; binders
     (work/e2/bind_0051d8e0.tsv) 0x00418DCA / 0x00496070 `...@@QAE@PAUCMainGameComponentParent@@@Z`, 0x0042EA8F `...@@QAE@PAVCGame@@@Z`.
  5. Members the ctor builds are opaque in the header: m_bInitMainGame (CMainGameComponentInit), the AVI block +0x88..+0xB1
     (PAVITextureShow / PAVITextureDecode / AVISrcBox / AVIDstBox / AVIPlaying / XMVCodeLoaded), m_LastInterp (CEngineInterpolationInfoSet: no header).
  6. The dtor (size TU) expands ~CGameComponent (two vtable stores, tail jump to ~CBaseClassNonCopyable 0x0099A300) and the def release;
     the HEAD row's comment says its local model needed both __forceinline (not re-measured here on the header). engine/CGameComponent.h declares `virtual ~CGameComponent();` out of line: an inline-body opt-in is
     add-only, but FORCEINLINE in a shared-header opt-in is barred until a /d2inlT / /d2inlS sweep has failed (coordinator 2026-11-20).
- Probe: a throwaway header TU calling the implicit destructor emits no ??_7 at all (VC7.1 stores no vptr in an implicit dtor), so the
  slot list above is read from retail; a real check needs the ctor declared (gap 3).
- Plan (needs a coordinator decision, Decision 1): one header unit in CNewFrontendGameComponent.h (an add-only opt-in on top of RUN +
  FILENAMES_FILETIMES + SETQUIT: typed members, ctor / dtor, `virtual void Init()`, the two device-reset overrides) and CGameComponent.h
  (an inline ~CGameComponent opt-in, after the /d2inl sweep on 0x0042F50B), then 0x0042F50B, 0x0042EA8F (leaf -> PDB) and 0x00412F90, with
  0x0051D8E0's three binders on the PDB spelling. That is a 4-row family across 2 headers, too wide for this hand-back's context.

## Entry 3 (q12632): 0x0054F410 / 0x0054F370 row ports -- done
- Both rows on the shared headers, first form, RELOCATION_MATCH, leaves unchanged: fable_string.h (FABLE_RETAIL_CCHARSTRING +
  _GETCRC_INLINE, FABLE_RETAIL_CWIDESTRING), engine/NGameText_CDataBank.h (CBANKFILE_FINDINDEXBYSYMBOLCRC), engine/CMainGameComponent.h
  (FABLE_RETAIL_CMAINGAMECOMPONENT + _TEXTBANK: GetTextBank(), Ego_r ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ,
  replaces the descriptive GetGameTextDataBank), engine/CNewFrontendGameComponent.h (Get / GetTextBank), engine/NUISystem_CText.h (the
  three SetValue overloads give slots 143-145). Every local model is gone (no SHARED_HEADER_AVAILABLE warning in recheck_final.log).
- Relocation diff HEAD v new (work/sc_e3.log): one: 0x0129AAF4 binds `?NullChar@CCharString@@1DB` (the table line, witness 0x00AE8D50)
  instead of a `""` literal. Pick-any (work/pk_e3.tsv): GetCRC / GetLength / GetAsCharArray EQ their owners 0x00404310 / 0x00403A00 /
  0x00403A10; HEAD's GetCRC copy was NEQ (relocs: the literal at 0x0129AAF4). HEAD's `?GetLength@CCharString@@QBEKXZ` (unsigned) copy is
  replaced by the PDB `QBEJXZ`.
- Link (work/e3/link_check.txt: 0x0054F410 0x0054F370 + 0x00404310 0x00403A00 0x00403A10 0x0054FBC0 0x0054FF10): 2 LNK4006
  (?GetCRC@CCharString@@QBEKXZ, each row's pick-any copy v the owner 0x00404310's strong definition), the same 2 with the HEAD objects
  (work/e3/link_check_head.txt). None new; the copies now equal the owner's bytes.
- Tests: new header tests for both (rows/0054f410_test.cpp, rows/0054f370_test.cpp): the body on the same headers; text, CText object,
  components and banks on raw storage; slot 143 through a test vtable (a same-signature recording member); ToCharString / ToWideString /
  the string ctors and dtors / CCRC::Calc / FindIndexBySymbolCRC stubbed; the private front-end static under `#define private public`
  around that header alone; the protected statics (PMainGameComponent, NullChar) through never-constructed subclasses. Four cases each:
  no look-up, the main game's bank, the front end's bank, no component. No ?g_ stand-in, no local model of a class with a header.

## Entry 1 tests
- 0x004336B0: new header test (Set(manager) / Set(0) reach PClass.Reset through an explicit-specialization stub; PClass defined in the
  test). Replaces the bare `printf` stub. 0x00411B00: new header test over the shared vector (a never-copied test subclass adopts a static
  28-byte array): middle / empty / tail ranges. It prints the row's build PassPattern `VECTOR_PUSH_COPY_PASS`, a name left from the stand-in
  (Decision 3). 0x00412450 / 0x00412B70 keep their HEAD host-model tests (PASS; they model CRetailVector locally: queued, Decision 4).

## Entry 4 (q12634): tests for 0x0041D98A, 0x00446720, 0x00599D5C, 0x00542140, 0x0059A238 -- open (context)
- Not started. Note for the next pass: 0x0041D98A's row itself still models CManager / CDefinitionManager / CIVCountedPointeeBase /
  CCharString / CDefPointer locally behind `#define X XDonor` include renames; a row port onto NUISystem_CManager.h first (as entry 3) would
  make its header test honest.

## Self-check
- recheck_final.log: one vland --recheck-landed run over the 6 changed rows (4 with new tests): 6/6 RELOCATION_MATCH + PASS, no
  RELOC_CONFLICT, no SHARED_HEADER_AVAILABLE; warnings only STANDIN_LEAF on 0x004336B0 / 0x00411B00 (catalog names: RENAMES section 1).
- Spellings per touched address after this hand-back (code binders + self leaf; no vtable slot involved except entry 2's, unchanged):
  0x004336B0 1; 0x00411B00 1; 0x00412450 2 (was 3; 0x00495780 held); 0x00412B70 1 (was 2); 0x004126E0 down one (CRetailVector insert gone);
  0x0040E350 down one (Vector_CopyImpl gone); 0x00433D3A 2 (stand-in Pool -> FableWin Reset; self row stand-in left); 0x013B8760 2 (was 3);
  0x0054F410 / 0x0054F370 unchanged (1 each); 0x0129AAF4: the rows move from a literal onto the existing NullChar table spelling. No new split.
- Defined-symbol diffs HEAD v new (work/sc_e1.log, work/sc_e3.log): own leaf, shared-header inlines and pick-any COMDATs only; every
  pick-any copy with a retail owner is EQ (pk_e1.tsv, pk_e3.tsv).

## Decisions for the coordinator
1. Entry 2 (CNewFrontendGameComponent vtables; nothing graded, the gap list is read from the headers, the PDBs, retail RTTI and the
   compiled binder census): approve the header unit in NOTES entry 2 (CNewFrontendGameComponent.h ctor / dtor / Init /
   device-reset overrides / typed members; CGameComponent.h inline ~CGameComponent after a /d2inl sweep, or a FORCEINLINE exception) and the
   4-row family (0x0042F50B, 0x0042EA8F, 0x00412F90, with 0x0051D8E0's binders 0x00418DCA / 0x00496070), or keep the split queued?
   Also: the table line `??_7CNewFrontendGameComponent@@6BCGameComponentInterface@@@ 01230c94` is a stand-in name; retail RTTI gives
   `...@@6BCDeviceResetCallback@@@` (no landed emitter yet, so not changed here).
2. 0x00495780 (the last allocator-less erase binder): queue its header port (CFrontendGameComponent.h members + Shutdown)?
3. 0x00411B00's build PassPattern `VECTOR_PUSH_COPY_PASS` names the old stand-in: rename to `00411b00_TEST PASS` at landing (the new
   test would then print that)?
4. 0x00412450 / 0x00412B70 / 0x00433D3A: queue header tests (the first two) and the 0x00433D3A self-row port (NAME_FIXES)?
