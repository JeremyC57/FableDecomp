# NF221 notes (2026-11-22, round 10): CCountedPointer<T>::ReleaseData family + q12403 CComponent ??1 binders

Lane: q.py grades with S=$A/pH (HEAD) / $A/pS (new); census tool tools/hb.sh (q.py + xref.py, relocations at family targets).
Census files: work/c1_callers.tsv (callers.py over Fable.exe), work/hb_head.tsv + hb_head2.tsv (HEAD bindings; every row RELOCATION_MATCH at HEAD).

## Family census (HEAD bindings, compiled + xref.py)
Target spellings (address -> new spelling; evidence column = FableWin.pdb name + retail body + callers):
- 0x004190B6 ?ReleaseData@?$CCountedPointer@VCLUA@@@@QAEXXZ (44-byte /Os release; jmp thunk 0x00419423 not landed)
- 0x00419108 ?ReleaseData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXXZ (jmp thunk 0x004194B5)
- 0x0042F974 ?ReleaseData@?$CCountedPointer@VCIEngine@@@@QAEXXZ (thunk 0x0042FCFF not landed)
- 0x0042F9A0 ?ReleaseData@?$CCountedPointer@VCASoundBank@@@@QAEXXZ (thunk 0x0042FBCB not landed)
- 0x0042918C ?ReleaseData@?$CCountedPointer@VCMeshDataBank@@@@QAEXXZ (thunk 0x00429BF9)
- 0x0042F948 ?ReleaseData@?$CCountedPointer@$$CBVCFontBank@@@@QAEXXZ (thunk 0x0042FC0E not landed)
- 0x00403750 ?ReleaseData@?$CCountedPointer@VCDataBank@NGameText@@@@QAEXXZ (53-byte /O2 shape, no thunk; NF220 Gate A)
- 0x004057A0 ?ReleaseData@?$CCountedPointer@VCFontBank@@@@QAEXXZ (53-byte /O2 shape, no thunk; called by ShareData<CFontBank> 0x004190E2
  and Reset<CFontBank> 0x0041947A; no CCountedPointer<CFont> in any of the three PDBs: CFontManager::GetFont returns CCountedPointer<CFontBank>)
- thunks 0x004194B5 ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ, 0x00429BF9 ??1?$CCountedPointer@VCMeshDataBank@@@@QAE@XZ
- stand-ins: 0x00419134 / 0x004291B8 / 0x0042F9F2 / 0x004190E2 ShareData<CGraphicDataBank / CMeshDataBank / const CFontBank / CFontBank>;
  0x004194BA ??4<CGraphicDataBank>, 0x00419463 ??4<CFontBank>, 0x0042FE34 (const CFontBank assignment; leaf to settle);
  Reset 0x00419428 <CLUA>, 0x0041947A <CFontBank>, 0x004194D1 <CGraphicDataBank>, 0x0042FC13 <const CFontBank>, 0x0042FC86 <CMeshDataBank> (leaf right).
HEAD binders per address (rows; spelling seen):
- 0x00419108: 00416c8a 00418b7a 0042dd28 0042f50b 0056f177 005958f5 005bff07 (??1<CGraphicDataBank>), 004175e5 (Release@CCountedPointer419108),
  00419134 (ReleaseCur_00419108@CCPtr), 004194b5 (_ForwardTarget_00419108), 004194d1 (Release<CPolymorphicData_004194D1>),
  0041e641 (??1SMember10), 0042a9b7 (??1CCountedPointer_CGraphicDataBank), 0042ddb3 (??1?$CCountedPointer_0042fe34<CGraphicDataBank>)
- 0x004190B6: 00418b7a (??1<CLUA>), 004175e5 (Release@CCountedPointer4190b6), 00419428 (Release<CPolymorphicData_00419428>)
- 0x0042F974: 0042dd28 0042f50b (??1<CIEngine>), 0042fd04 (ReleaseData: right)
- 0x0042F9A0: 0042f50b (??1<CASoundBank>), 0042fd6f (ReleaseData: right)
- 0x0042918C: 0042dd28 0042f50b 005958f5 0042fc86 (??1<CMeshDataBank>), 0041d198 (??1?$CCountedPointer_0042918c<CGraphicDataBank>),
  0041e641 (??1SMember18), 004291b8 (ReleaseCur_0042918c@CCPtr), 00429bf9 (_GFGetPreMainMemoryUsedImpl)
