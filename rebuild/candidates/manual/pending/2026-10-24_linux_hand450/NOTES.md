# Hand chunk 450 (strmbase memmoveInternal .. STLport __linear_insert): 32 of 40 byte-exact

The chunk holds three libraries, back to back:

- 0x00CB0310..0x00CB1F70 are the DirectShow base classes (strmbase) from the DX9 SDK: wxutil.cpp, msgthrd.cpp,
  wxlist.cpp and mtype.cpp. This is the retail build: no CBaseObject base on CBaseList, a non-virtual ~CMsgThread,
  and the pre-2005 bodies. They are linked next to the landed CBaseFilter/CMediaType rows.
- 0x00CB21A0..0x00CB5130 are DXUT ImeUi.cpp, the 2004/2005 SDK drop. It has no TSF/UI-less code, and the 22-entry
  aHotKeys table has no ATOK Ctrl+Tab entry; I read the table at 0x013AC760 to confirm. FableWin.pdb names the
  public functions `?ImeUi_*@@YI...`: the TU is /Gr, so they are __fastcall. LTCG gave the file-static helpers
  custom register conventions.
- 0x00CB5AD0..0x00CB6CE0 are game code: CScriptInfoManager, plus the STLport 4.6 algorithms it uses to sort its
  `CVectorMap<CCharString, CScriptInfo>`.

## What matched (32)

**strmbase (17): every row matched on the first grade.** The SDK sources compile verbatim.
- AMGetWideString 0x00CB0940 and AmGetLastErrorToHResult 0x00CB0BC0 are `extern "C" __stdcall`; their leaves are
  `_AMGetWideString@8` and `_AmGetLastErrorToHResult@0`. CopyMemory is the memcpy intrinsic. HRESULT_FROM_WIN32 is
  the SDK's `x <= 0 ? x : ...` macro form.
- CMsgThread::DefaultThreadProc 0x00CB0FC0 is a static __stdcall member. ~CMsgThread is not virtual, so the vtable
  slots are 0 GetThreadMsg, 1 OnThreadInit and 2 ThreadMessageProc. CMsg's default constructor zeroes its four
  members, sharing one zero register with the CoInitialize(NULL) push.
- CBaseList RemoveAll / FindI / AddTailI / AddHeadI / AddTail(CBaseList*) / AddBeforeI (0x00CB1340..0x00CB16A0):
  - Layout: m_pFirst +0, m_pLast +4, m_Count +8, then CNodeCache {m_iCacheSize +0xc, m_iUsed +0x10, m_pHead +0x14}.
  - CNode has an empty constructor, so `new CNode` is a plain scalar new (0x00BFEA1A) of 0xc bytes.
  - The inline Next, GetI and GetNextI keep their SDK NULL tests.
  - `m_pFirst = m_pLast = NULL` stores m_pLast first.
- CMediaType IsValid / AllocFormatBuffer / ReallocFormatBuffer / IsPartiallySpecified / MatchesPartial,
  CopyMediaType, CMediaType(const AM_MEDIA_TYPE&, HRESULT*) and CreateMediaType (0x00CB1920..0x00CB1F70):
  - Each GUID (in)equality is the guiddef.h `!memcmp` intrinsic: `repe cmpsd` against GUID_NULL @ 0x012C0694.
  - min() is the windef.h macro.
  - CopyMediaType has C++ linkage, `?CopyMediaType@@YGJPAUAM_MEDIA_TYPE@@PBU1@@Z`, with a `struct AM_MEDIA_TYPE`
    like the landed callers.
  - FAILED(hr) compiles to the `jge`.

**DXUT ImeUi (9):** ImeUi_SetCompStringAppearance, ImeUi_IgnoreHotKey, ImeUi_FinalizeString, SetCompStringColor,
ImeUi_SetAppearance, _PumpMessage, CheckToggleState, ImeUi_EnableIme and GetCandidateList.
- Model: public functions and function-pointer globals are `__fastcall`. imm32 is called through the linker import
  thunks 0x00CB551C..0x00CB5576 (`extern "C" __stdcall`, not dllimport). user32 and kernel32 calls are dllimport
  (IAT).
- ImeUi_FinalizeString 0x00CB2C60 took 2 grades. Its inlined InitCompStringData must be in DXUT order, counters
  first; the two immediate stores are then scheduled between the `rep stosd`. `lstrlen(g_szCompositionString)`
  stays as a call with a dead result.
- ImeUi_SetAppearance 0x00CB2E20: each `&= 0xffffff` is a byte store of 0 to the colour's top byte.
- CheckToggleState 0x00CB47C0: 0x0143CD28 (g_bIme) is a global, not a local, because it is re-read after calls.
  bIme = ImmIsIME() != 0 (2004/2005 body without the TIP hack).
- GetCandidateList 0x00CB2670: a 3-argument __fastcall static whose address is taken.
- The other rows are the DXUT bodies verbatim.

**CScriptInfoManager (2):**
- GetPScriptInfo 0x00CB5AD0 (1 grade):
  - CVectorMap::Find is inline: `it = LowerBound(key); last = end(); return (it == last || key < it->first) ? last
    : it;`.
  - Then a fresh `it != Scripts.end()` test and `&it->second`.
