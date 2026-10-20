# Chunk 222 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A refcount sequence of +1, +1, -1 (`inc edx ; mov ecx,edx ; inc ecx ; mov eax,ecx ; dec eax`, three stores, then `jne` release) comes from a static `__forceinline` pass-through `T Pass(T p) { return p; }`: param copy, return copy, param dtor. A by-value getter `return Obj;` gives one copy; a named `r` gives two. (009ae6b0)
- Smart-pointer operator= with `dec [ecx+4] ; jne ; mov edx,[ecx] ; call [edx+4]` (short release, no reload): keep the old pointer in a local (`T* old = P; if (old) { if (--old->Ref == 0) old->Destroy(); P = 0; }`). Decrementing through the member reloads `[ecx+4]`. (009ae6b0)
- An out-local in the dead argument slot (`lea edx,[esp+arg]` pushed as an out pointer while the argument lives in edi) needs the local block-scoped. Close the block after the call that consumes it. (009c1050)
- Struct-building stores in retail order: VC7.1 keeps source statement order for independent member stores. Write the field copies first and the computed fields last, as retail does. (009c1050)
- A clamp written as `cmp eax,20h ; jg ; mov eax,20h` is `long w = x; if (w <= 32) w = 32;`. The `x > 32 ? x : 32` ternary loads 32 into a register and compares against that. (009c1050)
- A `call` to a cdecl compare with `add esp,8` instead of the inline `strcmp` byte loop means `_stricmp`; VC7.1 inlines `strcmp` as an intrinsic. (009ae6b0)
- The mesh base's zero-initialised members must be member objects with inline ctors (CIVCountedPointer, ThemeChannelDesc). Plain init-list scalars let VC hoist the vtable stores above them and cross-jump the bodies. (00ba7820)
- A column loop counting up (`src[row*32+col]`, `1<<(31-col)`) unrolls by 4; a pointer walk stays rolled. (00b3c4b0)
- Three block-scoped bool locals share the dead argument slot. (009acb20)
- A same-TU `#pragma auto_inline(off)` stand-in for map operator[] lets VC see that the argument does not escape; that keeps `layer` in ebp and the camera in edi. (00b93f40, 00b94060)
- if/else plus an inline `Contains` (if / return true / return false) keeps two epilogues instead of cross-jumping. (00baca70)
- A camera delta as a C3DVector local gives the 0x1c frame. (00bbbf70)
- Transfer of a vector written as `switch { case 2: In; default: Out; }` gives `sub eax,2 ; je`. (0067e290, parked for other reasons)
- `test ah,41h ; jp` is `if (x <= y)` with the effect arm first. (0064d6a8)
- An out-parameter inline matrix product lets VC drop the unused elements; a by-value return keeps all twelve. (00b451d0)
- An unsigned refcount gives `jbe`; a null-safe GetRefCount gives the eax test. Declaration order picks the esi/edi colouring. (00b39fb0)
- STLport list in retail is one node pointer (4 bytes); Ego_r.pdb's list is 12 bytes. Every member after a list sits 8 bytes lower than the PDB says (CDisplayManager::LastDeviceCreationParameters is +0x268 in retail, PDB +0x270). (009c1050)

Review at landing:
- 009ae6b0 and 009c1050 use local base models although rebuild/include/engine/CDefinitionManager.h exists (CDisplayManager has no header listed). Rebase them on the headers and re-grade; the CDisplayManager offsets after DeviceResetCallbacks must use the retail (STLport) layout.
- Further local models next to existing headers: 009acb20 (CDefClassBase.h), 00b3c4b0 (CEngineResourceManager.h), 00b5ee80 (CEngineScreenEffectGlowRenderer.h; it keeps local shader classes for the dtor chain), 00ba7820 (CEnginePrimitiveManagerStaticMeshesBase.h).
- 00b93f40 and 00b94060 carry same-TU `auto_inline(off)` stand-in bodies for map operator[]. Check that the lander does not emit them as separate landed symbols.
- 00cac070 needs `// Flags: /EHsc`, leaf `??0CBasePin`. Already landed: 0x00CAC260 is named "CEntryRecord", but its stores are CBaseInputPin members; rename it.
- 0065196c needs `#pragma optimize("y",off)` + `("s",on)` (frame-pointer NPlayerGui TU).
- 00562ec0 hard-codes the literals "fableclangw.lionhead.com" and "/index.php"; they are the retail strings at 0x0124DB14 and 0x0124DB08.
- Lane gaps for the fleet:
  - inline-asm fistp: 00b947e0, 00b94bf0.
  - push_back pointer/tag in the dead argument slot: 00b47fd0, 00b480e0.
  - shrink-wrapped prologue: 00b905a0.
  - SIB operand order: 00be0cd0.
  - x87 fmul canonicalisation: 00b81050.
