# Chunk 72 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A deleting destructor that stores only the BASE vtable: the class declares no destructor (VC7.1's generated destructor
  stores no vtable of its own); the lane will not inline that generated destructor into `??_G`, and declaring one adds
  the class's own store. The wave-24m model matches: the polymorphic base as the FIRST MEMBER of a non-polymorphic
  class, a `__forceinline` destructor and counted-pointer release, and a `delete` helper (006e7bd0, 007e7520).
- A pointer argument stored back into its own stack slot (`mov [esp+N],eax` before its address is pushed) = a derived
  entry pointer bound to the container's base-pointer `const&` key: the conversion temporary lands in the argument's
  dead slot (all 7 CScreenEffectPriorityStack rows first try).
- Retail keeps a list head and an argument's string block in registers across `Compare` (0x00411570) when the loop
  writes no memory; the lane reloads both -- cache both in locals (the block read after the empty test) and walk with
  do/while (004af5b0).
- The 2^32 fix-up after `fild`: `fadd dword` = the unsigned value converted to float, `fadd qword` = to double; the
  fall-through branch is the `if` body (009f8ba0).
- Inside a destructor, `lea eax,[esi+0Ch] ; push eax ; call` to a `__stdcall` method = a COM-style interface method
  bound statically: model with `__stdcall` pure-virtual interface bases (00caa410, needs `/EHsc`).
- Pragmas: 0042be50 (`optimize("y",off)` + `("s",on)`), 00656c77 and 00cd2757 (`optimize("s",on)`).

Review at landing: parked 00c25860 (x87 operand order), 00cbe583 (vptr in eax vs edx, the 0x0053D000 gap) and 00caa3c0
(loop rotation, the 23f gap) are lane gaps; 006c59a0 reads the iterator through `find`'s result instead of its stack slot.
Label note: the manifest names 00cbe512 GetSquaredDistanceBetweenThings but it returns the square-rooted distance;
00cbe583 is the squared one. Corrections in NOTES.md.
