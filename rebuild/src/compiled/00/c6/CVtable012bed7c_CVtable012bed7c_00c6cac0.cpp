// CSalRenderPanDelay::~CSalRenderPanDelay() -- empty virtual destructor of the GameCODA HRTF-delay panner: own vtable 0x012BED7C, then the
//   CSalRender destructor (tail jump)
// Leaf: ??1CSalRenderPanDelay@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CAC0, 11 bytes: `mov dword ptr [ecx],offset vtable 0x012BED7C ; jmp 0x00C6E630`.
// Name (NF447, two sources): the class of the vtable it stores, CSalRenderPanDelay (constructor 0x00C6CA20: GameCODA 2.10 SDK CreateRenderer
//   relocations + the SetPosition / ClearFilterHistory body ties, NF441), and the SDK CodeView's virtual ~CSalRenderPanDelay; retail's SIMD
//   destructors 0x00C554E0 / 0x00C55570 and the deleting destructor 0x00C6D350 reach it. Was ??1CVtable_012bed7c@@UAE@XZ.
// Shape: an empty user destructor of a derived polymorphic class stores its own vtable and tail-jumps to the out-of-line base destructor
//   (0x00C6E630 ~CSalRender).
// Flags: /Ob1 (R-NF444-1, the render TUs).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C) : CSalRender (destructor 0x00C6E630)

CSalRenderPanDelay::~CSalRenderPanDelay()
{
}
