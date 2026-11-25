// CSalSw3DVoice::Initialize(CHRTFMODE mode) -- create the renderer for the HRTF mode (false when that fails), initialize it and reset the
//   voice's 3D state: azimuth / elevation 0, both macro-FX gains 1.0, the environment gain pair 0 and its update flag clear
// Leaf: ?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C55680, 96 bytes, thiscall `ret 4`.
// Name (two sources): (1) GameCODA 2.10 SDK CODAApi.lib Release/SalSw3DVoice.obj defines the private
//   ?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z, 96 bytes (= retail), whose two relocations ?CreateRenderer@CSalSw3DVoice and
//   ?Initialize@CSalRender are retail's two calls (0x00C55270, 0x00C6E660); (2) its only caller is CSalSw3DVoice::Create 0x00C556E0
//   (retail vftable 0x012B8AE0 slot 1, SDK relocation). Was the stand-in ?Open@CAudioSource@@QAE_NPAX@Z (NF444 / NF445 closure).
// Members: engine/CSalSw3DVoice.h (PASS23 retail layout): m_fAzimuth +0x210, m_fElevation +0x214, m_fMacroFXGain +0x218, m_pRender
//   +0x220, m_gainEnvironment +0x23C, m_gainEnvironmentNew +0x23E, m_bUpdateEnvironmentGain +0x240.
#define FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS
#define FABLE_RETAIL_PASS22_CCODA3D_CREATE
#define FABLE_RETAIL_CSALPLATFORM_CREATE_SW_PASS23
#define FABLE_RETAIL_CSALSW3D_CREATE
#include "engine/CSalSw3DVoice.h"
#include "engine/CSalRender.h"

bool CSalSw3DVoice::Initialize(CHRTFMODE mode)
{
    m_pRender = CreateRenderer(mode);
    if (!m_pRender)
        return false;
    m_pRender->Initialize();
    m_fAzimuth = 0.0f;
    m_fElevation = 0.0f;
    m_fMacroFXGain[0] = 1.0f;
    m_fMacroFXGain[1] = 1.0f;
    m_bUpdateEnvironmentGain = false;
    m_gainEnvironment = 0;
    m_gainEnvironmentNew = 0;
    return true;
}
