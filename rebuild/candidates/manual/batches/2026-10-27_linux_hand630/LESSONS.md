# Chunk 630 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A returned-by-value C3DVector whose only use is one component: `C3DVector v; v.Z = f().Z - z;` reads the component through the
  hidden-result pointer (`fld [eax+8]`) and parks the result in the dead temporary's words; `float h = f().Z - z` takes its own
  frame slot and a whole `C3DVector v = f(); v.Z -= z;` re-reads the frame copy (0x0084D7E0).
- An object's member reached through an inline accessor ON the pointee (`mgr->GetX()` = `return m_x.Get();`) gives
  `mov eax,[this+N] ; lea ecx,[eax+M]`; reading `mgr->m_x.Get()` from the caller gives `mov ecx,[this+N] ; add ecx,M` (0x0084D7E0).
- Message-filter callers: compile the FindMostRecentMessage<...> template body in the caller's TU (`__declspec(noinline)`, real
  list walk over `filter(message)`) -- the bound filter then does not escape and VC7.1 stores CFilter_Bind2::m_pFirst before the
  second filter's ctor call (0x0092F240; same lever as the landed 0x0090CA60 stand-in, here with the real body).
- 0x006AF290 is CMessageEventFilter_IsTargettedAtThing's ctor (0x006AF2B0 IsOriginatedByThing); check which ??R the searched
  FindMostRecentMessage instantiation calls (0x006AF1E0 Target +0x28, 0x006AF200 Originator +0x20) before naming the filter.
- x87 scheduling of a byte test around an `fadd`: naming the intermediate floats (`float length = v.GetLength(); float r2 = ...;
  d = length - (r1 + r2)`) moves `mov al,[arg] ; test al,al` ahead of the fadd; the one-expression form puts it after (0x00A872F0).
- A def out-slot released by hand: a FUNCTION-SCOPE `CDefPointer<T> slot; ... T* p = slot.m_p; ... if (p && --p->count == 0)
  p->Destroy();` gives `dec [esi+4] ; jne`; the same slot block-scoped or inside an inline helper gives the long
  `mov edx,[esi+4] ; lea eax,[esi+4] ; dec edx ; mov ecx,edx ; test ecx,ecx ; mov [eax],edx` (0x0078C150; cf. GOTCHAS retry-5
  "lane gap" for 0x69A8B0/0x7366C0/0x729410 -- retest those with a function-scope slot).
- Declaring the second of two `const T& v = arr[i]` refs first gives the retail "both loads before both imuls" prologue but
  swaps the registers (0x00AC82A0): register choice follows declaration order, the schedule follows first use.

Review at landing:
- 0x0092F240 changes the identity of 0x006AF290 / 0x006AF1C0 / 0x006AF1E0 / 0x0090DEE0 relative to several landed sources
  (NAME_FIXES.tsv): the several landed callers that declare "IsOriginatedByThing(const CThing*) 0x006AF290" call the IsTargettedAtThing
  ctor; their filters are IsTargettedAtThing (check each caller's FindMostRecentMessage target).
- Landed 0x0084D8B0 GetObservedThingCurrentMeshHeight is spelled without NCameraModes and as non-virtual non-const (PDB UBE).
- 0x00632830's chunk label (list<COpinionTransientOffset>) does not fit the bytes; the FableWin name GFSerialiseMapBinaryIn is used.
- Header debt: CGameCameraManager.h / CPlayer.h type CIntelligentPointer members as byte arrays; CAIStateGroup_* headers flatten
  the base.
- No row needed more than 4 grade rounds in this chunk; 0053e0c0 has now had ~10 forms across sweeps.
