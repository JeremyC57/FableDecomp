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
    void SetUserVertexShaderConstant(long index, const C4DVector* value)
    { SetVertexShaderConstant(CurrentVSConstantLayout->UserConstantBase + index, value->x, value->y, value->z, value->w); }
};
int main()
{
    InitDevice();
    CVertexShaderConstantLayout layout; layout.UserConstantBase = 0x10;
    CShaderRenderManager m; m.CurrentVSConstantLayout = &layout; m.D3DDevice = &g_device;
    C4DVector v; v.x = -1; v.y = -2; v.z = -3; v.w = -4;
    m.SetUserVertexShaderConstant(-2, &v);
    if (g_calls != 1 || g_reg != 0x0e || g_count != 1 || g_data[0] != -1 || g_data[1] != -2 || g_data[2] != -3 || g_data[3] != -4) { std::printf("00989ab0_TEST FAIL\n"); return 1; }
    std::printf("00989ab0_TEST PASS\n");
    return 0;
}