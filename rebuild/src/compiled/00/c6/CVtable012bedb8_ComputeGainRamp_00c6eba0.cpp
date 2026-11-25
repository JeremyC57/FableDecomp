// CSalRenderDisable::Params3DInit(FilterPan* pFilter) -- fill a FilterPan from the renderer gain alone: gain = min(FtoL(m_fGain * 32767),
//   0x7fff) >> 1 (unsigned) and its 128-sample ramp step gain >> 7; then (same retail object, address order) UpdateFilters
// Leaf: ?Params3DInit@CSalRenderDisable@@AAEXPAUFilterPan@@@Z
// Option S owner (retail SalRenderDisable.obj slice): this file holds Params3DInit 0x00C6EBA0 and UpdateFilters 0x00C6EBE0 in retail address
//   order; 0x00C6EBE0 is a thin row that includes it. VC7.1 expands Params3DInit into both arms of UpdateFilters and keeps this out-of-line
//   copy (no retail caller), as the SDK Release object does (UpdateFilters relocates no call to Params3DInit).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6EBA0, 62 bytes (+ int3 padding), thiscall `ret 4`; UpdateFilters @ 0x00C6EBE0, 106 bytes, `ret 4`.
// Retail-asm: 0x00C6EBAC `fstp dword ptr [esp] ; fld dword ptr [esp] ; fistp dword ptr [esp+4]` (Params3DInit) and 0x00C6EC0A / 0x00C6EC0E,
//   0x00C6EC17 / 0x00C6EC1B `fld dword ptr [esp+10h] ; fistp dword ptr [esp+8]` (its two expansions in UpdateFilters) -- libpk's inline-assembly
//   float-to-long helper FtoL (the SDK's sal_round; shared libpk/FtoL.h: byte path + FABLE_PORTABLE twin, docs/pipeline/ASM_RULES.md); this row
//   has no asm of its own. VC7.1 `(long)x` calls __ftol2 and /QIfist emits `fistp qword`.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderDisable.obj CodeView class / method record + CODAApi.lib Release/SalRenderDisable.obj body): private void Params3DInit(FilterPan*) / void UpdateFilters(const bool); (2) the Release bodies tie
//   exactly: Params3DInit 17 instructions / `ret 4` = retail 17 / `ret 4`; UpdateFilters 35 / `ret 4` = retail 35 / `ret 4`; and the Release
//   SetGain / SetPosition relocate their one call to UpdateFilters, as retail 0x00C6EC50 / 0x00C6EC80 call 0x00C6EBE0. Were
//   ?ComputeGainRamp@CVtable_012bedb8@@QAEXPAUSGainRamp@@@Z / ?RebuildGain@CVtable_012bedb8@@QAEX_N@Z (descriptive).
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate
//   (+0x04; the critical section +0x08); 32767.0f 0x0129C820.
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderDisable.h"   // CSalRenderDisable (vtable 0x012BEDB8) : CSalRender (m_mutexUpdate +0x04, m_fGain +0x20, m_bUpdate +0x24)
#include "libpk/FtoL.h"

void CSalRenderDisable::Params3DInit(FilterPan* pFilter)
{
    unsigned long gain = FtoL(m_fGain * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    unsigned short half = (unsigned short)((unsigned short)gain >> 1);
    pFilter->gain = half;
    pFilter->gainDelta = (short)(half >> 7);
}

void CSalRenderDisable::UpdateFilters(const bool bNew)
{
    m_mutexUpdate.Enter();
    m_bUpdate = bNew;
    if (bNew)
        Params3DInit(&m_pParamsNew->pan);
    else
        Params3DInit(&m_pParamsCurrent->pan);
    m_mutexUpdate.Leave();
}
