// CSalSw3DVoice::Create(const CCodaSource* pSource) -- create the 3D software voice: the base voice's create, then the voice's own
//   128-frame mix buffer (m_FrameBuffer +0x1F0) over the shared scratch block 0x0143BCB0 becomes the mix buffer (m_pMixBuffer +0xD8), then
//   Initialize with the 3D source's HRTF mode; 0, -5 when Initialize fails, or the base's negative result
// Leaf: ?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C556E0, 99 bytes, thiscall `ret 4`.
// Name (two sources): (1) GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj defines ?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z,
//   99 bytes (= retail), relocations CSalSwBaseVoice::Create, l_frameBuffer, CSalSwBuffer::Create, ?Initialize@CSalSw3DVoice (retail's
//   0x00C5A1D0, 0x0143BCB0, 0x00C5CE00, 0x00C55680); (2) retail vftable 0x012B8AE0 (CSalSw3DVoice, constructor 0x00C555E0) slot 1.
//   Was the stand-in ?Setup@CAudioSource@@UAEJPAUCVoiceDesc_00c556e0@@@Z (NF444 / NF445 closure).
// Callees keep the spellings the table binds today (out of this batch; respelling them is separate naming debt): the base create
//   0x00C5A1D0 as ?Setup@CSoftwareVoice_00c5a1d0@@UAEJPAUCVoiceDesc_00c556e0@@@Z, CSalSwBuffer::Create 0x00C5CE00 as
//   ?Init@CAudioRingBuffer@@QAEXPAEKKKKK@Z and l_frameBuffer 0x0143BCB0 as ?g_scratchRing_0143bcb0@@3PAEA. Their classes are
//   declaration-only shells (no layout); every member access goes through the shared headers.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#define FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE
#include "engine/CSalSw3DVoice.h"
#include "engine/CCoda3DSource.h"

struct CVoiceDesc_00c556e0;                             // spelling of the base create's parameter (the CCodaSource)
struct CSoftwareVoice_00c5a1d0 {                        // spelling of CSalSwBaseVoice::Create 0x00C5A1D0
    virtual long Setup(CVoiceDesc_00c556e0* desc);
};
struct CAudioRingBuffer {                               // spelling of CSalSwBuffer::Create 0x00C5CE00
    void Init(unsigned char* data, unsigned long capacity, unsigned long frameBytes, unsigned long channels,
              unsigned long readPos, unsigned long queued);
};
extern unsigned char g_scratchRing_0143bcb0[];          // l_frameBuffer 0x0143BCB0

int CSalSw3DVoice::Create(const CCodaSource* pSource)
{
    int hr = reinterpret_cast<CSoftwareVoice_00c5a1d0*>(this)->CSoftwareVoice_00c5a1d0::Setup(
        reinterpret_cast<CVoiceDesc_00c556e0*>(const_cast<CCodaSource*>(pSource)));
    if (hr < 0)
        return hr;
    reinterpret_cast<CAudioRingBuffer*>(&m_FrameBuffer)->Init(g_scratchRing_0143bcb0, 0x80, 2, 1, 0, 0);
    m_pMixBuffer = &m_FrameBuffer;
    return Initialize(pSource->Get3D()->GetHrtfMode()) ? 0 : -5;
}
