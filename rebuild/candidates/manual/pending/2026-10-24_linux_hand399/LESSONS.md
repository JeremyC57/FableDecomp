# Chunk 399 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- If a /Gr algorithm takes a by-value element whose copy constructor is non-trivial, plain `inline` helpers can stay out of line. `copy_backward` in `__linear_insert` and `__pop_heap` in `__partial_sort` stay calls. `__forceinline` on the helper restores retail's inline body (0x00818240, 0x00818450).
- A non-inline template `__unguarded_linear_insert` defined in the same TU is auto-inlined into `__unguarded_insertion_sort_aux`, as retail expects (0x0081A940, 0x0081A9E0).
- For an inline STLport `push_back`, the overflow's empty `__true_type` temporary is homed on a dead argument slot only when a same-TU `auto_inline(off)` + `__declspec(noinline)` `_M_insert_overflow` body never touches the tag (GOTCHA 1276).
  - Confirmed for back_insert_iterator::operator= (0x0080EB40).
  - Confirmed for transform into a back_inserter (0x0080EBE0, 0x0080EC50).
  - Confirmed for CVectorMap::Insert (0x0081B220). Here the tag lands on the dead by-value key slot.
- A comparator's implicit copy constructor is emitted out of line and called (0x7F9730) when the algorithm copies it twice (`sort`, 0x0080C020). It is expanded inline when copied once (`partial_sort`, 0x0080BCB0).
- In a static Create factory, `CThing* result = 0; ... result = thing; return result;` keeps the null in ebx and duplicates the success tail. An early `return thing` loses the zero register (0x0080C460, 0x0080C500).
- In an STLport `_Rb_tree`, `equal_range` matches only when `_M_lower_bound`/`_M_upper_bound` return a node pointer in eax. An iterator-returning helper adds a hidden result pointer (0x00813530).
- A TU that explicitly instantiates `_Rb_tree::erase(first,last)` also emits `erase(position)`. The `// Leaf:` needs the full mangled name, because `?erase@?$_Rb_tree@` matches two symbols (0x00813460).
- For a 12-byte map entry `pair<unsigned long,CSoundPair>`, modelling CSoundPair as two scalar `long`s gives retail's single-register copies. A 3-dword array member copies through esi/edi (0x0081A590).
- An inline base constructor `CEventBase() : WorldFrame(0) {}` has its own vtable store elided when the derived ctor stores its vtable (0x008111F0).
- A CCharString temporary built in the right operand of `||` gets a conditional-destruction flag (`mov ebx,1 ; test bl,1`). Retail's unconditional destroy means the call was its own statement (`found = ...;`) (0x00814C70).
- A pointer read at entry (`mov ebp,[edi+60h]`) and used only on a later fallback path comes from a named local in the source. VC does not hoist a member load across a call (0x00814C70).
- A pointer read once and reused across two calls (`[g_pMainGameComponent]+24h` kept in edi for both GetPRandomSeed calls) is likewise a named local (`CWorld* world = ...`) (0x0080FFB0).
- Unsolved pattern seen twice: retail loads the call object (`mov ecx,[reg+disp]`) before evaluating or pushing the arguments, where VC7.1 here loads it last. Seen at 0x0080FFB0 (a CRightHandedSet temporary) and 0x00814C70 (GetDummyObject). In 0x0080FFB0 retail also block-copies the 24-byte set in dword pairs, which suggests a whole trivially-copyable struct copy rather than member copies.
- GOTCHA 222 still applies to an inline `find_if` inside `remove_if`: the iterator-tag temporary takes a frame word instead of the live predicate slot (0x0080E2F0, parked).

Review at landing:
- No row needed more than 4 grades. 0080c190, 0080e2f0, 0080ffb0 and 00814c70 are parked in differ/ with their best source.
- The landed 0x0080C250 `CTCDCameraPoint::Construct` is a byte-twin label. It is really CTCDExplosion::Construct, called by both CTCDExplosion::Create overloads.
- The landed callers of 0x008137E0 declare it `ChooseNewTagger`. The real name is CTCCreatureGroupTag::FindNewTagger.
- Landed sources disagree about physics virtual slot 75 (+0x12C): some name it SetRHSet, others SetFacingAnglesFromRHSet. 0x0080FFB0 calls it with a CRightHandedSet. Settle this from the CTCPhysicsStandard vtable before landing.
- Local base models were used where no engine header exists: CPhysicalPrimitiveSphere, CRightHandedSet, CDefString and CCharString. The CTCPhysicsBase pad model in differ/00814c70.cpp sits next to `rebuild/include/engine/CTCPhysicsBase.h`, so switch to the header if the row is revisited.
- 0x0081B220 takes its key by value in the PC build, against the FableWin.pdb `Insert(const unsigned long&, const CSoundPair&)` signature. Confirm the mangled name at landing.
