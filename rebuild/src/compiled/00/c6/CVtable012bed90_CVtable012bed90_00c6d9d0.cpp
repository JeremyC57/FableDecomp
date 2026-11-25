// CSalRenderPan::CSalRenderPan() -- constructor of the GameCODA volume panner: the CSalRender base (0x00C6E6A0), own vtable 0x012BED90, both
//   six-entry parameter sets cleared with the current / new pointers on A / B, and the six pan gains zeroed
// Leaf: ??0CSalRenderPan@@QAE@XZ
// Flags: /Ob1 (R-NF444-1, the render TUs)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6D9D0, 94 bytes (+ int3 padding). Callers: 0x00C55093 / 0x00C550F3 (the SIMD constructors).
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderPan.obj CodeView class / method record + CODAApi.lib Release/SalRenderPan.obj body): public CSalRenderPan() of LF_CLASS CSalRenderPan : public CSalRender; (2) the Release body has
//   retail's exact shape at 4 entries (base call, vptr, A cleared through edx, B through edi, +0x48 / +0x4C pointers, gains zeroed through eax:
//   30 instructions; retail 36 = the same with 6 entries), and the SDK ??0CSalRenderPanSSE2 / MMX COMDATs call ??0CSalRenderPan where retail
//   0x00C55090 / 0x00C550F0 call 0x00C6D9D0. Was ??0CVtable_012bed90@@QAE@XZ (descriptive, vtable-named).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPan.h"   // CSalRenderPan (vtable 0x012BED90, 0x78 bytes) : CSalRender (constructor 0x00C6E6A0)
#include <string.h>

CSalRenderPan::CSalRenderPan()
{
    memset(&m_paramsA, 0, sizeof(m_paramsA));
    memset(&m_paramsB, 0, sizeof(m_paramsB));
    m_pParamsCurrent = &m_paramsA;
    m_pParamsNew = &m_paramsB;
    for (int i = 0; i < 6; ++i)
        m_fGainPan[i] = 0.0f;
}
