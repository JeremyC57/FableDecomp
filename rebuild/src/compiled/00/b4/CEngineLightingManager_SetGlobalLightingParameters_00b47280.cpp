// CEngineLightingManager::SetGlobalLightingParameters(const C3DVector& diffuseVector, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight) -- stores the global light direction (normalised) and the first theme channel's ambient / diffuse / backlight colours, flagging both state groups dirty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47280, length 143 bytes (144 with padding).
// Callees: C3DVector::Normalise @ 0x00A14440 (in place on the member; the manifest's "GetScaled" label is propagated).
// Shape: StateUpdateFlags |= 3 first (the theme-channel array pointer is loaded above it), then the three 16-byte colours
// are copied dword by dword through one register into channel 0 (+0x10 ambient, +0x00 diffuse, +0x20 backlight; the
// array pointer re-read for each), then the 12-byte direction into DiffuseVector (+0x30) which is normalised in place.
// Layout: PDB CEngineLightingManager has a 16-byte CArray<CThemeChannel> at +0x3C and StateUpdateFlags at +0xF0; the
// retail CArray is 12 bytes (its first word is the element pointer), so the flags sit at +0xE4 -- modelled locally with
// the PDB member names (CThemeChannel is the PDB's 0x70-byte record).
struct C3DVector {
    float X, Y, Z;
    C3DVector& Normalise();                        // 0x00A14440
};

struct CRGBFloatColour {
    float R, G, B, A;
};

struct CThemeChannel {                             // CEngineLightingManager::CThemeChannel, 0x70 bytes
    CRGBFloatColour DiffuseColour;                 // +0x00
    CRGBFloatColour AmbientColour;                 // +0x10
    CRGBFloatColour BacklightColour;               // +0x20
    CRGBFloatColour ReflectionColour;              // +0x30
    CRGBFloatColour FogColour;                     // +0x40
    float           FogStartZ, FogEndZ, ShadowFactor, FadedShadowFactor;   // +0x50
    float           FogTransform[4];               // +0x60
};

struct CEngineLightingManager {
    unsigned char  _pad00[0x18];
    C3DVector      SunVector;                      // +0x18
    C3DVector      MoonVector;                     // +0x24
    C3DVector      DiffuseVector;                  // +0x30
    CThemeChannel* ThemeChannels;                  // +0x3C (retail CArray: element pointer first)
    unsigned char  _pad40[0xe4 - 0x40];
    unsigned long  StateUpdateFlags;               // +0xE4
    void SetGlobalLightingParameters(const C3DVector& diffuseVector, const CRGBFloatColour& ambient,
                                     const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight);
};

void CEngineLightingManager::SetGlobalLightingParameters(const C3DVector& diffuseVector, const CRGBFloatColour& ambient,
                                                         const CRGBFloatColour& diffuse, const CRGBFloatColour& backlight)
{
    StateUpdateFlags |= 3;
    ThemeChannels[0].AmbientColour = ambient;
    ThemeChannels[0].DiffuseColour = diffuse;
    ThemeChannels[0].BacklightColour = backlight;
    DiffuseVector = diffuseVector;
    DiffuseVector.Normalise();
}