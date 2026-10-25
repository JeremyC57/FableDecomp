# Hand chunk 461 (std::list .. std::vector): 38 of 40 byte-exact

All 38 src/ rows grade MATCH or RELOCATION_MATCH through gall.sh (006F8EA0 is a full MATCH; the rest are
RELOCATION_MATCH). Every test passes under g++ (tall.sh) and VC7.1 (tall.py). calls.py finds every call target
equal to retail's, including the imports malloc 0xBFEA0E, free 0xBFEA14 and memmove 0xBFEAE6. Everything is modelled
as STLport 4.6 templates in namespace `_STL`. Line 1 says `std::` because STLport maps `std` onto `_STL`. Where a
template argument contains `::`, line 1 uses the plain template name and the description gives the element type.

## What matched (38)

### std::list
- 004046B0 `list<CStringParserCommentBoundaries>::~list`: frees every node, then the sentinel. The inline `clear()`
  advances the cursor before the free, so the cursor lives in esi and the sentinel is re-read from `this`. The
  element is a POD, so there is no destructor. The node free has no null test; the sentinel free does. About 90
  destructors share this body through COMDAT folding.
- 00534D30 `list::erase(first, last)` (list<NUISystem::CComponent*>): `while (first != last) erase(first++); return
  last;` with the single erase inline. The `_Nonconst_traits` iterator has a user copy ctor, so the result goes
  through the hidden pointer.
- 0067CF40 `list<CReactionMatch*>::operator=`: STLport 4.6 operator= as written in the source. The mutable iterator's
  user copy ctor is built in its argument slot (`push ecx ; mov ecx,esp`). The const_iterators are pushed directly.
  erase's result is homed in the dead `x` slot.
- 0076FF90 `list::operator=` (list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>): the same as 0067CF40, but
  `insert(pos, f, l)` is the inline forwarder to the out-of-line `_M_insert_dispatch(.., __false_type())`. Its tag
  lands on the dead `x` slot. The 28-byte payload copy is `rep movsd`.
- 00686ED0 `CTCBase::AddToGlobalTCs`: `if (!IsInGlobalUpdateTCs) { GlobalUpdateTCs.push_back(this); flag = true; }`
  with `push_back = insert(end(), x)` inline. end() is read before the node malloc, and the placement new is
  null-tested.
- 00687510 `CMessageEventManager::CMessageEventManager(const CWorld&)`: the world store, then the `_List_base`
  ctor. The proxy's `(_Node*)0` store is scheduled between the push and the malloc. The sentinel is self-linked
  through the local before it is published.

### std::map / _Rb_tree
- 0051BF90 `map<CThingBuilding*, list<CThingAICreature*> >::operator[]`: map::operator[] as written in the source,
  with `_M_lower_bound` inline and `__default_constructed((list*)0)` (the empty list temporary, then the copy into
  the pair). ~list, `_M_insert_dispatch` and `insert_unique` need same-TU noinline stand-ins. With them, VC7.1 treats
  the temporaries as uncaptured and keeps the sentinel in esi.
- 00553E60 `map<unsigned long, unsigned long>::operator[]` and 005692B0
  `map<CTCHeroOnlineScoreboard::EOnlineScoreboardActivity, float>::operator[]`: `_M_lower_bound` inline. The enum key
  compares signed (`jl`). Retail stores the value word late (`mov [esp+1Ch],0` after the pushes). That needs a
  same-TU out-of-line `insert_unique` stand-in that reads the value.
- 008AE1E0 `_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >>::erase(iterator)`: calls
  `_Rebalance_for_erase`, then destroys the pair inline (~vector, then ~CCharString), then the null-tested free and
  `--count`. `this` lives in the `push ecx` slot because four callee-saved registers are busy.
- 008AE380 `_Rb_tree::erase(first, last)` (same tree): calls `clear()` when the range covers the whole tree, else
  `erase(first++)`. Both the SGI `_M_increment` and clear() are inline. The old node is passed through the iterator's
  user copy ctor.
- 008AE510 `CTCAIScratchpad::GetGossip(CCharString)` and 008AE970 `CTCAIScratchpad::GetGossipFactions(CCharString)`:
  `it = m.find(k); if (it == m.end()) return 0; return &m[k];`. With the iterator NAMED, the `_M_find` call comes
  before the end() load. The one-expression form loads the header first.
