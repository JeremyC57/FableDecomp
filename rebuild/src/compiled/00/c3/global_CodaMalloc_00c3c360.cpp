// CodaMalloc(unsigned int size) -- GameCODA 2.10 Memory.obj allocator: call the host hook CODA_MemAlloc (0x0139C97C, installed by the API 0x00C332A0)
// Leaf: _CodaMalloc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C360, 12 bytes (+ int3 padding), `__cdecl`: `mov eax,[esp+4] ; push eax ; call [hook] ; ret` (the hook is `__stdcall`).
// Name (owner decision 2026-11-03 D3, "faithfully recreate what retail did"): the original GameCODA 2.10 Memory.obj defines extern "C"
//   CodaMalloc and relocates ?CODA_MemAlloc@@ (0x0139C97C); its body is byte-identical to this one after that one relocation (Codex pass 12
//   SDK_COMPARISON 00c3c360). Pass 11 spelled it a static operator of the root class (CPKObject, now CCodaObjectBase); the
//   class operators are inline forwarders to this function (engine/CCodaObjectBase.h), so every retail caller binds _CodaMalloc.
#include "engine/CodaMemory.h"
extern void* (__stdcall* CODA_MemAlloc)(unsigned int);   // 0x0139C97C

extern "C" void* __cdecl CodaMalloc(unsigned int size)
{
    return CODA_MemAlloc(size);
}
