# Hand chunk 448 (libvorbis icount .. strmbase CEnumPins::Release): 33 of 40 byte-exact

The 40 rows (0x00C9FE80..0x00CA7B00) come from two third-party libraries. None of them is game code.

- **0x00C9FE80..0x00CA3F00 (15 rows): Xiph libvorbis.** This is the copy libpk links statically ("Xiph.Org libVorbis I
  20030909"; the same tree as chunk 446). The rows come from res0.c, floor1.c, floor0.c, lsp.c and lookup.c (lookup.c is
  #included into lsp.c).
  - The rows were placed using the export bundles:
    - residue2_exportbundle @ 0x013ABE04: pack 0x00C9FEA0, unpack 0x00C9FF80, look 0x00CA00E0, free_info 0x00C9FDD0,
      free_look 0x00C9FDF0, class 0x00CA1310, forward 0x00CA1360, inverse 0x00CA15C0.
    - floor0_exportbundle @ 0x013AC244: unpack 0x00CA3600, look 0x00CA3850, free_info 0x00CA3580, free_look 0x00CA35A0,
      inverse1 0x00CA3890, inverse2 0x00CA39C0.
  - Calling conventions after LTCG:
    - API and table functions are `extern "C" __fastcall` (symbols `@name@N`).
    - qsort comparators stay `__cdecl`.
    - Static helpers got custom register conventions (the lane gap; all 7 parked rows).
- **0x00CA4780..0x00CA7B00 (25 rows): Microsoft DirectShow BaseClasses (strmbase).** These are renbase.cpp and
  amfilter.cpp, used by the movie renderer CTextureRenderer.
  - Build: UNICODE (PeekMessageW / PostThreadMessageW / lstrcmpW) and `/Gz`, so free functions are `__stdcall` (isqrt
    `ret 4`, llMulDiv, WaitDispatchingMessages, CompatibleTimeSetEvent).
  - SDK era: DX9-era sources. FindPin has the ASSERT form with an unconditional AddRef. QueryId does
    `CoTaskMemAlloc(8)` + lstrcpyW. CompatibleTimeSetEvent was built with WINVER < 0x0501.
  - The source text came from the public BaseClasses (renbase.cpp / renbase.h / amfilter.cpp), and every body is
    verbatim. Release-build ASSERT / NOTE / DbgLog lines are empty.
  - The class models are one shared prelude: probe/gen/{com,filter,pin,inpin,renderer,video}.txt, composed by
    probe/gen/gen.py from probe/gen/specs/<addr>.py.
  - Layouts from renbase.h / amfilter.h:
    - CBaseFilter is 0x50 bytes: CUnknown, IBaseFilter +0x0C, IAMovieSetup +0x10.
    - CBaseRenderer is 0xE0 bytes.
    - CBaseVideoRenderer: IQualProp +0xE0, IQualityControl +0xE4, statistics from +0xE8.
    - CBasePin is 0x9C bytes.
    - CEnumPins: m_cRef +0x14.

Verification:
- Every src/ file grades MATCH / RELOCATION_MATCH through gall.sh (grades.txt, 33 lines). Every `// Leaf:` was checked
  against `objdump -t`.
- Every test prints PASS under tall.sh (g++) and tall.py (VC7.1/Wine).
- I checked every IAT slot against the PE import table: EnterCriticalSection 0x0143FE0C, LeaveCriticalSection
  0x0143FE10, SetEvent 0x0143FDE0, ResetEvent 0x0143FED4, PeekMessageW 0x01440370, Sleep 0x0143FE1C, GetQueueStatus
  0x014403B0, GetCurrentThreadId 0x0143FE98, PostThreadMessageW 0x014403AC, lstrcmpW 0x0143FED8, lstrcpyW 0x0143FEDC,
  lstrlenW 0x0143FE48, CoTaskMemAlloc 0x01440638, timeGetTime 0x014404B4, MulDiv 0x0143FEE0, WaitForMultipleObjects
  0x0143FEC8, InterlockedDecrement 0x0143FD38, MSVCR71 free 0x01440154.
- I read every GUID constant from the retail .rdata: IID_IBaseFilter 0x012A9AA4, IID_IMediaFilter 0x012A9AB4,
  IID_IPersist 0x012C0B74, IID_IAMovieSetup 0x012A9A14, IID_IMediaEventSink 0x012A9974, IID_IMediaPosition 0x012AA064,
  IID_IMediaSeeking 0x012A9A04.
- CBaseFilter::JoinFilterGraph calls scalar `operator new` 0x00BFEA1A and scalar `operator delete` 0x00BFE9BC for its
  `new WCHAR[]` / `delete[]`, because strmbase declares no array operators. The source declares none either, so the
  object binds `??2@YAPAXI@Z` / `??3@YAXPAX@Z`, as retail does.

## What matched (33)

- **libvorbis (8):**
  - icount 0x00C9FE80 (res0.c bit count).
  - icomp 0x00CA1BC0 (floor1.c int-pointer comparator).
  - floor1_interpolate_fit 0x00CA2B00 (calls `_vorbis_block_alloc` 0x00C8F0D0 `__fastcall`).
  - floor0_free_look 0x00CA35A0 (free through the dllimport IAT, which retail hoists into edi; memset intrinsic over the
    0x20-byte look).
  - comp 0x00CA3B30 (lsp.c descending float comparator).
  - vorbis_coslook 0x00CA3E50, vorbis_invsqlook 0x00CA3EA0 and vorbis_fromdBlook 0x00CA3F00 (lookup.c): verbatim over
    the retail tables COS_LOOKUP 0x013AC260, INVSQ_LOOKUP 0x013AC468, FROMdB_LOOKUP 0x013AC5F8 and FROMdB2_LOOKUP
    0x013AC688, calling the landed vorbis_ftoi 0x00CA3B10 as `extern "C" long __stdcall`.
  - Shape lesson: write the Xiph source verbatim with `extern "C" __fastcall`. The `(float)` casts in lookup.c fold the
    constants exactly as retail does.
- **CBaseRenderer (15):**
  - NonDelegatingQueryInterface 0x00CA4780: the `||` shares the GetMediaPositionInterface block, and it tail-jumps to
    CBaseFilter::NonDelegatingQueryInterface.
  - WaitForReceiveToComplete 0x00CA47D0: MSG is the 28-byte frame. `HIWORD(GetQueueStatus()) & QS_POSTMESSAGE` compiles
    to `shr eax,10h ; test al,8`.
  - FindPin 0x00CA4910: the IBaseFilter thunk: `this` is at +0x0C, and GetPin runs through `[this-0Ch]`.
  - ScheduleSample 0x00CA4AA0: needed CRefTime as a class with `operator REFERENCE_TIME()`.
  - WaitForRenderTime 0x00CA65B0, GetState 0x00CA6620 (calls WaitDispatchingMessages 0x00CB09A0 with its C++ defaults),
    CompleteStateChange 0x00CA6670, EndOfStream 0x00CA6B00, BeginFlush 0x00CA6B40, CompleteConnect 0x00CA6B80 and
    BreakConnect 0x00CA6BC0: renbase.cpp verbatim. SetRepaintStatus 0x00CA4EA0 is out of line.
  - SendEndOfStream 0x00CA6F90: `(Signal - CurrentTime) / 10000` goes through _alldiv. CompatibleTimeSetEvent
    0x00CB0C10 is a C++ `__stdcall`; its body is just `jmp [timeSetEvent]`. It takes the EndOfStreamTimer callback
    0x00CA6F80. All three NotifyEndOfStream returns share one tail.
- **CRendererInputPin (2):**
  - EndOfStream 0x00CA4F90 (/EHsc, two CAutoLocks, then a direct call to CBaseInputPin::EndOfStream 0x00CA8550).
  - QueryId 0x00CA51C0.
- **CBaseVideoRenderer (7):**
  - OnRenderEnd 0x00CA5610: ThrottleWait sits in the same TU and both Sleep calls become tail jumps.
  - get_FramesDroppedInRenderer 0x00CA5D50, get_FramesDrawn 0x00CA5DA0, get_AvgFrameRate 0x00CA5DF0 (MulDiv) and
    get_AvgSyncOffset 0x00CA5E70: IQualProp thunks, so `this` is at +0xE0. The CAutoLock creates an EH frame only when a
    C++ callee is present; with extern "C" imports only there is none.
  - GetStdDev 0x00CA5F50: needs the single-expression
    `*piResult = isqrt((long)((llSumSq - llMulDiv(iTot, iTot, nSamples, 0)) / (nSamples - 1)));`. A named LONGLONG
    temporary leaves a dead high-word store.
  - JoinFilterGraph 0x00CA6110: CBaseVideoRenderer needs DECLARE_IUNKNOWN for an unambiguous QueryInterface.
- **isqrt 0x00CA5EE0 (1):** renbase.cpp verbatim, `__stdcall` because of /Gz.
- **CBaseFilter (2):**
  - NonDelegatingQueryInterface 0x00CA7500: IBaseFilter / IMediaFilter / IPersist need the real interface chain
    `IBaseFilter : IMediaFilter : IPersist`. All three casts fold into one null-checked GetInterface block, and the two
    null arms share `GetInterface(0, ppv)`.
  - JoinFilterGraph 0x00CA7850: /EHsc; scalar new / delete as described above; CopyMemory is `rep movsd ; rep movsb`.
- **CEnumPins (1):** Release 0x00CA7B00 (InterlockedDecrement, declared with a plain `long*` addend).

## Parked for Fable 5.1 (7)

- 0x00CA1310 libvorbis::res2_class DIFFER(59v66): retail `mov eax,ecx` at entry passes vb to `_2class` in eax, while ours passes ecx/edx with a different prologue | tried: Xiph res0.c verbatim with `_2class` declared `__fastcall` | next: lane gap: LTCG custom register convention of the static callee `_2class` 0x00CA0890
- 0x00CA1E00 libvorbis::render_point DIFFER(73v71): retail takes y1 in eax, y0 in edx and x in ecx, with x0/x1 on the stack (`ret 8`); ours is `__fastcall` with x0/x1 in ecx/edx | tried: floor1.c verbatim as `__fastcall` | next: lane gap: LTCG custom register convention (static helper)
- 0x00CA1E50 libvorbis::vorbis_dBquant DIFFER(43v43): only `fld dword ptr [eax]` (retail) against `fld dword ptr [ecx]` (ours) differs | tried: floor1.c verbatim as `__fastcall` | next: lane gap: LTCG custom register convention (x in eax)
- 0x00CA2010 libvorbis::render_line0 DIFFER(146v138): retail takes y1 in eax and y0 in ecx, with x0/x1/d on the stack (`ret 0Ch`); the register assignment differs throughout | tried: floor1.c verbatim as `__fastcall` | next: lane gap: LTCG custom register convention (static helper)
- 0x00CA26A0 libvorbis::post_Y DIFFER(34v23): retail takes A in eax, B in edx and pos in ecx with no stack arguments; ours loads pos from the stack | tried: floor1.c verbatim as `__fastcall` | next: lane gap: LTCG custom register convention (static helper)
- 0x00CA39C0 libvorbis::floor0_inverse2 DIFFER(114v114): retail `push ebx ; mov eax,esi ; call floor0_map_lazy_init` (vb in eax, look in edi) against ours (ecx/edx); the register allocation follows | tried: floor0.c verbatim with floor0_map_lazy_init declared `__fastcall` | next: lane gap: LTCG custom register convention of the static callee 0x00CA36E0
- 0x00CA3A40 libvorbis::cheby DIFFER(200v196): retail takes g in esi and ord in edi (unsaved), while ours is `__fastcall` ecx/edx with the push/pop of esi/edi; VC7.1's 4x unrolled inner loop matches | tried: lsp.c verbatim as `__fastcall` | next: lane gap: LTCG custom register convention (static helper)

## Skipped

None. Every row was either matched or parked with its best source in differ/. All parked rows got 1 grade (the LTCG
register-argument lane gap: park after 1-2 grades).

## Label corrections

Rows in this chunk. Every manifest label was a descriptive or propagated placeholder:
- 0x00C9FE80 Util_PopCount -> libvorbis res0.c `icount`
- 0x00CA1310 Geometry_ProcessNonzeroElements -> libvorbis res0.c `res2_class`
- 0x00CA1BC0 Int_Compare -> libvorbis floor1.c `icomp`
- 0x00CA1E00 Interpolate_ClampedLinear -> floor1.c static `render_point`
- 0x00CA1E50 Clamp_To10BitValue -> floor1.c static `vorbis_dBquant`
- 0x00CA2010 CurveInterpolation_InterpolateInts -> floor1.c static `render_line0`
- 0x00CA26A0 InterpolateInt_Conditional -> floor1.c static `post_Y`
- 0x00CA2B00 PixelBlend_InterpolateWithMemAlloc -> floor1.c `floor1_interpolate_fit`
- 0x00CA35A0 C3DGeneratorObject_Release -> floor0.c `floor0_free_look`
- 0x00CA39C0 Geometry_TransformAndScale -> floor0.c `floor0_inverse2`
- 0x00CA3A40 FloatArray_TransformWithUnroll -> lsp.c static `cheby`
- 0x00CA3B30 Float_CompareStrict -> lsp.c `comp`
- 0x00CA3E50 / 0x00CA3EA0 / 0x00CA3F00 Math_Interpolate{FloatTable,Angle,Power}Lookup -> lookup.c `vorbis_coslook` /
  `vorbis_invsqlook` / `vorbis_fromdBlook`
- 0x00CA4780 FileFormat_DispatchFormat -> CBaseRenderer::NonDelegatingQueryInterface
- 0x00CA47D0 ThreadMsg_PumpAndWaitForCompletion -> CBaseRenderer::WaitForReceiveToComplete
- 0x00CA4910 COM_GetInputPin -> CBaseRenderer::FindPin
- 0x00CA4AA0 Audio_ProcessEventWithCallback -> CBaseRenderer::ScheduleSample
- 0x00CA4F90 Engine_AddPrimitiveToRenderer -> CRendererInputPin::EndOfStream
- 0x00CA51C0 COM_GetOutputPinName -> CRendererInputPin::QueryId
- 0x00CA5610 Audio_UpdateFrameTimings -> CBaseVideoRenderer::OnRenderEnd
- 0x00CA5D50 COM_GetValue -> CBaseVideoRenderer::get_FramesDroppedInRenderer
- 0x00CA5DA0 CritSec_GetField44_Locked -> CBaseVideoRenderer::get_FramesDrawn
- 0x00CA5DF0 GetCalculatedValue_WithTimeElapsed -> CBaseVideoRenderer::get_AvgFrameRate
- 0x00CA5E70 CritSec_GetDividedValue_Locked -> CBaseVideoRenderer::get_AvgSyncOffset
- 0x00CA5EE0 Math_IntegerSquareRoot -> renbase.cpp `isqrt`
- 0x00CA5F50 AudioDevice_ComputeLatencyDelta -> CBaseVideoRenderer::GetStdDev
- 0x00CA6110 CSurface_HandleRefCountDecrement -> CBaseVideoRenderer::JoinFilterGraph
- 0x00CA65B0 Audio_WaitForBufferEvent -> CBaseRenderer::WaitForRenderTime
- 0x00CA6620 Thread_WaitAndGetStatus -> CBaseRenderer::GetState
- 0x00CA6670 SetOrResetEventBasedOnState -> CBaseRenderer::CompleteStateChange
- 0x00CA6B00 AudioThread_RequestStop -> CBaseRenderer::EndOfStream
- 0x00CA6B40 AudioThread_StopAndWait -> CBaseRenderer::BeginFlush
- 0x00CA6B80 CEngine_CheckAndClipBox -> CBaseRenderer::CompleteConnect
- 0x00CA6BC0 CEngine_FinalizeAndClipBox -> CBaseRenderer::BreakConnect
- 0x00CA6F90 ScheduleTimer_ForScreenRefresh -> CBaseRenderer::SendEndOfStream
- 0x00CA7500 Resource_HandleMovableResourceRequest -> CBaseFilter::NonDelegatingQueryInterface
- 0x00CA7850 Audio_SetDeviceSource -> CBaseFilter::JoinFilterGraph
- 0x00CA7B00 InterlockedDecrement_WithCleanup -> CEnumPins::Release

Callees and neighbours (outside the chunk), each checked by disassembly:
- 0x00CACBE0 "CanMoveResource" (CMovableResourceMemoryPool, accepted in the manifest) -> combase.cpp `GetInterface`
  (`CheckPointer(ppv) ; *ppv = pUnk ; pUnk->AddRef()`, `ret 8`).
- 0x00CACCB0 "GUID_CompareAndCallback" -> CUnknown::NonDelegatingQueryInterface (IID_IUnknown @ 0x012C06C4, else
  E_NOINTERFACE).
- 0x00CB0C10 "timeSetEvent" -> wxutil.cpp CompatibleTimeSetEvent. Its whole body is `jmp [timeSetEvent IAT 0x014404A4]`
  (WINVER < 0x0501); 0x00CB0C20 next to it is TimeKillSynchronousFlagAvailable.
- 0x00CB0370 "Math_Divide64bit" -> wxutil.cpp llMulDiv.
- 0x00CB09A0 "OS_WaitForMultipleObjectsWithMessages" -> wxutil.cpp WaitDispatchingMessages.
- 0x00CA6F80 -> renbase.cpp EndOfStreamTimer (`pRenderer->TimerCallback()`, `ret 14h`).
- 0x00CA4C60 -> CBaseRenderer::TimerCallback.
- 0x00CA8550 -> CBaseInputPin::EndOfStream.
- 0x00CA7930 -> CBaseFilter::NotifyEvent.
- CEnumPins vtable 0x012BF69C: QueryInterface 0x00CA7A80, AddRef 0x00CA7AE0, Next 0x00CAAFC0, Skip 0x00CAB0E0, Reset
  0x00CA7B30, Clone 0x00CABED0, scalar deleting destructor 0x00CAAF90.
- libvorbis: 0x00C9FDD0 is res0_free_info (memset of the 0x714-byte vorbis_info_residue0 + free) and 0x00C9FDF0 is
  res0_free_look.

Already-landed rows whose names are wrong (see LESSONS.md, Review at landing).