- AddScript 0x00CB5C90 (3 grades):
  - `section.GetLength() > 0` is the inline null-tested length.
  - `CTBaseSingleton<CCodeSectionManager>::Get()` (0x00414CE0) stays out of line and runs after both by-value
    CCharString arguments are built.
  - STLport's inline vector<CCharString>::push_back.
  - CVectorMap::Add must be `__forceinline`; it builds a local pair, calls push_back 0x00CB7210, destroys the pair
    and sets Dirty.
  - The `__false_type` tag passed to _M_insert_overflow 0x00433530 lands on the live by-value `section` slot only
    when _M_insert_overflow has a same-TU noinline stand-in body. Declaration-only, VC homes the tag in the dead
    `info` slot: one-byte diff, `[esp+30h]` vs `[esp+34h]`.

**STLport 4.6 over pair<CCharString, CScriptInfo> (4):** __unguarded_linear_insert, __unguarded_partition,
__make_heap and __linear_insert.
- They are templates in `std`, defined __fastcall and explicitly instantiated. CKeyPairCompareLess is an empty
  by-value comparator.
- __unguarded_partition 0x00CB67B0 took 2 grades. It needs the same-TU noinline CBasicString<char>::operator<
  stand-in (the landed 0x00429950 body) so the pivot handle stays in esi. With an extern compare it is reloaded
  after every call: 128v119.

