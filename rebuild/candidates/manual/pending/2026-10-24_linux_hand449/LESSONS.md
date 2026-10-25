# Chunk 449 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00CA7BB0..0x00CB1F70 (at least) is Microsoft DirectShow BaseClasses (strmbase, DX9 SDK era, UNICODE build), linked in for the
  movie texture renderer. Write it from the BaseClasses text (Win7-samples copy) and apply the DX9-era deltas:
  - QueryPinInfo calls lstrcpynW (0x00CA8420).
  - FindPin and ReceiveCanBlock have no `if (NULL == pPin) break;` (0x00CAADB0, 0x00CA8D50).
  - TryMediaTypes has no `pMediaType &&` guard (0x00CA7E90).
  - SetTimeAdvise tests `GetCount() > 0` and then calls `Get(GetHeadPosition())` (0x00CAF750).
- COM interfaces must be `__declspec(novtable)` (MIDL / DECLARE_INTERFACE), and CUnknown, CBaseFilter, CBasePin,
  CBaseOutputPin, CBaseInputPin, CBaseAllocator and CCmdQueue must be AM_NOVTABLE. Otherwise ctors store vtables that
  retail never writes (0x00CACC80 graded 46v40 until the interfaces were novtable).
- STDMETHODIMP is `__stdcall`, and its `this` is the declaring interface's subobject: IPin +0x0C,
  IMemInputPin +0x98, IMemAllocatorCallbackTemp +0x0C, IPinFlowControl +0xA0, IMediaSeeking +0x00 in CPosPassThru.
  Non-STDMETHOD members are thiscall. Write `riid == IID_x` as the memcmp-intrinsic `operator==` (`repe cmpsd`).
- When retail inlined a callee from its own TU, give that callee a same-TU body:
  - CBasePin::CheckConnect into CBaseOutputPin::CheckConnect (0x00CA8610).
  - CBaseOutputPin::CompleteConnect into CDynamicOutputPin::CompleteConnect (0x00CA9C80).
  - NotifySample into Decommit (0x00CAA160).
  - GetInterface into CUnknown::NDQI (0x00CACCB0).
  - CBaseObject(const WCHAR*) into the CUnknown ctor (0x00CACC80).
  - GetSeekingLongLong into GetCurrentPosition (0x00CAF090).
