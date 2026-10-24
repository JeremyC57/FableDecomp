# Chunk 346 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Dinkumware `_Adjust_heap` (all 18 /Gr rows here): write the bottom case as `first[hole] = first[idx - 1]; hole = idx - 1;` -- the `bottom - 1` spelling swaps eax/esi (0x00680750).
- An element copied into the `_Push_heap` slot as `mov eax,esp ; mov [eax],k ; add eax,4 ; mov [eax],..` = the second member's inline copy ctor written `{ *this = other; }`; a member-initialiser list folds the +4 into each store (0x0081AA50, 0x0086B070).
- A predicate result spilled to a frame dword (`and eax,1 ; mov [esp+14h],eax ; jne`) is a temporary bound to a const reference: `const unsigned long& bit = Random() & 1; return bit == 0;` (0x00807520).
- Retail CTurningAnimCompare (0x0087DE50) calls GetRotationInAnimation twice on its first argument: reproduce bugs as written when both calls use the same register.
- STLport `map::operator[]` with retail's duplicated return tail: `if (...) { It inserted = insert_unique(i, value_type(k, T())); i = inserted; } return i->second;` -- an early return reorders the blocks, `i = insert_unique(...)` reads the result through eax (0x004C6E00). A T() whose ctor leaves members uninitialised fills the temporary from eax (`mov [esp+0c]/[10]/[14]` chain).
- Compiler-generated copy ctor out of line: a placement-new helper `return new (where) T(source);` emits `??0T` as its own COMDAT (0x00D18C50, 0x00F0CA60); operator= via a one-line `a = b` helper (0x00D15170).
- `offset += m_position` (argument reused as the result) vs a separate local swaps eax/edx in small range checks (0x00C426F0).
- `wchar_t` is not a keyword under the lane flags; include `<string.h>` for `_wcsicmp` instead of declaring it (0x00497160).
- The /Os 0x4xxxxx TUs need `#pragma optimize("y", off)` + `("s", on)` for vector::resize(n) and TransferVectorOfSubComponents (ebp frame + `leave`); 004dfdbb needed only `("s", on)` plus `__forceinline` on the comparator chain.

Review at landing:
- 00807520 binds the coin bit to a const reference; plausible but unusual -- check it against any other CDoNotUseBinaryPredicate instantiation.
- 0071f650 / 0086f450 / 00adb000 / 00680750 / 004cbed0 put a user copy ctor on the pair/element (needed for the `mov eax,esp` slot build); Dinkumware's std::pair has none, so the real element types probably carry one (CDefString / enum wrappers) -- model with real types if found.
- 00742fd0 uses `__declspec(novtable)` on CModeDragonFlyingBase (precedent: landed 0x008CC280); CModeBase/CAnimFilterOfGroup are local models -- no engine headers exist.
- 009fc5c0, 005e1210 (parked): engine/CResource.h, CResourceList.h, CIVCountedPointeeBase.h are flattened PODs with `void* __vftable`, so the sources use local polymorphic models next to existing headers.
- 00c27e60 / 00c27f50 include engine/CHandleBasedMemoryPool.h; the landed InsertFreeArea 0x00C27EF0 and dtor 0x00C287E0 still use local pad models of the same class.
- 00d15170 / 00d18c50 / 00f0ca60 TUs carry a helper function (`AssignTeleportData` / `CopyTeleportData`) besides the leaf symbol.
- Landed 0x00743960 (CModeDragonStrafe scalar deleting dtor) is a fastcall free-function model with raw offsets -- worth a retype now that 0x00742FD0 is named.
