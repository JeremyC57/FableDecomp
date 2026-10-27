# Chunk 502 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (2026-10-27) Dynamic initializers (`.text$yc`) land with `// Leaf: _$E1` and the object defined in the TU with its real inline ctor; the atexit thunk is the TU-local `_$E2` and binds by relocation (0x01223EC0, 0x012243D0, 0x01224540, 0x01224CA5).
- An inline ctor/member holding a loop is never inlined by VC7.1 (a `call`/`jmp` appears); `__forceinline` on every level of it (ctor AND the SetIdentity it calls) restores retail's inline loop (0x01224340, 0x01224540).
- `++count` in an inline ctor on a counter defined in the same TU as `long count = 0;` folds to `mov [count],1` in the initializer, matching retail (0x01223EC0).
- VC7.1 RTM folds constant-evaluable static initializers into .data and emits no `_$E1`; retail's StreamingFontCacheBufferData_static records stayed dynamic, and empty user ctors / external linkage / late ctor definition do not stop the folding (0x01223F00, 0x01223F60 parked).
- An unreferenced TU-local static is dropped before any initializer is emitted: give it a reference (an accessor returning it) when probing.
- `new T(*this)` Clone of nested templates: the copy ctor must be `__forceinline` or VC7.1 calls the nested copies out of line; keep a retail out-of-line level out of line by declaring it as an explicit specialisation (0x00EF70B0..0x00EF7160 use 0x00EF6F40).
- `pow(base, k * inc)` in a 128 loop: write the exponent as its own `--exponent` counter; `-i * inc` lets VC7.1 merge index and exponent into one variable compared with -128 (0x00F35DA0).
- The two-argument `CPersistContext::Transfer(name, value)` is inline `Transfer(name, value, GetDefaultVal<T>())`, GetDefaultVal being a protected static `__fastcall` template (hidden result pointer in ecx) (0x00F27100).
- The GlobalConsoleCutsceneName CCharString("") initializer is size-optimised in retail and needs `#pragma optimize("s", on)` (0x01224CA5).
- A row that starts with a 5-byte `jmp` to the next 16-byte boundary followed by int3 padding and a second body is two functions: SKIP (0x00F29F60).

Review at landing:
- vland WARN RELOC_CONFLICT on `_$E2` (0x0122A5E0, 0x0122A610, 0x0122A790, 0x0122AAD0, 0x0122AC90, 0x0122AE74 here vs 0x0122A940 in landed 0x009F0240): `_$E2` is a TU-local compiler name, so the global reloc binding table cannot hold one address for it. Decide whether bindings for `_$E*` should be per-row only (no RELOC_DUPLICATE/RELOC_IDENTITY was raised).
- vland WARN SHARED_HEADER_AVAILABLE for CScriptThing, CDefClassBase, CPixelFormat, CNavQuadTree: the engine headers are data-only (NOTES.md "Header debt"); the local polymorphic models carry `// Layout:` lines.
- vland WARN RELOC_NAME: the `_$E2` targets carry junk propagated labels (NOTES.md "Label corrections"); 0x00C52E40 is labelled InitCPUCapabilities but bound here as `?DetectCpu@@YAXXZ` -- pick one name.
- 0x01224C20's object and function names are descriptive (libpk, no PDB symbol).
- 0x00F29F60 should be re-cut as a 5-byte thunk + the 0x00F29F70 body.
