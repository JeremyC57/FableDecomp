// CSalRenderPan::Params3DInit(FilterPan* pFilter, float fGainPan) -- fill one FilterPan from a pan gain and the renderer gain: Q15 gain =
//   min(FtoL(fGainPan * m_fGain * 32767), 0x7fff) and its 128-sample ramp step gain >> 7; then (same retail object, address order) UpdateFilters
// Leaf: ?Params3DInit@CSalRenderPan@@AAEXPAUFilterPan@@M@Z
// Option S owner (retail SalRenderPan.obj slice): this file holds Params3DInit 0x00C6DE60 and UpdateFilters 0x00C6DEA0 in retail address
//   order; 0x00C6DEA0 is a thin row that includes it. VC7.1 expands Params3DInit at the first two sites of each UpdateFilters branch and calls
//   it at the rear two (behind the output-channel test), as retail (four calls of 0x00C6DE60 from 0x00C6DEA0).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6DE60, 59 bytes (+ int3 padding), thiscall `ret 8`; UpdateFilters @ 0x00C6DEA0, 369 bytes, `ret 4`.
// Retail-asm: 0x00C6DE6E `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` -- libpk's inline-assembly rounding helper. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist (RTM 3077 and QFE 4035, with /Op, /Og or /G5-/G7) emits `fistp qword` into an 8-byte slot, /arch:SSE2 a cvttss2si: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Retail-asm: 0x00C6DED1 `fld dword ptr [esp+10h]` + 0x00C6DED5 `fistp dword ptr [esp+8]` (and 0x00C6DF0A / 0x00C6DF0E, 0x00C6DF74 / 0x00C6DF78, 0x00C6DFAD / 0x00C6DFB1) -- libpk's inline-assembly float-to-long helper (the landed ComputeGainRamp 0x00C6DE60 runs the same `fld ; fistp dword` block): the product homed in the dead argument slot and reloaded, a bare dword fistp. VC7.1 C++ `(long)x` calls __ftol2 and /QIfist emits `fistp qword` into an 8-byte slot: no C/C++ form gives the bare dword fistp, so it is the original's inline asm.
// Helper (2026-11-05 asm migration): the bare `fld` / `fistp dword` block is libpk's FtoL in the shared libpk/FtoL.h (byte-matching
//   path + FABLE_PORTABLE twin); the row has no asm of its own. C++/intrinsic attempts: asm/w3 ATTEMPTS.tsv family FISTP.
// Name (NF452, two sources): (1) the GameCODA 2.10 SDK (CODAApiDbg.lib Debug/SalRenderPan.obj CodeView class / method record + CODAApi.lib Release/SalRenderPan.obj body): private void Params3DInit(FilterPan*, float) / void UpdateFilters(const bool); (2) the Release
//   bodies tie exactly: Params3DInit 17 instructions / `ret 8` = retail 17 / `ret 8`; UpdateFilters 111 / `ret 4` = retail 111 / `ret 4`;
//   the Release SetGain / SetPosition relocate their one call to UpdateFilters, as retail 0x00C6E020 / 0x00C6E050 call 0x00C6DEA0. Were
//   ?ComputeGainRamp@CVtable_012bed90@@QAEXPAUSGainRamp@@M@Z / ?RebuildGain@CVtable_012bed90@@QAEX_N@Z (descriptive).
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) through CSalMutex::Enter / Leave on m_mutexUpdate
//   (+0x04; the critical section +0x08); 32767.0f 0x0129C820; g_pAudioMaster 0x0143BC2C (the landed CAudioMaster view of the SDK
//   g_pSal / CSalPlatform::m_nSWOutputChannels at +0x1BC, kept as in the HEAD row: follow-up port onto engine/CSalPlatform.h).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPan.h"   // CSalRenderPan (vtable 0x012BED90) : CSalRender (m_mutexUpdate +0x04, m_fGain +0x20, m_bUpdate +0x24)
#include "libpk/FtoL.h"
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };   // the landed Process rows' view of g_pSal (+0x1BC)
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C

void CSalRenderPan::Params3DInit(FilterPan* pFilter, float fGainPan)
{
    unsigned long gain = FtoL(fGainPan * m_fGain * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    pFilter->gain = (unsigned short)gain;
    pFilter->gainDelta = (short)((unsigned short)gain >> 7);
}

void CSalRenderPan::UpdateFilters(const bool bNew)
{
    m_mutexUpdate.Enter();
    m_bUpdate = bNew;
    if (bNew) {
        Params3DInit(&m_pParamsNew->pan[0], m_fGainPan[0]);
        Params3DInit(&m_pParamsNew->pan[1], m_fGainPan[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            Params3DInit(&m_pParamsNew->pan[2], m_fGainPan[2]);
            Params3DInit(&m_pParamsNew->pan[3], m_fGainPan[3]);
        }
    } else {
        Params3DInit(&m_pParamsCurrent->pan[0], m_fGainPan[0]);
        Params3DInit(&m_pParamsCurrent->pan[1], m_fGainPan[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            Params3DInit(&m_pParamsCurrent->pan[2], m_fGainPan[2]);
            Params3DInit(&m_pParamsCurrent->pan[3], m_fGainPan[3]);
        }
    }
    m_mutexUpdate.Leave();
}
