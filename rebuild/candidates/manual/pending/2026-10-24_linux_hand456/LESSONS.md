# Chunk 456 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- An implicit (compiler-generated) `operator=` is emitted out of line by taking its address in a global member pointer:
  `T& (T::*g_p)(const T&) = &T::operator=;` -- STLport `pair` declares none (0x009E6380, 0x004B7650, 0x00A85700, 0x006BA0D0).
- A single pair constructor is emitted with an explicit member instantiation, including the member-template converting
  constructor: `template pair<CIntelligentPointer<CThing>, long>::pair(const pair<CIntelligentPointer<CThing>, int>&);`
  (0x0080DE90, 0x00A20240).
- /Os TUs do not expand small inline helpers: a counted-pointer assignment inside an implicit `operator=` needs
  `__forceinline` on the assignment and on a `Release()` defined on the pointee (one load of the pointer, `dec [ecx+4] ; jne`)
  (0x0059308A, 0x005F853E).
- A counted-pointer assignment whose retail compare is `mov ecx,[this] ; mov eax,[other] ; cmp ecx,eax` reads both pointers
  into locals first (`T* object = Object; T* incoming = other.Object; if (object != incoming)`); `Object != other.Object`
  gives `cmp ecx,[esi+4]` (0x005E1B60).
- `d = 0.0` on a double member is `fld qword [__real@0] ; fstp`, the constant reloaded for every store (0x00AC6CE0).
- Zeroing one triple and then `a = b;` (inlined struct assignment) gives retail's copied-`this` (edx) / copied-zero (esi)
  registers and stores the copied triple first; a plain six-store reset has neither (0x00C6B9A0).
- An STLport deque's `_M_map` must be `_STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> >` built as `(allocator<_Tp*>(), 0)`;
  a plain pointer member schedules the `push 0` one store early (0x00A0E0A0).
- A class with a user (empty, inline) destructor stores its vtable in it; a class whose destructor is implicit does not --
  ~CModeBase stores only CAnimFilterBase's vtable at +8, never CAnimFilterOfGroup's (0x00739E20, as the landed 0x00743D40).
- `std::swap` on a pair whose second is polymorphic: the temporary's copy stores the class's own vtable, the vptr is never
  copied by the assignments, and the temporary dies through the base destructor call (0x00769CB0).
- A pair<K, V> copy ctor where V's user copy ctor has side effects (counters) still emits the whole memberwise copy first,
  then the side effects on `this`'s copied fields (0x00A20670: `inc [NoOfStates]`, then the flag tests on [eax+14h]/[eax+15h]).

Review at landing:
- Grade counts: 0x0059308A used 4 grades (one compile error, one with a mistyped leaf); 0x00C6B9A0 used 4 (three probes +
  the source); no row needed more than 4.
- Local models next to an existing engine header: engine/CDefaultEngineMemoryInit.h flattens the base into a `__vftable` at +0
  (retail has no vptr; 0x00B8FBF0 stores LandscapePoolSize at +0); engine/CScriptConversation.h carries Ego_r's 16-byte
  vectors (retail is 12 bytes; 0x006E6B60); engine/CDefClassBase.h has no member functions (0x00F27060/0x00F27190 model the
  base locally); CNavQuadTree.h / CTCHeroStats.h / CMiniMapDisplay.h exist for the OUTER classes of the nested `this`
  types (CNavigationLayer, MirroredCriminalStatus, CMarkerInfo) -- check strict mode accepts the local nested models.
- Already-landed label errors: 0x00F27650 "CFireheartMinigameDef" ctor is CHeroSoulsCreatureDef's default ctor (vtable
  0x012FB500); in that source 0x0042D9E0 "CPSCBase" is CDefClassBase's ctor; manifest 0x00406780 is
  CUserProfileManager::GetDebugSaveDir, 0x00431F10 CDefClassBase::operator=, 0x0046644C CDefClassBase copy ctor.
- Landed 0x00593924 / 0x00593965 / 0x00593E3E declare the pair at 0x0059308A as `std::pair`; this chunk models it in
  `std` (the retail mangling), so their declared callee names differ from this row's leaf (relocation-masked either way).
- Landed 0x00A205C0 "SCountedInstance_00a205c0 scalar deleting destructor" has exactly pair<const long, CAStarState>'s
  layout and ~CAStarState; worth checking whether it is that pair's (or the map node's) deleting destructor.
- The catalog groups in this chunk ("pair<long", "pair<unsigned_long", "pair<std") each hold unrelated functions;
  re-key them by the identities in STATUS.tsv.
