#include <stdio.h>
#include <string.h>
// Behaviour test for 0x009BEF20 CDisplayManager::BeginRender (links the row object).
// NF139 (2026-11-20, q4292 / q6168): rewritten on the shared headers the row now uses (engine/CDisplayManager.h,
//   engine/CRenderManager.h, engine/IDirect3DDevice9.h); the checks and the PassPattern are unchanged. The manager is a zeroed block
//   with PRenderManager (+0x08) and D3DDevice (+0x58) written in place; the device is a fake object whose slot 41 (+0xA4) is the
//   BeginScene fixture; CRenderManagerCore::BeginRender is a counting fixture.
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"

static int g_coreBeginCalls = 0;

bool CRenderManagerCore::BeginRender()
{
    ++g_coreBeginCalls;
    return true;
}

struct FakeDevice { void** vtable; };

static long g_sceneResult = 0;

static long __stdcall BeginSceneFixture(FakeDevice*)
{
    return g_sceneResult;
}

static double g_storage[(sizeof(CDisplayManager) + 7) / 8];
static double g_renderManager[(sizeof(CRenderManager) + 7) / 8];

int main()
{
    void* vtable[0x2A];
    memset(vtable, 0, sizeof vtable);
    vtable[0x29] = (void*)&BeginSceneFixture;
    FakeDevice device = {vtable};
    memset(g_storage, 0, sizeof g_storage);
    unsigned char* raw = (unsigned char*)g_storage;
    FakeDevice* devicePointer = &device;
    void* renderManagerPointer = g_renderManager;
    memcpy(raw + 0x08, &renderManagerPointer, 4);
    memcpy(raw + 0x58, &devicePointer, 4);
    CDisplayManager& display = *(CDisplayManager*)raw;

    g_sceneResult = 0;
    if (!display.BeginRender() ||
        raw[0x214] != 1 ||
        g_coreBeginCalls != 1)
    {
        printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR FAIL code=1\n");
        return 1;
    }

    raw[0x214] = 0;
    g_sceneResult = -1;
    if (display.BeginRender() ||
        raw[0x214] != 0 ||
        g_coreBeginCalls != 1)
    {
        printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR FAIL code=2\n");
        return 2;
    }

    printf("FABLETLC_DISPLAY_BEGIN_RENDER_BEHAVIOR PASS\n");
    return 0;
}
