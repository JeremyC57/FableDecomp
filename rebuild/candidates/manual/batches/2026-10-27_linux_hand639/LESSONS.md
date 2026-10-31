# Chunk 639 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-length register swap often goes away once the source calls the PDB inline accessors instead of reading members
  directly:
  - `Peek*` type getters (0x00BF9FA0).
  - `CSystemManager::GetWinInstanceHandle()` (0x00A60050).
  - `CWorld::PeekGameTimeManager()`, which fixed the `add eax,1Ch ; mov eax,[eax]` form at 0x0065BE6C.
  - Real CGameScriptInterface slot signatures (0x00E03D30).
  Check the Ego_r/FableWin name sets for `Peek*` / `Get*` accessors before parking a register tie.
- Indexing a vector of handles through a named begin() pointer (`T* handles = v.begin(); handles[i].X()`) gives retail's
  `mov eax,[v] ; lea ecx,[eax+i*N]`. `v[i].X()` and a handle reference do not (0x00D3A050).
- An inline counted-pointer release written as `if (--count == 0)` inside the pointee's member compiles to `dec [p+4] ; jne`.
  A `long Release() { return --RefCount; }` compared in the caller compiles to `dec [p+4] ; cmp dword ptr [p+4],0 ; jne`.
  This is true for signed and unsigned counts (0x0065BE6C, 0x0073ACC0). The release form also moves callee-saved
  register choice across the whole function (0x0065BE6C: the header form gives retail's ebx/edi, the dec;jne form swaps
  them).
- A by-value counted-pointer argument passed by reference to an out-of-line callee (`list::push_back(const T&)`) makes
  VC7.1 re-read the argument slot after every store. A same-TU `__declspec(noinline)` body of that callee (the real
  STLport push_back: operator new(0x10), copy, link before the sentinel) gives retail's cached block register
  (0x0073ACC0 / 0x007421C0).
- A loop helper defined in the class (`bool IsModeActive(unsigned long) const` with a find loop) is not expanded by VC7.1
  even in a speed TU. Retail expanded it at 0x0073ACC0 while keeping an out-of-line copy at 0x0073A8A0, so it needs
  `__forceinline`.
- A nested class used as a template argument of a member container (`list<CCountedPointer<Outer::CModeBase>> PModes`) can
  be modelled with a partial specialisation `template <class T> class list<CCountedPointer<T>, allocator<...> >` declared
  before Outer. An explicit specialisation would need the nested class first, and it cannot be forward-declared outside
  Outer.
- When a vtable slot is `_purecall` in the base, name it from a derived override in FableWin.pdb. A derived class that
  overrides exactly one bool-const virtual pins that slot. For example, CModeBriarRoseTeleportInvulnerable slot 11
  0x008C36A0 = IsRemoveOnDeactivate (0x0073ACC0).

Review at landing:
- Rows that needed more than 4 grades:
  - 0x00B92300 (5 probes this session, plus 3 in the sweep).
  - 0x0073ACC0 (7).
  - 0x0065BE6C (8).
  - 0x00641AE0 (6).
  - 0x00BA0FC0 (4).
  - 0x00894EC0 (5 before its match).
  - 0x00BF9FA0 / 0x00D3A050 / 0x00A60050 (3-4 each).
- Local models next to existing engine headers:
  - 0x00D3A050: row-local CScriptGameResourceObjectScriptedThingBase (the shared header expands the implicit
    ctor/dtor).
  - 0x00A60050: CJoystick/CMouse/CKeyboard local (the headers flatten the vptr).
  - 0x00894EC0: CThing local (engine/CThing.h is quarantined); CThingSearchTools uses the Layout include pattern.
  - 0x00BF9FA0: CLocalDetailPrimitiveMeshZSpriteBatch modelled over CMovableResource -> CLocalDetailPrimitiveBase virtuals.
  - Parked 0x0073ACC0 / 0x0065BE6C: local CCountedPointer / CDefPointer, because the shared header release shapes
    differ (NOTES.md Header debt).
- 0x00894EC0 matches through same-TU stand-in bodies for the four GetAllThings specialisations. The bodies call
  undefined helpers `FindThings_None/HasTC/HasDefName`, which are never referenced by the graded function. Confirm the
  lander accepts them as stand-in callees.
- Errors in ALREADY-LANDED sources:
  - The CModeBase virtual names used by RemoveMode 0x0073ABA0 (slots 5/6 "OnResume/OnLeave") and ClearModes 0x0073AE70
    (slot 6 "OnLeave") are descriptive and wrong. They are OnActivate (slot 5 = 0x0073AAC0, landed with that name) and
    OnDeactivate (slot 6 = 0x00739E60, `Active = false`, FableWin.pdb name set). Virtual slots do not appear in
    reloc_bindings, so these are not in NAME_FIXES.tsv; respell when those rows are next touched.
  - CIsHornTaskWithThisHorn::operator() 0x0065F116 is non-const and models PHorn as a base (NAME_FIXES.tsv).
- The landed caller 0x00BFACC0 spells SetupPrimitiveDesc QAE with an unqualified CAddPrimitiveDesc; the correct
  spelling is in NAME_FIXES.tsv.
