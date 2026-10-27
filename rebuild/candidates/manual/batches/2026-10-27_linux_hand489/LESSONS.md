# Chunk 489 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Naming by donor order. Sort ghidra_out/egor_pdb_names.tsv by donor address and the functions of a TU come out in retail order.
  Anchor on one landed neighbour, then read the gap's name off the donor sequence. This proved 0x00789A60 / 0x00789A80
  (CTCHeroRangedAttacks), 0x0078C780 / 0x0078C7F0 (CTCBerserk) and 0x007A6B20.
- The CTCBase virtual slots, in Ego_r donor order, are: 2 OnRemove, 3 ConstructFromParams, 4 OnCreate, 5 InitialActivate,
  6 OnInitialActivate, 7 RenderUpdate, 8 HasFrameUpdate, 9 FrameUpdate, 10 BulletTimeFrameUpdate,
  11 DrawPreparePrimitivesForRendering, 12 Copy, 13 OnDie, 14 OnKill, 15 OnLevelUnload, 16 OnSetInLimbo. The debug build's
  editor-only EditFrameUpdate and EditorDraw* are absent from retail. CTCTrapBase slots 28-31 are InitTrap, UpdateTrap,
  TriggerTrap and ResetTrap.
- Direct member stores followed by a virtual call: VC7.1 hoists the vptr load above the stores. The same stores done through an
  inline member function of the sub-object (`TrapOpener.Close()`) keep retail's order (stores, then `mov eax,[esi]`). Example:
  0x00787BF1.
- STLport `pair<iterator,iterator>` results: give the iterator STLport's user copy ctor. Without it VC7.1 sinks the pair.first store
  below the register pops (0x007776A0).
- A temporary CCharString passed to a call, followed by another call: scope the string in its own `{ }` block. Retail runs the
  string dtor before the next call (0x0079EC10).
- STLport `__pop_heap_aux` → `__pop_heap`: plain `inline` leaves __pop_heap out of line as a cdecl call; `__forceinline` matches
  (0x00758D10).
- A member read before a call, compared after it (`mov esi,[ecx] ; call ; cmp eax,esi`): write a named local copy of the member
  before the call. Written directly, VC7.1 keeps `this` and reloads the member after the call (0x00806820, 27 v 25).
- Implicit destructor of a class whose vector holds polymorphic elements: STLport `_Destroy(&*it)` calls the element's virtual
  dtor through slot 0 with flag 0 (`push 0 ; call [eax]`), then `free`, then a tail jump to the base dtor. Emit it through a helper
  `p->T::~T()` (0x007D1AA0, 0x0079A4D0).
- A `ret 4` filter functor that never reads ecx is an anonymous-namespace `CThingFilter_X::operator()(const CThing&) const`. Find
  its name through the CFilter_BindN operator() that calls it, matched against the BindN instantiation in the donor TU
  (0x007FDA20 ← 0x007FDAD0).
- Modes: CTCCreatureModeManager (interface 0x31) GetCurrentMode 0x0073A6E0 returns a CModeBase whose slot 1 is
  `GetType() const` (unsigned long). To find a mode's class, search the retail text for a `mov eax,<type> ; ret` body labelled
  NCreatureMode::CModeX::vfunc_1. Examples: 0x5f HitByEpicSpell, 0x42 ExplosiveForcePushed.
- The thing bitset word gives the interface kind: `test [thing+0x20+4*(k>>5)], 1<<(k&31)`. For example, +0x3c bit 26 = 0xFA
  (CTCAISummon) and +0x24 bit 17 = 0x31.

Review at landing:
- 007891f2 is two functions in one catalog row (CTCTrapBase::BulletTimeFrameUpdate thunk + the CTCTrapBase::InitTrap `ret` at
  0x007891F7). Split it.
- Local base models sit next to existing engine headers (see NOTES "Header debt"). In particular engine/CTCTrapBase.h models the
  vptr as a data member, so its virtual slots cannot be called; 00787bf1 / 00787bfe use a local model.
- Errors in already-landed sources and the manifest:
  - Landed 0x007BED90 is named SetTimeToDie; the PDB name is SetDieInSeconds.
  - Landed 0x007A0D80 is named FinalCleanup but is OnRemove (a tail jump to the real FinalCleanup 0x007A0950).
  - Landed 0x00787C0C calls 0x00787BF1 as extern "C" `Cleanup`; it is CTCTrapBase::Reset.
  - 0x005EDA50 is labelled CChunkedFileChunk::DestructorCleanup; it is CTCTavernGame::~CTCTavernGame.
  - The manifest's 0x0080E480 "CThingFilter_IsAffectedByObstructions" is the explosion TU's CThingFilter_IsAffectedByExplosions.
- vland RELOC_NAME warning on 008093d0: `CThingSearchToolsMethods::PeekAllThings` 0x0049C770 versus the landed label
  CThingSearchTools::PeekClassList. The model is copied verbatim from landed 0x0052B270, so this is a pre-existing spelling split
  between already-landed sources.
- 00806800 and 00806850 are byte-identical unreferenced __copy_ptrs instantiations with placeholder element types. 0080ff20 is an
  unreferenced GetInterface<CTCAISummon> COMDAT with a descriptive template name.
- Every row matched within the 4-grade budget.
