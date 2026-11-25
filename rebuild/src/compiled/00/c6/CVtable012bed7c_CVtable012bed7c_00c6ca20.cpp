// CSalRenderPanDelay::CSalRenderPanDelay() -- constructor of the GameCODA HRTF-delay panner: the CSalRender base (0x00C6E6A0), own vtable
//   0x012BED7C, m_pAudio = the 16-byte aligned sample block inside m_audioBuffer (after 32 samples of history), both ParamsPanDelay sets
//   cleared, m_pParamsCurrent / m_pParamsNew pointing at them, and the six per-channel pan gains and delays zeroed
// Leaf: ??0CSalRenderPanDelay@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6CA20, 150 bytes (+ int3 padding).
// Name (NF441 NAME_FIXES, two sources): (1) GameCODA 2.10 SDK CODAApi.lib SalSw3DVoice.obj ?CreateRenderer@CSalSw3DVoice relocates
//   ??0CSalRenderPanDelay@@QAE@XZ twice (SSE2 and MMX branches), at the two places where retail 0x00C55270 calls 0x00C6CA20; (2) the vtable
//   0x012BED7C it stores: SetPosition 0x00C6D150 is 150 instructions like the SDK ?SetPosition@CSalRenderPanDelay, ClearFilterHistory
//   0x00C6CAD0 is 23 bytes like the SDK's (NF441 CLASS_TABLE). Members: engine/CSalRenderPanDelay.h (SDK CodeView names, retail offsets).
//   Was ??0CVtable_012bed7c@@QAE@XZ (self) and ??0CEnvironmentSpellEffectInterface@@QAE@XZ (a propagated Fable PDB label on the table).
// Shape: after the vtable store, in member order: m_pAudio `(m_audioBuffer + 32 + 15) & ~15` bytes (`lea eax,[esi+77h] ; and eax,0FFFFFFF0h`);
//   the two parameter sets cleared with one intrinsic memset each (`rep stosd`, one shared zero register); the two set pointers; then a
//   body loop clearing the two six-entry arrays, which VC7.1 unrolls into interleaved stores (+0x1e4/+0x1fc, +0x1e8/+0x200, ...).
// Flags: /Ob1 (R-NF444-1, the render TUs).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalRenderPanDelay.h"   // CSalRenderPanDelay (vtable 0x012BED7C, 0x214 bytes) : CSalRender (constructor 0x00C6E6A0)
#include <string.h>

CSalRenderPanDelay::CSalRenderPanDelay()
{
    m_pAudio = reinterpret_cast<short*>((reinterpret_cast<unsigned long>(m_audioBuffer + 32) + 15) & ~15UL);
    memset(&m_paramsA, 0, sizeof(m_paramsA));
    memset(&m_paramsB, 0, sizeof(m_paramsB));
    m_pParamsCurrent = &m_paramsA;
    m_pParamsNew = &m_paramsB;
    for (int i = 0; i < 6; ++i) {
        m_fGainPan[i] = 0.0f;
        m_itd[i] = 0;
    }
}
