// CSalRenderPanDelay::Params3DInit(FilterPanDelay* pFilter, float fGainPan, unsigned itd) -- fill one channel's delay filter: the delay, the
//   Q15 gain FtoL(fGainPan * m_fGain * 32767) capped (unsigned) at 0x7fff, and its 128-sample ramp step gain >> 7
// Leaf: ?Params3DInit@CSalRenderPanDelay@@AAEXPAUFilterPanDelay@@MI@Z
// Option S: owner of unit 0x00C6CEF0 / 0x00C6CF40 (retail SalRenderPanDelay.obj slice): this file holds both bodies in retail address order;
//   0x00C6CF40 (UpdateFilters) is a thin row that includes it. In one translation unit VC7.1 expands Params3DInit at UpdateFilters' first two
//   sites per branch and calls it out of line at the rear two (its inline budget), exactly as retail.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CEF0, 66 bytes (+ int3 padding), thiscall `ret 0Ch`; @ 0x00C6CF40, 470 bytes, thiscall `ret 4`.
// Names (NF447, two sources): the GameCODA 2.10 SDK CODAApiDbg.lib Debug/SalRenderPanDelay.obj CodeView declares the private thiscall members
//   void CSalRenderPanDelay::Params3DInit(FilterPanDelay*, float, unsigned) and void CSalRenderPanDelay::UpdateFilters(const bool) (LF_ONEMETHOD
//   records 0x1208 / 0x120A, the class's only non-virtual methods besides the static mixers); the SDK Release CODAApi.lib SalRenderPanDelay.obj
//   calls UpdateFilters from SetGain (+0x1D) and SetPosition (+0x1F2) and Params3DInit from UpdateFilters, the retail call graph exactly
//   (0x00C6D13C / 0x00C6D341 -> 0x00C6CF40; 0x00C6D018 / 0x00C6D037 / 0x00C6D0E7 / 0x00C6D106 -> 0x00C6CEF0, no other callers). Bodies: SDK
//   Release 19 / 126 instructions = retail 19 / 126, argument bytes 12 / 4 = retail's `ret 0Ch` / `ret 4` (evidence/tie_table.tsv). Were ?ComputeTapGain@CVtable_012bed7c and ?RebuildGain@CVtable_012bed7c@@QAEX_N@Z (descriptive stand-ins).
// Retail-asm: 0x00C6CEFE `fstp [esp+0Ch] ; fld [esp+0Ch] ; fistp dword [esp]` (and the expanded copies in UpdateFilters 0x00C6CF7D / 0x00C6CF81,
//   0x00C6CFC5 / 0x00C6CFC9, 0x00C6D04F / 0x00C6D053, 0x00C6D097 / 0x00C6D09B) -- libpk's inline-assembly rounding helper (bare dword fistp; VC7.1
//   C++ `(long)x` calls __ftol2, /QIfist gives `fistp qword`), reached only through the shared libpk/FtoL.h; no asm in this row.
// Callees (UpdateFilters): CSalMutex::Enter / Leave (EnterCriticalSection / LeaveCriticalSection, IAT 0x0143FE0C / 0x0143FE10) on m_mutexUpdate
//   (+0x04); g_pAudioMaster 0x0143BC2C (+0x1BC: four or more output channels also fill filters 2 and 3).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#define FABLE_RETAIL_CSALMUTEX_ENTER_LEAVE
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C) : CSalRender (m_mutexUpdate +0x04, m_fGain +0x20, m_bUpdate +0x24)
#include "libpk/FtoL.h"
struct CAudioMaster { unsigned char _pad[0x1bc]; unsigned long m_outputChannels; };   // the landed Process 0x00C6CC60 view (+0x1BC)
extern CAudioMaster* g_pAudioMaster;                                        // 0x0143BC2C

void CSalRenderPanDelay::Params3DInit(FilterPanDelay* pFilter, float fGainPan, unsigned itd)
{
    unsigned long gain = FtoL(fGainPan * m_fGain * 32767.0f);
    if (gain > 0x7fff)
        gain = 0x7fff;
    pFilter->gain = (unsigned short)gain;
    pFilter->itd = itd;
    pFilter->gainDelta = (short)((unsigned short)gain >> 7);
}

// CSalRenderPanDelay::UpdateFilters(const bool bNew) -- under m_mutexUpdate record whether a new parameter set is pending (m_bUpdate) and
//   rebuild that set (m_pParamsNew when bNew, else the live m_pParamsCurrent) from m_fGainPan / m_itd: filters 0 and 1 always, 2 and 3 only
//   with four or more output channels
void CSalRenderPanDelay::UpdateFilters(const bool bNew)
{
    m_mutexUpdate.Enter();
    m_bUpdate = bNew;
    if (bNew) {
        Params3DInit(&m_pParamsNew->pan[0], m_fGainPan[0], m_itd[0]);
        Params3DInit(&m_pParamsNew->pan[1], m_fGainPan[1], m_itd[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            Params3DInit(&m_pParamsNew->pan[2], m_fGainPan[2], m_itd[2]);
            Params3DInit(&m_pParamsNew->pan[3], m_fGainPan[3], m_itd[3]);
        }
    } else {
        Params3DInit(&m_pParamsCurrent->pan[0], m_fGainPan[0], m_itd[0]);
        Params3DInit(&m_pParamsCurrent->pan[1], m_fGainPan[1], m_itd[1]);
        if (g_pAudioMaster->m_outputChannels >= 4) {
            Params3DInit(&m_pParamsCurrent->pan[2], m_fGainPan[2], m_itd[2]);
            Params3DInit(&m_pParamsCurrent->pan[3], m_fGainPan[3], m_itd[3]);
        }
    }
    m_mutexUpdate.Leave();
}