- 006BA420 `CFaction::CFaction()`: an empty Name, then two empty STLport sets. Each one is `_Rb_tree_base(a)`
  (the proxy's null store, then allocate(1)), then `_M_empty_initialize()` with the header re-read between stores.
- 0047AE50 `CHeroLogBook::Clear()`: `GivenQuests.clear()` uses the out-of-line fastcall `__copy` (0x0047C600) and
  `__destroy_aux` (0x0047C800). `LogEntries.clear()` is `_Rb_tree::clear()` with `_M_erase` 0x0047C6A0. Then
  `QuestBeingViewed = 0`. Both empty tags share the high byte of the `push ecx` word.

### std::vector: erase
- 00477E36 `vector<CEngineLocalDetailObjectDef>::erase(first, last)` and 00478C2D
  `vector<CBalverineBattleDef>::erase(first, last)` (0x88- and 0x48-byte defs): these are `#pragma optimize("s",
  on)` frame-pointer TUs. `__copy_ptrs` and the counted random-access `__copy` must be `__forceinline` (under "s",
  `inline` alone does not inline them). The count is homed in the dead `last` slot. `_Destroy` stays out of line
  (declaration only).
- 00CD3152 `vector<CScriptThing>::erase(position)`: `if (pos + 1 != end()) __copy_ptrs(..); --finish;
  _Destroy(finish);`. The tag sits in the top byte of the dead `position` slot (`lea eax,[ebp+0Bh]`).
- 006CA850 `CEngineTextureMorphSet::ClearTextureMorph(long)`: an erase-while loop with the single erase inline. The
  random-access tag lands on the LIVE `index` slot, so `__copy` needs a same-TU noinline stand-in.
- 006BC160 `CRegion::RemoveMap(long)`: `Maps.erase(_STL::remove(Maps.begin(), Maps.end(), map))`. `remove` is find +
  remove_copy, and the single erase becomes one memmove. find's tag is on the live `map` slot, which needs a same-TU
  noinline `__fastcall __find` stand-in (0x0050E540).

### std::vector: insert / push_back / _M_insert_overflow
- 005418C0 `vector::insert(pos, x)` (vector<NUISystem::CComponent*>): the STLport single insert. The trivial
  backward copy is `_Num > 0 ? memmove(..) : 0`. The overflow (0x00485DBC) is declaration-only.
- 005E29D0 / 0085E200 / 008657E0 / 0086E1C0 `vector<...>::_M_insert_overflow(.., const __false_type&, ..)` for the
  pair<EAnimComponent, CIVCountedPointer<..>>, CTextureManagerPoolInfo, NCreatureHitNotification::CFlash and
  CVectorMap<ulong, CTextureMorph> elements: the vector.c body as written in the source. `(max)` goes through
  references, fill_len == 1 is copied inline, then `_M_clear` and `_M_set`. Every tag-taking callee gets the address
  of the LIVE `atend` slot, which needs same-TU noinline stand-ins that never read the tag. When the destructor is
  trivial (CTextureManagerPoolInfo), the destroy loop folds away. `_M_clear` is declared out of line for the
  CVectorMap element.
- 0086E7C0 `vector::push_back` (vector<CRandomAppearanceMorph::CMorphSet::CMeshTextureSet>): the overflow is fully
  inlined and folded for fill_len 1 / atend. The head copy's tag is on the live `x` slot, so it needs a noinline
  `__uninitialized_copy` stand-in.
- 0086E880 `vector::push_back` (vector<CRandomAppearanceMorph::CBodyPartMesh>), 0088A350 `vector::push_back`
  (vector<NEnvironmentInternal::CBlendedEnvironmentTheme>, 500-byte elements) and 008B0B10
  `CCombatSequenceBase::PushAction` (push_back + `++NoAvailableActions`): the placement copy is behind a null test.
  The overflow has a same-TU noinline stand-in, so the tag is homed in x's argument slot.
- 006C8730 `back_insert_iterator<vector<pair<long, COpinionDeed*> > >::operator=`: push_back inline. The tag is homed
  the same way, through the overflow stand-in.

### std::vector: constructors / reserve / resize
- 0057FCDD `vector<CCharString>::vector(size_type n)`: `_Vector_base(n, allocator())` is out of line. The empty
  allocator and the tag both sit in the LIVE `n` slot (`lea eax,[ebp+0Bh]`). That needs noinline stand-ins plus a
  `__forceinline` `uninitialized_fill_n` forwarder under "s".
- 006B6840 `vector::vector(const vector&)` (vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>) and
  008899C0 `vector::vector(const vector&)` (vector<NEnvironmentInternal::CBlendedEnvironmentTheme>): the real
  `_Vector_base(n, a)` with three zero stores (`mov eax,0`), then the out-of-line `__uninitialized_copy` with the tag
  homed in the dead `x` slot.
- 0067D5C0 `vector<CBlurredAttitudesDef>::_M_allocate_and_copy`: allocate, then the inline `__false_type` copy loop.
  VC strength-reduces the destination to `first + (result - first)`.
- 008ACD30 `vector<CScriptThing>::reserve` and 00ADB960 `vector::reserve` (vector<CVertexGroupBlockOptimiser::
  CBlockInfo>): vector.c reserve with `_M_allocate_and_copy` inline. The copy's tag is on the LIVE `n` slot (noinline
  stand-in). `_M_clear` is declared out of line for CScriptThing; for the trivial CBlockInfo it is an inline null-tested
  free.
