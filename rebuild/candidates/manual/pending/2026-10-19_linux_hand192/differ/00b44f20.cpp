// Grade: DIFFER(195v189) -- retail stores the attenuation record's two zero terms after the `LightCount > 0` test from the zero index register (edi), hoisted out of the loop, and aligns the reset loop instead of the light loop; the lane stores them as immediates before the test (declared before the loop) or per iteration (loop-body local, constructor temporary)
// CEngineLightingManager::SetupShaderPointLights(const CEngineLocalLightArray*) -- when the local light set changes, load each local light into shader point-light slot i+1 (unit intensity, the light's distance-squared and constant attenuation terms), reset the slots the previous set used beyond the new count, and remember the count
// Leaf: SetupShaderPointLights
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B44F20, 189 bytes (+ int3 padding), `ret 4`.
// Callees: CShaderPointLight::CShaderPointLight(const C3DVector&, const CRGBFloatColour&, float, const CShaderLightAttenuation&)
//   0x00B49320 (landed; vtable 0x012A2114), CShaderRenderManager::InitialiseLight(unsigned long, CShaderLight*) 0x00989830 and
//   ResetLight(unsigned long) 0x00988640 (on the global manager 0x01436E14), CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300 (the light's base).
// Layout: engine/CEngineLightingManager.h carries the PDB offsets (ShaderPointLightsEnabledCount +0xa4,
//   CurrentShaderLocalLights +0xac) while retail reads them at +0xa0 / +0xa8 (4 bytes lower, a 12-byte retail container
//   before them), so `this` is a local model with the PDB names. CEngineLocalLightArray {LightCount, CEngineLocalLightElement
//   Lights[8] of 0x34 bytes: WorldPos +0, Colour +0x18, AttenuationDistSquared +0x28, AttenuationConstant +0x2c}.
// Shape: the attenuation record's two constant zero terms sit in the loop preheader in retail (stored from the zero index
//   register after the count test); the point light is a loop-scoped local (its implicit dtor only calls the base dtor); the reset loop
//   reuses `j + 1` as the next index.
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderLightAttenuation {
    float Attenuation0;                                // +0x00
    float Attenuation1;                                // +0x04
    float Attenuation2;                                // +0x08
    float AttenuationBase;                             // +0x0c
};

struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                          // 0x0099A300
};
struct CShaderLight : CBaseClassNonCopyable {
};
struct CShaderPointLight : CShaderLight {              // vtable 0x012A2114, 0x40 bytes
    C3DVector               Direction;                 // +0x04
    C3DVector               Position;                  // +0x10
    CRGBFloatColour         Colour;                    // +0x1c
    float                   Intensity;                 // +0x2c
    CShaderLightAttenuation Attenuation;               // +0x30
    CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity,
                      const CShaderLightAttenuation& attenuation);   // 0x00B49320
};

struct CShaderRenderManager {
    void InitialiseLight(unsigned long index, CShaderLight* light);   // 0x00989830
    void ResetLight(unsigned long index);              // 0x00988640
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

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
    attenuation.Attenuation0 = 0.0f;
    attenuation.Attenuation1 = 0.0f;
    for (long i = 0; i < lights->LightCount; ++i) {
        const CEngineLocalLightElement& element = lights->Lights[i];
        attenuation.Attenuation2 = element.AttenuationDistSquared;
        attenuation.AttenuationBase = element.AttenuationConstant;
        CShaderPointLight light(element.WorldPos, element.Colour, 1.0f, attenuation);
        g_pShaderRenderManager->InitialiseLight(i + 1, &light);
    }
    for (long j = lights->LightCount; j < ShaderPointLightsEnabledCount; ++j)
        g_pShaderRenderManager->ResetLight(j + 1);
    ShaderPointLightsEnabledCount = lights->LightCount;
}
