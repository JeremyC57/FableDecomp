# hand4d batch 10 (2026-11-19) -- first of the two GameCODA rows handed to Account B
- 0x00C77800 CFloatReverbParameterTranslator::UpdateDecayTimes(CFloatReverbParameters*,CHLRvbParams*) -- RELOCATION_MATCH 725 + test PASS
  (strict read-only vland). No flags. Authored C++, NOT library grade: it matched at the first grade.
- Identity: byte-identical (relocations masked) to GameCODA 2.10 CODAApi.lib member FloatReverbParameterTranslator.obj
  ?UpdateDecayTimes@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z (725 bytes, @comp.id 0x0060/3077 =
  a Rich-header entry). Its 12 relocations resolve in retail to the same constants (4f800000 / 3e143d3c / 3f800000) and to
  CFloatFilterParams::Set_ShelfLfHf 0x00C74850, itself byte-identical to the FloatFilter.obj member. The SDK caller Translate is
  byte-identical to retail 0x00C783F0 and calls 0x00C77800 at +0x0C6.
- Source: the CODAApiDbg.lib FloatReverbParameterTranslator.obj CodeView: parameters pReverbParams / pSrcParams; function-scope locals
  fDecaySamples, fDecayFactorHF, i, fDecayFactorLF, fGainRatioHF, uiFilterSpacing, fGainRatioLF, fGain (declared in reverse CodeView record order, i.e. ascending
  frame slot -4..-0x20 (audit 2026-11-19); no alternative order was graded); block locals uiBufferSamples in each loop body and j in the input-filter loop (VC7.1 for-scope block); statement and operand
  order from the /Od instruction order and its ASSERT line constants (no line table; no CodeView record gives statement order) (e.g. `-(n / fDecaySamples)`: fdiv then fchs; Set_ShelfLfHf argument order). The /Od body's ASSERT and
  TRACE blocks (lines 468-600, `_g_pDebugLevels` + OutputDebugPrintf) are compiled out of the release build and are not written.
  Constant 6.907755f (= 0x40DD0C54 exactly; ln 1000 rounds to ...55, so the literal is the 7-digit spelling).
- Overlay (hdr/, copied from today's repo headers): NEW opt-in FABLE_RETAIL_CFLOATREVERBPARAMETERTRANSLATOR_UPDATEDECAYTIMES in
  engine/CFloatReverbParameterTranslator.h (private UpdateDecayTimes declaration), engine/CFloatFilterParams.h (Set_ShelfLfHf + inline
  SetGain), engine/CHLRvbParams.h (friend class CFloatReverbParameterTranslator). No FABLE_STATIC_ASSERT in these headers; default path
  unchanged, so no landed user changes. Details and evidence: hdr/README.md of the workspace.
- Inferred lines: none in the row. Header Inferred: the CHLRvbParams friend (the /Od body reads private members directly; VC7.1 /Z7
  emits no LF_FRIEND records, measured). Descriptive: SetGain's parameter name `fGain` (the inline has no debug record).
- Test: row body verbatim vs an independent double model, 2 cases (ordinary values; sample rate >= 2^31 and an input delay that wraps
  below zero). 31/31 mutants killed: all four loop bounds on both sides (n-1 and <=), three loop starts, the input-sum init, input-delay index i+1, (i+1) spacing, >>2, MaxShort vs MaxLong, the /2,
  LF/HF factor swap, LF/HF reference swap, sample-rate argument, gain sign, the ln1000 constant, echo time source and store target,
  both decay-factor formulas, signed conversion of the sample rate and of the input delay, short buffer index, long gain receiver,
  long delay index.
- vland: WARN RELOC_NAME ?Set_ShelfLfHf@CFloatFilterParams@@QAEXMMMMI@Z -> 0x00C74850 labelled 'CFilter_Setup' (name fix below).
- NAME_FIXES.tsv: 0x00C77800 (catalog _global::CAudioChannel_SetupAudioFilter; binding ?Stage_00c77800@CEffectPart260@@...) and
  0x00C74850 (CFilter_Setup; queue line land0411zc ?SetShelves@CEffectFilter@@ is wrong). For Account A; not applied here.
- Landing (coordinator, 2026-11-19): b10 lands ahead of b11, which is held for its library-route checks. Its CFloatFilterParams.h is b11's
  copy with b11's FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD friend block removed, because an opt-in with no user does not land
  (advisor decision, owner delegated). The audit confirmed the b10 TU preprocesses identically with or without that block.
- Audit (independent, 2026-11-19): PASS.
  - Identity: two sources (the SDK member, plus the Translate call at 0x00C783F0 +0x0C6), and the Dbg S_GPROC32 name.
  - Header: additive (g++ -E -P md5 identical; hdrcheck2 PASS22=same).
  - The friend: the Dbg CodeView marks every member read Private, and the /Od body reads them directly.
  - Test: 25/25 own mutants killed.
