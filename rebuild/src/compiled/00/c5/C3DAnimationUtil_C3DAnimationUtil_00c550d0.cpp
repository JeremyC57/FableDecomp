// CSalRenderPanDelayMMX::CSalRenderPanDelayMMX() -- the out-of-line COMDAT copy of the MMX HRTF-delay panner's inline constructor: the base
//   CSalRenderPanDelay constructor (0x00C6CA20), then this class's vtable 0x012B8AA0
// Leaf: ??0CSalRenderPanDelayMMX@@QAE@XZ
// Option S: thin row of unit 0x00C55270 (retail SalSw3DVoice.obj slice). The constructor is the inline one of engine/CSalRenderPanDelayMMX.h;
//   CSalSw3DVoice::CreateRenderer expands it (`call 0x00C6CA20 ; mov dword ptr [esi],012B8AA0h`, immediate at 0x00C55459) and the same translation unit
//   keeps this out-of-line COMDAT copy (no retail caller), as the GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj does
//   (??0CSalRenderPanDelayMMX@@QAE@XZ is one of its COMDATs). Graded from the owner's object; no code of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C550D0, 18 bytes: `push esi ; mov esi,ecx ; call 0x00C6CA20 ; mov dword ptr [esi],012B8AA0h ;
//   mov eax,esi ; pop esi ; ret`.
// Name (NF447, two sources): (1) the SDK SalSw3DVoice.obj CreateRenderer constructs CSalRenderPanDelayMMX for C_HRTF_DELAY in its MMX branch,
//   where retail 0x00C55270 builds the 0x214-byte object with vtable 0x012B8AA0, and the SDK object defines this COMDAT; (2) the vtable's own
//   slots (ClearFilterHistory = the SDK size, Process over the MMX kernels; NF441 CLASS_TABLE). Was the shared stand-in ??0C3DAnimationUtil@@QAE@XZ
//   (a propagated label, also on 0x00C55090 .. 0x00C55110).
// Flags: /EHsc (the unit's).
#include "../src/compiled/00/c5/CAudioSource_CreateProcessor_00c55270.cpp"
