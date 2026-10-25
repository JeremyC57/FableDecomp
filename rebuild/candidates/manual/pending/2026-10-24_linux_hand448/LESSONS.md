# Chunk 448 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **What the range is.** 0x00CA4780..~0x00CB1xxx is Microsoft's DirectShow BaseClasses (strmbase: renbase.cpp, amfilter.cpp,
  combase.cpp, wxutil.cpp), linked for CTextureRenderer.
  - Build: DX9-era sources, UNICODE, built `/Gz`. Free functions (isqrt 0x00CA5EE0, llMulDiv 0x00CB0370,
    WaitDispatchingMessages 0x00CB09A0, CompatibleTimeSetEvent 0x00CB0C10) are C++ `__stdcall`.
  - Write the rows verbatim from the public renbase.cpp / amfilter.cpp under their real names. The manifest labels are all
    descriptive placeholders (Audio_*, CEngine_*, COM_*).
- **Interface overrides receive `this` at the interface sub-object.** An override of a secondary-base interface method
  gets `this` there, so its members appear shifted low: IBaseFilter +0x0C (FindPin 0x00CA4910, JoinFilterGraph
  0x00CA7850 reads m_pLock at `[ebx+2Ch]`), IQualProp +0xE0 (get_* 0x00CA5D50..). Model the real multiple inheritance
  (`CBaseFilter : CUnknown, IBaseFilter, IAMovieSetup`) and VC7.1 does the adjustment. Never hand-offset.
- **CRefTime must be a class.** Model it as a class with one REFERENCE_TIME and `operator REFERENCE_TIME() const`. A raw
  `__int64` m_tStart changes which registers carry the two halves (ScheduleSample 0x00CA4AA0, 122v124 until fixed).
- **QueryInterface needs DECLARE_IUNKNOWN.** A class with several IUnknown-derived bases (CBaseVideoRenderer: CBaseRenderer
  + IQualProp + IQualityControl) needs the DECLARE_IUNKNOWN QueryInterface / AddRef / Release declarations in the class.
  Without them `QueryInterface(...)` is ambiguous (C2385; JoinFilterGraph 0x00CA6110).
- **CBaseFilter::NonDelegatingQueryInterface (0x00CA7500) needs the real interface chain.** That chain is
  `IBaseFilter : IMediaFilter : IPersist : IUnknown`. The three casts to the same sub-object fold into one null-checked
  `GetInterface` block, and the null arms share one `GetInterface(0, ppv)` call.
- **GUID compares.** `riid == IID_x` (IsEqualGUID) is the memcmp intrinsic `mov ecx,4 ; mov edi,offset ; repe cmpsd`.
  Declare memcmp `extern "C"` and add `#pragma intrinsic(memcmp)`.
- **Array new/delete in strmbase.** strmbase declares no array operators, so `new WCHAR[n]` / `delete[] p` bind scalar
  `??2@YAPAXI@Z` 0x00BFEA1A / `??3@YAXPAX@Z` 0x00BFE9BC, which is what retail calls (0x00CA7850). Do NOT declare
  `operator new[]` / `operator delete[]` in strmbase rows.
- **When a CAutoLock gets an EH frame.** A CAutoLock body whose only callees are extern "C" imports has no EH frame even
  under /EHsc (get_FramesDrawn 0x00CA5DA0). With a C++ callee (llMulDiv, CheckStreaming, CBaseInputPin::EndOfStream)
  the frame and state 0 appear (GetStdDev 0x00CA5F50, CRendererInputPin::EndOfStream 0x00CA4F90).
- **GetStdDev's dead store.** In GetStdDev 0x00CA5F50, a named `LONGLONG x = llSumSq - llMulDiv(...)` temporary leaves a
  dead `mov [esp+38h],edx`. The single expression `*piResult = isqrt((long)((llSumSq - llMulDiv(iTot, iTot, nSamples, 0)) / (nSamples - 1)));`
  matches.
- **A `jmp [IAT]` stub can be a real function.** A wrapper whose whole body is `return Import(same args)`, with the same
  `__stdcall` signature, compiles to a 6-byte `jmp dword ptr [__imp_X]`, which looks exactly like a linker import thunk.
  0x00CB0C10 is wxutil.cpp CompatibleTimeSetEvent (built WINVER < 0x0501), not "timeSetEvent". It sits between wxutil
  functions, and TimeKillSynchronousFlagAvailable follows at 0x00CB0C20. The 16-byte-aligned stubs at 0x00CB0280..0x00CB02C0
  (lstrcpyW / lstrcmpW / lstrlenW slots) are probably the same kind of wxutil wrapper. Before naming such a stub after the
  import, check its neighbours.
- **libvorbis lookup.c.** vorbis_coslook, vorbis_invsqlook and vorbis_fromdBlook (0x00CA3E50..0x00CA3F00) match verbatim
  as `extern "C" float __fastcall`, calling the landed vorbis_ftoi 0x00CA3B10 declared `extern "C" long __stdcall`. The
  `(float)` casts in the Xiph source fold the constants exactly as retail does.
