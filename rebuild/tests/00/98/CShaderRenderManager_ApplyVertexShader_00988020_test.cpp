// Behaviour test for 0x00988020 CShaderRenderManager::ApplyVertexShader (NF151, q11717): rewritten on the shared headers the row now uses
// (engine/CShaderRenderManager.h, engine/CVertexShader.h, engine/CRenderManager.h); the row object is linked. The manager, render
// manager and shader are zeroed storage of the shared classes (their constructors are other rows); the private members are opened for
// the test only (ApplyVertexShader itself is public, so its symbol is unchanged). The device is a hand-built vtable: SetVertexShader
// slot 92 (+0x170), SetVertexDeclaration slot 87 (+0x15C). PassPattern unchanged.
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define private public
#define protected public
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_ISSOFTWAREPROCESSINGENABLED_INLINE
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DBoxF.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CVertexShader.h"
#include "engine/CRenderManager.h"
#undef private
#undef protected

static int g_CallCount;
static int g_CallKinds[4];
static void* g_CallValues[4];
static long __stdcall ObserveVertexShader(void*, void* shader)
{
    g_CallKinds[g_CallCount] = 1;
    g_CallValues[g_CallCount++] = shader;
    return 0;
}
static long __stdcall ObserveVertexDeclaration(void*, void* declaration)
{
    g_CallKinds[g_CallCount] = 2;
    g_CallValues[g_CallCount++] = declaration;
    return 0;
}
static long __stdcall WrongSlot(void*, void*) { g_CallCount += 100; return 0; }

static unsigned char g_ManagerStorage[sizeof(CShaderRenderManager)];
static unsigned char g_RenderStorage[sizeof(CRenderManager)];
static unsigned char g_ShaderStorage[sizeof(CVertexShader)];
static unsigned char g_DataStorage[sizeof(CVertexShaderData)];

int main()
{
    static void* vtable[128];
    for (int i = 0; i < 128; ++i) vtable[i] = (void*)&WrongSlot;
    vtable[0x170 / 4] = (void*)&ObserveVertexShader;
    vtable[0x15C / 4] = (void*)&ObserveVertexDeclaration;
    struct { void** vptr; } device = { vtable };

    if (offsetof(CShaderRenderManager, RenderManager) != 0x1D0 || offsetof(CShaderRenderManager, CurrentVertexShader) != 0x1D8 ||
        offsetof(CShaderRenderManager, SoftwareVertexShaderEnable) != 0x393 || offsetof(CShaderRenderManager, FixedFunctionVertexMode) != 0x395 ||
        offsetof(CRenderManager, FixedFunctionPipelineEnabled) != 0x3CFD ||
        offsetof(CRenderManager, DeviceState) + offsetof(CRenderStateManager::CDeviceState, SoftwareVertexProcessing) != 0x2908)
    {
        printf("FAIL offsets\n");
        return 9;
    }

    CShaderRenderManager* manager = (CShaderRenderManager*)g_ManagerStorage;
    CRenderManager* render = (CRenderManager*)g_RenderStorage;
    CVertexShader* shader = (CVertexShader*)g_ShaderStorage;
    CVertexShaderData* data = (CVertexShaderData*)g_DataStorage;
    shader->Data = data;
    data->VertexShader = (IDirect3DVertexShader9*)0x11110000;
    data->VertexDeclaration = (IDirect3DVertexDeclaration9*)0x22220000;
    manager->RenderManager = render;
    manager->D3DDevice = (IDirect3DDevice9*)&device;
    manager->CurrentVertexShader = data->VertexShader;
    manager->CurrentVertexShaderVertexDeclairation = data->VertexDeclaration;

    // Mode mismatch (shaders want hardware, the render manager is in software): the cached shader is dropped and both are reselected.
    render->DeviceState.SoftwareVertexProcessing.CurrentState = 1;
    render->FixedFunctionPipelineEnabled = true;
    manager->ApplyVertexShader(*shader, false);
    if (g_CallCount != 2 || g_CallKinds[0] != 1 || g_CallValues[0] != (void*)data->VertexShader || g_CallKinds[1] != 2 ||
        g_CallValues[1] != (void*)data->VertexDeclaration || manager->CurrentVertexShader != data->VertexShader ||
        manager->CurrentVertexShaderVertexDeclairation != data->VertexDeclaration || render->FixedFunctionPipelineEnabled)
        return 1;

    // Fixed-function mode wants software processing, which matches: nothing changes, `force` is not read.
    g_CallCount = 0;
    render->FixedFunctionPipelineEnabled = true;
    manager->SoftwareFixedFunctionEnable = true;
    manager->FixedFunctionVertexMode = true;
    manager->ApplyVertexShader(*shader, true);
    if (g_CallCount != 0 || !render->FixedFunctionPipelineEnabled)
        return 2;

    // A new declaration alone is enough to reselect.
    data->VertexDeclaration = (IDirect3DVertexDeclaration9*)0x44440000;
    manager->ApplyVertexShader(*shader, false);
    if (g_CallCount != 2 || g_CallValues[0] != (void*)data->VertexShader || g_CallValues[1] != (void*)0x44440000 ||
        manager->CurrentVertexShaderVertexDeclairation != data->VertexDeclaration)
        return 3;

    printf("FABLETLC_APPLY_VERTEX_SHADER_BEHAVIOR PASS\n");
    return 0;
}
