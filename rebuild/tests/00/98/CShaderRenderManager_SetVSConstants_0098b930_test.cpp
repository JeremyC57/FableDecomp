// Behaviour test for 0x0098B930 CShaderRenderManager::SetVSConstants. The member is private (Ego_r.pdb ..@@AAEXJPBXK@Z), so this test carries the
// row text (verbatim below) with the shared header's access opened for the test only, and drives it through a hand-built device vtable
// (slot 94 = +0x178, IDirect3DDevice9::SetVertexShaderConstantF).
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#define private public
#define protected public
// ---- row text (verbatim) ----
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"
typedef long HRESULT;
void CShaderRenderManager::SetVSConstants(long index, const void* data, unsigned long count)
{
    if (count == 0)
        return;
    HRESULT hr = D3DDevice->SetVertexShaderConstantF(index, (const float*)data, count);
}
// ---- end of row text ----
#undef private
#undef protected
static int g_calls; static void* g_device; static unsigned int g_start, g_count; static const float* g_data; static long g_result;
static long __stdcall FakeSetVertexShaderConstantF(void* device, unsigned int start, const float* data, unsigned int count)
{
    ++g_calls; g_device = device; g_start = start; g_data = data; g_count = count;
    return g_result;
}
static long __stdcall WrongSlot(void*, unsigned int, const float*, unsigned int) { g_calls += 100; return 0; }
int main()
{
    static void* vtable[128];
    for (int i = 0; i < 128; ++i) vtable[i] = (void*)&WrongSlot;
    vtable[0x178 / 4] = (void*)&FakeSetVertexShaderConstantF;
    struct { void** vptr; } device = { vtable };
    static unsigned char storage[sizeof(CShaderRenderManager)];
    memset(storage, 0, sizeof storage);
    CShaderRenderManager* manager = (CShaderRenderManager*)storage;
    if (offsetof(CShaderRenderManager, D3DDevice) != 0x1D4) { printf("FAIL offset\n"); return 1; }
    manager->D3DDevice = (IDirect3DDevice9*)&device;
    float constants[8] = { 0 };
    int fail = 0;
    manager->SetVSConstants(4, constants, 0);                            // zero rows: the device is not called
    if (g_calls != 0) ++fail;
    manager->SetVSConstants(4, constants, 2);
    if (g_calls != 1 || g_device != &device || g_start != 4 || g_data != constants || g_count != 2) ++fail;
    g_result = (long)0x80004005;                                         // a failing HRESULT changes nothing (hr is only checked in debug)
    manager->SetVSConstants(95, constants + 4, 1);
    if (g_calls != 2 || g_start != 95 || g_data != constants + 4 || g_count != 1) ++fail;
    manager->SetVSConstants(-1, 0, 0xFFFFFFFFu);                         // the arguments are forwarded unchanged
    if (g_calls != 3 || g_start != 0xFFFFFFFFu || g_data != 0 || g_count != 0xFFFFFFFFu) ++fail;
    manager->SetVSConstants(7, constants, 0);
    if (g_calls != 3) ++fail;
    if (fail) { printf("FABLETLC_SET_VS_CONSTANTS_BEHAVIOR FAIL (%d)\n", fail); return 1; }
    printf("FABLETLC_SET_VS_CONSTANTS_BEHAVIOR PASS\n");
    return 0;
}