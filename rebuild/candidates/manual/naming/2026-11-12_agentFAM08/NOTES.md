# agentFAM08 notes (family: 764 landed `CVtable_<vtable>` placeholder rows)

- **Name sources.** None of the 355 family vtables has an RTTI CompleteObjectLocator at vtable-4 (checked in Fable.exe, work/rt.py), and the
  PDB name sets are keyed by donor addresses (no `??_7` entries), so each class was settled from what stores the vtable (ctor / allocator /
  binder), its named slots, the PDB name sets (Ego_r / FableWin spellings) and reloc_bindings.
- **Renamed: 29 rows**, all RELOCATION_MATCH (2 MATCH), all tests PASS under VC7.1 /Od and g++ -std=c++11 (work/tests.tsv). Max 2 grades
  a renamed row (42 grades in all, incl. the 9 on the two held rows; work/gradelog.tsv).
  - 14 deleting destructors: CScriptDataBase, NScript::CalextestData / CEmptyData / Cjoss_test_scriptsData / CDanyalTestScriptsData,
    CLevelScriptNoMasterBase, CSpawnedFuncBase, NScript::CPredicateBase, NScript::CIsVariableFalse, the five-term
    NScript::CPredicate_And<...CIsThingAlive...>, CActiveEntityScriptBase, CFireHeartPatternDef, CConversation,
    NScript::CQ_HeroSoulsArenaScript. Emitter: the class's constructor defined `inline` in the row plus a descriptive external `New<Class>`
    function (FAM07 precedent); for the abstract bases (CScriptDataBase, CPredicateBase) `new` of a derived class with the base constructor
    inline emits the base vtable and its ??_G.
  - 10 constructors: def-class default ctors CArenaRoundDef / CHeroSoulsRoundDef / CArenaWaveDef / CHeroSoulsWaveDef / CFireHeartPatternDef
    (reloc_bindings already bound four of these spellings), the CArenaCreatureDef copy ctor, NScript::CQ_SunnyvaleMasterData,
    CLevelScriptNoMasterBase, and the strmbase CRendererPosPassThru (vendor-proven leaf).
  - 3 destructors: CScriptDefinitionManager, strmbase CMemAllocator and CDeferredCommand (vendor proofs already on the landed rows).
  - 2 `vfunc_4` = CSpawnedFunc<NScript::CQ_WaspBossScript / CCS_OakValeRevisitedScript>::SuspendableProcess (explicit instantiation over
    engine/CSpawnedFunc.h, as the 176 landed instantiations), and `_global::Alloc_CVtable_012ed0d4` = NScript::CV_HeroDuelScript::Alloc.
- **Left: 735** (LEFT.tsv). 733 are libpk (vtables 0x012B1A54..0x012BF1F4, code 0x00C00000..0x00C8FFFF): no RTTI and no PDB covers
  libpk, so no retail class or member name exists; per the brief they are recorded without grades. 53 of them sit in vtables whose functions
  carry a libpk trace-file string (PKCTRList.h, PKCTRArray.cpp, PKCTRRedblackTree.cpp, PKCTRMorphable.cpp, PKCDefaultFileManager.cpp,
  PKCFrallocStatic.h, CCodaFrameStreamer.cpp, PKRASlaveObject_Local.cpp); LEFT names the file per row, but a file name is one source and
  PKCTRList.h / PKCFrallocStatic.h are templates whose T is unknown, so nothing was applied.
  2 are proven but not reproduced: 0x00D30000 / 0x00DA0130 = ??_G?$CActiveEntityScript@V<T>@NScript@@@@UAEPAXI@Z (T = CQ_EndGameBossBattleScript
  / CQ_MinionCampScript). Retail expands the implicit ~CActiveEntityScriptBase into the derived ??_G; VC7.1 over the shared headers calls it
  out of line in every variant tried (work/held/). The landed rows are unchanged.
- **Headers**: 2 opt-ins and 8 new headers (hdr/README.md). The new engine/CArenaWaveDef.h / CArenaCreatureDef.h replace the local
  CArenaWaveDef model FAM07's 0x00F25B40 row carries (header debt for that row).
- **Tests**: 12 catalog tests were __fastcall / __stdcall shims that 64-bit g++ cannot build and 5 only printed their pass line; those 17
  rows get portable twins (rows/<addr>_test.cpp, PassPattern `<addr>_TEST PASS`, test-only Model* types). 0x00F25DD0 keeps the landed test
  respelled (placeholder class -> Model*, Ego_r member names). The other 11 rows keep their landed tests.
- **Strmbase rows** (0x00CAA410, 0x00CAF480, 0x00CAFCE0) keep their local vendor models (strmbase has no shared header) respelled with the
  DX9 BaseClasses names; 0x00CAFCE0 also takes the vendor parameter types (LPCTSTR = const unsigned short*, IUnknown) so its leaf is the
  vendor-proven ??0CRendererPosPassThru@@QAE@PBGPAUIUnknown@@PAJPAUIPin@@@Z; its `volatile long m_cRef` is inherited from the landed source
  (combase.h's volatile LONG m_cRef).
- **Landing hazard**: the EH funclet rows 0x01216DC0 (parent 0x00CAA410) and 0x01217210 / 0x01217218 (parent 0x00CAF480) `#include` the
  parent's landed source BY PATH (00/ca/CVtable012bf83c_CVtable012bf83c_00caa410.cpp, 00/ca/CVtable012bf8d4_CVtable012bf8d4_00caf480.cpp).
  If landing renames those files, update the three includes; codegen of the parents is unchanged (same EH states and callees).
- **NAME_FIXES.tsv**: the ??0CDefRecord34 binder of 0x00F33DC0, five vtable spellings bound by other landed rows (CPredicateBase,
  CIsVariableFalse, CArenaCreatureDef, CFireHeartPatternDef, CQ_HeroSoulsArenaScript, CMemAllocator), and row identities proven on the way:
  0x00CD4500 / 0x00CD4620 / 0x00CD4660 / 0x00CD46A0 / 0x00CD46E0 are the ??0 of CScriptDataBase and the four NScript data classes (landed as
  C3DAnimationUtil), 0x00CDBD70 = ??_GCQ_SunnyvaleMasterData, 0x00CDBD20 = CQ_SunnyvaleMasterData::Alloc. 0x00CD4A60 (CalextestData::Alloc)
  is not in the catalog.
- **PDB ??_E vs retail ??_G**: Ego_r / FableWin spell some of these slots ??_E (CConversation, CalextestData, CScriptDataBase,
  CLevelScriptNoMasterBase); every retail body here tests bit 0 only (scalar form) and VC7.1 emits ??_G for it, so the rows use ??_G.
- **Descriptive names**: only the emitter functions (`New<Class>`, `NewPredicateEmitter`, `NewScriptDataEmitter`) -- never graded.
