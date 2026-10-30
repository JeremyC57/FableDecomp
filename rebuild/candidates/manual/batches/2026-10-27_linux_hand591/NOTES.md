# Hand chunk 591 (std::vector<CMatrix3x4> .. WA_GetCandidateList): 10 of 10 byte-exact

All 10 rows are RELOCATION_MATCH. 9 are in src/; 1 (0x00C6B490) is grade `retail_asm` in retail_asm/. Every test prints
`<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py; the retail_asm test was run by hand). Both read-only vland runs
(payload.json and retail_asm_payload.json) report RELOCATION_MATCH PASS with warnings only. The warnings are
RELOC_NAME on catalog labels that NAME_FIXES.tsv corrects, plus SHARED_HEADER_AVAILABLE CBaseObject on 0x00CA6450,
which is the known strmbase name clash.

## What matched (10)
- **0x00AAFE50 std::vector<CMatrix3x4>::_M_insert_overflow** (305v305, 1 grade). STLport 4.6 `_vector.c` is used verbatim:
  the `__false_type` overload, with `_M_clear` inline (null-tested `free`), malloc 0xBFEA0E and free 0xBFEA14. Identity comes
  from the Ego_r PDB, where C3DAnimFileSequenceChunk::Matrices is `CArray<CMatrix3x4>`. The callers are the inline push_backs in
  AddMatrix 0xAAF800 and ReadFromFile 0xAAF850; the element stride is 48.
- **0x00B7C870 std::vector<CPreTransposedBoneMatrix>::_M_insert_overflow** (305v305, 1 grade). It is the byte twin of the row
  above; the class key is U (struct). Its only caller is `_M_fill_insert` 0xB7CAB0.
- **0x00B988E0 std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > >::operator[]** (302v302, 1 grade).
  The match needed the real STLport `_Rb_tree` pieces: `less` derives from `binary_function`, `_Rb_tree_iterator` is a struct
  with a user copy constructor, and `_STLP_alloc_proxy` takes 3 template arguments. The `value_type(k, mapped_type())`
  temporary needs an inline default ctor and an inline dtor (`clear()` plus a deallocate of the header). The callees are
  the inner map copy ctor 0xB97DE0, `insert_unique(iterator, const value_type&)` 0xB985D0 and the inner `_M_erase` 0xB97580.
  The only caller is CEngineScene::GetMapAt 0xB94BF0, and the mangled leaf is in the Ego_r name set.
- **0x00C172F0 Process_00c172f0** (269v269, 3 grades, `/EHsc /GS`; descriptive). This is the libpk bank-file load.
  It wraps the file in a PKCAutoFile and builds a CLoadLog, converts the wide name with WideToMultiByte, calls
  `SetSourceName`, then the same-TU static LoadBank 0xC17260 (CBankData ctor/dtor, ReadBankData, SerialiseBankMetaData),
  and finally `IsOk`. The missing piece was an `add esp,8` after the static call: retail's static is callee-pop, so
  `static void __stdcall LoadBank(...)` matched. VC7.1 also put `log` in esi on its own, as retail does.
- **0x00C6B490 ConvertFloat128BlockToIntInPlace** (307v307, retail_asm; descriptive). It is the libpk mixer's 128-float
  rounding: it saves and restores MXCSR (stmxcsr/ldmxcsr) and has an SSE2 cvtps2dq/movdqa loop, an MMX cvtps2pi/movq/emms
  loop and a scalar FtoL fallback. The `// Retail-asm:` line cites the instructions, and `purity.classify` returns
  `retail_asm`. The scalar loop keeps edx live across FtoL, so FtoL's own `__asm` body must be in the same TU.
- **0x00C75FF0 CCombFilter::Process** (303v303, 2 grades; descriptive). One reverb comb block through `CDspObject::s_pOps`
  0x143CCDC: AddFromRing 0xC76680, ReadRingScaled 0xC764E0, slot-7 Mix and WriteRing 0xC76460, then the ring position
  advances. Grade 1 was 307v303 because an inline `CRingBuffer::Forward` kept `&m_ring` in ebx across the virtual
  call. Writing the advance directly on the members reloads through `this`, which matched.
- **0x00C8FA60 libvorbis vorbis_analysis_wrote** (307v307, 1 grade). block.c is verbatim inside `extern "C"`, with the
  `__fastcall` public API. The same-TU static `_preextrapolate_helper` got v in esi, chosen by VC7.1 itself.
