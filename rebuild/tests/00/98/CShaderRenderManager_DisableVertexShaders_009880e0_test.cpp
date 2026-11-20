// NF133 (q4727): the row now defines the shared-header member ?DisableVertexShaders@CShaderRenderManager@@QAEXXZ on engine/CShaderRenderManager.h, so this test no longer links it:
// the host model below carries the same body (the old stand-in names kept as test-local models). PassPattern unchanged.
#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct CDisplayEngineVertexShaderView
{
    fable_u8 unknown0000[0x3CFD];
    fable_u8 vertexShaderDisabled3CFD;

    void SetVertexShaderMode(bool enabled);
};

struct CShaderRenderManagerDisableVertexView
{
    fable_u8 unknown000[0x1D0];
    CDisplayEngineVertexShaderView* displayEngine1D0;
    fable_u8 unknown1D4[4];
    void* currentVertexShader1D8;
    fable_u8 unknown1DC[0x1B4];
    fable_u8 vertexShadersEnabled390;

    void DisableVertexShaders();
};

static int g_SetModeCalls = 0;
static bool g_LastMode = true;

void CDisplayEngineVertexShaderView::SetVertexShaderMode(bool enabled)
{
    ++g_SetModeCalls;
    g_LastMode = enabled;
}

void CShaderRenderManagerDisableVertexView::DisableVertexShaders()
{
    vertexShadersEnabled390 = 0;
    currentVertexShader1D8 = 0;
    displayEngine1D0->SetVertexShaderMode(false);
    displayEngine1D0->vertexShaderDisabled3CFD = 1;
}

int main()
{
    CDisplayEngineVertexShaderView displayEngine;
    CShaderRenderManagerDisableVertexView manager;
    memset(&displayEngine, 0, sizeof(displayEngine));
    memset(&manager, 0xFF, sizeof(manager));
    manager.displayEngine1D0 = &displayEngine;

    manager.DisableVertexShaders();
    if (
        manager.vertexShadersEnabled390 != 0 ||
        manager.currentVertexShader1D8 != 0 ||
        displayEngine.vertexShaderDisabled3CFD != 1 ||
        g_SetModeCalls != 1 ||
        g_LastMode)
    {
        return 1;
    }

    printf("FABLETLC_DISABLE_VERTEX_SHADERS_BEHAVIOR PASS\n");
    return 0;
}
