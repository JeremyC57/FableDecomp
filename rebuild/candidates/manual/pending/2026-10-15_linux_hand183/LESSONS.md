# Chunk 183 lessons (for GOTCHAS)
- A literal passed to a `const CCharString&` parameter (IMPLICIT conversion temporary) is constructed before every
  other argument and before the object expression; an explicit `CCharString("..")` temporary is built in argument order.
  (0x958000, 0x93C600, 0x93C6D0)
- A value compared after a call and held in a callee-saved register is a NAMED local read before that call
  (`CThing* self = PBrain->PCreature; if (p.GetPItem() == self)`). (0x90CB80, 0x94BDF0)
- `lea ecx,[eax+N]` for an intelligent-pointer member of another object = the member reached through an inline getter
  (`CThing* GetPHome() { return PHome.GetPItem(); }`); direct member access gives `mov ecx,.. ; add ecx,N`. (0x957F20)
- An inline const `GetCreature() const { return PBrain->PCreature; }` accessor reorders the PBrain load like retail
  in action-constructor argument lists. (0x93C6D0)
- A bool computed inside a block of destructible locals and tested after it (kept in bl) is declared OUTSIDE the block.
  (0x921630)
- A member-function pointer called as `mov ecx,[this+off+4] ; add ecx,this ; call fn` needs the multiple-inheritance
  representation: `#pragma pointers_to_members(full_generality, multiple_inheritance)` (PDB size 0x10). (0x95F6F0)
- A class whose first data members after the vfptr include __int64 must model the vfptr as a plain `void*`, or MSVC
  pads the vfptr to 8. (0xCA52A0)
- `HasTC` helpers must be `__forceinline`; plain inline was emitted out of line in a larger body. (0x92E890)
- Lane gaps seen: in-place RMW (`and byte [eax+D4h],0FEh`, `dec dword [eax+D0h]`) on a GetPItem() result; div
  remainder sunk past a call; dead-argument-slot temporaries (0x922FC0, 0x965210); a constant-false `xor cl,cl ; jne`
  after a dead frame store in the inlined CMessageEvent sender accessor (0x744D00, 0x958B20); register swaps in
  list/loop bodies (0x958C00, 0x92E130).