- **0x00C93E30 libvorbis _vp_tonemask** (302v302, 3 grades). psy.c is verbatim inside `extern "C" {}` with the os.h
  min/max macros and seed_curve / seed_loop / seed_chase / max_seeds in the same TU. The missing piece was an `add esp,1Ch`:
  retail's seed_loop and max_seeds are callee-pop, so they are `static __stdcall`. VC then picked p in ebx (seed_loop)
  and p in edi (max_seeds), matching retail.
- **0x00CA6450 CBaseRenderer::GetMediaPositionInterface** (298v298, 2 grades, `/EHsc`). This is DirectShow strmbase
  renbase.cpp, a UNICODE build. The model was reused from the landed CPosPassThru::SetRate 0x00CAD810, with a renbase.h
  CBaseRenderer (`m_pPosition` +0x50, `m_ObjectCreationLock` +0xC4). The EH frame comes from the
  `new CRendererPosPassThru(NAME(...), ...)` expression. Grade 1 was SRC_FAIL ("NULL undeclared"), fixed with `#define NULL 0`.
- **0x00CB2310 WA_GetCandidateList** (302v302, 1 grade). DXUT ImeUi.cpp is used verbatim. It is `/Gr` (`__fastcall`),
  imm32 is called through the non-dllimport thunk 0xCB551C, and the allocators are `ImeUiCallback_Malloc` /
  `ImeUiCallback_Free`. It also uses MultiByteToWideChar and `g_uCodePage` 0x143E8BC.

## Parked for Fable 5.1 (0)
None.

## Skipped (0)
None.

## Label corrections
All 10 chunk labels were propagated or descriptive guesses, and every one was wrong:

| Address | Chunk label | Proven or descriptive name |
|---|---|---|
| 0x00C172F0 | Setup_ProcessGraphicsAndInventory | Process_00c172f0, the libpk bank load (spelling bound by caller 0xC09110) |
| 0x00CA6450 | CStreamContainer_GetData_CompressedInputStream | CBaseRenderer::GetMediaPositionInterface |
| 0x00B988E0 | std_map_find_or_insert_variant | std::map<long, map<long, CCountedPointer<CEngineSceneMap>>>::operator[] |
| 0x00C93E30 | FFT_InitializeWaveform | libvorbis _vp_tonemask |
| 0x00CB2310 | IME_GetCandidateListW | WA_GetCandidateList (the ANSI-IMM wrapper) |
| 0x00C75FF0 | Array_InsertWrappedWithCallbacks | CCombFilter::Process |
| 0x00AAFE50 | Vector_InsertAt | std::vector<CMatrix3x4>::_M_insert_overflow |
| 0x00B7C870 | Vector_Reallocate | std::vector<CPreTransposedBoneMatrix>::_M_insert_overflow |
| 0x00C6B490 | FloatArrayRound_CPU | ConvertFloat128BlockToIntInPlace (descriptive) |
| 0x00C8FA60 | Audio_ProcessConvolutionBuffer | libvorbis vorbis_analysis_wrote |

Callee labels that were also wrong are listed with their evidence in NAME_FIXES.tsv: PKCAutoFile ctor/dtor,
CLoadLog ctor/dtor/SetSourceName, the CRendererPosPassThru ctor spelling (`PBG` / `IUnknown`),
CCombFilter::Process/Blend (`CRingBuffer`, not `CDelayLine`), AddMatrix 0xAAF800 and the InsertGrow stand-ins of 0xAAFE50.

## Names proven vs descriptive
- **Proven:**
  - The two `_M_insert_overflow` rows: PDB member types, callers and the STLport body.
  - map::operator[]: in the PDB name set.
  - vorbis_analysis_wrote and _vp_tonemask: libvorbis source and the exact bodies.
  - CBaseRenderer::GetMediaPositionInterface: strmbase source, vtable slot and body.
  - WA_GetCandidateList: DXUT source and body.
  - PKCAutoFile ctor/dtor: the `.\PKCAutoFile.cpp` string on GetChecked 0xC31DB0.
  - CLoadLog ctor/dtor: shared vtable 0x012B4CC0, with IsOk 0xC307C0 reading +0x30.
- **Descriptive:**
  - Process_00c172f0: kept as the landed caller's spelling.
  - LoadBank, CBankData, ReadBankData, SerialiseBankMetaData and CLoadLog::SetSourceName.
  - ConvertFloat128BlockToIntInPlace.
  - CCombFilter::Process, plus the CRingBuffer / SCombSettings types, which follow the landed 0xC69FB0 / 0xC76680 family.

## Header debt
None. No `rebuild/include/engine` header exists for CMatrix3x4, CPreTransposedBoneMatrix, CEngineSceneMap or the
libpk / strmbase / libvorbis / DXUT classes. The local models use retail layouts: CBaseRenderer offsets come from
the retail accesses, and the STLport containers are 0xC bytes.
