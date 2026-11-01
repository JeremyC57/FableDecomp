# Hand chunk 677 (CTCCombat .. CQ_HangingTreeGoodScript): 9 of 10 byte-exact

All nine graded RELOCATION_MATCH (relocation targets masked) and vland confirmed all nine: RELOCATION_MATCH, behaviour PASS
(g++ and VC7.1), no RELOC_IDENTITY. vland warnings are RELOC_NAME only (callee labels that NAME_FIXES.tsv corrects) plus
SHARED_HEADER_AVAILABLE on 0x00CAF230 `CBaseObject`. That warning is a false positive: it is the DirectShow strmbase
CBaseObject (wxdebug/combase.h, release build: only the static object count), not the engine CBaseObject.

## What matched (9)
- **CTCCombat** 0x00735340 `~CTCCombat()`: clears the CombatAbilities multimap (out-of-line `_M_erase` 0x00738EB0), then a
  signed `long` loop with IS_THING_ALIVE / `Kill(true)` over FlourishParticles; after that come the implicit member
  destructors. What made it match: the STLport 4.6 `operator[](n) { return *(begin() + n); }`, which gives retail's re-read
  `mov eax,[esi+88h] ; lea ecx,[ebx+eax]`; `_M_start[n]` and `*(_M_start+n)` give `add ecx,ebx`. Shared headers:
  CCountedPointer, CDefPointer and CCombatAbility (probes 6 and 7, still RELOCATION_MATCH).
- **NScreenEffect::CScreenEffectPriorityStack** 0x00881210 `Clear()`: three `set::clear()` calls, then one counted loop over
  the three `list[3]` arrays, then a tail call to the private `Reset()` 0x00880A40. Built on the landed dtor 0x008814C0 model.
- **std::vector\<CTVertexWaterBackground\>** 0x00B7FDE0 `_M_fill_insert`: STLport 4.6 `_vector.c` text verbatim. The helpers
  must be `__forceinline`, as in the sibling `_M_insert_overflow` 0x00B7FFC0 TU; plain `inline` leaves them out of line
  (317 bytes).
- **CImaAdpcmCodec** 0x00C5F7F0 `DecodeS16` (static `__fastcall`, 7 parameters): the SDK stereo decoder. The SDK's
  `fSuccess = FALSE; break;` ... `return fSuccess;` gives retail's shrink-wrapped `mov eax,1 ; je` early exit. The header is
  read as one DWORD.
- **CEaxContext** 0x00C683C0 `SetContextProperty` (descriptive name): EAX 4.0 context property dispatcher. Case 6
  (LASTERROR) sets `m_lastError = -1` and falls into `default: result = -3; break;`, which gives the re-assigned `mov esi,-3`.
- **CDispParams** 0x00CAE9B0 `CDispParams(UINT, VARIANT*, HRESULT*)`: DX9 SDK ctlutil.cpp text with the oaidl
  `tagVARIANT` union, matched on the first probe. Only scalar `operator new` is declared (no `operator new[]` in scope), so
  `new VARIANT[n]` calls 0x00BFEA1A as retail does. The jump table is inside the graded bytes.
- **CDeferredCommand** 0x00CAF230 constructor (13 parameters, `ret 38h`): DX9 SDK ctlutil.cpp text with
  `CBaseDispatch::GetTypeInfo` defined in the TU, where VC7.1 inlines it as retail does. Needs `// Flags: /EHsc`: the
  m_DispParams / m_Dispatch members give it an EH frame (461v501 without the flag). The `tagVARIANT` / `tagDISPPARAMS`
  spellings give the real mangled names.
- **NScript::CQ_AmbushTradersScript** 0x00CF9350 PDB constructor: engine CLevelScriptBase\<CQ_SunnyvaleMasterData\> plus
  the inline `CTimer()` (RegisterTimer, interface slot 87), six STLport vectors, ten `CCharString[5]` arrays (inline 5-count
  loops, no EH), the STLport 4.6 `_Rb_tree_base` / `_Rb_tree(comp, alloc)` / `_M_empty_initialize` map, and the TeleportInfo
  vector. Matched on the first probe.
