#include <stdio.h>
#include <string.h>

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

static fable_u32 g_PixelShaderVersion = 0;

unsigned long CDisplayManager::GetPixelShaderVersion() const
{
    return g_PixelShaderVersion;
}

int main()
{
    static unsigned long displayManagerStorage[1024];   // the callee is stubbed above; no CDisplayManager member is read
    CDisplayEnginePixelShaderView displayEngine;
    CShaderRenderManagerPixelShaderView manager;
    memset(&displayEngine, 0, sizeof(displayEngine));
    memset(&manager, 0, sizeof(manager));
    displayEngine.displayManager3A3C = reinterpret_cast<CDisplayManager*>(displayManagerStorage);
    manager.displayEngine1D0 = &displayEngine;

    g_PixelShaderVersion = 999;
    manager.EnablePixelShaders();
    if (manager.pixelShadersEnabled391 != 0)
    {
        return 1;
    }

    g_PixelShaderVersion = 1000;
    manager.EnablePixelShaders();
    if (manager.pixelShadersEnabled391 != 1)
    {
        return 1;
    }

    printf("FABLETLC_ENABLE_PIXEL_SHADERS_BEHAVIOR PASS\n");
    return 0;
}