- 0x0042F948: 0042dd28 0042f50b (??1<const CFontBank>), 0042f9f2 (ReleaseCur_0042f948@CCPtr), 0042fc13 (Release<CPolymorphicData_0042FC13>)
- 0x00403750: 004175e5 00418b7a 0042f50b (??1<NGameText::CDataBank>)
- 0x004057A0 (7 spellings): 004168dc (??1<UCFont>), 004175e5 (Release@CCountedPointer4057a0 x2), 00418b7a 0048e78e (??1<CFontBank>),
  004190e2 (ReleaseCur@CCPtr), 0041947a (Release<CPolymorphicData_0041947A>), 0042ddb3 (??1?$CCountedPointer_0042fe34<UCFont>),
  0064560d (x2) 0065431d (??1CFontPointer)
- stand-in callers: 00419134 <- 004194ba (ShareData_00419134@CCPtr), 0042a9b7 (ShareData@CCountedPointer_CGraphicDataBank);
  004291b8 <- 0041d198 (ShareData?$CCountedPointer_0042918c<CGraphicDataBank>); 0042f9f2 <- 0042fe34 (ShareData_0042f9f2@CCPtr);
  004190e2 <- 00419463 (ShareData@CCPtr); 004194ba <- 00416c8a (??4<CGraphicDataBank>: right), 0042ddb3 (??4?$CCountedPointer_0042fe34<CGraphicDataBank>);
  00419463 <- 00499aa0 (??4<CFontBank>: right), 004168dc (to recheck); 0042fe34 <- 0042ddb3 (??4?$CCountedPointer_0042fe34<UCFont>);
  0041947a <- 00a5f1c0 (Reset@CCountedPointer_00a5f1c0 PAVCStreamingFontBank), 00a634f0 (Reset@CCountedPointer_00a634f0 PAUCStaticFontBank)