- **NScript::CQ_HangingTreeGoodScript** 0x00D782E0 implicit destructor, from the chunk 673 CQ_ArenaScript recipe: the PDB
  ctor is defined empty in the TU and the dtor is implicit. Matched on the first probe.

## Parked for Fable 5.1 (1)
- 0x00AC8360 QuadricOptimiserInternals::CVertexPairContraction::IsIllegal DIFFER(481v471): retail calls the out-of-line
  `set<unsigned long>::find` 0x00AD2790 (hidden-return iterator) for the second HasIndex (Index2); VC7.1 inlines that find
  as it does the first, and `this` lands in ebp instead of esi as a result | tried: local std::set vs the PDB CIndexList,
  inline / non-inline / defined-after HasIndex, non-inline `_Rb_tree::find` template, PDB accessors inline and non-inline,
  `_S_value`/`_S_key` layers, user ctor/dtor, real `insert_unique`, a direct `find != end` test, STLport allocator layering,
  /Ob1 (14 probes plus the flag try; the last 4 left the residue unchanged) | next: lane gap: per-function inline budget.
  Retail spent more of the budget before the second check. Candidates: a heavier inline CArray (the real Lionhead
  CArray/std::vector base) for SubContractions, or the exact STLport `_tree.h` text for `set::insert` / `~set`.

## Skipped
None.

## Label corrections
- 0x00735340 catalog "CChunkedFileChunk_dtor" -> `??1CTCCombat@@UAE@XZ`.
- 0x00881210 "CScreenEffectPriorityStack_Rebuild" -> `?Clear@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ`.
- 0x00AC8360 "QuadricOptimiser_VertexPairContraction_Initialize" -> `?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ`.
- 0x00B7FDE0 "Vector_Insert_14Elements" -> `vector<CTVertexWaterBackground>::_M_fill_insert`.
- 0x00C5F7F0 "Audio_ExpandADPCM_Stereo" -> `CImaAdpcmCodec::DecodeS16`.
- 0x00C683C0 "CDrawGuildSeal_ApplyEffect" (a propagated game name) -> descriptive `CEaxContext::SetContextProperty`.
- 0x00C67F60 "CTCHeroStats_UpdateAttributeEvents" -> descriptive `CEaxContext::CommitDeferredSettings`.
- 0x00CAE9B0 "Variant_CopyVariantArray" -> `CDispParams::CDispParams`.
- 0x00CAF230 "COM_InitializeDispatchContainer" -> `CDeferredCommand::CDeferredCommand`.
- 0x00CF9350 "CHeroMorphDef_Ctor" -> `NScript::CQ_AmbushTradersScript::CQ_AmbushTradersScript`. Also wrong: the landed
  Alloc 0x00CFA2F0 stand-in Create_CClass_00cf9350_00cfa2f0, and 0x00CFAD30 "CHeroMorphDef_Create" is probably wrong
  for the same reason (unchecked).
- 0x00D782E0 "CParticleEmitter_Dtor_5" -> `??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ`.
- More landed spellings are listed in NAME_FIXES.tsv (25 rows).

