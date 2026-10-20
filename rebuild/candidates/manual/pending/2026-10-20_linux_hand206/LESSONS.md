# Chunk 206 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A local that retail keeps in a callee-saved register although its address is later passed to an out-of-line STL helper
  (`_M_insert_overflow(finish, x, ...)`, a search template taking the result vector by reference) needs the helper's body in
  the TU under `#pragma auto_inline(off)`: VC7.1 then sees the address does not escape and stops re-reading the frame copy
  (0x007E4B60 component pointer, 0x00736590 CDefPointer copy, 0x007FD580 vector start). Retail had that knowledge because the
  instantiations sit in the same TU (adjacent addresses).
- `x + call()` where x is a `(long)(float)` conversion: written inline VC7.1 calls `call()` first and turns the add into a `sub`
  of the negated product (`fmul [-30.0]`); retail's conversion-first order needs the conversion in a named local (0x0073ABA0).
- Two calls in `a() - b()` are evaluated right operand first by VC7.1; retail's left-first order needs `T l = a(); l - b()`
  (0x0073A8E0 UpperBound/LowerBound).
- A by-value thing pointer argument read before the registry/vector lookup that produces the call target is a named local
  (`CThing* thing = ParentThing;` before `table.LowerBound(k)[i].Allocate(thing)`, 0x0073A8E0); likewise the object of a
  push_back whose argument is a temporary (`driver = ...; driver->list.push_back(T(x))`, 0x0078F4D0).
- `ParentThing->PhysicsTC->Radius` as a call argument picks edx/ecx in the opposite order from retail; an inline
  `GetPhysics()` accessor on the component reuses ecx as the base like retail (0x0078F4D0).
- The squared length of a C3DVector difference kept entirely on the x87 stack is a named-local `operator-` result plus
  `s = Z*Z; s += X*X; s += Y*Y;` (0x006CB780) -- the same formula should recover the parked CTCCreatureNavigation siblings
  0x6CB620 / 0x6CB6E0 / 0x6CB840 ("x87 squared-distance term order").
- A switch whose case bodies retail lays out 2, 1, 0 after the `sub eax,edi ; je / dec ; je / dec ; jne` dispatch is written
  with the cases in that order (0x007DDCC0, GOTCHAS chunk 200 confirmed).
- An if / else-if whose arms both end in the same call: VC7.1 duplicates the shared tail into the first arm; writing the call
  plus `return` in that arm instead changes the register of the second arm's reload (0x00750780).
- STLport's exception-free range check (`bitset::test`) is `puts("bitset"); abort();`; declare abort
  `__declspec(noreturn)` or VC7.1 saves the live value across it into a callee-saved register (0x008ED090).
- A component looked up and immediately used as `this` of a non-virtual call goes straight into ecx only through an accessor
  returning an unset local over a ONE-TERNARY GetTC (`return (it == last || k < it->type ? last : it)->component;`); the
  if-form GetTC routes it through eax (0x00749B40). The same ternary form serves a direct virtual call (0x007EDEE0, 0x0078E820).
- `PTarget = CIntelligentPointer(x)` needs a user copy-assignment (`SetPItem(other.GetPItem())`, with `GetPItem()` into a local
  first); with only a converting `operator=(const CBaseIntelligentPointer&)` VC7.1 picks the implicit memberwise copy. Converting
  assignments from another instantiation must be `__forceinline` or they are emitted out of line (0x0078E820).
- The STL search filters of CThingSearchTools are Ego_r.pdb types: `CFilter_Bind3<F1, F2, F3>` holds three filter pointers and
  is built as a temporary in the search call (its slot is then reused by later temporaries); CThingFilter_IsNotList is {PNotList,
  PNotList2}, CThingFilter_IsWithinArea {StartPos*, EndPos*, Radius, IncludeThingRad} (0x007FD580).
- A by-value functor argument retail builds in the argument area from registers while also storing one field to a dead frame
  word is a functor with the IMPLICIT copy ctor (a user copy ctor with a float member copies through the x87) (0x007FD580).
- Label pattern: catalog "vfunc_6" / "vfunc_9" / "vfunc_28" rows of CTCBase-derived classes are OnInitialActivate / FrameUpdate /
  OnActionUse; CThingCreatureBase slot 77 is IsPlayer().

Review at landing:
- 0x007FD580 compiles a STAND-IN body for CThingSearchTools::GetAllThingsInRoughArea (it only touches the result vector inline;
  it is not the real 0x007FDB10 code) and a plain for_each body above the graded function under `auto_inline(off)`, purely for
  VC7.1's escape knowledge. The graded symbol and its relocations are unaffected, but decide whether such stand-ins are
  acceptable; 0x007E4B60 does the same with a plausible `_M_insert_overflow` (real semantics, not the retail body).
- Local models next to existing engine headers: CTCCombat (engine/CTCCombat.h is PDB, retail containers 12 bytes: rows
  007337e0, 00733e10, 00735540, 00736590 parked), CTCCreatureGroupBoastingCrowd (header PDB +0x28 PHero vs retail +0x24),
  CTCCreatureOpinionOfHero (header leaves ParentThing unnamed and has PDB container sizes). 00733690 uses engine/CTCCombat.h
  (only +0x24 is read).
- 0078e820 (parked, one instruction off): the one-line next idea `PTarget = target;` should be tried at landing.
- Already-landed sources with wrong bindings: 00435000 (non-const LowerBound name -> 0x004365B0, non-const GetMainPlayer ->
  0x00449960), 0051fba0 (const GetMainPlayer -> 0x00449970); CTCNoise::ResetLifeTimer 0x007497C0 calls 0x00686D60 "GetWorld";
  0x004349D0 landed under the file name CTCCreditsUI_Deactivate although it is the display engine's letterbox end.
- 008ed090: reloc_identity reports RELOC_UNCHECKED (the TU also emits the non-inlined COMDAT copies of the bitset/lookup
  helpers); the grade itself is RELOCATION_MATCH.
