// CSalSw3DVoice::CreateRenderer(CHRTFMODE mode) -- create the voice's 3D renderer for the HRTF mode: detect the CPU once, then build the SSE2
//   or the MMX/C variant of the full-HRTF renderer (C_HRTF_FULL 0x100000), the HRTF-delay panner (C_HRTF_DELAY 0x200000), the
//   volume panner (C_HRTF_VOLPAN 0x400000) or the disabled (plain stereo) renderer (anything else); null when the allocation fails
// Leaf: ?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55270, 587 bytes, thiscall `ret 4` (`this` unused), EH-framed (states 0..7: one per `new`, the class
//   operator delete runs if a constructor throws; unwind funclets 0x01216180..0x012161CD are produced by this TU).
// Name (two sources): (1) GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj: the private ?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@
//   W4CHRTFMODE@@@Z is one of the two relocations of ?Initialize@CSalSw3DVoice (retail 0x00C55680 calls 0x00C55270 there); (2) the body:
//   CHRTFMODE values 0x100000 / 0x200000 / 0x400000 (engine/CodaProcessingTypes.h) select the SDK CSalRenderHrtf / PanDelay / Pan /
//   Disable families, each in an SSE2 and an MMX variant (CSalCpuInfo SSE2 flag 0x0143BC7D). Was the stand-in
//   ?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z (NF444 / NF445 closure).
// Callees keep the spellings the table binds today (out of this batch): DetectCpu 0x00C52E40 (?Initialize@CSalCpuInfo) behind
//   g_cpuDetected 0x0143BC78 / g_cpuHasSSE2 0x0143BC7D; CodaMalloc 0x00C3C360 (the CCodaObjectBase operator new); the constructors
//   CVtable_012bede0 0x00C70B00 (CSalRenderHrtfSSE2), CVtable_012bedcc 0x00C70120 (CSalRenderHrtfMMX) (local models over the shared
//   engine/CSalRender.h base, no derived header yet; their overrides carry the SDK CSalRender signatures); CSalRenderPanSSE2 0x00C55090 /
//   CSalRenderPanMMX 0x00C550F0 (engine/CSalRenderPanSSE2.h / MMX.h) and CSalRenderDisableSSE2 0x00C550B0 / CSalRenderDisableMMX 0x00C55110
//   (engine/CSalRenderDisableSSE2.h / MMX.h), NF452: inline constructors that VC7.1 calls out of line here, as retail and the SDK Release
//   object do; and CSalRenderPanDelay 0x00C6CA20, base of
//   CSalRenderPanDelaySSE2 / CSalRenderPanDelayMMX (engine/CSalRenderPanDelaySSE2.h / MMX.h, NF447), whose inline constructors are expanded
//   here (base call + vptr 0x012B8A64 / 0x012B8AA0; their out-of-line copies are 0x00C55070 / 0x00C550D0).
// Sizes (the CodaMalloc arguments): 0xC6C, 0x214, 0x78, 0x38 (SSE2) and 0x66C, 0x214, 0x78, 0x38 (MMX / C).
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALRENDER_MEMBERS
#include "engine/CSalSw3DVoice.h"
#include "engine/CSalRender.h"
#include "engine/CSalRenderPanDelaySSE2.h"   // NF447: CSalRenderPanDelaySSE2 (vtable 0x012B8A64) : CSalRenderPanDelay (0x00C6CA20)
#include "engine/CSalRenderPanDelayMMX.h"    // NF447: CSalRenderPanDelayMMX (vtable 0x012B8AA0)
#include "engine/CSalRenderDisableSSE2.h"    // NF452: CSalRenderDisableSSE2 (vtable 0x012B8A8C) : CSalRenderDisable (0x00C6E760)
#include "engine/CSalRenderDisableMMX.h"     // NF452: CSalRenderDisableMMX (vtable 0x012B8AC8)
#include "engine/CSalRenderPanSSE2.h"        // NF452: CSalRenderPanSSE2 (vtable 0x012B8A78) : CSalRenderPan (0x00C6D9D0)
#include "engine/CSalRenderPanMMX.h"         // NF452: CSalRenderPanMMX (vtable 0x012B8AB4)

extern bool g_cpuDetected;                                                  // 0x0143BC78
extern bool g_cpuHasSSE2;                                                   // 0x0143BC7D
void DetectCpu();                                                           // 0x00C52E40

class CVtable_012bede0 : public CSalRender {                                // full HRTF renderer (SSE2), 0xC6C bytes
public:
    CVtable_012bede0();                                                     // 0x00C70B00
    virtual ~CVtable_012bede0();                                            // 0x00C70BB0
    virtual void ClearFilterHistory();
    virtual void Process(const short*, int*(&)[4]);
    virtual void SetGain(float, const float*, bool);
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
    unsigned char m_state[0xc6c - 0x28];
};
class CVtable_012bedcc : public CSalRender {                                // full HRTF renderer (MMX / C), 0x66C bytes
public:
    CVtable_012bedcc();                                                     // 0x00C70120
    virtual ~CVtable_012bedcc();                                            // 0x00C701D0
    virtual void ClearFilterHistory();
    virtual void Process(const short*, int*(&)[4]);
    virtual void SetGain(float, const float*, bool);
    virtual void SetPosition(float, float, float, float, const float*, bool, bool);
    unsigned char m_state[0x66c - 0x28];
};

CSalRender* CSalSw3DVoice::CreateRenderer(CHRTFMODE mode)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        switch (mode) {
        case C_HRTF_FULL:
            return new CVtable_012bede0;
        case C_HRTF_DELAY:
            return new CSalRenderPanDelaySSE2;
        case C_HRTF_VOLPAN:
            return new CSalRenderPanSSE2;
        default:
            return new CSalRenderDisableSSE2;
        }
    } else {
        switch (mode) {
        case C_HRTF_FULL:
            return new CVtable_012bedcc;
        case C_HRTF_DELAY:
            return new CSalRenderPanDelayMMX;
        case C_HRTF_VOLPAN:
            return new CSalRenderPanMMX;
        default:
            return new CSalRenderDisableMMX;
        }
    }
}
