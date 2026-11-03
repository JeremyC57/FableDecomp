// CEngineLightingManager::UpdateGlobalLightingStates() -- push the global lighting state to the shader manager when it is dirty: blend the primitive diffuse / backlight colours between the two current theme channels (optionally dimmed by the faded shadow factor) into the global directional light and backlight, refresh the SIMD lighting block's diffuse colour; blend the ambient colour (lifted towards white by the self-illumination percentage); and, in shadow mode 1, the shadow fade factors
// Leaf: ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B46C80, 1531 bytes, esp frame 0x74, plain `ret`.
// Name: Ego_r.pdb ?UpdateGlobalLightingStates@CEngineLightingManager@@QAEXXZ.
// Callees: CShaderDirectionalLight ctor 0x00B49950 (??0CShaderDirectionalLight@@QAE@ABVC3DVector@@ABVCRGBFloatColour@@1@Z),
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (the light's inline destructors end there),
//   CShaderRenderManager::InitialiseLight 0x00989830 / SetLightGlobal 0x0098B2C0 / SetAmbientColour 0x00988680 /
//   SetShadowFadeFactors 0x00989960 (Ego_r spellings).
// Globals: g_pShaderRenderManager 0x01436E14; 1/255 0x01231724, 0.01 0x01230A5C, 1.0 0x0122DED8 (literals).
// Layout: CEngineLightingManager modelled locally as the landed rows of this TU (two polymorphic bases); offsets are retail
//   (Ego_r - 0xC after the STLport vector): ThemeChannels +0x3C (CThemeChannel stride 0x70), Primitive colours +0x48/+0x58/+0x68,
//   FadedShadowDiffuseFactorEnabled +0xCE, CurrentThemeChannel +0xE0, StateUpdateFlags +0xE4, SelfIlluminationAmbientPercentage
//   +0xE8, SIMDVarBlock +0x4690, ShadowMode +0x4694.
#include "engine/C3DVector.h"
#include "engine/CArray.h"
#include "engine/CBaseClassNonCopyable.h"
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour operator+(const CRGBFloatColour& c) const { return CRGBFloatColour(R + c.R, G + c.G, B + c.B, A + c.A); }
};
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector(const CRGBFloatColour& c) : X(c.R), Y(c.G), Z(c.B), W(c.A) { }   // ??0C4DVector@@QAE@ABVCRGBFloatColour@@@Z
};
class CShaderLight : public CBaseClassNonCopyable { public:
};
class CShaderDirectionalLight : public CShaderLight { public:
    CShaderDirectionalLight(const C3DVector& direction, const CRGBFloatColour& colour, const CRGBFloatColour& backColour);   // 0x00B49950
    C3DVector Direction;                                   // +0x04
    C3DVector Position;                                    // +0x10
    CRGBFloatColour Colour;                                // +0x1C
    CRGBFloatColour BackColour;                            // +0x2C
};
class CShaderRenderManager { public:
    void InitialiseLight(unsigned long index, const CShaderLight& light);                // 0x00989830
    void SetLightGlobal(unsigned long index, const C4DVector& value);                   // 0x0098B2C0
    void SetAmbientColour(const CRGBFloatColour& colour);                               // 0x00988680
    void SetShadowFadeFactors(float a, float b, float c);                               // 0x00989960
};
extern CShaderRenderManager* g_pShaderRenderManager;      // 0x01436E14

class CEnginePrimitiveThemeChannelDesc { public:
    unsigned char ChannelIndex0;
    unsigned char ChannelIndex1;
    unsigned char ChannelBlendFactor;
    unsigned char Pad;
};
class CLightingSIMDVarBlock { public:                      // Ego_r.pdb, 0x110 bytes
    float Zero[4];
    float One[4];
    float DiffuseVectorX[4];
    float DiffuseVectorY[4];
    float DiffuseVectorZ[4];
    float DiffuseColourR[4];                               // +0x50
    float DiffuseColourG[4];                               // +0x60
    float DiffuseColourB[4];                               // +0x70
    float BackColourR[4];
    float BackColourG[4];
    float BackColourB[4];
    float AmbientColourR[4];
    float AmbientColourG[4];
    float AmbientColourB[4];
    float ObjectPosX[4];
    float ObjectPosY[4];
    float ObjectPosZ[4];
};