- The chunk-422 lesson (a same-TU noinline body of an out-of-line callee that takes a local's address) holds for
  COM out-pointers. At 0x00CAF090, `GetPeerSeeking(&pMS)` declaration-only makes VC7.1 reload pMS after the
  member-pointer vcall (79v75). With the same-TU body, pMS stays in a callee-saved register as in retail. The
  retail neighbours 0x00CAF0E0/0x00CAF120/0x00CAF160 (GetStopPosition/GetDuration/GetPreroll) show the same shape.
- How `delete[]` and `new[]` bind: strmbase TUs never see an `operator delete[]` / `operator new[]` declaration.
  - VC7.1 then binds `delete[] p` on a POD to the scalar `??3` (0x00BFE9BC), and `new T[n]` to `??2` (0x00BFEA1A).
    Examples: ~CDispParams 0x00CAEBB0, the CDispParams ctor near 0x00CAE9B0, and ~CBasePin 0x00CA7C80 (`delete[] m_pName`).
  - Declaring `operator delete[]` binds `??_V` (0x00BFEB1C), and q.py STILL prints RELOCATION_MATCH. The in-row
    reloc check does not catch two symbols that resolve to one retail address. Check call targets with `objdump -r`.
  - A delete-expression on a POD emits no null test (operator delete handles NULL), so "no null test" does not mean
    `::operator delete` was called directly.
- A function defined inline in a class body that retail nonetheless keeps out of line (CCmdQueue::CheckTime 0x00CAE170,
  called from GetDueCommand/GetCommandDueFor) must be written out of the class so the TU emits it. The bytes match unchanged.
- MSVC pads the vfptr to the class alignment. CCmdQueue (virtual dtor + LONGLONG/CRefTime members) puts m_Lock at
  +8, not +4. The landed models' "opaque +0..+7" is the vfptr plus padding, not a field.
- `/EHsc` is needed when a CAutoLock spans a C++ (virtual or non-extern-"C") call: FindPin, Decommit.
  `/EHsc /GS` is needed when a CMediaType local exists, because of its dtor and the GUID char arrays (0x00CAB2B0).
  A CAutoLock around extern "C" imports only needs neither (RegisterMediaTime, EOS, GetProperties).
- VC7.1 folds a second null test of a pointer that was already dereferenced: GetTypeInfo's `if (NULL == pptinfo)` after
  `*pptinfo = NULL` (0x00CACD90).
- Model GUID as `typedef struct _GUID {...} GUID;` so that exported names carry `ABU_GUID@@`, as the real headers give.
- 0x00CB0160 EnlargedUnsignedDivide is the wxutil.h x86 inline-assembler body: one `div`, a remainder pointer test, and
  ulResult homed at [ebp-4]. It is a retail_asm candidate, not a C++ shape.

Review at landing:
- No row needed more than 4 grades. 0x00CAF090 had 4 grades plus 1 regrade of an assembled file whose body was
  missing (my file-assembly slip, not a source change). 0x00CACC80 and 0x00CAEBB0 had 2 grades; every other row had 1.
- 0x00CAF090 carries a same-TU external-linkage `__declspec(noinline)` stand-in of CPosPassThru::GetPeerSeeking. It
  has the same symbol `?GetPeerSeeking@CPosPassThru@@AAEJPAPAUIMediaSeeking@@@Z` as 0x00CAD460's source, as the
  stand-in rule asks. Check that the lander or linker accepts the duplicate.
- Other same-TU bodies that export strong symbols:
  - CBasePin::CheckConnect (0x00CA8610).
  - CBaseOutputPin::CompleteConnect (0x00CA9C80).
  - CBaseAllocator::NotifySample (0x00CAA160).
  - CBaseObject::CBaseObject(const WCHAR*) (0x00CACC80).
  - CPosPassThru::GetSeekingLongLong (0x00CAF090).
  - `extern "C" GetInterface` = `_GetInterface@8` (0x00CACCB0). Its retail address 0x00CACBE0 is already landed under
    the wrong C++ name `AcquireInterfaceReference`.
- 0x00CACC80 / 0x00CACB80 are byte-identical CUnknown overloads (LPCTSTR = const WCHAR* vs LPCSTR). The bytes cannot
  tell which address is which. This row takes const WCHAR*, so its symbol differs from landed 0x00CACB80 (const char*).
- strmbase's CBaseObject collides by name with the game's own polymorphic CBaseObject (engine/CBaseObject.h). The
  mangled names still differ: strmbase's dtor is non-virtual `??1CBaseObject@@QAE@XZ`, the game's is `UAE`.
  Landed strmbase rows (0x00CA9D00 ~CBaseAllocator, 0x00CAEFB0, 0x00CAF480) model it as `N_00cacaf0::CBaseObject`.
  This chunk's 0x00CACAF0 / 0x00CACC80 use the real global name. Reconcile the two spellings.
- Errors noticed in already-landed sources:
  - 0x00CACBE0 `global_AcquireInterfaceReference` is `STDAPI GetInterface(LPUNKNOWN, void**)` (extern "C", `_GetInterface@8`).
  - 0x00CACD20 (CUnknown::NonDelegatingRelease) and 0x00CACB80 (CUnknown ctor) declare the Interlocked imports with the
    qualifier that the purity rules ban.
  - 0x00CA9820 "CAudioThreadOwner_00ca9820::ReleaseThread" is CDynamicOutputPin::BlockOutputPin.
  - 0x00CAB870 "CAudioThreadOwner...Attach00cab870" is CDynamicOutputPin::AsynchronousBlockOutputPin.
  - 0x00CAC400 is CDynamicOutputPin::SynchronousBlockOutputPin.
  - 0x00CAF500 "CSyncQueue_00caf500" is CCmdQueue::CCmdQueue. The CCmdQueue ctor/dtor/GetDueCommand rows
    (0x00CAF500/0x00CAF580/0x00CAF8F0) keep +0..+7 opaque; it is the AM_NOVTABLE vfptr plus 8-byte padding.
    0x00CAF8F0's file name says "PeekNextConnectedTrackNode", but its body is CCmdQueue::GetDueCommand.
  - 0x00CA7C60 "CTestPos_Init" is CEnumMediaTypes::Reset.
  - 0x00CA80A0 CBasePin::SetMediaType uses a pad model instead of the CBasePin layout.
  - 0x00CA7C80 "unattributed_CVolumeData00ca7c80" is almost certainly CBasePin::~CBasePin:
    - `delete[] m_pName` +0x14 is the scalar delete; the m_mt CMediaType dtor is at +0x34; the CUnknown/CBaseObject base is at +4.
    - Its comment's "not a delete-expression" reading is wrong (see the delete[] lesson above).
  - 0x00CA8660 "CSoundStream_00ca8660::ReleaseBuffers" is very likely CBaseOutputPin::BreakConnect. The evidence:
    m_pAllocator +0x98 Decommit (slot 6, the failure is returned) then Release, m_pInputPin +0x9C Release, NOERROR.
  - 0x00CAB990 "CAudioBank::LoadAsync" is very likely CDynamicOutputPin::ChangeMediaType. The evidence:
    `!m_Connected` (+0x18) gives VFW_E_NOT_CONNECTED; QI for IID 0x012A93F4 = IID_IPinConnection
    {4A9A62D3-27D4-403D-91E9-89F540E55534} into a QzCComPtr; DynamicQueryAccept; ChangeMediaTypeHelper / DynamicReconnect.
