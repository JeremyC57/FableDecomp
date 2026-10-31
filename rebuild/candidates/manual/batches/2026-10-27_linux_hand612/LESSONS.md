# Chunk 612 -- lessons

## GOTCHAS one-liners
- (chunk 612) An argument expression `run ? 5 : 3` inlined in a long call colours the bool in cl/edx; a named
  `EMovementType movement = run ? 5 : 3;` local before the out-param local gives retail's bl/eax (0x009852C0 match).
- (chunk 612) VC7.1 rotates scratch registers eax -> ecx -> edx per load; a one-step phase shift over the whole tail
  (brain loads, key lea/push, vector copy) is fixed by changing which of several equal loads goes through an inline
  accessor: `GetCreature()` for the first call only, `PBrain->Creature` for the rest (0x0093C2F0 match).
- (chunk 612) CThing::PeekTC<T> as `if (HasTC(t)) { lookup; return true; } return false;` gives retail's
  `test dword [eax+24h], mask`; the `bool has = bitset.test(t)` form materialises a byte (`shr ; and cl,1`, frame +4).
- (chunk 612) The chunk-240 stand-in rule holds for the const LowerBound 0x004365B0 too: without a same-TU
  auto_inline(off) body the CIntelligentPointer temporary does not overlay the key slot (sub esp,0Ch v 8). Write it as a
  member of an explicit CVectorMap<ETCInterfaceType, CTCBase*> specialisation (template members inline anyway).
- (chunk 612) STLport helper calls in a vector member (`_M_fill_insert`): same-TU __declspec(noinline) bodies for ALL the
  out-of-line helpers (__uninitialized_copy, __copy_backward, fill) fix an ebx/ebp swap and a spill slot (0x007D9D70).
- (chunk 612) An empty filter class without a user ctor is zeroed when value-initialised (`T()` -> `mov byte [esp+x],0`);
  give it `T() { }`.
- (chunk 612) A generated header whose vptr is a member (`void* __vftable`) cannot be a base under the layout rename when
  the derived class declares virtuals: everything shifts +4. Use engine/CBaseClass.h plus `sizeof(Layout) - 8` filler.

## Review at landing
- 0x0093C2F0 defines a same-TU stand-in body for the retail symbol
  ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBEPBU?$pair@...@Z (0x004365B0) in its object (GOTCHAS chunk
  240 precedent); only the IsRunnable leaf is graded. Likewise 0x007D9D70 defines the three STLport helper instantiations.
- vland warns RELOC_NAME for 0x007F6170 (labelled CTCTargetingAI::IsThingAttackable; our Ego_r spelling
  IsThingValidTarget) and 0x007D98C0 (labelled _global::UninitCopy_SElement16_007d98c0): apply NAME_FIXES.tsv and respell
  the landed 0x007F6170 file and its callers.
- reloc_bindings.tsv binds both ??$__uninitialized_copy@...YI... and ??$__uninitialized_fill_n@...YI... to 0x007D98C0;
  only __uninitialized_copy is proven (our object inlines uninitialized_fill_n, YA).
- 0x0093C2F0 test/ models the logic standalone (as the other chunk tests); the CTCTargetingAI interface number 8 is proven
  from bit tests, the class from the callee only.
- 0x00970DF0 stays at frame 0x58 v 0x50 with every lever above tried; the next idea is a source shape where the
  away vector dies inside an inlined callee while the box temporaries stay live (e.g. a helper that builds the Bind2).
