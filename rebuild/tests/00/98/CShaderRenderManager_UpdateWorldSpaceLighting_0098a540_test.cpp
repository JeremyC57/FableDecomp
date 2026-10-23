#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
typedef long long __int64;
#endif
// behaviour test: CShaderRenderManager::UpdateWorldSpaceLighting flushes dirty light runs, fills disabled lights with (0,0,0,1), uploads attenuations and moves the dirty flags


struct C4DVector {
    float X, Y, Z, W;
    C4DVector() {}
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct CShaderConstantRange {
    virtual ~CShaderConstantRange();
    long          Offset;                        // +0x4
    unsigned long Count;                         // +0x8
};
struct CVertexShaderConstantLayout {
    unsigned char        _pad00[0x4c];
    unsigned long        LightSize;              // +0x4c
    CShaderConstantRange LightArray;             // +0x50
    CShaderConstantRange LightGlobals;           // +0x5c
    CShaderConstantRange LightAttenuations;      // +0x68
};
struct CShaderGenericLight {
    C4DVector     Parameters[2];                 // +0x00
    long          Types[2];                      // +0x20
    float         Attenuation2;                  // +0x28
    float         AttenuationBase;               // +0x2c
    unsigned long Size;                          // +0x30
};
struct CLightInfo {
    CShaderGenericLight Light;                   // +0x00
    bool          IsEnabled;                     // +0x34
    bool          UpdateFlag;                    // +0x35
};
template <class T> struct CArray {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T* GetData() const
    {
        const T* p = (const T*)this;
        if (size() > 0)
            p = _M_start;
        return p;
    }
};
struct IDirect3DDevice9 {
    virtual void d000() const {} virtual void d001() const {} virtual void d002() const {} virtual void d003() const {} virtual void d004() const {} virtual void d005() const {}
    virtual void d006() const {} virtual void d007() const {} virtual void d008() const {} virtual void d009() const {} virtual void d010() const {} virtual void d011() const {}
    virtual void d012() const {} virtual void d013() const {} virtual void d014() const {} virtual void d015() const {} virtual void d016() const {} virtual void d017() const {}
    virtual void d018() const {} virtual void d019() const {} virtual void d020() const {} virtual void d021() const {} virtual void d022() const {} virtual void d023() const {}
    virtual void d024() const {} virtual void d025() const {} virtual void d026() const {} virtual void d027() const {} virtual void d028() const {} virtual void d029() const {}
    virtual void d030() const {} virtual void d031() const {} virtual void d032() const {} virtual void d033() const {} virtual void d034() const {} virtual void d035() const {}
    virtual void d036() const {} virtual void d037() const {} virtual void d038() const {} virtual void d039() const {} virtual void d040() const {} virtual void d041() const {}
    virtual void d042() const {} virtual void d043() const {} virtual void d044() const {} virtual void d045() const {} virtual void d046() const {} virtual void d047() const {}
    virtual void d048() const {} virtual void d049() const {} virtual void d050() const {} virtual void d051() const {} virtual void d052() const {} virtual void d053() const {}
    virtual void d054() const {} virtual void d055() const {} virtual void d056() const {} virtual void d057() const {} virtual void d058() const {} virtual void d059() const {}
    virtual void d060() const {} virtual void d061() const {} virtual void d062() const {} virtual void d063() const {} virtual void d064() const {} virtual void d065() const {}
    virtual void d066() const {} virtual void d067() const {} virtual void d068() const {} virtual void d069() const {} virtual void d070() const {} virtual void d071() const {}
    virtual void d072() const {} virtual void d073() const {} virtual void d074() const {} virtual void d075() const {} virtual void d076() const {} virtual void d077() const {}
    virtual void d078() const {} virtual void d079() const {} virtual void d080() const {} virtual void d081() const {} virtual void d082() const {} virtual void d083() const {}
    virtual void d084() const {} virtual void d085() const {} virtual void d086() const {} virtual void d087() const {} virtual void d088() const {} virtual void d089() const {}
    virtual void d090() const {} virtual void d091() const {} virtual void d092() const {} virtual void d093() const {}
    virtual long __stdcall SetVertexShaderConstantF(unsigned int startRegister, const float* data, unsigned int vector4fCount); // slot 94 (+0x178)
};
struct CShaderRenderManager {
    void*                        __vftable;
    CVertexShaderConstantLayout* CurrentVSConstantLayout;    // +0x004
    unsigned char                _pad008[0x164 - 0x8];
    CLightInfo*                  Lights;                     // +0x164
    C4DVector*                   LightBuffer;                // +0x168
    unsigned long                LightCount;                 // +0x16c
    unsigned char                _pad170[0x1a0 - 0x170];
    CArray<C4DVector>            LightAttenuations;          // +0x1a0
    unsigned char                _pad1ac[0x1d4 - 0x1ac];
    IDirect3DDevice9*            D3DDevice;                  // +0x1d4
    unsigned char                _pad1d8[0x3d8 - 0x1d8];
    unsigned long                UpdateFlags;                // +0x3d8
    void SetVSConstants(long startRegister, const C4DVector* data, unsigned long count)
    {
        if (count != 0)
            D3DDevice->SetVertexShaderConstantF(startRegister, (const float*)data, count);
    }
    void UpdateWorldSpaceLighting();
};

void CShaderRenderManager::UpdateWorldSpaceLighting()
{
    long first = -1;
    long last = 0;
    long lightsUsed = 0;
    for (unsigned long i = 0; i < LightCount; ++i) {
        CLightInfo& light = Lights[i];
        if (!light.UpdateFlag && !light.IsEnabled) {
            if (first != -1) {
                SetVSConstants(CurrentVSConstantLayout->LightArray.Offset + first, &LightBuffer[first], last - first);
                first = -1;
            }
        } else {
            light.UpdateFlag = false;
            unsigned long lightSize = CurrentVSConstantLayout->LightSize;
            long base = lightSize * i;
            C4DVector* dst = &LightBuffer[base];
            if (first == -1)
                first = base;
            last = base + lightSize;
            lightsUsed = i + 1;
            if (light.IsEnabled) {
                unsigned long size = light.Light.Size;
                for (unsigned long j = 0; j < size; ++j)
                    dst[j] = light.Light.Parameters[j];
            } else {
                for (unsigned long j = 0; j < CurrentVSConstantLayout->LightSize; ++j)
                    dst[j] = C4DVector(0.0f, 0.0f, 0.0f, 1.0f);
            }
        }
    }
    if (first != -1)
        SetVSConstants(CurrentVSConstantLayout->LightArray.Offset + first, &LightBuffer[first], last - first);
    if (lightsUsed > 1) {
        long reg = CurrentVSConstantLayout->LightAttenuations.Offset;
        unsigned long count = (lightsUsed + 2) / 4 * 2;
        SetVSConstants(reg, LightAttenuations.GetData(), count);
    }
    UpdateFlags = (UpdateFlags & ~0x200) | 0x100;
}

CShaderConstantRange::~CShaderConstantRange() {}
static int g_calls = 0; static unsigned int g_reg[8], g_cnt[8]; static const float* g_data[8];
long __stdcall IDirect3DDevice9::SetVertexShaderConstantF(unsigned int r, const float* d, unsigned int n)
{ if (g_calls < 8) { g_reg[g_calls] = r; g_data[g_calls] = d; g_cnt[g_calls] = n; } ++g_calls; return 0; }
static int fails = 0;
static void Check(bool ok, int line) { if (!ok) { std::printf("fail line %d\n", line); ++fails; } }
int main()
{
    static IDirect3DDevice9 dev;
    static CVertexShaderConstantLayout layout;
    layout.LightSize = 2; layout.LightArray.Offset = 10; layout.LightAttenuations.Offset = 40;
    static CLightInfo lights[4];
    static C4DVector buffer[8];
    static C4DVector att[3];
    static CShaderRenderManager m;
    m.CurrentVSConstantLayout = &layout; m.Lights = lights; m.LightBuffer = buffer; m.LightCount = 4; m.D3DDevice = &dev;
    m.LightAttenuations._M_start = att; m.LightAttenuations._M_finish = att + 3; m.LightAttenuations._M_end_of_storage = att + 3;
    m.UpdateFlags = 0x200 | 0x1;
    for (int i = 0; i < 8; ++i) buffer[i] = C4DVector(-1, -1, -1, -1);
    // light 0 enabled (1 parameter), light 1 dirty+disabled, light 2 idle, light 3 enabled (2 parameters)
    lights[0].IsEnabled = true; lights[0].UpdateFlag = false; lights[0].Light.Size = 1; lights[0].Light.Parameters[0] = C4DVector(1, 2, 3, 4);
    lights[1].IsEnabled = false; lights[1].UpdateFlag = true;
    lights[2].IsEnabled = false; lights[2].UpdateFlag = false;
    lights[3].IsEnabled = true; lights[3].UpdateFlag = true; lights[3].Light.Size = 2;
    lights[3].Light.Parameters[0] = C4DVector(5, 6, 7, 8); lights[3].Light.Parameters[1] = C4DVector(9, 10, 11, 12);
    m.UpdateWorldSpaceLighting();
    Check(buffer[0].X == 1 && buffer[0].W == 4 && buffer[1].X == -1, __LINE__);
    Check(buffer[2].X == 0 && buffer[2].Z == 0 && buffer[2].W == 1 && buffer[3].X == 0 && buffer[3].W == 1, __LINE__);
    Check(buffer[4].X == -1 && buffer[6].X == 5 && buffer[7].Z == 11, __LINE__);
    Check(!lights[1].UpdateFlag && !lights[3].UpdateFlag, __LINE__);
    // run 0..3 flushed at the idle light, run 6..7 at the end, then (4+2)/4*2 = 2 attenuation vectors
    Check(g_calls == 3, __LINE__);
    Check(g_reg[0] == 10 && g_data[0] == &buffer[0].X && g_cnt[0] == 4, __LINE__);
    Check(g_reg[1] == 16 && g_data[1] == &buffer[6].X && g_cnt[1] == 2, __LINE__);
    Check(g_reg[2] == 40 && g_data[2] == &att[0].X && g_cnt[2] == 2, __LINE__);
    Check(m.UpdateFlags == (0x100 | 0x1), __LINE__);
    // nothing dirty: no uploads; one light used: no attenuations; empty table passes the array object
    g_calls = 0; m.UpdateFlags = 0x200;
    lights[0].IsEnabled = false; lights[3].IsEnabled = false;
    m.UpdateWorldSpaceLighting();
    Check(g_calls == 0 && m.UpdateFlags == 0x100, __LINE__);
    lights[2].UpdateFlag = true; lights[3].UpdateFlag = true; m.LightAttenuations._M_finish = att;
    m.UpdateWorldSpaceLighting();
    Check(g_calls == 2 && g_reg[0] == 14 && g_cnt[0] == 4 && g_cnt[1] == 2 && g_data[1] == (const float*)&m.LightAttenuations, __LINE__);
    if (fails) { std::printf("0098a540_TEST FAIL\n"); return 1; }
    std::printf("0098a540_TEST PASS\n");
    return 0;
}