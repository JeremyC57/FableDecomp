// CSalRenderDisableSSE2::CSalRenderDisableSSE2() -- the out-of-line COMDAT copy of the SSE2 plain-gain renderer's inline constructor: the base
//   CSalRenderDisable constructor (0x00C6E760), then this class's vtable 0x012B8A8C
// Leaf: ??0CSalRenderDisableSSE2@@QAE@XZ
// Option S: thin row of unit 0x00C55270 (retail SalSw3DVoice.obj slice). The constructor is the inline one of engine/CSalRenderDisableSSE2.h;
//   CSalSw3DVoice::CreateRenderer calls this copy (`call 0x00C550B0` at 0x00C552DE) instead of expanding it, as the GameCODA 2.10 SDK CODAApi.lib
//   Release/SalSw3DVoice.obj does (??0CSalRenderDisableSSE2@@QAE@XZ is one of its COMDATs and its CreateRenderer relocates a call to it). Graded
//   from the owner's object; no code of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C550B0, 18 bytes: `push esi ; mov esi,ecx ; call 0x00C6E760 ; mov dword ptr [esi],0x012B8A8C ;
//   mov eax,esi ; pop esi ; ret`.
// Name (NF452, two sources; NF441 NAME_FIXES): (1) the SDK Release SalSw3DVoice.obj CreateRenderer calls ??0CSalRenderDisableSSE2 in the default
//   (disabled-HRTF) case of its SSE2 branch, where retail 0x00C55270 calls 0x00C550B0 after CodaMalloc(0x38), in the same call order (CodaMalloc +
//   DisableSSE2, PanSSE2, PanDelay...); the SDK COMDAT is byte-identical in shape (base ctor call + vftable store, 18 bytes); (2) the vtable's own
//   slot 2 (Process 0x00C6EEF0 over the SSE2 kernel 0x00C6ED70; NF441 CLASS_TABLE). Was the shared stand-in ??0C3DAnimationUtil@@QAE@XZ (a propagated label).
// Flags: /EHsc (the unit's).
#include "../src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp"
