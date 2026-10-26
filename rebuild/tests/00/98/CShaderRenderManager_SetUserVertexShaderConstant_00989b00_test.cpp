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
class C4DVector { public: float x, y, z, w; };
class CVertexShaderConstantLayout { public: long UserConstantBase; };
class CShaderRenderManager { public:
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    IDirect3DDevice9*            D3DDevice;
    void SetVertexShaderConstant(unsigned long startRegister, float x, float y, float z, float w)
    {
        C4DVector v; v.x = x; v.y = y; v.z = z; v.w = w;
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, startRegister, &v.x, 1);
    }
    void SetUserVertexShaderConstant(long index, float x, float y, float z, float w)
    { SetVertexShaderConstant(CurrentVSConstantLayout->UserConstantBase + index, x, y, z, w); }
};
int main()
{
    InitDevice();
    CVertexShaderConstantLayout layout; layout.UserConstantBase = 0x20;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &g_device;
    m.SetUserVertexShaderConstant(1, 0.1f, 0.2f, 0.3f, 0.4f);
    if (g_calls != 1 || g_reg != 0x21 || g_count != 1 || g_data[0] != 0.1f || g_data[1] != 0.2f || g_data[2] != 0.3f || g_data[3] != 0.4f) { std::printf("00989b00_TEST FAIL\n"); return 1; }
    std::printf("00989b00_TEST PASS\n");
    return 0;
}