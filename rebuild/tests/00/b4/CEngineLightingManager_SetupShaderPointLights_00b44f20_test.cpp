#include <stdio.h>
// Behaviour test for CEngineLightingManager::SetupShaderPointLights (0x00B44F20).
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderLightAttenuation {
    float Attenuation0;                                // +0x00
    float Attenuation1;                                // +0x04
    float Attenuation2;                                // +0x08
    float AttenuationBase;                             // +0x0c
};

struct CBaseClassNonCopyable {
    virtual void v00() {}
    ~CBaseClassNonCopyable() {}
};
struct CShaderLight : CBaseClassNonCopyable {
};
struct CShaderPointLight : CShaderLight {
    CShaderPointLight() {}              // vtable 0x012A2114, 0x40 bytes
    C3DVector               Direction;                 // +0x04
    C3DVector               Position;                  // +0x10
    CRGBFloatColour         Colour;                    // +0x1c
    float                   Intensity;                 // +0x2c
    CShaderLightAttenuation Attenuation;               // +0x30
    CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity,
                      const CShaderLightAttenuation& attenuation)
        : Position(position), Colour(colour), Intensity(intensity), Attenuation(attenuation) { }
};

struct CShaderRenderManager {
    void InitialiseLight(unsigned long index, CShaderLight* light) { CShaderPointLight* p = (CShaderPointLight*)light; inits[index] = *p; ++nInit; }
    void ResetLight(unsigned long index) { resets[nReset++] = index; }
    CShaderPointLight inits[10]; long nInit; unsigned long resets[10]; long nReset;
    CShaderRenderManager() : nInit(0), nReset(0) {}
};
CShaderRenderManager* g_pShaderRenderManager;

struct CEngineLocalLightElement {                      // 0x34 bytes
    C3DVector       WorldPos;                          // +0x00
    float           Radius;                            // +0x0c
    float           RadiusSquared;                     // +0x10
    float           SpotRadius;                        // +0x14
    CRGBFloatColour Colour;                            // +0x18
    float           AttenuationDistSquared;            // +0x28
    float           AttenuationConstant;               // +0x2c
    unsigned long   ChannelMask;                       // +0x30
};
struct CEngineLocalLightArray {
    long                     LightCount;               // +0x00
    CEngineLocalLightElement Lights[8];                // +0x04
};

struct CEngineLightingManager {
    unsigned char                 _pad00[0xa0];
    long                          ShaderPointLightsEnabledCount;   // +0xa0 (retail)
    unsigned char                 _pada4[4];
    const CEngineLocalLightArray* CurrentShaderLocalLights;        // +0xa8 (retail)
    void SetupShaderPointLights(const CEngineLocalLightArray* lights);
};

void CEngineLightingManager::SetupShaderPointLights(const CEngineLocalLightArray* lights)
{
    if (CurrentShaderLocalLights == lights)
        return;
    CurrentShaderLocalLights = lights;
    CShaderLightAttenuation attenuation;
    long i = 0;
    if (i < lights->LightCount) {
        attenuation.Attenuation0 = 0.0f;
        attenuation.Attenuation1 = 0.0f;
        do {
            const CEngineLocalLightElement& element = lights->Lights[i];
            attenuation.Attenuation2 = element.AttenuationDistSquared;
            attenuation.AttenuationBase = element.AttenuationConstant;
            CShaderPointLight light(element.WorldPos, element.Colour, 1.0f, attenuation);
            g_pShaderRenderManager->InitialiseLight(i + 1, &light);
        } while (++i < lights->LightCount);
    }
    for (long j = lights->LightCount; j < ShaderPointLightsEnabledCount; ++j)
        g_pShaderRenderManager->ResetLight(j + 1);
    ShaderPointLightsEnabledCount = lights->LightCount;
}

int main()
{
    int ok = 1;
    CShaderRenderManager* rm = new CShaderRenderManager; g_pShaderRenderManager = rm;
    CEngineLightingManager lm; lm.ShaderPointLightsEnabledCount = 4; lm.CurrentShaderLocalLights = 0;
    CEngineLocalLightArray a; a.LightCount = 2;
    for (int i = 0; i < 2; ++i) {
        a.Lights[i].WorldPos.X = (float)i; a.Lights[i].WorldPos.Y = 1; a.Lights[i].WorldPos.Z = 2;
        a.Lights[i].Colour.R = 0.5f; a.Lights[i].Colour.G = 0.25f; a.Lights[i].Colour.B = 1; a.Lights[i].Colour.A = 1;
        a.Lights[i].AttenuationDistSquared = 10.0f + i; a.Lights[i].AttenuationConstant = 20.0f + i;
    }
    lm.SetupShaderPointLights(&a);
    if (rm->nInit != 2 || rm->inits[1].Position.X != 0 || rm->inits[2].Position.X != 1.0f || rm->inits[2].Intensity != 1.0f) ok = 0;
    if (rm->inits[2].Attenuation.Attenuation0 != 0 || rm->inits[2].Attenuation.Attenuation1 != 0 || rm->inits[2].Attenuation.Attenuation2 != 11.0f || rm->inits[2].Attenuation.AttenuationBase != 21.0f) ok = 0;
    if (rm->nReset != 2 || rm->resets[0] != 3 || rm->resets[1] != 4 || lm.ShaderPointLightsEnabledCount != 2 || lm.CurrentShaderLocalLights != &a) ok = 0;
    lm.SetupShaderPointLights(&a);
    if (rm->nInit != 2 || rm->nReset != 2) ok = 0;
    CEngineLocalLightArray b; b.LightCount = 0;
    lm.SetupShaderPointLights(&b);
    if (rm->nInit != 2 || rm->nReset != 4 || rm->resets[2] != 1 || rm->resets[3] != 2 || lm.ShaderPointLightsEnabledCount != 0) ok = 0;
    if (ok) { printf("00b44f20_TEST PASS\n"); return 0; }
    printf("00b44f20_TEST FAIL\n"); return 1;
}