## Proven
0x00735340 ??1CTCCombat@@UAE@XZ -- first store is vtable 0x0126724C (RTTI .?AVCTCCombat@@); the only caller is ??_G 0x00739D20 in slot 0; Ego_r lists ??1CTCCombat@@UAE@XZ
0x00881210 ?Clear@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ -- Ego_r.pdb + FableWin.pdb Clear; tail-calls the private Reset 0x00880A40; walks the landed dtor's members
0x00880A40 ?Reset@CScreenEffectPriorityStack@NScreenEffect@@AAEXXZ -- Ego_r/FableWin private Reset; Clear's tail-call target
0x00AC8360 ?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ -- Ego_r/FableWin: the only no-arg non-const bool member; CleanUp first; sole caller SimplifyByVertexPairs 0x00ACDA3C
0x00AC7F90 ?HasIndex@CIndexList@QuadricOptimiserInternals@@QBE_NK@Z -- Ego_r/FableWin member; body is `call find 0x00AD2790 ; cmp ; setne`
0x00B7FDE0 ?_M_fill_insert@?$vector@VCTVertexWaterBackground@@V?$allocator@VCTVertexWaterBackground@@@std@@@std@@QAEXPAVCTVertexWaterBackground@@IABV3@@Z -- sole caller resize 0x00B7FD00 (`insert(end(), n-size(), x)`); 56-byte Ego_r element; STLport 4.6 body matches
0x00C5F7F0 CImaAdpcmCodec::DecodeS16 (?DecodeS16@CImaAdpcmCodec@@SIHPAE0IIIPAH1@Z) -- SDK imaadpcm.cpp stereo decoder; Initialize's converter table 0x012B9274 row {DecodeM16, DecodeS16}; the body matches the SDK text
0x00CAE9B0 ??0CDispParams@@QAE@IPAUtagVARIANT@@PAJ@Z -- DX9 SDK ctlutil.cpp text matches byte for byte; called by the CDeferredCommand ctor for m_DispParams +0x34
0x00CAF230 ??0CDeferredCommand@@QAE@PAVCCmdQueue@@PAUIUnknown@@PAJ1NPAU_GUID@@JFJPAUtagVARIANT@@4PAFH@Z -- DX9 SDK ctlutil.cpp text matches; E_ABORT store, DISPID_PROPERTYPUT fix-up, inlined GetTypeInfo strings "LoadRegTypeLib"/"LoadTypeLib"/L"control.tlb", `ret 38h`
0x00CF9350 ??0CQ_AmbushTradersScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- only caller Alloc 0x00CFA2F0 (new 0x1D0), registered at 0x00CD5838; conversion_queue S_QAT -> Q_AmbushTraders -> 0x00CFA2F0; Ego_r ctor symbol (ego_r.exe.signatures.tsv 0x00F82370); Ego_r layout -0x14/-4 fits every store
0x00CFA2F0 ?Alloc@CQ_AmbushTradersScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- the factory above; Ego_r symbol (0x00F8E7E0)
0x00D782E0 ??1CQ_HangingTreeGoodScript@NScript@@UAE@XZ -- only caller ??_G 0x00D782C0 (slot 0 of 0x012D260C) <- ctor 0x00D6EF20 <- Alloc 0x00D78290 beside "S_QHTG"; conversion_queue Q_HangingTreeGood
0x00D782C0 ??_GCQ_HangingTreeGoodScript@NScript@@UAEPAXI@Z -- slot 0 of vtable 0x012D260C
0x00D6EF20 ??0CQ_HangingTreeGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z -- Ego_r/FableWin ctor; only caller Alloc 0x00D78290

Descriptive (not proven): 0x00C683C0 CEaxContext::SetContextProperty, 0x00C60410 CEaxContext::ClearEvents and
0x00C67F60 CEaxContext::CommitDeferredSettings. The EAX emulation layer has no PDB or RTTI; the class comes from the
landed CEaxContext rows and the EAX 4.0 EAXCONTEXT_* order. 0x00AD2790 `set<unsigned long>::find`: the STLport member is
proven, but const vs non-const is not.

## Header debt
- engine/CTCCombat.h is flat: the containers are byte arrays and the CTCBase base is data words. 0x00735340 restates the
  members with real types (offsets match the header). The CTCBase / CIntelligentPointer / CThing models are local copies of
  the landed ~CTCChest 0x00750850's; that landed source has a `#pragma pack(push,1)` around CBaseIntelligentPointer, which
  is not needed (removed here, still a match).
- engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line, but retail's is implicit. 0x00D782E0 keeps
  a local CLevelScriptBase copy without it. 0x00CF9350, a ctor, uses the engine header directly.
- No engine/CCharString.h: the quest rows use the one-pointer local CCharString model.
- No engine header for CScreenEffectPriorityStack, CImaAdpcmCodec, CEaxContext, or the strmbase classes (CDeferredCommand,
  CDispParams, CBaseDispatch, CUnknown, and strmbase's own CBaseObject).
