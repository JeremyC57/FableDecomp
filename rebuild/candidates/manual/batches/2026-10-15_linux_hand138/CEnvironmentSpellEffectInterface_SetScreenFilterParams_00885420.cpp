// CEnvironmentSpellEffectInterface::SetScreenFilterParams(float alpha, float saturation, float contrast, float brightness, const CRGBFloatColour& tint, bool interpolate) -- build a screen-filter parameter block and store it as the current parameters, and also as the previous frame's when not interpolating
// Retail: Fable.exe (TLC, VC7.1) @ 0x00885420, 116 bytes (+ int3 padding).
// Callees: none.
// Layout (Ego_r.pdb, matches retail): CScreenFilterParams { Alpha, Saturation, Contrast, Brightness, CRGBFloatColour Tint } is
//   32 bytes; CEnvironmentSpellEffectInterface keeps ScreenFilterParams at +0x64 and PrevFrameScreenFilterParams at +0xe4.
// Shape: the block is built in a 0x20-byte frame local (the tint copied dword by dword: a plain POD assignment) and copied
//   with `rep movsd` (8 dwords); the bool is read after the first copy.
struct CRGBFloatColour { float R, G, B, A; };

struct CScreenFilterParams {
    float           Alpha;                             // +0x00
    float           Saturation;                        // +0x04
    float           Contrast;                          // +0x08
    float           Brightness;                        // +0x0c
    CRGBFloatColour Tint;                              // +0x10
};

struct CEnvironment;

struct CEnvironmentSpellEffectInterface {
    CEnvironment*       Environment;                   // +0x00
    unsigned char       _pad4[0x64 - 0x4];             // radial blur, position and displacement params
    CScreenFilterParams ScreenFilterParams;            // +0x64
    unsigned char       _pad84[0xe4 - 0x84];           // previous-frame radial blur, position and displacement params
    CScreenFilterParams PrevFrameScreenFilterParams;   // +0xe4
    void SetScreenFilterParams(float alpha, float saturation, float contrast, float brightness,
                               const CRGBFloatColour& tint, bool interpolate);
};

void CEnvironmentSpellEffectInterface::SetScreenFilterParams(float alpha, float saturation, float contrast, float brightness,
                                                             const CRGBFloatColour& tint, bool interpolate)
{
    CScreenFilterParams params;
    params.Alpha = alpha;
    params.Saturation = saturation;
    params.Contrast = contrast;
    params.Brightness = brightness;
    params.Tint = tint;
    ScreenFilterParams = params;
    if (!interpolate)
        PrevFrameScreenFilterParams = params;
}
