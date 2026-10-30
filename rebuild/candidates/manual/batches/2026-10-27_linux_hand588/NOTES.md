# Hand chunk 588 (zlib / LZO / libvorbis / STLport vectors / CEngineScene / libpk reverb / strmbase / ImeUi): 9 of 10 byte-exact

1 MATCH + 8 RELOCATION_MATCH, every test PASS under g++ and the lane's VC7.1 (tall.sh / tall.py); read-only vland: 9 wins,
no RELOC_IDENTITY (warnings only: RELOC_NAME against catalog placeholders, see NAME_FIXES.tsv; one false-positive
SHARED_HEADER_AVAILABLE, see Header debt). One row parked (prologue order only).

## What matched (9)
- **zlib 1.2.1** -- 0x00C02350 `updatewindow` (MATCH): inflate.c `local` function verbatim; the leaf is the static
  `_updatewindow` inside `extern "C" { static __declspec(noinline) }` with the real 1.2.1 `z_inflateSetDictionary` as its
  same-TU caller, which gives VC7.1's private convention (out in eax, strm on the stack) exactly as retail's two callers
  (z_inflate 0x00C03756, z_inflateSetDictionary 0x00C03977) use it. First grade.
- **libvorbis 1.0.1 mdct.c** -- 0x00C9C500 `mdct_bitreverse`: the function after mdct_clear in source order; STIN static
  with a same-TU caller (the head of mdct_backward) gets init in eax, x in edx as retail's callers (0x00C9C71E,
  0x00C9C9F9) pass them. HALVE = `x*.5f` (0x0122F59C holds 0.5f, checked in the exe). First grade.
- **STLport 4.6 vector::_M_fill_insert x3** (vector.c verbatim over the real `_Vector_base` + `_STLP_alloc_proxy` model of
  the landed 0x00AE82C0):
  - 0x00AE6E60 vector<C3DQuad2> -- `/GS` (the x_copy C3DQuad2 holds `unsigned short VertexIndex[4]`); both tags homed on the
    live x slot -> same-TU noinline `__uninitialized_copy` and `__copy_backward_ptrs` (both out of line in retail). First grade.
  - 0x00AE7340 vector<pair<CCharString,unsigned long> > -- x_copy built with the CCharString copy ctor, destroyed at the
    end (no EH frame); here `__copy_backward_ptrs` is inline and `__copy_backward` (0x009B1E20) is the out-of-line one,
    with the tag on the live n slot -> same-TU noinline `__uninitialized_copy` + `__copy_backward`. First grade.
  - 0x00BBD950 vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator> -- the 0x00AE6E60 source shape without
    /GS; the element's copy is memberwise (byte + dword) only once STLport's `_List_iterator` carries its real user copy
    constructor (`_List_iterator(const iterator&)`); with a trivial iterator the copy was two dwords (grade 1, 280v280).
- **CEngineScene** -- 0x00B94180 `RenderSceneOcclusion(EEngineRenderLayer, const CEngineCamera&, EEngineOcclusionLayer)`:
  nested STLport map walks (MapList: map<long, map<long, CCountedPointer<CEngineSceneMap> > >) with the inline
  `_Rb_global<bool>::_M_increment` body; RenderStats[layer] through the out-of-line map::operator[] whose STLport body is
  kept in the TU under `__declspec(noinline)`. Declared-only, layer/camera landed in other registers (287v283); the
  noinline body (key reference only read) gives retail's esi/ebx allocation. Grades: 287v283, 283v283 (body inlined: it
  passed the key by value), RELOCATION_MATCH.
- **libpk reverb (descriptive)** -- 0x00C75AB0 `CReverbDiffuser::ProcessRamped`: the 8-argument ramped twin of the landed
  CReverbDiffuser::Process 0x00C75A20 on the same model. Retail computes `j % channels` before the stage-index test: a
  named `channel` local (grade 2); with `inputs[j % ...]` in both arms the div was duplicated (318v277).
- **DirectShow strmbase** -- 0x00CAA290 `CMemAllocator::SetProperties`: slot 3 of the IMemAllocatorCallbackTemp vtable
  0x012BF810 (stored by CMemAllocator::CreateInstance 0x00CAC9D0); the CMemAllocator override (GetSystemInfo, granularity
  check), on the landed CBaseAllocator::GetProperties 0x00CA9EC0 model (trimmed to what the class needs). First grade.
