# Chunk 450 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 0x00CB0310..~0x00CB2000 is the DX9 SDK DirectShow base-class library (strmbase: wxutil/msgthrd/wxlist/mtype).
  Write the SDK bodies verbatim; 17 of 17 matched on the first grade.
  - CBaseList here has no CBaseObject base and no vtable. CNode has an empty constructor, so `new CNode` is a plain
    scalar new of 0xc bytes.
  - ~CMsgThread is not virtual: GetThreadMsg is vtable slot 0.
  - Model GUID compares as the guiddef.h `!memcmp`, which VC inlines as `repe cmpsd` (GUID_NULL @ 0x012C0694).
- 0x00CB21A0..0x00CB55xx is DXUT ImeUi.cpp from the 2004/2005 SDK: no TSF, no UI-less code, a 22-entry aHotKeys
  table.
  - FableWin.pdb names the public functions `?ImeUi_*@@YI...`, so they are __fastcall under /Gr. Function-pointer
    globals are __fastcall too.
  - imm32 goes through linker import thunks (0x00CB551C..0x00CB5576): declare `extern "C" __stdcall` without
    dllimport.
  - LTCG gave the static helpers custom register conventions (eax/esi/edi/ebx arguments), which is a lane gap.
    Their stack-only callers still differ, because every call site sets up the custom registers (0x00CB2710).
- An inlined helper whose stores are written in its source order (DXUT InitCompStringData: counters first, then
  two memsets) gives retail's schedule: immediate stores between the two `rep stosd` (0x00CB2C60). Out of line
  (0x00CB27F0) both orders compile to the same bytes, so the inlined copy is the evidence for the order.
- `x &= 0xffffff` on a DWORD member compiles to a byte store of 0 to its top byte (0x00CB2E20).
- A byte at a fixed address that is re-read after calls is a global, not a local (g_bIme 0x0143CD28 in
  CheckToggleState 0x00CB47C0).
- Inline CVectorMap::Add (construct a local pair, push_back, Dirty = true) must be `__forceinline` inside a caller
  that is already large. A plain inline member stays an out-of-line call (0x00CB5C90: 148v228).
- STLport inline vector::push_back of a by-value CCharString argument: retail homes the `__false_type` tag for
  _M_insert_overflow on the LIVE by-value argument slot. With _M_insert_overflow declaration-only, VC7.1 homes it
  in a dead argument slot, a one-byte `lea` difference. A same-TU noinline stand-in body of _M_insert_overflow that
  never reads the tag fixes it (0x00CB5C90; this is EXTRA_RULES line 12 applied to vector<CCharString>).
- `CTBaseSingleton<T>::Get()` is not inlined in every TU: in AddScript 0x00CB5C90 it is an out-of-line call
  (0x00414CE0), issued after both by-value arguments are built. In CXMVPlayer it is forceinlined. Declare it
  without a body where retail calls it.
- In STLport partition over CCharString keys, the same-TU noinline CBasicString<char>::operator< stand-in keeps the
  by-value pivot's handle in esi across the calls (0x00CB67B0: 128v119 with an extern compare).
- A /GS row where retail loads a global between the cookie load and the cookie store (0x00CB36E0 SetImeApi) does
  not follow source order, RTM vs QFE 4035, or /G5 or /G7. Park it as a /GS prologue-scheduling gap.
- Lane test pitfall: in g++ (x64) behaviour tests, model HRESULTs as `int`, not `long`. `(long)0x8007000EL` is
  positive on x64, so FAILED() tests silently pass the wrong branch.

Review at landing:
- 0x00CB36E0 SetImeApi had 5 grades (over the 4-grade budget): RTM, kl first, QFE 4035, /G7 and /G5.
- 0x00CB5C90 AddScript carries a same-TU noinline stand-in body for `_STL::vector<CCharString>::_M_insert_overflow`
  (it is not the retail 0x00433530 body). Its symbol is the real STLport mangling already in reloc_bindings.tsv.
- The rows reference the landed callees by their real STLport/strmbase symbols, not the landed rows' descriptive
  class names. reloc_identity reports only RELOC_NAME warnings, all from descriptive labels; no hard errors.
- Already-landed rows whose names or models look wrong (all byte-matched, so names and comments only):
  - 0x00CB13F0 "CPersistTraits::RemoveNodeAndRecycle" is strmbase CBaseList::RemoveI.
  - 0x00CB1210 "CNodeChainOwner_00cb1210::DeleteNodeChain" is CBaseList::CNodeCache::~CNodeCache (m_pHead at +8,
    next at +4).
  - 0x00CB1380 "CCharString_GetLength" (a __stdcall stand-in) is CBaseList::GetNextI(POSITION& rp) const. It is
    __thiscall with an unused ecx (`rp = pn->Next(); return pn->GetData();`).
  - 0x00CB13A0 "CEditControlCentre_PeekConnectedToTrackNode" is CBaseList::GetI(POSITION) const (same `ret 4`
    shape).
  - 0x00CB1AC0 "CCodecState_00cb1ac0::Reset" is CMediaType::InitMediaType: ZeroMemory, lSampleSize (+0x28) = 1,
    bFixedSizeSamples (+0x20) = TRUE.
  - 0x00CB1D00 CMediaType::CMediaType() is described as a "libpk record". It is the strmbase CMediaType default
    constructor (InitMediaType inline).
  - 0x00CB1CC0 (file CMediaType_CMediaType_00cb1cc0.cpp, class CComHolder_00cb1cc0) is ~CMediaType, the inline
    FreeMediaType body.
  - 0x00CB7210 "CStringPairRecordVector_00cb7210::push_back" says the element type is not identified. It is
    `_STL::vector<_STL::pair<CCharString, CScriptInfo>>::push_back` (CScriptInfoManager::Scripts; caller
    AddScript 0x00CB5D47).
  - 0x00CB65D0 CVectorMap::LowerBound's comment says "Dinkumware-style"; the TLC STL is STLport.
  - 0x00CB27F0 InitCompStringData writes its memsets before the counter stores. Both orders give the same bytes out
    of line, but DXUT and the inlined copy in 0x00CB2C60 put the counters first; consider reordering for fidelity.
- Catalog rows near 0x00CB6xxx (__copy_backward 0x00CB62F0, __adjust_heap 0x00CB68C0) are landed as plain
  non-template models or propagated "std::" names. A retemplating pass into `_STL` would give them their retail
  mangled names.
