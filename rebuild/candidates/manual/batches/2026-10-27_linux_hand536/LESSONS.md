# Chunk 536 retry -- lessons (GOTCHAS one-liners)

- **Inline `GetTC<T>(T** out)` and the key.** Write the key as a temporary inside the compare, not as a named local:
  `components.LowerBound((ETCInterfaceType)T::InterfaceType)`, then `if (p == end() || key < p->first) p = end();`.
  The temporary packs into the out variable's slot or a dead argument slot, as in retail. A named key gets its own slot.
  This closed 00924520 and fixed the frame on 00929020 and 00947110.
- **Const PeekTC.** It spills `bool present = HasTC(...)` as a byte and calls the out-of-line const LowerBound 0x004365B0.
  In some TUs it uses an inline lower_bound instead.
- **Sizes when modelling CThing.** CThing is the PDB layout minus 0x10 after Components, because the STLport CVectorMap is
  12 bytes. CClass is 0x14 bytes, with the Type byte at +0x10, so derived pads start at 0x14.
- **Inline accessors decide the codegen (0070d580).** Look up the PDB name set's inline accessors and use them, not raw
  member code.
  - `CAnimSet::SetAsFinished(long)` modifies its by-value parameter. The parameter's frame word is stored before the
    `!Finished` test, which no hand-inlined `max` form reproduces.
  - `IsValid()` with a cached count gives retail's single `sar` size CSE.
- **STLport 4.6 list iterators.** Model them as `_List_iterator<T, _Nonconst_traits<T> >` over `_List_iterator_base`, with
  the node and copy constructors. By-value iterator arguments to a `/Gr` for_each are then built in place
  (`push ecx ; mov eax,esp ; mov [eax],x`). Plain-POD iterators give bare pushes.
- **Out-of-line list members.** STLport 4.6 `list::clear` forwards to the out-of-line `_List_base<T,Alloc>::clear`. One-argument
  `push_back` stays out of line for big elements: 0x00711A10, list<CAnimSet>.
- **Float globals.** `1.0f - eps` evaluated at run time (`fld 1.0 ; fsub [g]`) means eps is a variable. A literal or a
  `static const` folds the subtraction.
- **Store order at a function's end.** `x = !(flags & 2)` before `y = -z` gives retail's `mov ecx,[flags]` ahead of
  `fchs`. The reverse source order schedules the float store first.
- **Fold order of `a = a + b`.** `it->X = it->X + Member` loads the element first (`fld [eax+40h] ; fadd [esi+8Ch]`).
  `it->X += Member` loads the member first.
- **Early return vs wrapped body.** An early `return` duplicates the destructor/epilogue tail. Retail's single shared
  tail (`je` to the vector free) means the body is wrapped in `if (cond) { ... }`.
- **Same-TU stand-ins.** VC7.1 IPA-folds a trivial noinline stand-in body, so a same-TU stand-in for a real retail callee
  must be opaque. Better: leave it out of line.
- **Inline bool bitfield reads.** `bool Dead:1` with `IsDead(){ return Dead != 0; }` gives the `mov cl,1 ; test cl,dl`
  register sharing.
  - A free loop register can also hoist the mask (`mov bl,10h ; test bl,al`). Keep the loop's zero register live
    (an `ebx = 0` CSE) by not hoisting.
  - A local `CAnimSet* p = *it` with inline accessor calls avoided it in 0070d580.
- **Name hygiene.** The catalog label for 0x007FC800 is wrong (it is `CTCKraken::EndAttackBeam`). 0x0049D870 is
  `?GetFrame@CWorld@@SIJXZ`; GFGetCurrentFrame is not in any PDB name set.

## Review at landing
- Anonymous-namespace `?A0x` hashes:
  - 0x00924520 FinishAction and its ctors 0x009255E0 / 0x00925680. Their bound spellings lack the namespace.
  - 0x008C9A40 AttachEffectToStaff.
- The `_Use` emission devices in both anonymous-namespace rows.
- 0x0143B934 = `g_EngineInterpolationInfo` + 0x54 (0x00B812F0).
- 0x004B8050 clear: the allocator argument of its spelling.
- 0x0129BA3C: the descriptive spelling `g_epsilon_0129ba3c`, one of 49 bound.
- 0x00C11D00 (parked): the CVtable / CAudioDevice cast debt.
