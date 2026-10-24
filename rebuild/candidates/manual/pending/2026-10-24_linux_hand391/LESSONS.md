# Chunk 391 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `return f;` of a by-value functor that holds a C3DVector with a USER copy ctor (implicit memberwise copy) -> three
  position loads into ecx/edx/eax before the first store. A trivially copyable C3DVector gives a block copy through
  ecx instead (79 v 77) (0x00726610, 0x00727500).
- STLport `__find_if(first, last, pred, random_access_iterator_tag)` -> unrolled body: a `(last - first) >> 2` trip
  count, four tests per pass, then `switch (last - first)` lowered to a `dec eax ; je` chain (0x00724A90).
- POD vector erase as STLport `__copy_trivial` -> `last == first ? result : memmove(result, first, n) + n`. The
  `last == first` test survives even when it is trivially true (`cmp eax,eax`) (0x00724240, 0x0072C6E0).
- STLport `__pop_heap` is inline in one __partial_sort instantiation (0x00721400, pair<CEntry,float>) and out of line in
  another (0x00721960 calls 0x00720610). Check the call list before choosing the form.
- An inline CCompositeBlendDesc copy via CRGBColour `Set(r,g,b,a)` -> all four colour bytes loaded, then stored
  R, G, B, A (0x00721960).
- Empty STL tag temporaries -> they live in dead or live argument slots, or in the high byte of a `push ecx` frame word
  (`lea edx,[esp+0Fh]`) (0x00721600, 0x00721D30, 0x00724240).
- CDefPointer operator= with `--Object->RefCount` written inline -> Object is reloaded after the store (may-alias) and
  the count re-tested. Move the decrement into a pointee member `void RemoveRef() { if (--RefCount == 0) Release(); }`
  so retail's `dec [ecx+4] ; jne` falls out (99 v 92) (0x00738720).
- A presence test on an interface bit that is the top bit of a byte -> `mov cl,[eax+30h] ; test cl,cl ; js/jns`
  (interface 0x87 at 0x0072AAF0, interface 0xA7 at 0x007340B0).
- `if (present) return false; ...; return true;` -> the `xor al,al` arm comes first. Retail's arm-last order needs the
  wrapped form `if (!present) { ...; return true; } return false;` (0x0072AAF0).
- An interface fetch where retail reuses eax (`mov eax,[eax+4] ; mov ecx,[eax+18h]`) -> use the out-pointer
  `GetInterface(T** out, long type)` (landed SetupBerserk 0x0072CBA0 form). A pointer-returning GetTC puts the
  component in ecx (0x0072B3B0, 0x00733F70, 0x007340B0).
- STLport vector default ctor -> it must be `explicit vector(const allocator_type& a = allocator_type())` with an
  `_STLP_alloc_proxy(a, 0)` end-of-storage member. The address-taken allocator temporary stops VC7.1 forwarding the
  zero members into registers in a following inline resize (162 v 161) (0x0072C6E0).
- A per-element reset written as direct member stores -> the tree right-link load gets hoisted above the stores. An
  inline `CCombatAbility::Reset() { ActiveTimer = -1; RepeatTimer = -1; }` called through `this` keeps the stores
  first (0x007334F0).
- STLport tree `++it` -> the static `_M_node = _Rb_global<bool>::_M_increment(_M_node)` (node in, node out) walks in
  eax and stores once. An in-place member increment stores through a reference iterator at every step (122 v 106)
  (0x00733620).
- Looping over one multimap key -> `last = upper_bound(k); for (it = lower_bound(k); it != last; ++it)`, both bounds
  inline with upper walked first. `equal_range(k)` stays an out-of-line pair-returning call (190 v 219) (0x00733F70,
  0x007340B0, 0x00734340).
- An empty iterator tag passed as a temporary -> give the tag a user ctor `random_access_iterator_tag() {}`. A
  value-initialised POD tag adds a `mov byte ptr [esp+1Ch],0` (102 v 97) (0x007352D0).
- STLport `_Construct` placement new -> declare it `operator new(unsigned int, void* p) throw()`. The throw() spec is
  what keeps retail's null test on the placement result (0x00739670, 0x0071F2E0).
- `"STF_" + name` on CCharString -> `CCharString __fastcall operator+(const char*, const CCharString&)` (0x0099F690):
  hidden result in ecx, the literal in edx, the string on the stack. Each temporary is built in a dead argument slot
  (0x007329E0).
- /Gr template algorithms returning a class with a user ctor (back_insert_iterator, functor) -> the hidden result
  pointer goes in ecx and the first real argument in edx (0x00721F70, 0x00726610). A thiscall member returning an
  iterator with a user ctor takes the hidden pointer at [esp+4] (0x00738D10, 0x00739B00).

Review at landing:
- engine/CTCCombat.h carries the Ego_r.pdb layout (MSVC 16-byte containers). Retail CTCCombat uses STLport 12-byte
  vectors and multimap, so offsets differ from +0x2c on. The six CTCCombat rows (0x007334F0, 0x00733620, 0x00733F70,
  0x007340B0, 0x00734340, 0x007352D0) use a local retail model next to the existing header.
  - Retail offsets: BlockCounterTimer +0x50, AlwaysBlockThings +0x54, PerformingAction +0x60, StartingToBlock +0x62,
    Blocking +0x63, BlockHoldTimer +0x64, BlockingNumFrames +0x68, CombatAbilities +0x70, ReadyForMelee +0x80,
    IsInMiddleOfFlourishAction +0x94; size 0x98.
  - The header needs an STLport-layout regeneration before these rows can include it.
- 0x0072C6E0 uses a local `class CTCBase` model because engine/CTCBase.h declares no `CTCBase(CThing&)` ctor
  (retail 0x00686800). Add the ctor to the header, then switch the row to the include.
- CTCSpecialAbilities, CTCClock, CTCWeatherVane and CTCHeroMorph have no engine header. The rows use local models with
  retail offsets (CTCSpecialAbilities 0x28: UndeadFollowers +0x14 list, Creature +0x18, AbilityAvailability +0x1c).
- The 0x007329E0 symbol is `?PlaySoundW@CTCClock` (the PDB name went through the <windows.h> `PlaySound` macro). Keep
  the W.
- 0x007329E0 and 0x00738C90 have no direct callers in retail; they are out-of-line copies of bodies inlined elsewhere.
- Already-landed labels to fix: 0x007338A0 is CTCCombat::UnsetBlocking (landed under a propagated label), 0x00686800 is
  CTCBase::CTCBase(CThing&) (label "CEngineSubPrimitive2DClockSprite_ctor"), 0x004C73D0 "CVertexBufferWin32::DoSizeof"
  is a folded body that also serves as the CThing position accessor.
  - The landed 0x00733580 / 0x00733750 call 0x007334F0 / 0x00733620 under the catalog names ResetCombatState /
    FindBlockAbility. They are ClearCombatVariables / GetCombatAbilityData.
- 0x0072AAF0 parks with the same ecx-before-pushes shape as the unlanded twins 0x0072A970 / 0x0072A9B0. Author all
  three together.
- Rows graded more than 4 times (counting probe forms), all parked: 0x00721A50 (6), 0x00722BD0 (5), 0x0072AAF0 (9),
  0x0072AB70 (5), 0x00732D40 (5). Every matched row needed 4 grades or fewer. No catalog row held more than one
  function.
