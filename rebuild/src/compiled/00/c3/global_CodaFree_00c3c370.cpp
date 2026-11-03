// CodaFree(void* p) -- GameCODA 2.10 Memory.obj free: call the host hook CODA_MemFree (0x0139C980, installed by the API 0x00C332A0)
// Leaf: _CodaFree
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C370, 12 bytes (+ int3 padding), `__cdecl`: `mov eax,[esp+4] ; push eax ; call [hook] ; ret` (the hook is `__stdcall`).
// Name (owner decision 2026-11-03 D3, "faithfully recreate what retail did"): the original GameCODA 2.10 Memory.obj defines extern "C"
//   CodaFree and relocates ?CODA_MemFree@@ (0x0139C980); its body is byte-identical to this one after that one relocation (Codex pass 12
//   SDK_COMPARISON 00c3c370). Pass 11 spelled it a static operator of the root class (CPKObject, now CCodaObjectBase); the
//   class operators are inline forwarders to this function (engine/CCodaObjectBase.h), so every retail caller binds _CodaFree.
#include "engine/CodaMemory.h"
extern void (__stdcall* CODA_MemFree)(void*);   // 0x0139C980

extern "C" void __cdecl CodaFree(void* p)
{
    CODA_MemFree(p);
}
