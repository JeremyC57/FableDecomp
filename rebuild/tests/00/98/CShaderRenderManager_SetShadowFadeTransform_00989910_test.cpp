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
struct C4DVector { float x, y, z, w; };
struct CVertexShaderConstantLayout { unsigned long ShadowFadeTransformRegister; };
struct CShaderRenderManager {
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    IDirect3DDevice9*            D3DDevice;
    void SetVertexShaderConstant(unsigned long startRegister, float x, float y, float z, float w)
    {
        C4DVector v; v.x = x; v.y = y; v.z = z; v.w = w;
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, startRegister, &v.x, 1);
    }
    void SetShadowFadeTransform(const C4DVector* t)
    { SetVertexShaderConstant(CurrentVSConstantLayout->ShadowFadeTransformRegister, t->x, t->y, t->z, t->w); }
};
int main()
{
    InitDevice();
    CVertexShaderConstantLayout layout; layout.ShadowFadeTransformRegister = 33;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &g_device;
    C4DVector t; t.x = 1; t.y = 2; t.z = 3; t.w = 4;
    m.SetShadowFadeTransform(&t);
    if (g_calls != 1 || g_reg != 33 || g_count != 1 || g_data[0] != 1 || g_data[1] != 2 || g_data[2] != 3 || g_data[3] != 4) { std::printf("00989910_TEST FAIL\n"); return 1; }
    std::printf("00989910_TEST PASS\n");
    return 0;
}