// CShaderRenderManager::EnablePixelShaders() -- enable the pixel shaders when the display manager reports version 1.0 or later
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988110, 36 bytes.
// Name (round 7, NF53): the callee 0x009BEE00 is ?GetPixelShaderVersion@CDisplayManager@@QBEKXZ (Ego_r.pdb 0x00B23A80 / FableWin.pdb
//   0x03022F70; the landed self row 0x009BEE00 defines it so); the display manager pointer (+0x3A3C of the render manager) is now the
//   shared engine/CDisplayManager.h class (FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION) instead of the local
//   CDisplayManagerPixelShaderView. The render-manager / shader-render-manager views are unchanged (header debt).
#include "rebuild_abi.h"
#include "engine/Direct3D9Types.h"
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/CDisplayManager.h"

struct CDisplayEnginePixelShaderView
{
    fable_u8 unknown0000[0x3A3C];
    CDisplayManager* displayManager3A3C;
};

struct CShaderRenderManagerPixelShaderView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEnginePixelShaderView* displayEngine1D0;
    fable_u8 unknown1D4[0x1BD];
    fable_u8 pixelShadersEnabled391;

    void EnablePixelShaders();
};

void CShaderRenderManagerPixelShaderView::EnablePixelShaders()
{
    if (displayEngine1D0
            ->displayManager3A3C
            ->GetPixelShaderVersion() >= 1000)
    {
        pixelShadersEnabled391 = 1;
    }
}
