// CEngineLightingManager::SetupShaderPointLights(const CEngineLocalLightArray& lights) -- when the local light set changed, load each of its lights into shader light slot i+1 as a CShaderPointLight (intensity 1, attenuation {0, 0, dist^2, constant}), reset the slots the previous set used beyond the new count, and remember the count
// Grade: DIFFER(195v189) -- retail stores the attenuation's two zero terms once in the loop preheader from the zero register holding i (`mov [esp+10h],edi`) and leaves the loop head unaligned; ours stores immediates every pass (inner local, outer local, ctor temporary forms) and pads the loop head
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44F20, length 192 bytes (ret 4).
// Callees: CShaderPointLight::CShaderPointLight(const C3DVector&, const CRGBFloatColour&, float, const CShaderLightAttenuation&)
// 0x00B49320 (vtable 0x012A2114 by RTTI; manifest "CShaderDirectionalLight" is a propagation), ~CBaseClassNonCopyable 0x0099A300
// (the light's compiler-generated destructor), CShaderRenderManager::InitialiseLight(unsigned long, CShaderLight*) 0x00989830 and
// ResetLight(unsigned long) 0x00988640 on the shader render manager 0x01436E14.
// Layout: retail ShaderPointLightsEnabledCount +0xA0 and CurrentShaderLocalLights +0xA8 (PDB +0xA4/+0xAC: the earlier CArray is
// 4 bytes narrower); CEngineLocalLightArray LightCount +0, Lights +4 (0x34-byte elements: WorldPos +0, Colour +0x18,
// AttenuationDistSquared +0x28, AttenuationConstant +0x2C).
// Shape: the attenuation is a function-scope local filled in the loop body; its two invariant zero terms are hoisted into
// the loop preheader (stored from the zero register that holds i); the element walk is a pointer
// advanced by 0x34 (`add esi,34h`); the light is a block-scoped local destroyed through the out-of-line base destructor;
// both loops compare signed against the re-read counts.
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };

struct CShaderLightAttenuation {
    float Attenuation0;                         // +0x00
    float Attenuation1;                         // +0x04
    float Attenuation2;                         // +0x08
    float AttenuationBase;                      // +0x0c
};

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();           // 0x0099A300
};
class CShaderLight : public CBaseClassNonCopyable {
public:
    C3DVector Direction;                        // +0x04
};
class CShaderPointLight : public CShaderLight { // vtable 0x012A2114, 0x40 bytes
public:
    C3DVector               Position;           // +0x10
    CRGBFloatColour         Colour;             // +0x1c
    float                   Intensity;          // +0x2c
    CShaderLightAttenuation Attenuation;        // +0x30
    CShaderPointLight(const C3DVector& position, const CRGBFloatColour& colour, float intensity,
                      const CShaderLightAttenuation& attenuation);   // 0x00B49320
};

class CShaderRenderManager {
public:
    void InitialiseLight(unsigned long index, CShaderLight* light);   // 0x00989830
    void ResetLight(unsigned long index);                             // 0x00988640
};
extern CShaderRenderManager* g_pShaderRenderManager;   // 0x01436E14

struct CEngineLocalLightElement {               // 0x34 bytes
    C3DVector       WorldPos;                   // +0x00
    float           Radius;                     // +0x0c
    float           RadiusSquared;              // +0x10
    float           SpotRadius;                 // +0x14
    CRGBFloatColour Colour;                     // +0x18
    float           AttenuationDistSquared;     // +0x28
    float           AttenuationConstant;        // +0x2c
    unsigned long   ChannelMask;                // +0x30
};
struct CEngineLocalLightArray {
    long                     LightCount;        // +0x00
    CEngineLocalLightElement Lights[8];         // +0x04
};

class CEngineLightingManager {
public:
    unsigned char                 _pad000[0xa0];
    long                          ShaderPointLightsEnabledCount;   // +0xa0
    unsigned char                 _pad0a4[4];
    const CEngineLocalLightArray* CurrentShaderLocalLights;        // +0xa8
    void SetupShaderPointLights(const CEngineLocalLightArray& lights);
};

void CEngineLightingManager::SetupShaderPointLights(const CEngineLocalLightArray& lights)
{
    if (CurrentShaderLocalLights == &lights)
        return;
    CurrentShaderLocalLights = &lights;
    CShaderLightAttenuation attenuation;
    for (long i = 0; i < lights.LightCount; ++i) {
        const CEngineLocalLightElement& element = lights.Lights[i];
        attenuation.Attenuation0 = 0.0f;
        attenuation.Attenuation1 = 0.0f;
        attenuation.Attenuation2 = element.AttenuationDistSquared;
        attenuation.AttenuationBase = element.AttenuationConstant;
        CShaderPointLight light(element.WorldPos, element.Colour, 1.0f, attenuation);
        g_pShaderRenderManager->InitialiseLight(i + 1, &light);
    }
    for (long j = lights.LightCount; j < ShaderPointLightsEnabledCount; ++j)
        g_pShaderRenderManager->ResetLight(j + 1);
    ShaderPointLightsEnabledCount = lights.LightCount;
}
