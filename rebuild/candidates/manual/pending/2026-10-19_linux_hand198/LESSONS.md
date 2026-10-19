# Lessons from chunk 198
- Def-class operator= rows are found from vtable slot 19 (the 5-byte `jmp` thunk right before the body); the rows are size-optimised (`#pragma optimize("s", on)`), a `float[N]` member copies as a `push N ; pop edx` loop and a `bool : 1` member as `mov cl,[src] ; xor cl,[dst] ; and cl,1 ; xor [dst],cl`.
- Propagated "~CPatchTesselationEdgeStrip" destructor labels resolve fast: find the single caller (the deleting dtor) and look its address up in rtti-vtables.tsv slot 0.
- Intrusive (CResource) pointer release must be an inline `Release()` member (`dec [ecx+4] ; jne` without a reload); open-coding `--p->RefCount` reloads the pointer.
- A load that retail places after a loop guard comes from a pointer declared inside an explicit `if (count > 0)`.
- An element-destroying vector dtor loop caches `last` only through an inline `DestroyRange(first, last)` helper.
- An if/else of two member calls that differ in one argument tail-merges into one call (pushes shared); the ternary argument version does not.
- Ushort bitfield groups: declare every field of the group (here `Emitter2DLayer : 7`) or the next uchar group lands 2 bytes early.
- Two retail functions with the same propagated name (0x00AE89E0 / 0x00AE8A80) need distinct C++ names, or the landing verifier sees one symbol bound to two targets.
- Tests: a source that declares `operator delete` cannot be followed by `#include <new>` under g++; call destructors qualified (`p->T::~T()`) on raw buffers so no vptr is needed; on 64-bit compare sizes with `sizeof`, not retail constants.