class CEnginePrimitiveManager { public:
    virtual void v0();
    void* SceneList;                                       // +0x04
    bool Enable;                                           // +0x08
};
class CEngineComponent { public:
    virtual void v0();
    void** GlobalPointerAddress;
    bool RenderEnabled;
    unsigned char KeepAlive;
};
class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineComponent {
public:
    struct CThemeChannel {
        CRGBFloatColour DiffuseColour;
        CRGBFloatColour AmbientColour;
        CRGBFloatColour BacklightColour;
        CRGBFloatColour ReflectionColour;
        CRGBFloatColour FogColour;
        float FogStartZ;
        float FogEndZ;
        float ShadowFactor;                                // +0x58
        float FadedShadowFactor;                           // +0x5C
        float FogTransform[4];
    };
    C3DVector SunVector;                                   // +0x18
    C3DVector MoonVector;                                  // +0x24
    C3DVector DiffuseVector;                               // +0x30
    CArray<CThemeChannel> ThemeChannels;                   // +0x3c
    CRGBFloatColour PrimitiveDiffuseColour;                // +0x48
    CRGBFloatColour PrimitiveAmbientColour;                // +0x58
    CRGBFloatColour PrimitiveBacklightColour;              // +0x68
    unsigned char _pad78[0xcc - 0x78];
    bool LocalLightsEnabled;                               // +0xcc
    bool ShadowedSpotLightsEnabled;                        // +0xcd
    bool FadedShadowDiffuseFactorEnabled;                  // +0xce
    CRGBFloatColour MistEffectColour;                      // +0xd0
    CEnginePrimitiveThemeChannelDesc CurrentThemeChannel;  // +0xe0
    unsigned long StateUpdateFlags;                        // +0xe4
    long SelfIlluminationAmbientPercentage;                // +0xe8
    unsigned char _padec[0x4690 - 0xec];
    CLightingSIMDVarBlock* SIMDVarBlock;                   // +0x4690
    long ShadowMode;                                       // +0x4694
    void UpdateGlobalLightingStates();
};

void CEngineLightingManager::UpdateGlobalLightingStates()
{
    float blend = CurrentThemeChannel.ChannelBlendFactor * (1.0f / 255.0f);
    if (StateUpdateFlags & 1) {
        if (CurrentThemeChannel.ChannelBlendFactor > 0) {
            PrimitiveDiffuseColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour * (1.0f - blend)
                                   + ThemeChannels[CurrentThemeChannel.ChannelIndex1].DiffuseColour * blend;
            PrimitiveBacklightColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour * (1.0f - blend)
                                     + ThemeChannels[CurrentThemeChannel.ChannelIndex1].BacklightColour * blend;
        } else {
            PrimitiveDiffuseColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].DiffuseColour;
            PrimitiveBacklightColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].BacklightColour;
        }
        if (FadedShadowDiffuseFactorEnabled) {
            float factor;
            if (CurrentThemeChannel.ChannelBlendFactor > 0)
                factor = (1.0f - blend) * ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor
                       + blend * ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor;
            else
                factor = ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
            PrimitiveDiffuseColour = PrimitiveDiffuseColour * factor;
        }
        CShaderDirectionalLight light(DiffuseVector, PrimitiveDiffuseColour, PrimitiveBacklightColour);
        g_pShaderRenderManager->InitialiseLight(0, light);
        g_pShaderRenderManager->SetLightGlobal(0, C4DVector(PrimitiveBacklightColour));
        StateUpdateFlags &= ~1;
        for (long i = 0; i < 4; ++i) {
            SIMDVarBlock->DiffuseColourR[i] = PrimitiveDiffuseColour.R;
            SIMDVarBlock->DiffuseColourG[i] = PrimitiveDiffuseColour.G;
            SIMDVarBlock->DiffuseColourB[i] = PrimitiveDiffuseColour.B;
        }
    }
    if (StateUpdateFlags & 2) {
        if (CurrentThemeChannel.ChannelBlendFactor > 0)
            PrimitiveAmbientColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour * (1.0f - blend)
                                   + ThemeChannels[CurrentThemeChannel.ChannelIndex1].AmbientColour * blend;
        else
            PrimitiveAmbientColour = ThemeChannels[CurrentThemeChannel.ChannelIndex0].AmbientColour;
        if (SelfIlluminationAmbientPercentage > 0) {
            float lift = SelfIlluminationAmbientPercentage * 0.01f;
            lift *= lift;
            float blue = PrimitiveAmbientColour.B;
            float green = PrimitiveAmbientColour.G;
            float red = PrimitiveAmbientColour.R;
            float r = (float)sqrt(red * red + lift);
            float g = (float)sqrt(green * green + lift);
            float b = (float)sqrt(blue * blue + lift);
            if (r > 1.0f)
                r = 1.0f;
            if (g > 1.0f)
                g = 1.0f;
            if (b > 1.0f)
                b = 1.0f;
            PrimitiveAmbientColour = CRGBFloatColour(r, g, b, 1.0f);
        }
        g_pShaderRenderManager->SetAmbientColour(PrimitiveAmbientColour);
        StateUpdateFlags &= ~2;
    }
    if (ShadowMode == 1 && (StateUpdateFlags & 8) && !FadedShadowDiffuseFactorEnabled) {
        float shadow;
        float faded;
        if (CurrentThemeChannel.ChannelBlendFactor > 0) {
            float inverse = 1.0f - blend;
            shadow = blend * ThemeChannels[CurrentThemeChannel.ChannelIndex1].ShadowFactor
                   + inverse * ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor;
            faded = blend * ThemeChannels[CurrentThemeChannel.ChannelIndex1].FadedShadowFactor
                  + inverse * ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
        } else {
            shadow = ThemeChannels[CurrentThemeChannel.ChannelIndex0].ShadowFactor;
            faded = ThemeChannels[CurrentThemeChannel.ChannelIndex0].FadedShadowFactor;
        }
        float unshadowed = 1.0f - shadow;
        g_pShaderRenderManager->SetShadowFadeFactors(unshadowed, 1.0f - faded - unshadowed, faded);
    }
}
