# Chunk 494 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Dinkumware (libpk) `vector::operator[]` is `*(begin() + _Pos)` over the `_Ptrit` class. It leaves a dead
  `lea eax,[eax+ecx*4]` after the load through the slot (0x00C283A0).
  - A raw `T**` table, a slot pointer or a slot reference all drop it (19v22).
- Feeding two virtual getters' `*result` into one call: retail reads the second word straight after its call (0x00C19E90).
  - Write `long second = *GetSecond(); return F(*GetFirst(), second);`.
  - The one-expression form defers the dereference (48v43).
  - Two named locals hoist the first load above the comparer load (43v43).
- An unsigned char argument normalised into a member with `m = flag != 0` (0x00C26A60 / 0x00C26EC0):
  - into a short member: `xor edx,edx ; test al,al ; setne dl ; mov word`.
  - into a bool member: `test al,al ; setne dl ; mov byte`.
- A long result of an unsigned `count > 0` is `xor ecx,ecx ; cmp ecx,eax ; sbb eax,eax ; neg eax` (0x00C34A50).
- MFC's `operator==(const CString&, const CString&)` returned as an int compiles to `neg eax ; sbb al,al ; inc al ; movzx`.
  `operator<` compiles to `test ; setl cl ; movzx` (0x00C2DF40 / 0x00C2DF60).
- VC7.1 keeps a loop whose only result is dead, such as an ASSERT-only `largest` walk (0x00C280E0, like the engine
  Validate 0x00A024C0). Write the walk; do not expect it to fold away.
- Reading a call's arguments: pushes made BEFORE an inner no-argument call can belong to the outer call (0x00C32F60).
  The inner virtual was `mov ecx,[ecx+4] ; jmp getter`, and the outer callee's `ret 18h` proves six arguments.
- In a mangled Leaf, a repeated pointer ARGUMENT type is the argument back-reference `0`, not `1`
  (`...PBUCItem@@0@Z`). A hand-written `1` matched no symbol, and q.py still graded MATCH. Take leaves from `objdump -t`.
- The vtable that a gap-scan row heads is the one whose address a constructor stores (xref.py on the slot address).
  It is not the slot before it: 0x012B401C is its own vtable, not slot 3 of 0x012B4010.

Review at landing:
- 0x00C1EA30 has two bound spellings: CParticle::IncrementLifetimer (kept, the catalog name) and CSoundSample::AddRef
  (landed 0x00C262E0). 0x00C1EA40 has four spellings and is landed as "IncrementLifetimer" although it decrements.
  0x00C1EA20 / 0x00C1EA30 / 0x00C1EA40 are one class's reference-count trio at +0x1E.
- 0x00C20720 (CSoundEvent::CheckState) has 7 bound spellings, and 0x00C20EE0 had a stale ?GetValue48@CPlayer@@ binding.
- The libpk pool reuses the engine's class name CHandleBasedMemoryPool. 0x00C28220's symbol
  ?MoveResource@CHandleBasedMemoryPool@@MAEXPAXJ00K@Z is also the engine PDB symbol of 0x00A02120, which is landed under
  the invented stand-in CHandleBasedMemoryPool_Methods. Renaming that row to the PDB spelling would give two addresses one
  symbol. The landed libpk siblings (0x00C28050..0x00C283E0) already share the engine name.
- 0x00C34C40 / 0x00C34C80 type their list as CPtrArray<CBlendEntry*>, so that Grow 0x00C1E640 and Remove 0x00C1E900 bind
  to the existing symbols. The real element type of the 0x012B4FEC list is unproven.
- Landed 0x00C34A40 spells 0x00C42AC0 as CBaseIntelligentPointer::GetPItem_00c42ac0. It is CPtrArray's count getter
  (`mov eax,[ecx+8]`).
- The landed PKCWorkerThread::ProcessNextRequest 0x00C26B20 models pointers at +0x14/+0x18/+0x1C. The constructor
  0x00C26A30 embeds the 0x30-byte worker queue at +0xC (CVtable_012b401c), so that layout looks wrong.
- Landed 0x00C25A20 (the 0x012B3FA0 constructor) is a raw `struct C` with Leaf OnPostParentRelocation. It is
  PKCTimerManager's base or own constructor: vtable, shutdown byte +4, target +8, rate +0xC, +0x10.
- The landed 0x00C2A580 header says "slot 1 of vtable 0x012B4180". The vtable stores (0x00C2A698) show that 0x012B4180
  starts with it: slot 0, with the scalar deleting destructor at slot 1.