## Parked for Fable 5.1 (8)
- 00cb0310 memmoveInternal DIFFER(73v81): retail is wxutil.cpp's two inline-assembly blocks (`cld ; rep movsd ; rep movsb` forward, `std ; rep movsb ; cld` backward); ours is the C++ reference loops | tried: the C++ reference body (1 grade, retail_asm candidate) | next: grade it retail_asm with a `// Retail-asm:` evidence line (wxutil.cpp `#ifdef _X86_` body; VC7.1 C++ cannot emit `std`)
- 00cb21a0 _strtoul DIFFER(148v143): retail `test edx,edx ; ... mov cl,[edx] ... ret` (psz in EDX, ecx unused, the dead ppEnd/nBase dropped); ours `test ecx,ecx ... ret 4` | tried: DXUT body as __fastcall (1 grade) | next: lane gap: LTCG custom register convention
- 00cb2590 ComposeCandidateLine DIFFER(105v87): retail receives index+g_iCandListIndexBase in EAX (`cdq ; idiv` straight away), &g_szCandidate[index] in ECX, the candidate in EDX; ours loads the global and builds both | tried: DXUT body as __fastcall (1 grade) | next: lane gap: LTCG custom register convention
- 00cb26b0 SendControlKeys DIFFER(84v83): retail `mov esi,eax` (num in EAX) and vk in EDI (unsaved); ours takes vk/num in ecx/edx and saves edi | tried: DXUT body as __fastcall (1 grade) | next: lane gap: LTCG custom register convention
- 00cb2710 CancelCompString DIFFER(210v215): the row's own convention matches (stack-only, `ret 0Ch`), but every SendControlKeys call sets up eax/edi in retail vs ecx/edx in ours, which reshuffles the body's register allocation | tried: DXUT level-3 body as __stdcall (1 grade) | next: lane gap: LTCG custom convention of the callee 0x00CB26B0
- 00cb2820 DrawCaret DIFFER(63v50): retail takes x in ECX, y in ESI and height in EBX (all unsaved); ours saves esi/ebx and reads height from the stack | tried: DXUT body as __fastcall (1 grade) | next: lane gap: LTCG custom register convention
- 00cb2d90 SetSupportLevel DIFFER(49v50): retail `cmp eax,2 ; jb ; cmp eax,3` (dwImeLevel in EAX, ecx left free for the inlined FinalizeString's `xor cl,cl`); ours `cmp ecx,2` | tried: DXUT body as __fastcall (1 grade) | next: lane gap: LTCG custom register convention
- 00cb36e0 SetImeApi DIFFER(178v179): retail `mov eax,[cookie] ; mov ecx,[g_hklCurrent] ; mov [esp+108h],eax ; push 104h ; lea eax,[esp+4] ; push eax ; push ecx`; ours stores the cookie after `push 104h` and loads kl into eax late (`mov eax,[g]`, 5 bytes vs 6) | tried: the DXUT order, kl read first, QFE 4035 cl, /G7, /G5 (5 grades) | next: lane gap: /GS prologue scheduling (the same under RTM and QFE)

## Skipped
None.

## Label corrections
These are this chunk's manifest labels. All of them are propagated or descriptive except ImeUi_SetCompStringAppearance,
SetCompStringColor, SetImeApi, ImeUi_EnableIme and AddScript, which are right.

| address | manifest label | real name (evidence) |
|---|---|---|
| 0x00CB0310 | MemMove | memmoveInternal (strmbase wxutil.cpp) |
| 0x00CB0940 | COM_AllocateWideString | AMGetWideString (wxutil.cpp) |
| 0x00CB0BC0 | Win32ErrorToHRESULT | AmGetLastErrorToHResult (wxutil.cpp) |
| 0x00CB0FC0 | COM_CoInitializeMainLoop | CMsgThread::DefaultThreadProc (msgthrd.cpp) |
| 0x00CB1340 | LinkedList_DestroyAll | CBaseList::RemoveAll (wxlist.cpp) |
| 0x00CB13C0 | List_SearchByFieldValue | CBaseList::FindI |
| 0x00CB1460 | List_PushNode | CBaseList::AddTailI |
| 0x00CB14D0 | List_PushNodeReusePool | CBaseList::AddHeadI |
| 0x00CB1540 | List_CopyAllNodes | CBaseList::AddTail(CBaseList*) |
| 0x00CB16A0 | List_InsertBefore | CBaseList::AddBeforeI |
| 0x00CB1920 | Std_MemCmp_Dwords | CMediaType::IsValid (mtype.cpp) |
| 0x00CB19E0 | COM_AllocAndCacheBuffer | CMediaType::AllocFormatBuffer |
| 0x00CB1A40 | COM_AllocAndCacheBuffer_WithCopy | CMediaType::ReallocFormatBuffer |
| 0x00CB1AE0 | AudioBank_MatchesSignature | CMediaType::IsPartiallySpecified |
| 0x00CB1B20 | AudioBank_CompareTransactionKeys | CMediaType::MatchesPartial |
| 0x00CB1BD0 | COM_CopyVariant | CopyMediaType |
| 0x00CB1D60 | COM_CopyVariantSafe | CMediaType::CMediaType(const AM_MEDIA_TYPE&, HRESULT*) |
| 0x00CB1F70 | ImmContext_Create | CreateMediaType |
| 0x00CB21A0 | ParseIntFromString | _strtoul (ImeUi.cpp static) |
| 0x00CB2590 | ImeUI_FormatCompStringDisplay | ComposeCandidateLine |
| 0x00CB2670 | IME_GetCompositionStringW | GetCandidateList |
| 0x00CB26B0 | ImeUi_SendCompositionEvents | SendControlKeys |
| 0x00CB2710 | ImeUi_ProcessComposition | CancelCompString |
| 0x00CB2820 | Graphics_DrawRectConditional | DrawCaret |
| 0x00CB2B90 | ImeUI_CheckCompositionKeyMatch | ImeUi_IgnoreHotKey |
| 0x00CB2C60 | IME_ResetComposition | ImeUi_FinalizeString |
| 0x00CB2D90 | ImeUI_OnModeChange | SetSupportLevel |
| 0x00CB2E20 | SetCurrentInterpolateInfo | ImeUi_SetAppearance |
| 0x00CB2F00 | ProcessAllMessages | _PumpMessage |
| 0x00CB47C0 | ImeUi_QueryInputStatus | CheckToggleState |
| 0x00CB5AD0 | QuestPersist_FindInfoByName | CScriptInfoManager::GetPScriptInfo (Ego_r.pdb) |
| 0x00CB6420 | CParticleMorphEntry_InsertSorted | std::__unguarded_linear_insert<pair<CCharString,CScriptInfo>*, ...> |
| 0x00CB67B0 | MusicEntry_Partition | std::__unguarded_partition<...> |
| 0x00CB6BD0 | ScriptInfo_BuildHeapify | std::__make_heap<...> |
| 0x00CB6CE0 | ScriptInfo_InsertElement | std::__linear_insert<...> |

Callees outside the chunk:

| address | manifest label | real name |
|---|---|---|
| 0x00CB37A0 | ImeUI_SetupLayoutFromKeyboard | ImeUi CheckInputLocale |
| 0x00CB6230 | swap<CMusicManager...> | std::iter_swap over pair<CCharString,CScriptInfo>* |
| 0x00CB5180 | | ImeUi_Initialize |
| 0x00CB2440 / 0x00CB2310 | | likely the ImeUi WA_ImmGetCompositionString / WA_GetCandidateList wrappers (unverified) |

Already-landed rows whose names look wrong (no bytes change; details in LESSONS.md):

| address | landed as | real name |
|---|---|---|
| 0x00CB1210 | CNodeChainOwner_00cb1210::DeleteNodeChain | CBaseList::CNodeCache::~CNodeCache |
| 0x00CB1380 | CCharString_GetLength | CBaseList::GetNextI(POSITION&) |
| 0x00CB13A0 | CEditControlCentre_PeekConnectedToTrackNode | CBaseList::GetI(POSITION) |
| 0x00CB13F0 | CPersistTraits::RemoveNodeAndRecycle | CBaseList::RemoveI |
| 0x00CB1AC0 | CCodecState_00cb1ac0::Reset | CMediaType::InitMediaType |
| 0x00CB1D00 | CMediaType::CMediaType() modelled as a "libpk record" | strmbase CMediaType() |
| 0x00CB1CC0 | CComHolder::Release / ~CMediaType | ~CMediaType: the inline FreeMediaType body |
| 0x00CB7210 | CStringPairRecordVector_00cb7210::push_back | std::vector<pair<CCharString,CScriptInfo>>::push_back (CVectorMap Scripts; caller AddScript 0x00CB5D47) |
