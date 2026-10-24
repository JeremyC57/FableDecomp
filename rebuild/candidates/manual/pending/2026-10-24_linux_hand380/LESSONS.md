# Chunk 380 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **STLport list iterators.** An STLport list `operator=` (0x005E9630) passes `iterator` arguments built in place at
  esp (`push ecx ; mov ecx,esp ; mov [ecx],eax`) and `const_iterator` arguments as plain `push`. Model the 4.x
  iterator: the base has no copy ctor, and the derived template declares `_List_iterator(const iterator&)`. That is
  iterator's user copy ctor but only a converting ctor for const_iterator. A user copy ctor on the base builds every
  iterator in place (232v224).
- **STLport list insert link order.** A range `_M_insert_dispatch` whose link stores `tmp->prev` before `tmp->next`
  (0x005E9280) is the STLport 4.6 insert: `n = pos._M_node ; p = n->_M_prev ;` as named locals, then next, prev,
  `p->next`, `n->prev`. The 4.0 form that re-reads `pos._M_node->_M_prev` stores next first (149v146).
- **list::erase(first, last) needs no special model.** The textbook `while (first != last) erase(first++); return
  last;` with erase(pos) inline matches as-is (0x005E90B0). `this` is never read with the stateless malloc
  allocator, and the result is a hidden-pointer iterator (`ret 0Ch`). The list node deallocate is a plain `free(p)`
  (no null test).
- **Size-TU inlining.** In the CTCInventoryAbilities TU (`optimize("s", on)`, plus `("y", off)` for the frame-pointer
  rows), CVectorMap::Insert `{ push_back(pair(k, v)); Dirty = true; }` stays out of line unless `__forceinline`.
  Update `{ if (Dirty) Sort(); }` inlines on its own and re-reads the Dirty byte VC7.1 just stored
  (`mov byte [esi+0xd],1 ; cmp byte [esi+0xd],0`): retail shows the same store/reload (0x005F6423, 0x005F66D9).
- **CVectorMap member names (FableWin.pdb).** Find (const and non-const), Insert(const K&, const V&), Update(), Sort(),
  LowerBound/UpperBound, SetDirty, IsDirty. The non-const and const LowerBound are separate retail copies
  (0x005F7F4C / 0x005F7F89, same bytes, not folded), so a const member must call a `const` overload.
- **Returning a callee's by-value result.** `return GetAbilityPDef(it->second);` in a function returning the same
  CDefPointer passes the caller's hidden result pointer straight through (no copy). The empty arm
  `return CDefPointer<T>();` in a size TU is `and dword ptr [eax],0` (0x005EE365).
- **CBasicString<char>::operator< callee-clobber trick.** Defining the real body above its callers under
  `auto_inline(off)` (0x00429950) fixes every STLport sort piece over CCharString (11 rows). The pivot/val payload then
  stays in esi across the compares, where a bare declaration re-reads it after each call.
- **Two-level CVectorMap lookup with an immediate compare (F88).** A const local key gives retail's single hoisted
  zero store but re-reads the key per compare (120v125). A by-value `find(K)` over `LowerBound(const long&)` folds the
  compares but zeroes the key per branch (107v125). Nothing reproduces both (0x005E0380): park the family.

Review at landing:
- Four CTCInventoryAbilities rows (0x005EE365, 0x005EED4C, 0x005F6423, 0x005F66D9) use a local retail-layout
  `struct CTCInventoryAbilities` next to the existing `engine/CTCInventoryAbilities.h`. The header carries Ego_r.pdb
  offsets (16-byte vectors, 20-byte CVectorMaps); retail is 0x20 to 0x40 lower (Abilities +0x160, AbilityButtons
  +0x16C, ExpressionButtons +0x1C8). The header itself needs a retail-layout pass. The landed siblings (0x005EE31D,
  0x005EDF33) use local `CTCInventoryAbilities_Retail` models.
- Landed 0x005EDFD9 `GetAbilityPDef` is modelled as `PDef** GetAbilityPDef(PDef** out, int index)`, an
  `Entry* table` at +0x160. The PDB signature is `CDefPointer<const CHeroAbilityDef> GetAbilityPDef(EHeroAbility)
  const`, and +0x160 is the Abilities vector's start pointer. Retype it in the sweep.
- Landed sibling 0x005EE31D binds 0x005F7F89 (the const LowerBound) as a non-const `CButtonMap::LowerBound`. This row
  set binds it as `CVectorMap<long, EHeroAbility, ...>::LowerBound(const long&) const`, and 0x005F7F4C as the
  non-const one. RELOC_IDENTITY may need the names reconciled.
- 0x005E0380 was graded 4+ times across the session (1 before the summary, then 107v125 / 120v125). It belongs to the
  F88 family already parked in FINDINGS_LOG.
- The test bodies of the STLport rows reinterpret a plain array of pointer/POD records as the element type, because the
  models have no default constructors (pair, CCharString). Every model is standard-layout with the mirrored member
  types, so this holds on g++ x64 and VC7.1.
