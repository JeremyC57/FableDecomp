# Lessons from chunk 127 (for GOTCHAS)

- CCreatureActionBase-family constructors: the animation lookup 0x006924B0 is a `__fastcall(owner, const CCharString&)` thunk and
  the name literal is an IMPLICIT conversion temporary (`GetRandomLoadedAnimation(owner, "NAME")` with `CCharString(const char*, long = -1)`):
  built before the owner load, `lea`-re-addressed, destroyed after the base call. The same implicit form reproduces
  `SetAnimationName("LIT")` in a constructor body (the derived vtable store lands between the temporary's pushes and its ctor call)
  and `healSelf ? "A" : "B"` (one temporary from a register-selected literal pointer).
- CIntelligentPointer members must be modelled 8 bytes (vptr + item): a vptr-only model puts the following member 4 bytes low
  (008c94d0 +0x11c vs +0x120).
- A C3DVector copied into a byte-packed member with three dword moves straight into the member is the memberwise USER copy
  constructor (`X(o.X), Y(o.Y), Z(o.Z)`); a POD copy forms the member address once (`lea edx,[esi+115h]`) and stores through it.
- A component pointer read from a thing and used as `this` (`mov ecx,[eax+60h] ; mov edx,[ecx]`) needs an inline accessor
  (`GetPhysics()`), as wave 24a says; the plain member read gives `mov eax,[eax+60h] ; mov edx,[eax] ; mov ecx,eax`. The same
  accessor form also moves a constant argument push (`push 0FFh`) after the object load.
- `__fastcall(creature, def)` where retail reads `def` into a callee-saved register before the GetPItem call: name the def in a
  `long` local per call (two statements re-read the member twice).
- `GetInterface(&p, type)` with the result IGNORED (BeDazed / BeStunned) compiles to the `jmp` join with `mov esi,[esp+4]`
  on the not-present path -- keep the call unchecked, do not add a null test.
- The `&&` of a long-returning (BOOL) inline helper and a `!bool` call materialises as `mov eax,1` / `xor eax,eax` branches
  (00692ec0 body), where a bool `&&`, `!x` or `?:` gives setcc idioms (`neg al ; sbb eax,eax ; inc eax`).
- An unoptimised TU (`#pragma optimize("", off)`) with a `throw` of a one-byte exception object needs `/EHsc`; the exception
  object is built at [ebp-2] and copied byte-wise to [ebp-1] by the throw; `g = g + 1` and `++g` both give
  `mov eax,[g] ; add eax,1 ; mov [g],eax` there.
- A void method whose `if (state != 0) { Check(); return; }` branch calls a bool member and returns compiles to the
  `pop esi ; jmp Check` tail call (00dcec70); nested `p->B(&t, p->A())` on a member pointer shares one vtable load with
  the member reloaded after the inner call, while a separate statement reloads the vtable.
- Retail CCreatureActionBase is 0xa8 bytes and CCombatActionBase 0x114 (PDB: 0xb4 / 0x12c): every derived member in this
  chunk sits 12 / 24 bytes below its engine-header offset; model locally and say so.
- Lane gaps seen (parked): the push schedule after an argument call (00692aa0), the esi/edi tie-break between `this` and the
  pointer-member address (008b9dd0), an early `return false` kept as its own `xor al,al` block (00692ec0), the register
  through which a def-pointer local is loaded (007c3070).
