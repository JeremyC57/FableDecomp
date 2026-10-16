# Lessons from chunk 178 (for GOTCHAS)
- A negated inline equality keeps retail's unfolded `xor ecx,ecx ; test al,al ; sete cl ; mov al,cl` after every exit only when the equality is a file-static `__forceinline` free function (member or class-scope inline operator== lets VC7.1 fold the constants) -- 0x0099B150.
- Callee mod/ref knowledge: defining the REAL callee body above the caller under `auto_inline(off)` also tells VC7.1 the callee writes only through `this`/nothing, so loop-invariant member loads (0x00411570 compare in 0x004FC210) and values cached in callee-saved registers across the call (C2DBoxI::ClipTo in 0x004FC4D0) match. It does not help when the callee allocates (0x00AAD5D0 vector operator=).
- STLport vector operator[] as `*(begin() + n)` gives `mov eax,[start] ; lea ecx,[ebx+eax]` / `add ecx,eax` register choices where `Start[n]` gives `mov edx/ecx ; add` (0x004FC190, 0x004FC8A0).
- A truncated float pair converted through a `C2DCoordI(long, long)` constructor keeps retail's X-then-Y `_ftol2` order; two named long locals convert Y first (0x004FC0C0).
- Range checks with two separate `if (...) { Clear(); return -1; }` blocks give retail's layout; `a || b` places the failure block after the success code (0x00C3AA50).
- A scoped_ptr argument dereferenced through an inline `operator*` moves its load ahead of the constant pushes (0x0049EBF0).
- Building a char buffer {c, 0} in the dead argument slot: `return *this = c;` through an inline copy of operator=(char) (0x0099F180).
- Lane gap (4 rows): /GS functions reading a 1- or 2-byte payload value into the dead `event` argument slot (CWorld::EA*/LEA* game-event actions 0x00629390, 0x00629420, 0x0085CA80, 0x0085CAF0) -- every other byte matches; ours adds 4 frame bytes.