- 00B6CBD0 `vector<CVertexShader>::resize(n)`: `resize(n, _Tp())` over erase / `_M_fill_insert`. The leaf is pinned
  to the full 1-arg mangled name because the 2-arg inline overload is emitted too.

### Algorithms
- 006F8EA0 `_STL::__find_if<const NAnimationEvents::CEvent*, CIsEventIndexEqualTo>`: the four-way unrolled
  random-access `__find_if` as written in the source (`__fastcall`). `pred.Index` (a 30-bit bitfield) is cached in
  esi, followed by the 3/2/1 switch remainder. This is a full MATCH.

## Parked for Fable 5.1 (2)
- 005672D0 basic_string::_M_range_initialize DIFFER(90v89): byte-exact over the 89-byte row plus the one 0xCC VC7.1 emits after the trailing noreturn `_String_base::_M_throw_length_error` call (retail 0x00567329 is that int3) -- int3-after-noreturn | tried: the range constructor `basic_string(f, l, a)` (wrong function: retail never zeroes the members and returns finish), then the `_M_range_initialize(f, l, forward_iterator_tag())` member template | next: land against the 90-byte span (int3-after-noreturn; source + passing test in differ/)
- 00476F9B vector::erase DIFFER(92v92): only the `__destroy_aux` tag address differs -- ours `lea eax,[ebp+0Fh]` (the dead `last`/count slot), retail `lea eax,[ebp+0Bh]` (the LIVE `first` slot) | tried: plain inline STLport helpers (not inlined under "s"), `__forceinline` `__copy`/`__copy_ptrs`/`__destroy`/`_Destroy`, a named `__false_type __tag` local, the full STLport `__value_type`/`__iterator_category`/`__distance_type` chain | next: a same-TU `__declspec(noinline)` stand-in body for `__destroy_aux` 0x00450B1B under `auto_inline(off)` that never reads the tag (the fleet's live-slot recipe; untried within the 4-grade budget)

## Skipped
- None. No row holds two functions.

## Label corrections
The member names of STLport template instances come from the STLport 4.6 source (Ego_r.pdb's STL is Dinkumware), and
the class methods come from the Ego_r PDB name sets. No row here needs a "descriptive name".
- 0067CF40 `list::merge` -> `list<CReactionMatch*>::operator=`
- 00686ED0 `list::push_back` -> `CTCBase::AddToGlobalTCs()`
- 004046B0 list dtor -> `list<CStringParserCommentBoundaries>::~list` (the element type comes from the catalog; about 90 folded twins)
- 00534D30 `list_node::splice` -> `list<NUISystem::CComponent*>::erase(first, last)`
- 00687510 `list_node::Initialize` -> `CMessageEventManager::CMessageEventManager(const CWorld&)`
- 0051BF90 / 00553E60 / 005692B0 `map::insert` -> `map<..>::operator[]`; 005692B0 is map<EOnlineScoreboardActivity, float>, not <int,int>
- 008AE1E0 / 008AE380 `map::_Erase_node` / `_Erase_range` -> `_Rb_tree<CCharString, pair<const CCharString, vector<CCharString> >>::erase(iterator)` / `erase(first, last)`
- 008AE510 / 008AE970 `map::_Find_or_default_construct` -> `CTCAIScratchpad::GetGossip` / `GetGossipFactions` (static)
- 006BA420 `pair<CCharString,CCharString>::pair` -> `CFaction::CFaction()`
- 005672D0 `string::string` -> `basic_string<char>::_M_range_initialize(const char*, const char*, const forward_iterator_tag&)`
- 00476F9B / 00477E36 / 00478C2D `vector::erase` -> `vector<CEnvironmentThemeDef / CEngineLocalDetailObjectDef / CBalverineBattleDef>::erase(first, last)`
- 0047AE50 `vector::erase_with_tree_cleanup` -> `CHeroLogBook::Clear()`
- 005418C0 `vector::insert_single` -> `vector<NUISystem::CComponent*>::insert(pos, x)`
- 005E29D0 `vector::_Reallocate` -> `vector<pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > >::_M_insert_overflow`
- 006F8EA0 `vector::Find_ByMask` -> `_STL::__find_if<const NAnimationEvents::CEvent*, CIsEventIndexEqualTo>`
- 0085E200 `Reallocate_element` -> `vector<CTextureManagerPoolInfo>::_M_insert_overflow`
- 008657E0 `Reallocate_pair_animation` -> `vector<NCreatureHitNotification::CFlash>::_M_insert_overflow`
- 0086E1C0 `Reallocate_with_fill_16byte` -> `vector<CVectorMap<ulong, CRandomAppearanceMorph::CTextureMorph> >::_M_insert_overflow`
- 0086E7C0 `PushBack_with_realloc` -> `vector<CRandomAppearanceMorph::CMorphSet::CMeshTextureSet>::push_back`
- 0086E880 `PushBack_CEntry_24byte` -> `vector<CRandomAppearanceMorph::CBodyPartMesh>::push_back`
- 0088A350 `_Push_back_500byte_object` -> `vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::push_back`
- 008899C0 `vector<CFeatDef>::vector` -> `vector<NEnvironmentInternal::CBlendedEnvironmentTheme>::vector(const vector&)` (PDB CFeatDef is 100 bytes; the element is 500)
- 008ACD30 `_Reserve` -> `vector<CScriptThing>::reserve`
- 008B0B10 `vector::push_back` -> `CCombatSequenceBase::PushAction(CCombatAction&)`
- 00ADB960 `reserve_12byte` -> `vector<CVertexGroupBlockOptimiser::CBlockInfo>::reserve`
- 00B6CBD0 `vector::erase` -> `vector<CVertexShader>::resize(size_type)` (already corrected in the chunk-316 labels)
- 00CD3152 `vector::pop_back` -> `vector<CScriptThing>::erase(iterator)`
- 0067D5C0 `vector<CConversation>::allocate_and_construct` -> `vector<CBlurredAttitudesDef>::_M_allocate_and_copy`
- 006CA850 `vector<CEngineTextureMorph*>::erase` -> `CEngineTextureMorphSet::ClearTextureMorph(long)`
- 0076FF90 `vector<CLipSyncPhoneme>::operator=` -> `list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::operator=`
- 006B6840 `vector<CMusicEntry>::vector` -> `vector<NEnvironmentInternal::CEnvironmentThemePriorityStackEntry>::vector(const vector&)`
- 006BC160 `vector<int>::erase` -> `CRegion::RemoveMap(long)`
- 006C8730 `vector<pair<long,?>>::emplace_back` -> `back_insert_iterator<vector<pair<long, COpinionDeed*> > >::operator=`
- 0057FCDD `vector<ulong>::vector` -> `vector<CCharString>::vector(size_type)`
- Callees: 0x0042A34E "CMemoryAllocatorVariableSize::Erase" is `_Rb_global<bool>::_Rebalance_for_erase`; the
  catalog's CFeatDef labels on 0x00889700 / 0x00889880 / 0x0088A410 were propagated across folded bodies and belong to
  the CBlendedEnvironmentTheme vector; the landed comment on 0x0050E540 calls it "Dinkumware _Find", but it is the
  STLport `__find`.
- Already-landed names that look wrong (see LESSONS.md "Review at landing"): 0x006BAB80 `CEntry` is CFaction;
  0x0086BDE0 `CEntryLayers` is CRandomAppearanceMorph; landed 0x0085E570 `Element` is CTextureManagerPoolInfo;
  `CReactionMatch_` has a trailing underscore; the caller note on the landed 0x004CD9D0 `_Vector_base(n, a)` says
  vector<ulong>, but 0x0057FCDD (vector<CCharString>) also calls it. It is a folded 4-byte-element body, so the note
  should not name a single element type.
