// CSalRenderDisable::SetPosition(float, float, float, float fGain, const float*, bool bNew, bool bForce) -- vtable slot 4 of the GameCODA
//   plain-gain renderer (and of its SSE2 / MMX classes): the position is ignored; unless forced, an unchanged gain does nothing, else the gain
//   is stored in m_fGain and the filters are rebuilt (UpdateFilters 0x00C6EBE0)
// Leaf: ?SetPosition@CSalRenderDisable@@UAEXMMMMPBM_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EC80, 44 bytes (+ int3 padding), thiscall `ret 1Ch`; slot 4 of 0x012BEDB8 / 0x012B8A8C / 0x012B8AC8.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView class / method record + CODAApi.lib Release/SalRenderDisable.obj body): virtual void SetPosition(float, float, float, float, const float*, bool, bool) (slot 0x10 of
//   CSalRender); (2) the Release body ties exactly (15 instructions / `ret 1Ch` = retail 15 / `ret 1Ch`; one call, to UpdateFilters).
//   Was ?SetParameters@CVtable_012bedb8@@UAEXJJJMJ_N0@Z (descriptive stand-in; NF445 DEBT_SLOTS).
// Shape: `m_fGain != fGain` loads the argument first for `fucompp ; test ah,44h ; jnp`; the call shares the `ret 1Ch` epilogue.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderDisable.h"   // CSalRenderDisable (vtable 0x012BEDB8) : CSalRender (m_fGain +0x20)

void CSalRenderDisable::SetPosition(float fAzimuth, float fElevation, float fDistance, float fGain, const float* pfGainPan, bool bNew, bool bForce)
{
    if (bForce || m_fGain != fGain) {
        m_fGain = fGain;
        UpdateFilters(bNew);
    }
}
