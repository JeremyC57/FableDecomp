// CSalRenderPanDelayMMX::ClearFilterHistory() -- vtable slot 1 of the GameCODA MMX HRTF-delay panner: zero the 32 samples of delay history below the aligned sample block (m_pAudio - 32) with eight MMX quadword stores, then emms
// Leaf: ?ClearFilterHistory@CSalRenderPanDelayMMX@@UAEXXZ
// Retail-asm: 0x00C6F313..0x00C6F335 `pxor mm0,mm0 ; movq qword ptr [edi],mm0 .. movq [edi+38h],mm0 ; emms` -- a hand MMX store block on a fixed edi loaded from a frame local (`mov [esp+4],eax ; mov edi,[esp+4]`): the VC7.1 RTM include set has no mmintrin.h (GOTCHAS 0xA44250), and the C path of the same slot (0x00C6CAD0) compiles to `rep stosd`; the same shape as the landed MMX copy 0x00C70040. The pointer arithmetic, the local and the frame are C++.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared libpk/MixKernelsSIMD.h (byte-matching path + FABLE_PORTABLE twin); the row has
//   no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family SIMD_LIBPK.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6F300, 58 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012B8AA0 (word at 0x012B8AA4), the MMX derivative of the HRTF-delay panner: ctor 0x00C550D0 = the base
//   ctor 0x00C6CA20 (CSalRenderPanDelay) then this vtable (CSalRenderPanDelayMMX, was the propagated label "C3DAnimationUtil").
// Name (NF447, two sources): the GameCODA 2.10 SDK CodeView declares the virtual CSalRenderPanDelayMMX::ClearFilterHistory() (slot 1 of CSalRender,
//   engine/CSalRender.h), and the SDK Release ?ClearFilterHistory@CSalRenderPanDelayMMX@@UAEXXZ is 58 bytes, this body's size (NF444 notes); the class
//   is the one CSalSw3DVoice::CreateRenderer 0x00C55270 builds with this vtable (NF441 CLASS_TABLE). Was ?ClearHistory@CVtable_012b8aa0@@UAEXXZ.
#include "libpk/MixKernelsSIMD.h"
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelayMMX.h"   // CSalRenderPanDelayMMX : CSalRenderPanDelay (m_pAudio +0x178)

void CSalRenderPanDelayMMX::ClearFilterHistory()
{
    short* history = m_pAudio - 32;
    LIBPK_MMX_ZERO64(history);
}