- not-a-binder (already right / bind only unchanged leaves): 0041afa0 (??1<CGraphicDataBank> at thunk 0x004194B5), 0042fd04, 0042fd6f,
  0041e3f6, 0042db20, 0042fb40, 0042e98f, 004a6ab0, 004a6e30, and the 10 CTCInventory* ctors (bind only 0x005BFF07's ctor, unchanged).

## q12403: the 4 rows binding ??1<CComponent> at 0x004291DE: done
- Census (tools/hb.sh over every landed caller row of 0x004291DE / 0x00429C53, 115 rows, all compiled at HEAD, all RELOCATION_MATCH;
  work/hb_cc.tsv): ReleaseData<CComponent> at 0x004291DE bound by 105 rows, ??1<CComponent> at 0x004291DE by exactly 4 rows
  (0x0042D45D x2, 0x005C8459 x18, 0x005C9A95 x16, 0x0058F016 x11), ??1 at 0x00429C53 by 6 rows (0x00544D20 0x00563920 0x0056AD40
  0x00601310 0x00626110 0x00626750). 0x00570332 (the table witness of ??1 -> 0x004291DE) binds ReleaseData at all 9 sites: stale.
- Rows: 0x0042D45D +RELEASEDATA_DECL_ONLY and 0x0058F016 DTOR_DECL_ONLY -> RELEASEDATA_DECL_ONLY (one form each); 0x005C8459 / 0x005C9A95
  _DECL_ONLY DIFFERs (same length / -2 bytes: c2 needs the body), RELEASEDATA_NOINLINE (visible out of line, NF73's opt-in) gives
  RELOCATION_MATCH with every site on ReleaseData and the defined-symbol set unchanged (HEAD already emitted a pick-any ReleaseData
  copy, the default Info-first body; now the NOINLINE opt-in's Data-first text -- copy bytes v owner 0x004291DE not measured). After this pass no landed row binds ??1 at 0x004291DE.
- Table action (coordinator): replace reloc_bindings line `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ 004291de 00570332`
  by `??1?$CCountedPointer@VCComponent@NUISystem@@@@QAE@XZ 00429c53 00563920` (0x00563920 binds ??1 at 0x00429C53 x15 in today's object).
  Codex pass 26's PICKANY_CORRECTED files agree. The ReleaseData<CComponent> -> 0x004291DE line (witness 0x0041DED2) stays.

## Family q12226-q12229 / q12402: 42 rows ported (RELOCATION_MATCH each, tools/sc.sh diff v HEAD), 2 rows OPEN -> family BLOCKED (hold it whole)
Recipes (each proven on one row, then scripted: tools/mkself.py, mkdecl.py, mkstand.py; logs work/sc_b1..b4.log):
- Self ReleaseData rows (8): FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA + `template void CCountedPointer<T>::ReleaseData();` (size pragma kept on
  the 44-byte rows; none on the /O2-shaped 0x00403750 / 0x004057A0). Thunks 0x004194B5 / 0x00429BF9: the landed CAction twin 0x00429B72 text
  (RELEASEDATA_DECL_ONLY + explicit ~CCountedPointer instantiation): the 5-byte `jmp ReleaseData<T>`.
- Header callers (6: 0x0042F50B 0x00416C8A 0x0042DD28 0x0056F177 0x005958F5 0x00418B7A, plus 0x0048E78E): +RELEASEDATA_DECL_ONLY, one form each.
- Stand-in ports onto engine/CCountedPointer.h (12): ShareData x4 (RELEASEDATA_DECL_ONLY), operator= x3 (SHAREDATA_DECL_ONLY; 0x0042FE34 is the
  FableWin.pdb converting member template ??$?4VCFontBank@@@?$CCountedPointer@$$CBVCFontBank@@@@..., no plain ??4 exists for const CFontBank),
  Reset x5 (RELEASEDATA_DECL_ONLY; the header's inline DeleteData<T> now emits a pick-any copy where HEAD declared it: T is only declared,
  so the copy is a plain `operator delete`, not retail's virtual delete -- same as the landed header Reset rows 0x0042FD04 / 0x0042FD6F).
- Local-model rows: 0x004175E5 (local CCountedPointer4190b6/4057a0/419108 stand-ins -> header; `x.ReleaseData(); x.Data = 0;` per site;
  members respelled to Ego_r.pdb PTextBank / PLUA / PDebugFont / PCutSceneMainFont / PMainGraphicBank; CCountedPointer41915a (0x0041915A,
  outside the family) stays local); 0x0041D198 / 0x0042A9B7 (stand-ins -> header; self leaves move to the FableWin/Ego_r SetMeshBank /
  SetGraphicsBank spellings their callers already bind; members PMeshBank / PGraphicsBank respelled in the HEAD local CManager model);
  0x0041E641 (SMember10 / SMember18 wrappers -> CCountedPointer<CGraphicDataBank> / <CMeshDataBank>); 0x005BFF07 (in-place respell of its HEAD
  local CCountedPointer model: `~CCountedPointer() { ReleaseData(); }` + declared ReleaseData; a full port waits on its SerialisedItems pointees);
  0x0042DDB3, 0x004168DC, 0x0064560D, 0x0065431D (CFont / CFontPointer / CFontBankManager stand-ins -> CCountedPointer<CFontBank> or
  <const CFontBank> + engine/CFontManager.h GetFont).
- T identities, two sources each: FableWin.pdb ReleaseData / ShareData / Reset / ??4 names per T + the retail bodies / callers; Ego_r.pdb
  CMainGameComponent (PTextBank CDataBank, PLUA CLUA, PDebugFont / PCutSceneMainFont CFontBank, PMainGraphicBank CGraphicDataBank),
  CNewFrontendGameComponent (m_pMenuFont CCountedPointer<const CFontBank>, m_pFrontEndGraphicBank CGraphicDataBank), NUISystem::CManager
  (PGraphicsBank, PMeshBank). No CCountedPointer<CFont> exists in Ego_d / Ego_r / FableWin: the CFont pointee was wrong.
Spelling counts after this hand-back (landed rows, from the sc.sh relocation lists; not counting the two open rows):
- 0x004190B6 0x00419108 0x0042F974 0x0042F9A0 0x0042918C 0x0042F948 0x00403750 0x004057A0: one spelling each (ReleaseData<T>), self and every landed
  binder (HEAD bindings per address are in the census section above).
- 0x004194B5 / 0x00429BF9: ??1<T> only. ShareData 0x00419134 / 0x004291B8 / 0x0042F9F2 / 0x004190E2 and ??4 0x004194BA / 0x00419463 / 0x0042FE34:
  one spelling each. Reset 0x00419428 / 0x004194D1 / 0x0042FC13 / 0x0042FC86: one spelling (no landed callers).
- 0x0041947A Reset<CFontBank>: self on the new spelling, but 0x00A5F1C0 / 0x00A634F0 still bind ?Reset@CCountedPointer_00a5f1c0@@QAEXPAVCStreamingFontBank@@@Z
  / ?Reset@CCountedPointer_00a634f0@@QAEXPAUCStaticFontBank@@@Z: 3 spellings, as at HEAD (no growth), but not "every referencer on the new spelling".
- Callees outside the family respelled onto their own self leaves (count goes down, never up): 0x009E2C80 CFontManager::GetFont (4 rows),
  0x009F83D0 CGraphicBankManager::OpenGraphicBank (0x0042DDB3), DeleteData 0x0041901D / 0x00419028 (not landed, one binder each), 0x00419036 /
  0x0042F864 (spelling count unchanged: one stand-in binder replaced by the PDB spelling; self leaves stay, NAME_FIXES).
### Open (next agent; locks kept NF221): 0x00A5F1C0 CStreamingFontDataBankEntry::AcquireFont, 0x00A634F0 CFontDataBankEntry::AcquireFont
- Both keep a local CCountedPointer_<addr> stand-in whose Reset binds 0x0041947A. Their landed callers 0x009E2C10 / 0x009E2BA0 already bind
  ?AcquireFont@...@@QAE?AV?$CCountedPointer@VCFontBank@@@@XZ (non-const; the PDBs say QBE, NAME_FIXES), so a port that returns the header
  CCountedPointer<CFontBank> by value (non-const) is a self rename onto a spelling every referencer already binds.
- Recipe to try: RELEASEDATA_DECL_ONLY + RESET_CAMERA_DECL (declaration-only Reset) on engine/CCountedPointer.h; m_font -> CCountedPointer<CFontBank>
  (the header copy constructor is the inlined copy into the hidden return slot); 0x00A5F1C0's CreateBank returns CStreamingFontBank*
  (engine/CStreamingFontBank.h: check it derives from CFontBank, else the Reset argument needs the header base). 0x00A634F0 defines a local
  `struct CStaticFontBank` (engine/CStaticFontBank.h exists, `: public CFontBank`): port onto the header (FABLE_RETAIL_CSTATICFONTBANK_CTOR declares
  ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z for 0x00AB8E10, which respells that callee: census its binders first) or record "needs header".
  Not attempted (context).
### Table actions at landing (coordinator)
- reloc_bindings lines whose spelling no landed row binds after this pass (witness rows listed; check with the exe gate): the ??1<T> lines at
  the ReleaseData addresses (0x00419108 / 00416c8a, 0x0042F948 / 0x0042F974 / 0x0042918C / 0042dd28, 0x0042F9A0 / 0042f50b, 0x00403750 /
  0x004057A0 / 0x004190B6 / 00418b7a), ?ReleaseCur@CCPtr / ?Release@CCountedPointer4057a0/4190b6/419108 / ??1CFontPointer / ??1<UCFont> /
  ??1?$CCountedPointer_0042fe34<...> / ??4?$CCountedPointer_0042fe34<...> / ??1SMember10 / ??1SMember18 / ??1CCountedPointer_CGraphicDataBank /
  ?ShareData@CCountedPointer_CGraphicDataBank / ?ShareData@?$CCountedPointer@VCGraphicDataBank@@..PAUCCPPointerInfo (0x004291B8, witness 0x0041D198)
  / ?Release@?$CCountedPointer@VCPolymorphicData_* / ?DeleteData@?$CCountedPointer@VCPolymorphicData_* (0x0041901D / 0x00419036 / 0x0042F864),
  ?GetFont@CFontBankManager@@QAE?AUCFontPointer (0064560d), ?GetFont@CFontBank@@...CCountedPointer@UCFont (004168dc),
  ?CreateAndInitialize@CGraphicDataBankManager...CCountedPointer_0042fe34 (0042ddb3); each wants the new ReleaseData<T> / PDB line instead.
  Plus the q12403 move above.
### Pick-any copies new in the objects (no reloc_bindings line, header inlines; bytes v owners NOT measured, cmpcopy not run): DeleteData<T> (Reset rows), ??1<T> copies
  (0x0041D198 0x0042A9B7 0x0042DDB3 0x0064560D 0x0065431D 0x005BFF07 0x0058F016; owners 0x004194B5 / 0x00429BF9 / 0x00429C53 where landed),
  ??4<T> (0x0041D198, 0x0042A9B7; owner 0x004194BA for CGraphicDataBank), ??$?4VCFontBank (0x0042DDB3; owner 0x0042FE34), ??1<void> (0x005BFF07).
### Other
- Tests: unchanged except 0x0048E78E (rows/0048e78e_test.cpp: the row object now needs ReleaseData<CFontBank>; the test defines
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA and explicitly instantiates it). The first recheck run had LINK_FAIL there; fixed and rerun.
- EH funclets / includers: no landed row or test includes any changed row file (grep of rebuild/src, rebuild/tests).
- Headers: none changed (no overlay). Flags lines: none added. Descriptive names: none new.
- q12403 lands alone as exactly 0x0042D45D, 0x0058F016, 0x005C8459, 0x005C9A95 (+ the table move); 0x0041E641 is a family row (it moves
  0x00419108 / 0x0042918C) and must not go with q12403 alone.
- Coordinator decision 1 (0x005BFF07): its in-place respell gives the HEAD local CCountedPointer model an inline ~CCountedPointer, so the object
  now DEFINES pick-any ??1?$CCountedPointer@VCGraphicDataBank@@@@QAE@XZ (owned by the landed thunk 0x004194B5) and ??1?$CCountedPointer@X@@QAE@XZ
  from a local-model body: condition (2) of the in-place-respell rule. Not self-accepted. Alternative if refused: an explicit specialization of
  only the <CGraphicDataBank> destructor in the row, or the full port (SerialisedItems pointees first).
- Coordinator decision 2 (0x0041947A): 3 spellings at HEAD (self stand-in + the two AcquireFont stand-ins) and 3 after (self on the PDB spelling).
  Reading A ("every referencing row ends on the new spelling, or the rename waits"): the family waits for 0x00A5F1C0 / 0x00A634F0. Reading B
  ("an existing split may stay if it does not grow"): the 42 rows could land now with that split left as it is. Not pre-decided here.
- Open-row lookups (no compile): engine/CStreamingFontBank.h line 26 `class CStreamingFontBank : public CFontBank`, so 0x00A5F1C0's Reset argument
  converts on the header; 0x00AB8E10 (CStaticFontBank ctor) has one landed binder, 0x00A634F0 (reloc_bindings ??0CStaticFontBank@@QAE@JJ@Z,
  witness 0x00A634F0), and its self row's leaf is already ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z: porting 0x00A634F0 onto
  engine/CStaticFontBank.h (_CTOR) moves it onto the self spelling (count goes down).
- Evidence check: each of the 23 RENAMES section-1 leaves is present in ghidra_out/fablewin_pdb_names.tsv (grep -cF, all >= 1).
- Locks: 44 rows NF221 (the 42 changed + the 2 open); the other 20 census rows released (work/released.txt).
