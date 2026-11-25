// CSalRenderPan::SetGain(float fGain, const float* pfGainPan, bool bNew) -- vtable slot 3 of the GameCODA volume panner (and of its SSE2 /
//   MMX classes): a changed gain is stored in m_fGain and rebuilds the filters (UpdateFilters 0x00C6DEA0); the pan gains are unused
// Leaf: ?SetGain@CSalRenderPan@@UAEXMPBM_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6E020, 36 bytes (+ int3 padding), thiscall `ret 0Ch`; slot 3 of 0x012BED90 / 0x012B8A78 / 0x012B8AB4.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderPan.obj CodeView class / method record + CODAApi.lib Release/SalRenderPan.obj body): virtual void SetGain(float, const float*, bool) (slot 0x0C of CSalRender); (2) the Release body ties
//   exactly (12 instructions / `ret 0Ch` = retail 12 / `ret 0Ch`; one call, to UpdateFilters). Was ?SetVolume@CVtable_012bed90@@UAEXMK_N@Z
//   (descriptive stand-in; NF445 DEBT_SLOTS).
// Shape: `m_fGain != fGain` loads the argument first for `fucompp ; test ah,44h ; jnp` (equal -> return); the float store is an integer move.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPan.h"   // CSalRenderPan (vtable 0x012BED90) : CSalRender (m_fGain +0x20)

void CSalRenderPan::SetGain(float fGain, const float* pfGainPan, bool bNew)
{
    if (m_fGain != fGain) {
        m_fGain = fGain;
        UpdateFilters(bNew);
    }
}
