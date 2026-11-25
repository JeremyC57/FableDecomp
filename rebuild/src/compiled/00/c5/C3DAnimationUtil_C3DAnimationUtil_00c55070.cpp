// CSalRenderPanDelaySSE2::CSalRenderPanDelaySSE2() -- the out-of-line COMDAT copy of the SSE2 HRTF-delay panner's inline constructor: the base
//   CSalRenderPanDelay constructor (0x00C6CA20), then this class's vtable 0x012B8A64
// Leaf: ??0CSalRenderPanDelaySSE2@@QAE@XZ
// Option S: thin row of unit 0x00C55270 (retail SalSw3DVoice.obj slice). The constructor is the inline one of engine/CSalRenderPanDelaySSE2.h;
//   CSalSw3DVoice::CreateRenderer expands it (`call 0x00C6CA20 ; mov dword ptr [esi],012B8A64h`, immediate at 0x00C55358) and the same translation unit
//   keeps this out-of-line COMDAT copy (no retail caller), as the GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj does
//   (??0CSalRenderPanDelaySSE2@@QAE@XZ is one of its COMDATs). Graded from the owner's object; no code of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55070, 18 bytes: `push esi ; mov esi,ecx ; call 0x00C6CA20 ; mov dword ptr [esi],012B8A64h ;
//   mov eax,esi ; pop esi ; ret`.
// Name (NF447, two sources): (1) the SDK SalSw3DVoice.obj CreateRenderer constructs CSalRenderPanDelaySSE2 for C_HRTF_DELAY in its SSE2 branch,
//   where retail 0x00C55270 builds the 0x214-byte object with vtable 0x012B8A64, and the SDK object defines this COMDAT; (2) the vtable's own
//   slots (ClearFilterHistory = the SDK size, Process over the SSE2 kernels; NF441 CLASS_TABLE). Was the shared stand-in ??0C3DAnimationUtil@@QAE@XZ
//   (a propagated label, also on 0x00C55090 .. 0x00C55110).
// Flags: /EHsc (the unit's).
#include "../src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp"
