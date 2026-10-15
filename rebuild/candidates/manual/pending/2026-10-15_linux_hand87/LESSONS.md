# Chunk 87 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- `return a || b;` / `return !a && !b && !c;` on bool members compiles to full-register 0/1 exits (`xor eax,eax` / `mov eax,1`), while `if (x == 0 || x == 9) return true; return false;` keeps byte exits (`mov al,1` / `xor al,al`) -- 0x0049D310, 0x00661600, 0x0082CF40 vs 0x007E2790.
- CIVCountedPointer / CDefPointer `= 0` is the counted release followed by a SECOND unconditional zero store (`mov [esi+4],0` twice); model it as `if (Object) { Object->Release(); Object = 0; } Object = 0;` in a `__forceinline operator=(int)` -- 0x0071AC20, 0x0071AF30, 0x0071B040, 0x00780D20.
- The const-reference `Min` template (`b < a ? b : a`) selects an ADDRESS (`lea eax,[esi+18h]` / `lea ecx,[esp+4]`) and then dereferences it; a plain `a < b ? a : b` on values does not reproduce it -- 0x00750BA0 (and the parked 0x00569330).
- A static `__stdcall` helper written as `GetX()->StaticHelper(v)` keeps the getter call for its side effects and discards its result (no `this` load for the helper, nothing popped after it) -- 0x0071AA50 (as landed 0x006588DC).
- A `bool` default passed by const reference (`Transfer("Enabled", Enabled, true)`) is a one-byte temporary homed in the dead bool argument slot (`mov byte ptr [esp+14h],1 ; lea eax,[esp+8]`) -- 0x007B0330.
- A switch whose every case ends in a member `operator=` call compiles each case to a tail `jmp` that reuses the switch argument's stack slot for the source address (`mov [esp+4],offset g ; add ecx,6Ch ; jmp`) -- 0x007C8660.
- A virtual call on `this` as the last statement of a body becomes `jmp dword ptr [eax+N]`; because engine/CTCBase.h is a flat struct (no virtuals), such rows need a local polymorphic CTCBase model with the vtable slots -- 0x006C3B50, 0x00750CD0.
- A void member call with a test before it and an argument push stays `call ; ret` (no tail jump) -- 0x00750B80, 0x007E8A00, 0x007E8A20.
- Inlined `GetTCDriver(&out)` style out-parameter templates read the uninitialised `push ecx` frame word on the absent path when the caller does not test the result -- 0x00750DD0 (matches GOTCHAS 24l GetInterface note).
- STLport sentinel-list walks (`for (n = head->next; n != head; n = n->next)`) re-read the head from `this` at every back edge when the loop body calls out; returning a node record needs `T& rec = n->value;` in a local (`lea edi,[esi+8]`) -- 0x006D4D70, 0x0072A910.
- Frame-pointer size-optimised CTC constructors (`push ebp ; mov ebp,esp ; push dword ptr [ebp+8]`, `and dword ptr [..],0` zero stores) match under `#pragma optimize("y", off)` + `#pragma optimize("s", on)` -- 0x004DF2D4.
- Profile-locked audio-library getters: a float result is copied as a dword into a frame slot before the lock's destructor and `fld`ed after it; an `unsigned short` result rides in esi (`movzx esi`) -- 0x00C11150, 0x00C1E4D0.

Review at landing:
- No row needed more than 4 grades to match; 0x00569330 used its 4 and is parked (next idea in NOTES.md).
- Lane gaps to re-grade on the Windows lane: 0x00567D00 (extra trailing int3 after a noreturn call -- the grader may need to trim it), 0x006C3B70 / 0x006C3BD0 (inline-asm `fistp dword` rounding helper, asm_bake class).
- Local polymorphic CTCBase models (engine/CTCBase.h is flat): 0x006C3B50, 0x00750CD0, 0x004DF2D4, 0x007E2450. 0x007F6170 models CTCTargetingAI's base as a 0x38-byte pad (no engine header for CTCTargetingBase/CTCTargetingAI).
- ALREADY-LANDED label error: factory 0x004E181D "Create_CTCFishingRod" really constructs CTCAISummon (0x004DF2D4, vtable 0x01241C8C); the 0x00686D60 accessor is landed as CViewBase::GetDisplayEngine (propagated) while landed callers call it GetSubsystem (0x007E1F70) and GetWorld (0x007497C0) -- pick one name.
- Catalog rows that are not their labels' class: 0x00567D00, 0x00569330, 0x00C0AA10, 0x00C0AC00, 0x00C11150, 0x00C15F10, 0x00C1E4D0, 0x00C3CAA0, 0x00661600, 0x0082CF40, 0x0072A8D0, 0x0072A910, 0x007F6170 (see NOTES.md Label corrections). No catalog row in this chunk holds two functions.
