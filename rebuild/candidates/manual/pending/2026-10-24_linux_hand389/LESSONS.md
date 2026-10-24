# Chunk 389 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `CThing::HasTC` written as `if (bits & mask) return true; return false;` -> `test edi,mask ; je` on the bitset word.
  The `return (bits & mask) != 0;` form gives `shr ; test al,1` instead. (0x007034D0)
- When a function calls the non-const `CInterfaceMap::LowerBound` 0x40F020, giving it a same-TU noinline stand-in body
  (a binary search, under `#pragma auto_inline(off)`) lets VC keep the CThing bitset word in edi across the call.
  With only a declaration, VC reloads it. (0x007034D0)
- `const T* p = 0; thing->GetInterface(TYPE, &p); if (p && ...)` -> the inline GetInterface is jump-threaded: a clear
  bit skips straight past the `p` test. Take interface type numbers from each class's GetInterfaceType (vtable slot 21,
  `mov eax,imm ; ret`): CTCHeroStats 4, CTCCarryable 0x47, CTCWeapon 0x48, CTCWeaponTrail 0x4C, CTCQuestCard 0x6C.
  (0x00702810, 0x007034D0)
- A retail store order of `_M_end_of_storage`, then `_M_finish`, then `_M_start` after a fill comes from a local-pointer
  construction: `data = malloc; last = fill(data, n); eos = data + n; finish = last; start = data;`. (0x00701750; parked
  2 bytes off)
- Declaring the zero `result` AFTER the allocating call -> `xor ebp,ebp` after the call. Declared before the call, VC
  zeroes it ahead of the call. A named `physics = object->PhysicsTC` local hoists the `[esi+60h]` load out of the
  if/else. (0x00703210; parked on an ebx/edi swap)
- `_Vector_base(n, a)` with `_M_start(0), _M_finish(0), _M_end_of_storage(0)` in the init list and the allocation in the
  body -> the three zero stores land before the size computation. (0x006F43C0)
- STLport `pair<T1,T2>` needs its explicit copy constructor. Implicit copy changes how by-value pairs are built in
  argument slots. (0x006F3260, the SKeyedEvents algorithms)
- A tree iterator's `++` must be STLport's static `_Rb_global<bool>::_M_increment(node)` returning the node. A member
  increment inlines differently. (0x006F34B0, 0x00702810)
- In a function with a float accumulator, `float result = 0.0f` declared FIRST -> the `fldz` is preloaded and kept on
  the x87 stack. (0x006FD4F0)
- `end = Start + n` computed once in a fill loop -> `cmp` against a register. A `Start + Size` bound re-reads memory
  each iteration. (0x006FAFE0)
- Wide string literals in the lane: VC7.1 here has no built-in wchar_t, so model `CWideString(const unsigned short*)`
  and cast `L"..."`. The code is the same. (0x006F9EE0)
- Same-TU noinline bodies for `__copy_backward`, `__destroy_aux` and `__find_if` let an empty tag temporary share a dead
  argument slot (GOTCHAS 24l, confirmed again). (0x006FD330, 0x006F9AC0)
- Find's key copy (`K key = k; LowerBound(key)`) is homed in the dead argument slot of the caller's first parameter.
  When a second local also wants that slot (an out index, a find_if tag), VC7.1 may give the key its own `push ecx`
  word instead. (0x006F9640, parked)

Review at landing:
- Stand-in callee bodies that are not the real code (only the call matters):
  - `_M_insert_overflow` in 006f4930, 006fc8e0, 006fcc80 and 006fcd90;
  - `CInterfaceMap::LowerBound` 0x40F020 in 007034d0.

  All are same-TU noinline under `#pragma auto_inline(off)`, the accepted stand-in pattern.
- The early SKeyedEvents probes (006fb8a0..006fcea0) model `NAnimationEvents::CEvent` as a descriptive
  `struct CEvent { long m_a, m_b; }` (the Index/Time bitfield appears only where used) and use a local STLport-shaped
  `pair`, not the engine header. Retype them if the engine header for CEvent matches.
- 006f2760 and 006f2870 use `CThingCreatureBase` / `SCreatureByUID` value types. The tree code is value-type agnostic.
- 006f3260 uses a pack(1) byte-packed C3DVector, following the landed precedent.
- Already-landed sources disagree on interface type numbers:
  - TC 0x47 appears as CTCBoxContents, CTCClaim and CTCCarryable. The vtable GetInterfaceType says CTCCarryable.
  - TC 0x48 appears as CTCCarrying and CTCWeapon. The vtable says CTCWeapon.

  Grep the landed tree for those enums.
- Callee 0x00686D50 is labelled both `CTCBase::GetPlayerManager` and `_global::GetGlobal_013b8a1c_deref0x30`; pick one.
- Retail layout offsets differ from the PDB header in places:
  - `CThing::PhysicsTC` is at retail +0x60 (PDB +0x70).
  - `CTCHeroStats::RenownLevel` is at +0x70.
  - `CQuestCardDef::Prerequisites` is at +0x5C.

  The sources use the engine headers where they agree and say so where they don't.
- Local models sit next to existing engine headers:
  - `CTCQuestCard`, `CTCHeroStats` and `CTCPhysicsBase` (00702810, 00703210);
  - `CGameCameraBase` (006fd4f0);
  - `CReactionFrequencyTraits_PeriodicFrequency` (006fed30);
  - `CManager` (the NAnimationEvents rows).

  Two reasons. First, the generated headers are flat pack(1) PODs with `__vftable` and no ctor, virtuals or inline
  members, but the rows need a real ctor, vtable or inline members. Second, the PDB's 16-byte STL containers shift
  every later offset (retail uses 12 bytes). The models use the PDB member names at the retail offsets. Retype them
  once the headers model STLport containers at 12 bytes.
- No row needed more than 4 grades on a single body form. Every parked row stopped at 4 attempts.
