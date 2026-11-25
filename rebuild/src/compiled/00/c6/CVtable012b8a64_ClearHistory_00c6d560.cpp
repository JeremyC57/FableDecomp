// CSalRenderPanDelaySSE2::ClearFilterHistory() -- vtable slot 1 of the GameCODA SSE2 HRTF-delay panner: zero the 32 samples of delay history below the aligned sample block (m_pAudio - 32) with four aligned 16-byte SSE stores
// Leaf: ?ClearFilterHistory@CSalRenderPanDelaySSE2@@UAEXXZ
// Retail-asm: 0x00C6D573..0x00C6D584 `xorps xmm0,xmm0 ; movdqa xmmword ptr [edi],xmm0 ; movdqa [edi+10h],xmm0 ; movdqa [edi+20h],xmm0 ; movdqa [edi+30h],xmm0` -- a hand SSE2 store block on a fixed edi loaded from a frame local (`mov [esp+4],eax ; mov edi,[esp+4]`, the asm's `mov edi,history`): VC7.1 RTM has no intrinsic that names edi or keeps the pointer in a frame slot, and the C path of the same slot (0x00C6CAD0) compiles to `rep stosd`; the libpk SIMD kernels are hand asm (GOTCHAS rasm1; landed twins 0x00C5D4B0 / 0x00C6D370). The pointer arithmetic, the local and the frame are C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6D560, 44 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012B8A64 (word at 0x012B8A68), the SSE2 derivative of the HRTF-delay panner: ctor 0x00C55070 = the base
//   ctor 0x00C6CA20 (CSalRenderPanDelay) then this vtable (CSalRenderPanDelaySSE2, was the propagated label "C3DAnimationUtil").
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelaySSE2::ClearFilterHistory() (slot 1 of CSalRender,
//   engine/CSalRender.h), and the SDK Release ?ClearFilterHistory@CSalRenderPanDelaySSE2@@UAEXXZ is 44 bytes, this body's size (NF444 notes); the class
//   is the one CSalSw3DVoice::CreateRenderer 0x00C55270 builds with this vtable (NF441 CLASS_TABLE). Was ?ClearHistory@CVtable_012b8a64@@UAEXXZ.
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelaySSE2.h"   // CSalRenderPanDelaySSE2 : CSalRenderPanDelay (m_pAudio +0x178)

void CSalRenderPanDelaySSE2::ClearFilterHistory()
{
    short* history = m_pAudio - 32;
    LIBPK_SSE_ZERO64(history);
}