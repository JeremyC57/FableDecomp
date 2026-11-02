# Chunk 704 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 704) A tail-jumping twin pair whose bodies differ in one def offset and one stored mode (0x007FC7A0 / 0x007FC7D0) are two
  sibling members, named by the PDB donor order plus the def field each reads (DelayBeforeLowBeam +0x4C vs DelayBeforeHighBeam
  +0x50). A `mode store ; jmp Action` tail is the class's own SetMode(K) defined above in the TU, inlined and switch-folded by VC7.1
  (the direct `Mode = K; Action();` form gives the same bytes).
- (chunk 704) `fild [ConstantFPS] ; fmul [def+N] ; call _ftol2` is `(long)(def->X * CMainGameComponent::GetConstantFPS())` through the
  PDB inline static accessor -- the fild form reproduces (only the fidiv form is the open item from naming wave 18).
- (chunk 704) A dtor with no vtable store reached from both a ??_G and a CCountedPointer DeleteData is a non-virtual class's ??1:
  follow the DeleteData address to the CCountedPointer<T>::Reset that stores it and to the `new (size)` site that fills the
  pointer (0x006B1550 = ~CAtmosProcess via CWorld::PostInit +0x24). An odd member offset (+0x0A) is a pack(1) CIntelligentPointer.
- (chunk 704) DirectShow strmbase dtors: the real SDK hierarchy (CUnknown and CBaseFilter AM_NOVTABLE, MIDL interfaces
  __declspec(novtable), CBaseObject empty in release) gives the per-subobject vtable stores and the real vtable symbols
  (??_7CBaseVideoRenderer@@6BCUnknown@@@ / 6BIBaseFilter / 6BIAMovieSetup / 6BIQualProp / 6BIQualityControl). Slot 0 of each stored
  vtable (NonDelegatingQueryInterface) names the class even without RTTI.
- (chunk 704) A protected/private callee of another class (CHeightMap::Update AAE called by CMap) keeps its PDB mangling with a
  `friend class CMap;` line in the callee's model.
- (chunk 704) An inline PDB member that retail never emits out of line (CPhysicsState::Reset: CullPhysics store + two list clears)
  is written as an in-class inline over real STLport lists; `list::clear()` forwarding to a declaration-only
  `_List_base::clear()` yields retail's two out-of-line calls (the second as the tail jump).

Review at landing:
- 0x007FC7A0 / 0x007FC7D0 define SetMode, InitTentaclesOnly and InitTentaclesAndHead in the TU (emitted COMDATs under the PDB
  spellings of 0x007FC730 / 0x007FC6F0 / 0x007FC710); the landed 0x007FC6F0 / 0x007FC710 rows are free __fastcall functions sharing
  one spelling -- respell per NAME_FIXES before the bindings meet.
- 0x00CA7310 introduces the real DirectShow vtable spellings for 0x012BF5D8 .. 0x012BF54C; the landed ctor 0x00CA7360 and
  ~CBaseRenderer 0x00CA6370 still bind CVolume* names (RELOC conflict expected until the NAME_FIXES are applied).
- Landed 0x00747110 (CTCStealth::OnSetInLimbo) has `// Leaf: vfunc_16`, which matches no symbol (vland grades the largest function):
  recheck it after the respell. Landed 0x0056CAD0 has `// Leaf: SortTreeRecursively` (same problem).
- No row needed more than 2 grades; no local base model sits beside an existing engine header for `this` except the flat
  generated headers listed under Header debt in NOTES.md.
