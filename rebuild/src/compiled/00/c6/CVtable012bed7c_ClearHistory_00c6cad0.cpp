// CSalRenderPanDelay::ClearFilterHistory() -- vtable slot 1 of the GameCODA HRTF-delay panner: zero the 32 samples of delay history just
//   below the aligned sample block (m_pAudio - 32), the plain C path
// Leaf: ?ClearFilterHistory@CSalRenderPanDelay@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6CAD0, 23 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012BED7C (word at 0x012BED80). The MMX class 0x012B8AA0 overrides it with 0x00C6F300 (`pxor/movq`) and
//   the SSE2 class 0x012B8A64 with 0x00C6D560 (`xorps/movdqa`), the same 64 bytes.
// Name (NF447, two sources): the GameCODA 2.10 SDK CODAApiDbg.lib CodeView declares the virtual CSalRenderPanDelay::ClearFilterHistory()
//   (slot 1 of CSalRender, engine/CSalRender.h), and the SDK Release SalRenderPanDelay.obj ?ClearFilterHistory@CSalRenderPanDelay@@UAEXXZ
//   is 23 bytes, this body's size (9 instructions = 9, NF441 CLASS_TABLE / NF444 notes); the class is the one its constructor 0x00C6CA20
//   names. Was ?ClearHistory@CVtable_012bed7c@@UAEXXZ (descriptive stand-in).
// Shape: the intrinsic memset of 0x40 bytes is `mov ecx,10h ; xor eax,eax ; rep stosd` (`this` moved to edx for edi).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C, m_pAudio +0x178)
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

void CSalRenderPanDelay::ClearFilterHistory()
{
    memset(m_pAudio - 32, 0, 32 * sizeof(short));
}
