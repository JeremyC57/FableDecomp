# Chunk 378 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- STLport `map::operator[]` with an inline `_Tp()` ctor needs the named `T def; i = insert(i, value_type(k, def));`
  form. Retail re-addresses def with `lea` after its ctor, def sits at the top of the frame and the insert result goes
  into the dead `k` slot (0x005B3100, as 0x005812DF). With an out-of-line `CWideString()` default, neither the named
  form nor the bare form gives retail's frame (0x00594A08, parked).
- When an inline `list<T>::push_back(const T&)` is fed a map key, it binds the reference to the node key itself
  (`lea eax,[esi+10h]`) only when T is the key type exactly. If T is a base-class pointer, VC7.1 builds a conversion
  temporary (`mov [ebp-4],eax ; lea eax,[ebp-4]`). So model the list with the key's own type (0x0059DE04 /
  0x0059DE76, `list<CComponent*>` for CAction::UIActionParams).
- In a size TU an inline (non-forced) copy ctor stays an out-of-line call even inside a `__forceinline`
  `_Copy_Construct`. A refcounted `CDefPointer` add-ref copy (`mov eax,[x] ; test ; mov [edi],eax ; je ; inc [eax+4]`)
  needs `__forceinline` on the copy ctor itself (0x005B3275; 172 v 176 without it). The same goes for `_Construct`
  in push_back (0x00594782).
- To keep a deque cursor cached across an out-of-line call (`_M_finish._M_cur` in esi across back()), VC7.1 needs
  same-TU `auto_inline(off)` bodies of BOTH back() and the `_M_decrement` it calls; back() alone still reloads
  (0x005959AB).
- A `/Gr` STLport free function returning a deque iterator by value takes the hidden result pointer in ecx only when
  the iterator is non-POD. Use STLport's user ctors (cursor/node + default) with an implicit copy: a POD iterator
  pushes the pointer on the stack, a user copy ctor is called, and a forced-inline one emits mov pairs instead of
  `movsd` (0x0059AFD7). The element-load vs cursor-load order of `*__result = *__first` is still unsolved.
- The STLport sort/heap helpers over a comparator class (CPriorityDefPair, 0x00592FFB .. 0x00594E4D) take the
  comparator by value in its own slot. Each comparator call is a thiscall on that slot. Empty tags (`__false_type`,
  iterator tags) land in the high bytes of dead argument slots (`[ebp+0Bh]` / `[ebp+13h]` / `[ebp+1Bh]`) only when
  the callee bodies are visible in the TU (`auto_inline(off)` stand-ins).
- Inline CThing interface lookup (`GetTC` / GetInterface): the key copy is homed in the dead `type` slot or a
  block-scoped local, which can share a frame word with a later loop local (0x0059DE76: key and the action's list at
  [ebp-4]). The miss test must compare the parameter, not the copy (0x005943A2).

Review at landing:
- 0x0059DE04 / 0x0059DE76 model `CAction::UIActionParams` as `list<CComponent*>`, against both PDBs'
  `list<CActionParam*>`. Those PDBs come from later builds; this build's bytes demand the key type.
- Local models sit next to existing engine headers:
  - CFadeInFadeOutBase (0x00594880 / 0x00594A73): the header cannot declare the out-of-line copy ctor.
  - CFrontEndManager (0x0059525D / 0x005959AB): engine/CFrontEndManager.h has the Dinkumware Ego_r layout.
  - CTCInventoryTrade (0x0059DE04 / 0x0059DE76): a padded view with retail offsets.
  - CHeroMarriageInfo (0x005806DF .. 0x005810DA): its copy ctor is out of line.
- 0x005943A2 may be `CThing::PeekTC` (const) rather than GetTC: the bytes are an ICF-folded member template, the
  const LowerBound 0x004365B0 suggests const, and the instantiation T is not recoverable.
- 0x005959AB and 0x0059DE04 each took 3 grades; 0x00594A08 and 0x0059AFD7 used all 4 and are parked.
- Already-landed sources with wrong names:
  - `CTCCarryable_OnKill_0059af2a.cpp` is `map<FRONTEND_SCREEN, CComponent*>::clear`.
  - `CTCCarryable_OnKill_0042ade5.cpp` is a `map<CComponent*, CComponent*>::clear`.
  - `stdvectorCHeroMarriageInfo_Mfillinsert_0048608c.cpp` works on a 28-byte record, not the 24-byte
    CHeroMarriageInfo.
  - `DestroyrangestdallocatorNCreatureHitNotification_CFlash_005b26e9.cpp` is `_Destroy` over a CDefPointer range.
- 0x005B3275 serves seven callers. Six of them (0x0071D34F .. 0x0076BA5A) are other TUs' inline push_backs of other
  CDefPointer vectors folded onto this body. CTattooDef is the instantiation of this TU, from the caller
  ApplyTattooCard 0x005AD39D, which tests `Permanent` at +0x3D.
