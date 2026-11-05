// CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& interpolation_info) -- render one frontend frame: switch the FPU to round-to-nearest, set the virtual window, clear, draw the frontend screens, the error message, the engine, the 2D draw lists and the IME, then present
// Leaf: ?Draw@CNewFrontendGameComponent@@AAEXABVCEngineInterpolationInfo@@@Z
// Flags: /O1 /Oy-
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DF9E, 478 bytes, `ret 4`; unaligned entry right after 0x0042DF9B `leave ; ret 4` (no padding):
//   a size-optimised frame-pointer TU (fldz stores, leave) -> /O1 /Oy-, as the landed CNewFrontendGameComponent rows (optimize("s") + ("y",off)).
// Retail-asm: 0x0042DFD8 `fnstcw word ptr [ebp-4]` + 0x0042DFDB `and byte ptr [ebp-3],0F3h` + 0x0042DFDF `fldcw word ptr [ebp-4]`, through the
//   shared helper engine/GFFPUSetRoundingNearest.h (FableWin.pdb ?GFFPUSetRoundingNearest@@YIXXZ); 32 plain-C++ and 12 intrinsic spellings
//   first (ATTEMPTS_0042df9e_fpu.tsv): none emits fnstcw / fldcw. The cw slot [ebp-4] is reused for the clear colour.
// Callees (Ego_r.pdb spellings): CInterpolationInfo() 0x00415A60 (an unused local), GFGetTime 0x009E1BC0, CRenderStateManager::SetFogEnable
//   (inline, DeviceState.FogEnable +0x2944), CRenderManagerCore::SetVWindow 0x00A0B560, CDisplayManager::GetVVisibleTelevisionScreenArea
//   0x009BECE0 (result dropped), CRenderManager2D::ClearRenderTarget 0x009D8CF0, CDisplayManager::BeginRender 0x009BEF20,
//   NUISystem::CFrontEndManager::GetInstance 0x00595582 / Draw 0x00595222, NUISystem::CManager::GetInstance 0x0041E5F2 / HasErrorMessage
//   (inline, +0x9C) / RenderErrorMessage 0x0041D03C, CIEngine::Render (slot 8, +0x20), SetTextureMipMapMode(0, TEXTURE_MIPMAP_LINEAR) (inline,
//   TextureMipMapMode[0] +0x2E44, table 0x01396E64), CRenderManager2D::DrawBufferedPolys 0x009D9C80 / Render2DDrawList 0x009DA9F0 (twice),
//   NUISystem::CIME::Get 0x00404A80 / Render 0x00404C00, CDisplayManager::EndRender 0x009BEF50 / SwapScreens 0x009BEEB0.
// Globals: VIRTUAL_SCREEN_WIDTH / HEIGHT 0x013961E8 / 0x013961F0 (doubles), PRenderManager 0x013B8384, GPDisplayManager 0x013B8390.
// Layout: CNewFrontendGameComponent retail = Ego_r.pdb with STLport vectors: m_pEngine +0x58 (CCountedPointer<CIEngine>), m_fLastRenderTime +0xBC.
// Headers (chunk 774 overlay): CIEngine.h FABLE_RETAIL_CIENGINE_RENDER (slot 8 Render), CDisplayManager.h, CRenderManager2D.h, NUISystem_CManager.h,
//   CFrontEndManager.h (declarations in their retail branches), engine/GFFPUSetRoundingNearest.h (new helper).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CIENGINE_RENDER
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CFRONTENDMANAGER
#include "fable_string.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBColour.h"
#include "engine/CCountedPointer.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CFrontEndManager.h"
#include "engine/GFFPUSetRoundingNearest.h"
double __fastcall GFGetTime();                                           // 0x009E1BC0 ?GFGetTime@@YINXZ
extern double VIRTUAL_SCREEN_WIDTH;                                      // 0x013961E8
extern double VIRTUAL_SCREEN_HEIGHT;                                     // 0x013961F0
extern CRenderManager* PRenderManager;                                   // 0x013B8384
extern CDisplayManager* GPDisplayManager;                                // 0x013B8390
class CInterpolationInfo {                                               // Ego_r.pdb 0x34 bytes; no shared header
public:
    CInterpolationInfo();                                                // 0x00415A60 ??0CInterpolationInfo@@QAE@XZ
    unsigned char _data[0x34];
};
namespace NUISystem {
class CIME {                                                             // no shared header
public:
    static CIME& __fastcall Get();                                       // 0x00404A80 ?Get@CIME@NUISystem@@SIAAV12@XZ
    void Render();                                                       // 0x00404C00 ?Render@CIME@NUISystem@@QAEXXZ
};
}
class CNewFrontendGameComponent {                                        // no shared header; retail offsets (Ego_r.pdb - 8 after the two vectors)
public:
    virtual ~CNewFrontendGameComponent();
    unsigned char             _pad04[0x58 - 0x04];
    CCountedPointer<CIEngine> m_pEngine;                                 // +0x58
    unsigned char             _pad60[0xbc - 0x60];
    float                     m_fLastRenderTime;                         // +0xbc
private:
    void Draw(const CEngineInterpolationInfo& interpolation_info);
};
void CNewFrontendGameComponent::Draw(const CEngineInterpolationInfo& interpolation_info)
{
    CInterpolationInfo info;
    m_fLastRenderTime = (float)GFGetTime();
    C2DBoxF screen(0.0f, 0.0f, (float)VIRTUAL_SCREEN_WIDTH, (float)VIRTUAL_SCREEN_HEIGHT);
    GFFPUSetRoundingNearest();
    PRenderManager->SetFogEnable(false);
    PRenderManager->SetVWindow(screen);
    GPDisplayManager->GetVVisibleTelevisionScreenArea();
    PRenderManager->ClearRenderTarget(CRGBColour(0, 0, 0, 255), 0);
    GPDisplayManager->BeginRender();
    NUISystem::CFrontEndManager::GetInstance()->Draw(*m_pEngine, 0);
    if (NUISystem::CManager::GetInstance()->HasErrorMessage())
        NUISystem::CManager::GetInstance()->RenderErrorMessage(*m_pEngine);
    m_pEngine->Render(interpolation_info, 0);
    PRenderManager->SetTextureMipMapMode(0, TEXTURE_MIPMAP_LINEAR);
    PRenderManager->DrawBufferedPolys();
    PRenderManager->Render2DDrawList(true);
    NUISystem::CIME::Get().Render();
    PRenderManager->DrawBufferedPolys();
    PRenderManager->Render2DDrawList(true);
    GPDisplayManager->EndRender();
    GPDisplayManager->SwapScreens();
}
