# Hand chunk 498 (CBasePin .. CPosPassThru): 39 of 40 byte-exact

19 MATCH + 20 RELOCATION_MATCH, no retail_asm rows, 1 SKIP. Every src/ row graded with gall.sh, every test PASS under
g++ (tall.sh) and the lane's VC7.1 (tall.py); the read-only vland dry run reported WINS 39 with no RELOC_IDENTITY /
RELOC_DUPLICATE (only the strmbase `CBaseObject` shared-header warning, see Header debt, and one RELOC_NAME label warning,
see Label corrections). All rows are DirectShow BaseClasses (strmbase, DX9-SDK era, UNICODE release build): amfilter.cpp
(pins, CMediaSample, CBaseAllocator) and ctlutil.cpp (CMediaPosition, CPosPassThru, CDeferredCommand). Models are the
landed strmbase models (CDynamicOutputPin::CompleteConnect 0x00CA9C80, CMediaSample::SetMediaType 0x00CA93F0,
CPosPassThru::GetCurrentPosition 0x00CAF090, CCmdQueue::CheckTime 0x00CAE170) pasted verbatim, plus ITypeInfo's slots to
Invoke and the IMemAllocatorNotifyCallbackTemp definition. Grades: 1 per row, except 0x00CA9FC0 (2: a SRC_FAIL on the
undefined callback interface, then RELOCATION_MATCH).

## Names

