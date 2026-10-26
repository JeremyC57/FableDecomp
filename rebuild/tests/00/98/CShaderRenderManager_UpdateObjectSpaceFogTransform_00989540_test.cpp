#include <cstdio>
#include <cstring>
// Behaviour model of CShaderRenderManager::UpdateObjectSpaceFogTransform @ 0x00989540.
#ifdef _MSC_VER
#define STDCALL __stdcall
#else
#define STDCALL
#endif
struct IDirect3DDevice9;
struct IDirect3DDevice9Vtbl {
    void* _slots[94];
    long (STDCALL *SetVertexShaderConstantF)(IDirect3DDevice9* self, unsigned long startRegister, const float* data, unsigned long count);
};
struct IDirect3DDevice9 { IDirect3DDevice9Vtbl* lpVtbl; };
class CShaderConstantRange { public: int vft; int Offset; unsigned int Count; };
class CVertexShaderConstantLayout { public: unsigned char _pad[0x80]; CShaderConstantRange FogTransform; };
class CMatrix4x4 { public: float m[16]; };
class C4DVector { public: float X, Y, Z, W; };
class CShaderRenderManager { public:
    CVertexShaderConstantLayout* CurrentVSConstantLayout;
    IDirect3DDevice9* D3DDevice;
    CMatrix4x4 WorldMatrix;
    C4DVector FogTransform;
    unsigned long UpdateFlags;
    void UpdateObjectSpaceFogTransform()
    {
        C4DVector fog;
        fog.X = FogTransform.X * WorldMatrix.m[0] + FogTransform.Y * WorldMatrix.m[4] + FogTransform.Z * WorldMatrix.m[8];
        fog.Y = FogTransform.X * WorldMatrix.m[1] + FogTransform.Y * WorldMatrix.m[5] + FogTransform.Z * WorldMatrix.m[9];
        fog.Z = FogTransform.X * WorldMatrix.m[2] + FogTransform.Y * WorldMatrix.m[6] + FogTransform.Z * WorldMatrix.m[10];
        fog.W = FogTransform.X * WorldMatrix.m[3] + FogTransform.Y * WorldMatrix.m[7] + FogTransform.Z * WorldMatrix.m[11] + FogTransform.W;
        D3DDevice->lpVtbl->SetVertexShaderConstantF(D3DDevice, CurrentVSConstantLayout->FogTransform.Offset, &fog.X, 1);
        UpdateFlags = (UpdateFlags & ~0x4000u) | 0x2000u;
    }
};
static IDirect3DDevice9* g_self; static unsigned long g_reg, g_count; static float g_v[4];
static long STDCALL Spy(IDirect3DDevice9* self, unsigned long reg, const float* data, unsigned long count)
{
    g_self = self; g_reg = reg; g_count = count; std::memcpy(g_v, data, sizeof(g_v)); return 0;
}
int main()
{
    static IDirect3DDevice9Vtbl vt; vt.SetVertexShaderConstantF = Spy;
    IDirect3DDevice9 dev; dev.lpVtbl = &vt;
    CVertexShaderConstantLayout layout; std::memset(&layout, 0, sizeof(layout)); layout.FogTransform.Offset = 17;
    CShaderRenderManager m;
    m.CurrentVSConstantLayout = &layout; m.D3DDevice = &dev;
    for (int i = 0; i < 16; ++i) m.WorldMatrix.m[i] = (float)(i + 1);
    m.FogTransform.X = 1.0f; m.FogTransform.Y = 2.0f; m.FogTransform.Z = 3.0f; m.FogTransform.W = 0.5f;
    m.UpdateFlags = 0x4001;
    m.UpdateObjectSpaceFogTransform();
    // column c: 1*m[c] + 2*m[4+c] + 3*m[8+c]  with m[i] = i+1
    int ok = g_self == &dev && g_reg == 17 && g_count == 1
          && g_v[0] == 1.0f * 1 + 2.0f * 5 + 3.0f * 9 && g_v[1] == 1.0f * 2 + 2.0f * 6 + 3.0f * 10
          && g_v[2] == 1.0f * 3 + 2.0f * 7 + 3.0f * 11 && g_v[3] == 1.0f * 4 + 2.0f * 8 + 3.0f * 12 + 0.5f
          && m.UpdateFlags == 0x2001;
    if (!ok) { std::printf("00989540_TEST FAIL\n"); return 1; }
    std::printf("00989540_TEST PASS\n");
    return 0;
}