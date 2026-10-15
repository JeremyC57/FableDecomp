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
struct CVertexShaderConstantLayout { long UserConstantBase; };
struct CShaderRenderManager {
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    IDirect3DDevice9*            D3DDevice;
    void SetUserVertexShaderConstants(unsigned long index, unsigned long count, const float* data)
    {
        if (count != 0)
            D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, CurrentVSConstantLayout->UserConstantBase + index, data, count);
    }
};
int main()
{
    InitDevice();
    CVertexShaderConstantLayout layout; layout.UserConstantBase = 0x60;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &g_device;
    float data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    m.SetUserVertexShaderConstants(5, 2, data);
    if (g_calls != 1 || g_reg != 0x65 || g_count != 2 || g_data[7] != 8.0f || g_self != &g_device) { std::printf("009896d0_TEST FAIL\n"); return 1; }
    m.SetUserVertexShaderConstants(1, 0, data);
    if (g_calls != 1) { std::printf("009896d0_TEST FAIL\n"); return 1; }
    std::printf("009896d0_TEST PASS\n");
    return 0;
}