- **LTCG statics: park after 1 grade.** libvorbis static helpers with LTCG register arguments (render_point, render_line0,
  post_Y, vorbis_dBquant, cheby) are the lane gap; park them after 1 grade. A caller whose only problem is such a
  callee's argument registers is the same gap: res2_class 0x00CA1310 (`_2class` takes vb in eax) and floor0_inverse2
  0x00CA39C0 (floor0_map_lazy_init takes vb in eax, look in edi).
- **Forbidden words in comments.** The purity grep also matches comments. renbase.h declares `m_bInReceive` with a cv
  qualifier; say "cv-qualified" in comments. The plain `int` model matches WaitForReceiveToComplete 0x00CA47D0, because
  the loop re-reads the member after its calls anyway.

Review at landing:
- 0x00CA5F50 CBaseVideoRenderer::GetStdDev used the full 4-grade budget and matched on the 4th.
- **No engine headers for strmbase.** The strmbase class models (probe/gen/*.txt, inlined into each source) are local:
  rebuild/include/engine has no CBaseFilter / CBaseRenderer / CBasePin / CEnumPins header. Landed relatives
  (CBaseRenderer_Stop_00ca66d0.cpp, CBaseFilter_*_00ca76xx.cpp, CBaseVideoRenderer_*_00ca5xxx.cpp) carry their own
  models. A shared `strmbase` header would remove the duplication.
- **Already-landed sources with wrong names** (each checked by disassembly or vtable slot):
  - 0x00CA4C60 `unattributed_ClearPending00ca4c60` (CAudioManager_00ca4c60::ClearPending) -> CBaseRenderer::TimerCallback.
    Called from EndOfStreamTimer 0x00CA6F80; locks m_RendererLock +0x94, clears m_EndOfStreamTimer +0xC0, then
    SendEndOfStream (slot 24).
  - 0x00CA4CC0 `unattributed_Restart00ca4cc0` (CAudioStream_00ca4cc0::Restart) -> CBaseRenderer::NotifyEndOfStream.
  - 0x00CA4EA0 `CLockedOwner00ca4ea0_SetContext` -> CBaseRenderer::SetRepaintStatus (m_bRepaintStatus +0xB0 under
    m_RendererLock).
  - 0x00CA4E30 `unattributed_Shutdown00ca4e30` -> CBaseRenderer::StopStreaming (CBaseRenderer vtable 0x012BF498 slot 32).
  - 0x00CA7080 `unattributed_CheckAutoStop00ca7080` -> CBaseRenderer::SendRepaint (m_bAbort / IsConnected / IPin cast
    with null check / NotifyEvent / SetRepaintStatus).
  - 0x00CA7120 `unattributed_StartRendering00ca7120` -> very likely CBaseRenderer::OnDisplayChange (IsConnected under
    m_RendererLock, then AddRef / NotifyEvent / ClearPendingSample / Release).
  - 0x00CA7B30 `CStreamState00ca7b30_Reset` -> CEnumPins::Reset (CEnumPins vtable 0x012BF69C slot 5).
  - 0x00CA7B60 `CStreamState00ca7b60_Reset` -> CEnumPins::Refresh. It is called directly and no vtable references it;
    it re-reads the filter's GetPinVersion / GetPinCount (slots 5 / 6) and zeroes m_Position.
  - 0x00CACBE0 "CanMoveResource" (CMovableResourceMemoryPool, accepted) -> combase.cpp GetInterface. Callers:
    CBaseFilter_SetSyncSource_00ca7680.cpp, CBaseFilter_EnumPins_00cac890.cpp.
  - 0x00CACCB0 "GUID_CompareAndCallback" -> CUnknown::NonDelegatingQueryInterface.
  - libvorbis:
    - 0x00C9FE70 `global_UtilCountHighBits` -> res0.c `ilog`.
    - 0x00CA1790 `global_UtilCountBits` -> floor1.c `ilog`.
    - 0x00CA1750 `global_ClearAndFreeBlock` -> floor1_free_info (0x460-byte vorbis_info_floor1).
    - 0x00CA3580 `_Cons_val<...C3DGeneratorObject>` -> floor0_free_info. It is a 96-byte vorbis_info_floor0, and its
      source models the CRT `free` IAT call as a global named `g_ctorptr`.
    - 0x00C9FDD0 `_Cons_val<...C3DGeneratorObject>` -> res0_free_info (memset of the 0x714-byte vorbis_info_residue0
      + free; residue2_exportbundle slot 3).
    - 0x00CA3B10 `global_FtoL` -> vorbis_ftoi (the body already says so; grade retail_asm).
- **Manifest label on a callee.** 0x00CB0C10 is labelled "timeSetEvent" in the manifest, but it is CompatibleTimeSetEvent
  (see above). Rows that call it should name it that way.
