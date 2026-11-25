// CGameCameraManager::TellEngineThatCameraHasMoved(bool fastMode) -- build the message "telling engine that the camera has moved and" + (" " | " not ") + "using fast mode", then tell the engine's renderer (global 0x013B86A0 -> +0x28 -> +0x2c, virtual slot 56) with the flag and 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697860, length 96 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the literal @ 0x0125C2C0, -1), operator+= @ 0x0099F100 twice (" " @ 0x01258C1C or " not "
// @ 0x0125C2B8, then "using fast mode" @ 0x0125C2A8), virtual slot 56 (+0xe0) of the renderer, ~CCharString @ 0x0099EAE0. Shape: a one-word frame for the
// string, the flag kept in ebx, the ternary literal selected with `mov eax,A ; jne ; mov eax,B`, `ret 4`.
// Literals (NF418, 2026-11-25): were extern data g_text_0125c2c0 / g_text_01258c1c / g_text_0125c2b8 / g_text_0125c2a8 (table
//   ?g_text_<va>@@3QBDB); now spelled at the use sites: retail's pooled literals at those VAs (/GF-merged ??_C@).

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    const CCharString& operator+=(const char* text);     // 0x0099F100
    ~CCharString();                                // 0x0099EAE0
};

// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its display engine (PDisplayEngine +0x28, FableWin.pdb ?GetDisplayEngine@CMainGameComponent@@QAEAAVCDisplayEngine@@XZ) and that engine's Engine3D +0x2C (FableWin.pdb ?Get3DEngine@CDisplayEngine@@QAEAAVCIEngine@@XZ), slot 56 CIEngine::CutscenePreload(bool, bool) (engine/CIEngine.h); was the stand-in CEngine_00697860* g_pEngine_013b86a0 with its CEngineDevice_00697860 / CRenderer_00697860::SetCameraMoved stand-ins.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"

class CGameCameraManager { public:
    void TellEngineThatCameraHasMoved(bool fastMode);
};

void CGameCameraManager::TellEngineThatCameraHasMoved(bool fastMode)
{
    CCharString message("telling engine that the camera has moved and", -1);
    message += fastMode ? " " : " not ";
    message += "using fast mode";
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(fastMode, true);
}