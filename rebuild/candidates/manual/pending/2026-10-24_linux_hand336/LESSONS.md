# Chunk 336 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `#pragma auto_inline(off)` around a TEMPLATE member stand-in does not stop VC7.1 from inlining it at the
  instantiation; add `__declspec(noinline)` on the definition (STLport `vector<T*>::_M_insert_overflow` stand-in,
  0x005198B0: without it the overflow branch folded into the push and the body shrank 140 -> 116).
- vector<CThing*> push_back of the loop's own `CThing* thing` binds `const T&` to the address-taken local and VC
  stores it at the top of the loop; pass a by-value temporary (`ThingCast<T>(thing)`) and the store moves to just
  before the push, in the dead argument slot shared with the `__true_type` tag (0x005198B0, 0x005ED860, 0x006C1DD0).
  A cast to the same type (`(CThing*)thing`, `static_cast`) is not enough.
- GetAllThings-family filters take the thing by reference (`filter(*(T*)thing)`): `std::const_mem_fun_ref_t`
  instances (`mov ecx,edi ; call [eax]`, 0x00617B20) need it, and every other filter grades the same.
- A five-way CFilter_Bind5 operator() exceeds VC7.1's inline budget inside GetAllThings: mark it `__forceinline`
  or it is called out of line (0x006DFA50).
- STLport list push_back = `insert(end(), x)` with `SNode* prev = n->_M_prev;` read once (retail `mov ecx,[esi+4]`
  before the node stores); writing `position._M_node->_M_prev->_M_next = tmp` re-reads it (0x004ACD70).
- A bool-selected bit test `if (flag) { if (b & 0x10) return true; } else if (b & 8) return true; return false;`
  gives retail's `je / test / jne / jmp / test / je` layout; `return flag ? (b & 0x10) != 0 : (b & 8) != 0;` gives
  `shr/and` (0x0091E660).
- A local model that pads a derived class up to a member offset must pad from `sizeof(base)`, not from the last
  base member (the base model rounds up to 4: 0x92 -> 0x94, 0x007BB660 graded off by 4 until fixed).
- CTCEntityEvents::GetEvent<TEvent, TFilter>: reverse_iterator walk of the STLport event list with a counted
  pointer copy per event (user copy ctor + releasing dtor), `GetType()` (vtable slot 1) == TEvent::EventType, then
  the filter by reference; the event id of an instance is the RTTI vtable slot-1 constant of the event class
  (scan `rtti-vtables.tsv` for NEntityEvents::CEvent* slot 1 -> `mov eax, imm`).
- Byte-identical template twins at two addresses (0x00617860 / 0x00617970, 0x004ACD70 / 0x00617CA0) need two
  distinct instantiations, or the COMDATs fold to one symbol; a const-element container is a code-neutral way.

Review at landing:
- 0x00617970 and 0x00617CA0 are modelled as the const-element (`list<const T*>`) twins of 0x00617860 / 0x004ACD70;
  the real distinguishing template argument is unproven (code-neutral either way).
- 0x006D4600 keeps the label's CThingBuilding for type list 4 while the landed 0x005C0D9D (GetPFirstLeafThing)
  uses CThingBuilding = type list 3; one of the two names is wrong (callers: CTCWife::GlobalUpdate,
  CTCActionUseBed::GlobalUpdate -> list 4 is probably beds or houses).
- Local base models next to existing engine headers: CThing (no CThing.h exists; the retail layout sits 0x10 below
  Ego_r.pdb), the CThingAICreature home/work pointers (+0x168 / +0x170 retail, PDB +0x190 / +0x198); `this` uses
  engine/CThingSearchTools.h and engine/CTCEntityEvents.h (Events is opaque there and read through a cast).
- 0x0095D920, 0x0096BC20, 0x0094E530 are register-allocation ties against a template that matches 7 siblings;
  the first needed 4 grades.
- No catalog row holds two functions; all lengths include only trailing int3 padding.
