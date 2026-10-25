# Hand chunk 449 (CEnumMediaTypes .. CAMMsgEvent): 39 of 40 byte-exact

All 40 rows (0x00CA7C30..0x00CB01C0, every one `_global` in the manifest) are Microsoft DirectShow BaseClasses
(strmbase: amfilter.cpp, combase.cpp, ctlutil.cpp, wxutil.cpp/.h). This is the DX9-SDK-era UNICODE release build,
linked in for the movie renderer. It is not Xiph or libpk. The sources follow the BaseClasses text, using
`hr < 0` / `hr >= 0` for FAILED/SUCCEEDED and literal HRESULTs. The shared models live in probe/*.inc:
core (combase/wxutil/reftime), media (amfilter interfaces, CBaseFilter, CBasePin), pins, alloc, list, enum,
disp, pos, evt and cmdq. Each src/ file is header + includes + body; probe/build.txt rebuilds them with probe/mk.sh.

## What matched (39)
- **CEnumMediaTypes** (0xCA7C30 Release, 0xCAB2B0 Skip, 0xCABF70 ctor): interlocked release with `delete this`;
  Skip advances and probes `GetMediaType(m_Position - 1)` into a CMediaType local (needs `/EHsc /GS`, because the
  CMediaType local holds GUID char arrays and has a dtor); the ctor AddRefs the pin and either snapshots the
  version or clones it.
- **CEnumPins** (0xCAB0E0 Skip): the out-of-sync test goes through the filter's GetPinVersion, and the function
  returns S_FALSE past the end.
- **CBasePin** (0xCA7CE0 NonDelegatingQueryInterface, 0xCA7DC0 AttemptConnection, 0xCA7E90 TryMediaTypes, 0xCA7FB0
  AgreeMediaType, 0xCA8420 QueryPinInfo): STDMETHODIMP `this` is the IPin subobject (+0x0C). `riid == IID_x` must be
  the memcmp-intrinsic `operator==` (`repe cmpsd`). These rows use the DX9-era text: QueryPinInfo calls lstrcpynW
  through the IAT, and TryMediaTypes has no `pMediaType &&` guard.
- **CBaseOutputPin** (0xCA8610 CheckConnect, 0xCA86D0 DecideAllocator): CheckConnect only matches with a same-TU
  CBasePin::CheckConnect body, which VC inlines. DecideAllocator uses `ZeroMemory` = the intrinsic memset.
- **CBaseInputPin** (0xCA8CF0 ReceiveMultiple, 0xCA8D50 ReceiveCanBlock, 0xCA8E40 EndFlush): `this` is the
  IMemInputPin subobject (+0x98). ReceiveCanBlock is the DX9 text, with no `if (NULL == pPin) break;`.
- **CMediaSample** (0xCA9080 Release, 0xCA93F0 SetMediaType): the IMediaSample2 primary base; SetMediaType is slot 14.
- **CDynamicOutputPin** (0xCA9860 UnblockOutputPin, 0xCA9A00 StopUsingOutputPin, 0xCA9B70 ChangeMediaTypeHelper,
  0xCA9C80 CompleteConnect, 0xCAC920 Block): the m_BlockStateLock CAutoLock pattern. CompleteConnect needs a
  same-TU CBaseOutputPin::CompleteConnect body (inlined). Block is the IPinFlowControl STDMETHOD (`this` +0xA0).
- **CBaseAllocator** (0xCA9EC0 GetProperties, 0xCAA040 GetFreeCount, 0xCAA160 Decommit): `this` is the
  IMemAllocatorCallbackTemp subobject (+0x0C), and the lock is the CCritSec base. Decommit needs a same-TU
  NotifySample body and `/EHsc` (its CAutoLock spans the virtual Free()).
- **CBaseFilter** (0xCAADB0 FindPin): a CAutoLock around a lstrcmpW loop (`/EHsc`). The DX9 text has no NULL-pin break.
- **combase.cpp** (0xCACAF0 ~CBaseObject, 0xCACB20 LoadOLEAut32, 0xCACC80 CUnknown ctor, 0xCACCB0 CUnknown::
  NonDelegatingQueryInterface): every interface must be `__declspec(novtable)` and CUnknown AM_NOVTABLE. Otherwise
  the ctor stores a vtable (first grade 46v40). The CUnknown ctor inlines a same-TU CBaseObject(const WCHAR*) body,
  and NDQI inlines a same-TU `extern "C"` GetInterface body. LoadOLEAut32 keeps its own static L"OleAut32.dll".
- **CBaseDispatch** (0xCACD90 GetTypeInfo): the second `NULL == pptinfo` test folds away after `*pptinfo = NULL`.
  GetProcAddress goes through the IAT (held in edi), and AmHresultFromWin32 is `or eax, 80070000h`.
- **CPosPassThru** (0xCAD400 GetPeer, 0xCAD460 GetPeerSeeking, 0xCAF090 GetCurrentPosition): the primary base is
  IMediaSeeking at +0, so the new virtual GetMediaTime is slot 20 (+0x50). GetCurrentPosition inlines a same-TU
  GetSeekingLongLong. It also needs a same-TU `__declspec(noinline)` GetPeerSeeking stand-in; without it VC7.1
  treats `&pMS` as captured and reloads pMS after the vcall-thunk call (79v75).
- **CRendererPosPassThru** (0xCADCC0 RegisterMediaTime(LONGLONG, LONGLONG), 0xCADDF0 EOS): m_StartMedia is at
  +0x38 (8-aligned after the CCritSec at +0x1C). EOS calls GetStopPosition virtually through IMediaSeeking slot 11.
- **CCmdQueue** (0xCAE170 CheckTime, 0xCAF750 SetTimeAdvise): CheckTime is inline in ctlutil.h, but retail keeps an
  out-of-line copy, so the source defines it out of the class. The DX9 SetTimeAdvise tests `GetCount() > 0`. The
  modelled vfptr + `virtual ~CCmdQueue` puts m_Lock at +8 by itself (MSVC pads the vfptr to the class's 8-byte
  alignment).
- **CDispParams** (0xCAEBB0 ~CDispParams): the source keeps `delete[] rgvarg`, but no `operator delete[]` may be
  declared. Then VC7.1 binds it to the scalar `??3` (0x00BFE9BC), as retail does (the ctor's `new VARIANT[n]`
  likewise calls 0x00BFEA1A).
- **CAMMsgEvent** (0xCB01C0 WaitMsg): the MsgWaitForMultipleObjects / PeekMessageW loop. The UNICODE build calls
  PeekMessageW, and timeGetTime is the WINMM import.

## Parked for Fable 5.1 (1)
- 0x00CB0160 EnlargedUnsignedDivide DIFFER(53v34): retail `mov eax,[ebp+8]; mov edx,[ebp+0Ch]; mov ecx,[ebp+14h]; div dword [ebp+10h]; or ecx,ecx; jz; mov [ecx],edx; mov [ebp-4],eax` vs ours `call _aullrem` + `call _aulldiv` | tried: C++ reference body (the non-x86 branch of wxutil.h), 1 grade | next: lane gap: retail inline asm (the wxutil.h `#ifdef _X86_` inline-assembler block; land as grade retail_asm with the minimal block and a `// Retail-asm:` line)

## Skipped
- None. No row holds a second function within its length.

## Label corrections
Manifest name -> real name. Most are propagated game/audio names; a few are descriptive.
- 0x00CA7C30 InterlockedDecrement_WithCleanup_AtOffset4 -> CEnumMediaTypes::Release
- 0x00CA7CE0 MovableResourceMemoryPool_CheckGUID -> CBasePin::NonDelegatingQueryInterface
- 0x00CA7DC0 VirtualFileSystem_TransactionExecute -> CBasePin::AttemptConnection
- 0x00CA7E90 AudioBank_ExecuteAsyncTransaction -> CBasePin::TryMediaTypes
- 0x00CA7FB0 AudioBank_LoadAsyncWithRetry -> CBasePin::AgreeMediaType
- 0x00CA8420 GetWideStringData_CopyToOutput -> CBasePin::QueryPinInfo
- 0x00CA8610 COM_QueryInterface_012a9a24 -> CBaseOutputPin::CheckConnect
- 0x00CA86D0 Com_CreateWithFallback -> CBaseOutputPin::DecideAllocator
- 0x00CA8CF0 FindMatchingElement_InArray -> CBaseInputPin::ReceiveMultiple
- 0x00CA8D50 Enumerate_QueryInterface_012a9a24 -> CBaseInputPin::ReceiveCanBlock
- 0x00CA8E40 Audio_ResetFlaggedState -> CBaseInputPin::EndFlush
- 0x00CA9080 DecrementRefcount_AndCleanup -> CMediaSample::Release
- 0x00CA93F0 CBankFileAsyncData_SetImmContext -> CMediaSample::SetMediaType
- 0x00CA9860 Audio_SignalShutdown -> CDynamicOutputPin::UnblockOutputPin
- 0x00CA9A00 NHitLocations_TransferBinaryOut_WithRefcount -> CDynamicOutputPin::StopUsingOutputPin
- 0x00CA9B70 CAudioBank_ExecuteCommand -> CDynamicOutputPin::ChangeMediaTypeHelper
- 0x00CA9C80 Object_AcquireField26 -> CDynamicOutputPin::CompleteConnect
- 0x00CA9EC0 GetVectorFields_Locked -> CBaseAllocator::GetProperties
- 0x00CAA040 GetBufferOffset_Locked -> CBaseAllocator::GetFreeCount
- 0x00CAA160 CScriptDefinitionManager_SignalCompletion -> CBaseAllocator::Decommit
- 0x00CAADB0 FindEntryByString_InList -> CBaseFilter::FindPin
- 0x00CAB0E0 SkipNodes_InList_ByCount -> CEnumPins::Skip
- 0x00CAB2B0 CParticleMorphs_SkipEntries_ByCount -> CEnumMediaTypes::Skip
- 0x00CABF70 CAudioIterator_Initialize -> CEnumMediaTypes::CEnumMediaTypes(CBasePin*, CEnumMediaTypes*)
- 0x00CAC920 AsyncOp_ValidateAndReset -> CDynamicOutputPin::Block
- 0x00CACAF0 Refcount_DecrementUnloadDLL -> CBaseObject::~CBaseObject (strmbase's CBaseObject, not the game's)
- 0x00CACB20 OleAut32_LoadLibrary -> LoadOLEAut32
- 0x00CACC80 CThingMap_ctor -> CUnknown::CUnknown(const WCHAR*, IUnknown*)
- 0x00CACCB0 GUID_CompareAndCallback -> CUnknown::NonDelegatingQueryInterface
- 0x00CACD90 COM_LoadTypeLibrary -> CBaseDispatch::GetTypeInfo
- 0x00CAD400 COM_QueryInterface_Wrapper2 -> CPosPassThru::GetPeer
- 0x00CAD460 COM_QueryInterface_Wrapper -> CPosPassThru::GetPeerSeeking
- 0x00CADCC0 SetTargetVertexNormal -> CRendererPosPassThru::RegisterMediaTime(LONGLONG, LONGLONG)
- 0x00CADDF0 CSurface_GetRefCountAndLock -> CRendererPosPassThru::EOS
- 0x00CAE170 StackCheck_ValidateFrame -> CCmdQueue::CheckTime
- 0x00CAEBB0 Unwind_Cleanup -> CDispParams::~CDispParams
- 0x00CAF090 CThingFilter_CheckThing -> CPosPassThru::GetCurrentPosition
- 0x00CAF750 CModifierGraphic_UpdatePosition -> CCmdQueue::SetTimeAdvise
- 0x00CB0160 EnlargedUnsignedDivide (correct)
- 0x00CB01C0 WaitForObjectWithMessagePump -> CAMMsgEvent::WaitMsg

The callees these rows reach are identified as follows:
- 0x00CA7BB0 CEnumMediaTypes::QueryInterface; 0x00CA7C10 AddRef; 0x00CA7C60 Reset (landed as "CTestPos_Init").
- 0x00CAB160 CEnumMediaTypes::Next; 0x00CABFD0 Clone; 0x00CABE00 CEnumPins ctor.
- 0x00CAC400 / 0x00CAB870 / 0x00CA9820 CDynamicOutputPin::Synchronous/AsynchronousBlockOutputPin / BlockOutputPin.
- 0x00CB1AE0 CMediaType::IsPartiallySpecified; 0x00CB1B20 MatchesPartial; 0x00CB1D00 / 0x00CB1CC0 CMediaType ctor / dtor.
- 0x00CB1F20 DeleteMediaType; 0x00CB1F70 CreateMediaType; 0x00CB0BC0 AmGetLastErrorToHResult.
- 0x00CB13A0 CBaseList::GetI; 0x00CB0190 / 0x00CB01B0 CAMEvent ctor / dtor; 0x00CAEF70 the IMediaSeeking vcall{48} thunk.
- 0x00CACC70 max<ULONG> (LTCG, eax/ecx); 0x00CACBB0 CUnknown(TCHAR*, LPUNKNOWN, HRESULT*); 0x00CACBE0 GetInterface.
- 0x00CAF500 / 0x00CAF580 CCmdQueue ctor / dtor; 0x00CAF8F0 CCmdQueue::GetDueCommand.

The IIDs and strings were read from the image:
- IIDs: 0x012A9B04 IID_IPin, 0x012A99B4 IID_IQualityControl, 0x012A9A24 IID_IMemInputPin, 0x012AA064 IID_IMediaPosition,
  0x012A9A04 IID_IMediaSeeking, 0x012C06C4 IID_IUnknown, 0x012AA0B4 LIBID_QuartzTypeLib, 0x012A93F4 IID_IPinConnection.
- Strings: L"OleAut32.dll" 0x012BF854, "LoadRegTypeLib" 0x012BF894, "LoadTypeLib" 0x012BF888, L"control.tlb" 0x012BF870.
