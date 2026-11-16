// Behaviour test for CShaderRenderManager::InitialiseLight (0x00989830): self-contained doubles. Slot `index` receives the
// converted light, is enabled and flagged for update; slots >= 1 publish Attenuation2 / AttenuationBase into component (index-1)%4
// of the vector pair ((index-1)/4)*2 and +1; the light constants are marked dirty (0x700). Slot 0 leaves the table alone.
#include <stdio.h>
#include <string.h>
struct C4DVector { float X, Y, Z, W; };
struct CShaderGenericLight { float Body[10]; float Attenuation2; float AttenuationBase; float Tail; };
struct CLightInfo { CShaderGenericLight Light; bool IsEnabled; bool UpdateFlag; };
struct CShaderLight {
    float A2, Base; mutable int Calls;
    virtual void ConvertToGenericLight(CShaderGenericLight& g) const { ++Calls; g.Attenuation2 = A2; g.AttenuationBase = Base; g.Body[0] = A2 + Base; }
};
struct CShaderRenderManager {
    CLightInfo Lights[9]; C4DVector LightAttenuations[4]; unsigned long UpdateFlags;
    void InitialiseLight(unsigned long index, const CShaderLight& light)
    {
        CLightInfo& info = Lights[index];
        light.ConvertToGenericLight(info.Light);
        info.IsEnabled = true;
        info.UpdateFlag = true;
        if (index >= 1) {
            unsigned long pair = (index - 1) >> 2;
            long component = (long)(index - 1) & 3;
            float* attenuation = &LightAttenuations[pair * 2].X;
            attenuation[component] = info.Light.Attenuation2;
            attenuation[component + 4] = info.Light.AttenuationBase;
        }
        UpdateFlags |= 0x700;
    }
};
int main()
{
    int ok = 1, cases = 0;
    for (unsigned long index = 0; index < 9; ++index) {
        CShaderRenderManager m; memset(&m, 0, sizeof(m)); m.UpdateFlags = 0x11;
        for (int v = 0; v < 4; ++v) { m.LightAttenuations[v].X = m.LightAttenuations[v].Y = m.LightAttenuations[v].Z = m.LightAttenuations[v].W = -1.0f; }
        CShaderLight light; light.A2 = 0.5f + (float)index; light.Base = 2.0f * (float)(index + 1); light.Calls = 0;
        m.InitialiseLight(index, light);
        ok = ok && light.Calls == 1 && m.Lights[index].IsEnabled && m.Lights[index].UpdateFlag && m.UpdateFlags == 0x711;
        ok = ok && m.Lights[index].Light.Body[0] == light.A2 + light.Base;
        for (unsigned long other = 0; other < 9; ++other) if (other != index) ok = ok && !m.Lights[other].IsEnabled && !m.Lights[other].UpdateFlag;
        const float* table = &m.LightAttenuations[0].X;
        for (unsigned long cell = 0; cell < 16; ++cell) {
            float expected = -1.0f;
            if (index >= 1) {
                unsigned long j = index - 1, base = (j / 4) * 8 + (j % 4);
                if (cell == base) expected = light.A2;
                if (cell == base + 4) expected = light.Base;
            }
            ok = ok && table[cell] == expected;
        }
        ++cases;
    }
    ok = ok && cases == 9;
    printf(ok ? "00989830_TEST PASS\n" : "00989830_TEST FAIL\n");
    return ok ? 0 : 1;
}