- **DirectX SDK ImeUi.cpp** -- 0x00CB37A0 `CheckInputLocale` (`?CheckInputLocale@@YIXXZ`, /GS, /Gr TU): SDK body; the
  same-TU static `_strtoul` (0x00CB21A0) got retail's private convention (psz in edx, the unused endptr/base dropped) by
  itself; GetImeId (0x00CB28F0, private callee-pop `push 0 ; call`) is declared `__stdcall`, which gives the same call
  site without its large body. First grade.

## Parked for Fable 5.1 (1)
- 0x00C071B0 swd_accept DIFFER(279v279): prologue only -- retail `test ecx,ecx ; je ret ; push ebx ; push esi ; mov esi,ecx`, ours `push esi ; mov esi,ecx ; test esi,esi ; je ; push ebx` (+ the matching epilogue); the loop body 0x00C071C0..0x00C072BD (inlined swd_remove_node, HEAD3/HEAD2 inserts, call swd_getbyte) is identical | tried: plain static (swd_remove_node not inlined, 185v279), `__inline` (same), `__forceinline` swd_remove_node (279v279), `if (n == 0) return; do {} while (--n)` (same) | next: `while (n--)`-style or a different counter variable (n copied to a local before the test) to move the copy after the test; else lane gap: VC7.1 shrink-wrap order of a register-argument static. Test differ/00c071b0.test.cpp PASS (g++ and VC7.1).

## Skipped
None. Every row was checked: one function each, first/last instructions as dis_all.txt (no int3 inside).

## Names and evidence
Proven (two sources each):
- 0x00C02350 `updatewindow`: zlib 1.2.1 source (body, inflate_state offsets, ZALLOC) + neighbours z_inflateReset/z_inflateInit2_ landed + callers z_inflate/z_inflateSetDictionary.
- 0x00C071B0 `swd_accept`: LZO 1.08 lzo_swd.ch body + manifest neighbours (swd_getbyte 0x00C07090, swd_remove_node 0x00C07140 landed, find_match 0x00C07610 its caller).
- 0x00C9C500 `mdct_bitreverse`: libvorbis 1.0.1 mdct.c body + source order after mdct_clear 0x00C9C4C0 + its two callers in mdct_backward / mdct_forward.
- 0x00AE6E60 / 0x00AE7340 / 0x00BBD950 `_M_fill_insert`: STLport 4.6 vector.c body + the spelling each landed resize (0x00AE7AA0, 0x00AE7D10, 0x00BBDD40) already binds; element types from the PDB resize names.
- 0x00B94180 `CEngineScene::RenderSceneOcclusion`: Ego_r.pdb name set (3-argument signature, `ret 0Ch`) + its unique callee RenderCellClippedOcclusion + PDB source order of the Render* family (RenderSceneAndCheckVisibility 0x00B942A0 and RenderEntireScene 0x00B943F0 are landed under their PDB names).
- 0x00CAA290 `CMemAllocator::SetProperties`: vtable slot 3 of CMemAllocator's interface vtable + DirectShow BaseClasses amfilter.cpp body (the GetSystemInfo variant is CMemAllocator's, not CBaseAllocator's).
- 0x00CB37A0 `CheckInputLocale`: FableWin.pdb + Ego_r.pdb name sets (CheckInputLocale / GetImeId / _strtoul) + ImeUi.cpp body + the landed binding `?CheckInputLocale@@YIXXZ`.
Descriptive:
- 0x00C75AB0 `CReverbDiffuser::ProcessRamped`, and its callees `CReverbDiffuser::UpdateGainRamped` 0x00C75820 and `CReverbDiffuserStage::ProcessRamped` 0x00C89350 (libpk: no PDB / RTTI; class and struct names from the landed CReverbDiffuser rows). The row has no reference in the retail image (dead code).
- ImeUi.cpp globals (g_hklCurrent, g_bVerticalCand, g_aszIndicator, g_pszIndicatior, g_uCodePage, LeadByteTable, the function-static hklPrev): the SDK source's names; no PDB data symbols exist to confirm them.
- GetImeId 0x00CB28F0 is declared `__stdcall` (`?GetImeId@@YGKI@Z`) to model retail's private callee-pop convention; its real symbol is a TU-local static.

