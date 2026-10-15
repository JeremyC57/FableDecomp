# Chunk 110 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A bool function whose exits mix `mov eax,1 / xor eax,eax` with `mov al,1 / xor al,al` returns a logical or compare
  EXPRESSION on the int-sized exits (`return Get(...) && x->F();`, `return a || b || c;`, `return dx*dx+dy*dy < r*r;`)
  and literal `true`/`false` on the byte exits; the constant tail placed LAST means the expression case is the `if` body
  (`if (Get(...)) return a || b || c; return true;`) -- 0x006A8B40, 0x006A8450, 0x006A76B0, 0x0083B3B0.
- x87 range filters: a member loaded before the deltas (`fld [ecx+0Ch]` first) was read into a named local (`float range
  = Range;`); all three deltas loaded before any product = a C3DVector local assigned member by member (separate float
  locals interleave `fsub ; fmul`) -- 0x006A76B0, 0x006A7780, 0x006A7AD0. `fabs(a - b) < r` with <math.h> gives
  `fabs ; fcomp [r] ; test ah,5 ; jp` (0x006A7020).
- A hidden-pointer C3DVector result read through the returned eax (`fld [eax]`) instead of its frame slot is bound to an
  inline helper's `const C3DVector&` parameter (`Overlaps(physics->GetMapwhoPosition(), physics)`); a named local or a
  const-reference local reads the slot -- 0x0063EF50 (parked on the square order only).
- A `??_G` with no vtable store that releases a member inline and then calls the base destructor on `this`: the
  polymorphic-derived/no-destructor model leaves the release out of line, and `__forceinline` on the release alone
  makes the ??_G call `??1` out of line; the match is a NON-polymorphic class holding the base as its FIRST MEMBER with
  `__forceinline ~T() {}` plus a `delete` helper -- 0x004C3CD0, 0x005E0DE0.
- A `??_G` that stores only the most-derived vtable but releases an intermediate base's member inline (no intermediate
  vtable store) = the intermediate base is abstract `__declspec(novtable)` with a `__forceinline virtual ~Base() {}`; a
  generated intermediate destructor is called out of line -- 0x00724DE0 (CTCPhysicsBase under CTCPhysicsStandard).
- `delete p` of a trivially destructible type emits `push ; call operator delete` with NO null test; retail's `test ;
  je ; push ; call` is an explicit `if (p) delete p;` -- 0x00724DE0.
- An unaligned member object (CIntelligentPointer at +0x11) = `#pragma pack(push,1)` around the derived class only
  (the polymorphic base keeps its natural 12 bytes) -- 0x0079C570.
- A temporary intelligent pointer assigned to a member (`m = CIntelligentPointer<T>(0);`): inline derived constructor
  over the out-of-line base constructor (derived vtable stored after the call), inline `operator=` =
  `SetPItem(o.GetPItem())`, compiler-generated derived destructor (no vtable reset, base dtor out of line); four
  adjacent one-bit clears merge into `and byte ptr [m],0F0h` -- 0x00838A40.
- A def-pointer LOCAL copy whose release has no null test and goes load/dec/store (`mov eax,[ecx+4] ... dec eax ; mov
  [ecx+4],eax ; jne`) is a CDefPointer class with copy constructor `if (p) ++p->count` and destructor `long c =
  p->count - 1; p->count = c; if (c == 0) p->Destroy();`; the member-release form (`dec dword ptr [ecx+4] ; jne`, clear
  inside the null test) is `T* p = m_p; if (p) { if (--p->count == 0) p->Destroy(); m_p = 0; }` -- 0x00838B20 vs
  0x00696060.
- In a size TU with a frame pointer (`optimize("y",off)` + `("s",on)`) the GetInterface presence test is the
  out-of-line `std::bitset::test` (0x00410DE0) on `&thing->bits`; the same template otherwise -- 0x007892EA.
- A `__fastcall` callee taking (ecx, edx, one stack word) whose stack word is pushed BEFORE a virtual call that produces
  the edx value is plain right-to-left evaluation of `F(m_owner->m_seq, GetTime(), m_extra)` -- 0x00C1E9F0.

Review at landing:
- 0063ef50 parked after 4 grades (square order only); next idea in NOTES.md.
- Local models next to existing engine headers: CThingGameObject.h (rows 008388d0, 00838a40, 00838b20, 00838b60) --
  the header pads +0x70/+0x74 and places PThingStandingOn at the PDB +0xD8 while retail uses +0xC0 (its retail `flags`
  +0xC8 agrees); CTCBase.h / CTCPhysicsStandard.h (rows 00724de0, 0079c570, 007a62d0) carry the vptr as a data word, so
  the virtual destructor chain had to be restated locally.
- Already-landed sources worth a look: rebuild/src/compiled/00/41/CObject_CObject_0041c4a0.cpp is an `asm_bake` body
  (inline-assembly byte emission) sitting in compiled/; rebuild/src/compiled/00/5d/CTCWeapon_IsAnalogueLoading_005d9270.cpp
  casts a vtable slot to a function pointer -- the CDefPointer local-copy class of 0x00838B20 is the clean form.
- Catalog rows that hold several functions: 005271b0, 00ac20d0, 00caef70 (split before authoring).
- Several rows were re-attributed from propagated labels (see NOTES.md "Label corrections"); line 1 of those sources
  carries the corrected Class::Name, so the landed file names will differ from the manifest labels.
