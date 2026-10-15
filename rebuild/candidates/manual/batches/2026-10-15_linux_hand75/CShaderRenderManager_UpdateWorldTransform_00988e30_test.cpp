#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[94];
    long (__stdcall *SetVertexShaderConstantF)(IDirect3DDevice9* self, unsigned long startRegister, const float* data, unsigned long vector4fCount);
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };

static int g_calls = 0;
static IDirect3DDevice9* g_self = 0;
static unsigned long g_reg = 0, g_count = 0;
static float g_data[16];
static long __stdcall FakeSetVSConstF(IDirect3DDevice9* self, unsigned long reg, const float* data, unsigned long count)
{
    ++g_calls; g_self = self; g_reg = reg; g_count = count;
    for (unsigned long i = 0; i < count * 4 && i < 16; ++i) g_data[i] = data[i];
    return 0;
}
static IDirect3DDevice9Vtbl g_vtbl;
static IDirect3DDevice9 g_device;
static void InitDevice() { for (int i = 0; i < 94; ++i) g_vtbl._slots[i] = 0; g_vtbl.SetVertexShaderConstantF = FakeSetVSConstF; g_device.lpVtbl = &g_vtbl; }
struct CVertexShaderConstantLayout { unsigned long WorldRegister; unsigned long WorldVectorCount; };
struct CMatrix4x4 { float m[16]; };
struct CShaderRenderManager {
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    IDirect3DDevice9*            D3DDevice;
    CMatrix4x4                   WorldMatrix;
    unsigned long                UpdateFlags;
    void UpdateWorldTransform()
    {
        CVertexShaderConstantLayout* layout = CurrentVSConstantLayout;
        unsigned long count = layout->WorldVectorCount < 4 ? layout->WorldVectorCount : 4;
        if (count != 0)
            D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, layout->WorldRegister, WorldMatrix.m, count);
        UpdateFlags &= ~0x10u;
    }
};
int main()
{
    InitDevice();
    CVertexShaderConstantLayout layout; layout.WorldRegister = 12; layout.WorldVectorCount = 6;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &g_device; m.UpdateFlags = 0xff;
    for (int i = 0; i < 16; ++i) m.WorldMatrix.m[i] = (float)i;
    m.UpdateWorldTransform();
    if (g_calls != 1 || g_self != &g_device || g_reg != 12 || g_count != 4 || g_data[15] != 15.0f || m.UpdateFlags != 0xef) { std::printf("00988e30_TEST FAIL\n"); return 1; }
    layout.WorldVectorCount = 3; m.UpdateWorldTransform();
    if (g_calls != 2 || g_count != 3) { std::printf("00988e30_TEST FAIL\n"); return 1; }
    layout.WorldVectorCount = 0; m.UpdateFlags = 0x10; m.UpdateWorldTransform();
    if (g_calls != 2 || m.UpdateFlags != 0) { std::printf("00988e30_TEST FAIL\n"); return 1; }
    std::printf("00988e30_TEST PASS\n");
    return 0;
}
