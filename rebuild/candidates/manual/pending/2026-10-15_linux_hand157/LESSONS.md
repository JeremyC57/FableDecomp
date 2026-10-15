# Chunk 157 lessons (for GOTCHAS)
- A global read into a frame copy and `fild`ed from there (not `fild [global]`) is a local assigned BEFORE intervening stores that could alias the global (0x8522B0).
- Copies rotating through only ecx/edx while eax is free = eax is live: the function returns a callee's result held in eax (0xC4D440/0xC4D4B0 return the helper's long).
- A 36-byte POD placed by placement new is `rep movsd` with the implicit copy even over named fields; a `__forceinline` memberwise copy ctor gives the dword pairs (0x778210).
- `return D3DXMATRIX(16 floats)` from an inline ctor: right-to-left argument evaluation loads the last seven floats on the x87 stack and spills the first five through integer registers (0x988990).
- Null tests of `this` and `this+1` in a ctor come from inlined `Set(bool v, bool* old) { if (old) *old = cur; ... }` setters called with `&member` (0x9D8AD0).
- By-value CScriptThing: model PImp as an embedded CCountedPointer with its own inline copy ctor; loose Data/Info fields CSE the member address into another register (0xE2A0F0).
- A templated GetInterface member in a local model must be `__forceinline`, else it is emitted out of line (0x904310).
- `if ((m = p->x) != 0)` tests the loaded register; `m = p->x; if (p->x)` re-reads memory (0xC34370).
- Parked twice: X-term-only operand-order swaps in camera-module float sums (0x852220, 0x8531E0) -- treat as scheduling.