## Label corrections
- 0x00C02350 catalog `updatewindow` correct.
- 0x00C75AB0 "Graphics_ProcessTerrainStreamingChunks" -> CReverbDiffuser::ProcessRamped (libpk reverb; descriptive). Its callees' manifest labels "Animation_InterpolateAndInsert" (0x00C75820), "Graphics_LoadTerrainChunk" (0x00C756C0), "Partition_ProcessAnimationData" (0x00C89350), "Graphics_ProcessStreamData" (0x00C891A0) are wrong the same way.
- 0x00AE7340 "Vector_Insert_ResizeLogic_Large" -> std::vector<pair<CCharString,unsigned long> >::_M_fill_insert.
- 0x00C071B0 `swd_accept` correct.
- 0x00BBD950 "std_vector_insert_fill_complex" -> std::vector<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>::_M_fill_insert.
- 0x00CB37A0 "ImeUI_SetupLayoutFromKeyboard" -> CheckInputLocale; 0x00CB21A0 "ParseIntFromString" -> ImeUi.cpp _strtoul.
- 0x00CAA290 "AllocateAndValidateMemoryRange" -> CMemAllocator::SetProperties.
- 0x00C9C500 "Mesh_TransformVertices" -> libvorbis mdct_bitreverse.
- 0x00B94180 "CEngineScene_RenderWithOcclusion" -> CEngineScene::RenderSceneOcclusion; the landed siblings 0x00B93F40
  "RenderLayerCells" / 0x00B94060 "RenderLayerCellsShadow" are CEngineScene::RenderScene / RenderSceneShadowBuffer
  (NAME_FIXES.tsv), and their "current-cell marker +0x44 (reset to -1)" is CurrentOcclusionMask = all layers.
- 0x00AE6E60 "Vector_Insert_ResizeLogic" -> std::vector<C3DQuad2>::_M_fill_insert.
- Landed placeholder callee names respelled in NAME_FIXES.tsv (19 rows): the C3DQuad2 / pair<CCharString,unsigned long> /
  CPrimitiveIterator vector helpers, 0x009B71B0 (QAE -> IAE), the CEngineScene / CEngineSceneGrid / RenderStats map
  spellings, and the ImeUi globals.

## Callee names
All callees use their bound spelling where it is right (reloc_bindings.tsv): 0x00AE5880, 0x009B2680, 0x009B1E20, 0x00AE5820,
0x0099EAE0, 0x00BBD550, 0x00BBD7F0, 0x00C756C0, 0x00C891A0, 0x0099EC30 (`??0CCharString@@QAE@ABV0@@Z`, one of its many
bindings), ?CheckInputLocale@@YIXXZ, ?g_hklCurrent@@3PAUHKL__@@A. Respelled (NAME_FIXES.tsv): 0x00AE4D80, 0x00AE4F60,
0x00AE4F90, 0x00AE6A70, 0x00AE5850, 0x009B71B0, 0x00BBD520, 0x00BBD380, 0x00BBD580, 0x00B97B60, 0x00B92790, 0x00B91760,
0x0143E8C0, 0x013AC74C. New descriptive: 0x00C75820, 0x00C89350; new modelled: 0x00CB28F0 GetImeId (__stdcall model),
0x00CB21A0 _strtoul (TU-local static body), the ImeUi globals 0x0143E8D8 / 0x013AC748 / 0x0143E8BC / 0x0143CE08.

## Header debt
- No engine header for CEngineScene, CEngineSceneMap, CEngineSceneGrid, CEngineSceneRenderStats, CCountedPointer, CArray,
  C3DQuad2, CCharString (local models with `// Layout:` comments). CEngineScene's Ego_r layout has 16-byte Dinkumware maps:
  retail members after MapList sit 4 lower (RenderStats +0x2C, CurrentOcclusionMask +0x44).
- engine/CEngineInternalPrimitiveParticleGroup.h: struct key (PDB V), no nested CPrimitiveIterator, PrimitiveIDToIterator
  flattened to `unsigned char[0xc]`; vland's SHARED_HEADER_AVAILABLE warning on 0x00BBD950 is a false positive (`this` is the
  vector; the group class only scopes the element type).
- engine/CBaseObject.h is Fable's class, not strmbase's CBaseObject (0x00CAA290 uses the landed strmbase model, as 0x00CA9EC0).
- No libpk / zlib / LZO / libvorbis / strmbase / ImeUi headers (library code; local upstream-source models).