Strmbase is in neither PDB and has no RTTI; every name below is the DirectShow BaseClasses source name, proven by the
shipped source body (constants, offsets, IIDs read from .rdata) plus the vtable slot it sits in (strmif.h / control.h
interface order and the classes' declaration order), cross-checked against the landed neighbours in the same tables.

### Proven
- 0x00CA85A0 CBasePin::NewSegment (IPin slot 17 of both pin vtables, cells 0x012BF404 / 0x012BF7A4)
- 0x00CA86B0 CBaseOutputPin::InitAllocator (CDynamicOutputPin primary slot 18; CoCreateInstance(CLSID_MemoryAllocator, IID_IMemAllocator))
- 0x00CA87C0 CBaseOutputPin::GetDeliveryBuffer (primary slot 16)
- 0x00CA87F0 CBaseOutputPin::Deliver (primary slot 17)
- 0x00CA8820 CBaseOutputPin::DeliverEndOfStream (primary slot 19)
- 0x00CA88F0 CBaseOutputPin::DeliverNewSegment (primary slot 22)
- 0x00CA89A0 CBaseInputPin::NonDelegatingQueryInterface (renderer input pin primary slot 0; IID_IMemInputPin, +0x98)
- 0x00CA8E70 CBaseInputPin::Notify (IQualityControl slot 3 next to the landed CBasePin::SetSink)
- 0x00CA8ED0 CBaseInputPin::CheckStreaming (primary slot 14; VFW_E_WRONG_STATE / S_FALSE / VFW_E_RUNTIME_ERROR)
- 0x00CA9000 CMediaSample::QueryInterface (slot 0 of 0x012BF6E0; IID_IMediaSample / IID_IMediaSample2 / IID_IUnknown)
- 0x00CA9100 CMediaSample::GetPointer (slot 3)
- 0x00CA9130 CMediaSample::GetTime (slot 5)
- 0x00CA91A0 CMediaSample::SetTime (slot 6)
- 0x00CA9210 CMediaSample::GetMediaTime (slot 17)
- 0x00CA9250 CMediaSample::SetMediaTime (slot 18)
- 0x00CA9290 CMediaSample::IsSyncPoint (slot 7)
- 0x00CA92E0 CMediaSample::IsDiscontinuity (slot 15)
- 0x00CA9330 CMediaSample::IsPreroll (slot 9)
- 0x00CA9390 CMediaSample::SetActualDataLength (slot 12)
- 0x00CA93B0 CMediaSample::GetMediaType (slot 13)
- 0x00CA9700 CDynamicOutputPin::AddRef (IPin slot 1 of 0x012BF760; target of its adjustor thunks 0x00CAA730 / 0x00CAA760)
- 0x00CA9D70 CBaseAllocator::NonDelegatingQueryInterface (primary slot 0 of 0x012BF83C; IID_IMemAllocator / IID_IMemAllocatorCallbackTemp)
- 0x00CA9FC0 CBaseAllocator::SetNotify (IMemAllocatorCallbackTemp slot 9, next to the landed GetFreeCount)
- 0x00CAA770 CDynamicOutputPin::Release`adjustor{148}' (the compiler's `?Release@CDynamicOutputPin@@WJE@AGKXZ`; IPinFlowControl slot 2 of 0x012BF738)
- 0x00CAD290 CMediaPosition::GetTypeInfoCount (IDispatch slot 3 of the IMediaPosition tables)
- 0x00CAD2B0 CMediaPosition::GetTypeInfo (IDispatch slot 4; calls the landed CBaseDispatch::GetTypeInfo)
- 0x00CAD300 CMediaPosition::Invoke (IDispatch slot 6; GUID_NULL test, DISP_E_UNKNOWNINTERFACE)
- 0x00CAD380 CPosPassThru::NonDelegatingQueryInterface (primary slot 0 of both pass-through vtables)
- 0x00CAD680 CPosPassThru::ConvertTimeFormat (IMediaSeeking slot 13)
- 0x00CAD6D0 CPosPassThru::SetPositions (IMediaSeeking slot 14)
- 0x00CAD720 CPosPassThru::GetPositions (IMediaSeeking slot 15)
- 0x00CAD790 CPosPassThru::GetAvailable (IMediaSeeking slot 16)
- 0x00CAD810 CPosPassThru::SetRate (IMediaSeeking slot 17)
- 0x00CAD970 CPosPassThru::put_StopTime (IMediaPosition slot 11)
- 0x00CADA80 CPosPassThru::put_Rate (IMediaPosition slot 14)
- 0x00CAEC30 CDeferredCommand::NonDelegatingQueryInterface (primary slot 0 of 0x012BF8D4; IID_IDeferredCommand 0x012AA014)
- 0x00CAECC0 CDeferredCommand::GetHResult (IDeferredCommand slot 6)
- 0x00CAECF0 CDeferredCommand::Postpone (IDeferredCommand slot 5; calls the landed CCmdQueue::CheckTime)
- 0x00CAF160 CPosPassThru::GetPreroll (IMediaSeeking slot 19; GetSeekingLongLong through the {76} vcall thunk 0x00CAEF50)

### Probable
Not authored (named from the same tables, for the catalog):
- 0x00CA96F0 CDynamicOutputPin::QueryInterface, 0x00CA9710 CDynamicOutputPin::Release (IPin slots 0 / 2 of 0x012BF760)
- 0x00CAA470 / 0x00CAA480 / 0x00CAA490 CBaseAllocator::QueryInterface / AddRef / Release (DECLARE_IUNKNOWN, slots 0-2 of the
  IMemAllocatorCallbackTemp table 0x012BF810; the concrete class is CMemAllocator, the inline bodies are CBaseAllocator's)
- 0x00CAECB0 CDeferredCommand::Confidence (IDeferredCommand slot 4)

### Descriptive
None.

## What matched (39)
- CBasePin / CBaseInputPin / CBaseOutputPin (9): amfilter.cpp verbatim on the landed pin model. STDMETHODIMP bodies are
  __stdcall on the interface sub-object (NewSegment writes m_tStart at [this+0x74]); the virtual helpers are thiscall.
  CreateMemoryAllocator folds to `call [IAT] ; ret 4`; CheckStreaming's bool test is `neg al ; sbb ; and`.
- CMediaSample (11): amfilter.cpp verbatim; the S_OK/S_FALSE flag tests fold to `not cl ; movzx ; shr ; and 1`.
  This SDK drop's SetMediaTime has no `NULL == pTimeEnd -> E_POINTER` test.
- CBaseAllocator (2): NDQI's `A || B && flag` form; SetNotify needs /EHsc (CAutoLock across AddRef/Release) and
  `CAutoLock lck(this)` keeps the null-checked cast to the CCritSec base.
- CDynamicOutputPin (2): AddRef is DECLARE_IUNKNOWN's inline body, written out of the class so the TU emits it; the
  Release adjustor thunk is emitted with the vftables once the TU defines the destructor.
- CMediaPosition / CPosPassThru (12): ctlutil.cpp verbatim on the landed CPosPassThru model; forwarders keep the noinline
  same-TU GetPeer / GetPeerSeeking stand-ins (landed lesson); NDQI inlines CMediaPosition::NonDelegatingQueryInterface
  defined first in the TU; GetPreroll inlines GetSeekingLongLong (member pointer -> vcall thunk).
- CDeferredCommand (3): ctlutil.cpp on the landed CCmdQueue model; Postpone's COARefTime is `(LONGLONG)(t * UNITS)` (no
  +0.5 in this drop) and CheckTime stays an out-of-line call.

## Parked for Fable 5.1 (0)

## Skipped
- 0x00CAA470: the row holds three functions (0x00CAA470 15 bytes + int3, 0x00CAA480, 0x00CAA490): CBaseAllocator's
  DECLARE_IUNKNOWN QueryInterface / AddRef / Release, each `mov eax,[esp+4] ; mov eax,[eax-8] ; ... ; jmp [ecx+N]`.
  The catalog must re-split it.

## Label corrections
- 0x00CA9700 `_global::Thunk_VirtualMethod_1` -> CDynamicOutputPin::AddRef; likewise 0x00CA96F0 `Thunk_VirtualMethod_0` ->
  CDynamicOutputPin::QueryInterface and 0x00CA9710 `Thunk_VirtualMethod_2` -> CDynamicOutputPin::Release (vland
  RELOC_NAME warning on 0x00CAA770's jump target).
- The other 38 rows carried `_gapscan::sub_<addr>` placeholders.
- Landed 0x00CAEC80 `CAdviseHolder_00caec80::Unadvise` is CDeferredCommand::Cancel (IDeferredCommand slot 3 of 0x012BF8B8,
  VFW_E_ALREADY_CANCELLED 0x80040234 when m_pQueue is null, then m_pQueue->Remove(this)).
- Landed 0x00CAECB0 `gapscan_sub00caecb0` is CDeferredCommand::Confidence (slot 4).

## Callee names
All callees reuse the landed spellings: GetInterface 0x00CACBE0, CUnknown::NonDelegatingQueryInterface 0x00CACCB0,
CBasePin::NonDelegatingQueryInterface 0x00CA7CE0, CreateMediaType 0x00CB1F70, CBaseDispatch::GetTypeInfo 0x00CACD90,
CPosPassThru::GetPeer 0x00CAD400 / GetPeerSeeking 0x00CAD460, CCmdQueue::CheckTime 0x00CAE170 (reloc_bindings.tsv still
spells it `NativeDeferredCommandQueue::CheckTime`; the landed source and symbol are CCmdQueue), __ftol2 0x00BFEA70,
CoCreateInstance / EnterCriticalSection / LeaveCriticalSection IATs.

## Header debt
- None of these classes has an engine header. vland warns SHARED_HEADER_AVAILABLE for `CBaseObject` (the game's own
  CBaseObject header shares the name of strmbase's combase.h CBaseObject); the same warning sits on the landed strmbase rows.
