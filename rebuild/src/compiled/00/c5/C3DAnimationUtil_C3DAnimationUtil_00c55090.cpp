// CSalRenderPanSSE2::CSalRenderPanSSE2() -- the out-of-line COMDAT copy of the SSE2 volume panner's inline constructor: the base CSalRenderPan
//   constructor (0x00C6D9D0), then this class's vtable 0x012B8A78
// Leaf: ??0CSalRenderPanSSE2@@QAE@XZ
// Option S: thin row of unit 0x00C55270 (retail SalSw3DVoice.obj slice). The constructor is the inline one of engine/CSalRenderPanSSE2.h;
//   CSalSw3DVoice::CreateRenderer calls this copy instead of expanding it, as the GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj does
//   (??0CSalRenderPanSSE2@@QAE@XZ is one of its COMDATs and its CreateRenderer relocates a call to it). Graded from the owner's object; no code
//   of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55090, 18 bytes: `push esi ; mov esi,ecx ; call 0x00C6D9D0 ; mov dword ptr [esi],0x012B8A78 ; mov eax,esi ;
//   pop esi ; ret`.
// Name (NF452, two sources; NF441 NAME_FIXES): (1) the SDK Release SalSw3DVoice.obj CreateRenderer calls ??0CSalRenderPanSSE2 in the C_HRTF_VOLPAN
//   case of its SSE2 branch, where retail 0x00C55270 calls 0x00C55090 after CodaMalloc(0x78), in the same call order (CodaMalloc + DisableSSE2, PanSSE2,
//   PanDelay...); the SDK COMDAT has the same 18-byte shape (base ctor call + vftable store); (2) the vtable's own slot 2 (Process 0x00C6E410 over the
//   SSE2 kernel 0x00C6E290; NF441 CLASS_TABLE). Was the shared stand-in ??0C3DAnimationUtil@@QAE@XZ (a propagated label).
// Flags: /EHsc (the unit's).
#include "../src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp"
