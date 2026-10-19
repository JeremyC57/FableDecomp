// CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface(CEnvironment* environment) -- default all current
// Grade: DIFFER(ours v retail) -- 183v183 scheduling only: prev-frame screen-filter zero stores after the tint, push esi earlier, track-mode zero stores after the second lea
// and previous-frame screen-effect parameter blocks
// Leaf: ??0CEnvironmentSpellEffectInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008852E0, length 192 bytes, `ret 4`.
// Callees: none (every parameter-block constructor is inlined).
// Layout (PDB, 0x104 bytes, retail agrees): Environment +0, RadialBlurParams +4 (0x30), RadialBlurPositionParams
// +0x34 (0x20), DisplacementParams +0x54 (0x10), ScreenFilterParams +0x64 (0x20), and the PrevFrame copies at
// +0x84/+0xB4/+0xD4/+0xE4.
// Shape: the parameter structs have inline constructors (radial blur: all zero; position: track modes zero, the
// two C3DVectors left alone; screen filter: SetToDefault = alpha 0, saturation 1, contrast 0, brightness 0, white
// tint); the displacement colour is left alone by its constructor and zeroed in the body; the body copies the current
// radial blur blocks into the previous-frame blocks (`rep movsd`).

class CEnvironment;

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
};

class CRGBFloatColour {
public:
    float R, G, B, A;
    CRGBFloatColour() {}
    void Set(float r, float g, float b, float a) { R = r; G = g; B = b; A = a; }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { RADIAL_BLUR_TRACK_NONE = 0 };

class CEnvironmentSpellEffectInterface {
public:
    struct CRadialBlurParams {
        CRGBFloatColour ThresholdColour;
        CRGBFloatColour IntensityColour;
        float Length, Fade, Range, Twist;
        CRadialBlurParams() {}
    };
    struct CRadialBlurPositionParams {
        C3DVector AreaCenter;
        C3DVector BlurCenter;
        ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;
        ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;
        CRadialBlurPositionParams() {}
    };
    struct CDisplacementParams {
        CRGBFloatColour MonochromeEffectColour;
        CDisplacementParams() {}
    };
    struct CScreenFilterParams {
        float Alpha, Saturation, Contrast, Brightness;
        CRGBFloatColour Tint;
        CScreenFilterParams() { SetToDefault(); }
        void SetToDefault()
        {
            Alpha = Contrast = Brightness = 0.0f;
            Saturation = 1.0f;
            Tint.R = 1.0f;
            Tint.G = 1.0f;
            Tint.B = 1.0f;
            Tint.A = 1.0f;
        }
    };

    CEnvironment* Environment;
    CRadialBlurParams RadialBlurParams;
    CRadialBlurPositionParams RadialBlurPositionParams;
    CDisplacementParams DisplacementParams;
    CScreenFilterParams ScreenFilterParams;
    CRadialBlurParams PrevFrameRadialBlurParams;
    CRadialBlurPositionParams PrevFrameRadialBlurPositionParams;
    CDisplacementParams PrevFrameDisplacementParams;
    CScreenFilterParams PrevFrameScreenFilterParams;

    CEnvironmentSpellEffectInterface(CEnvironment* environment);
};

CEnvironmentSpellEffectInterface::CEnvironmentSpellEffectInterface(CEnvironment* environment)
{
    Environment = environment;
    RadialBlurParams.Length = RadialBlurParams.Fade = 0.0f;
    RadialBlurParams.Range = RadialBlurParams.Twist = 0.0f;
    RadialBlurParams.ThresholdColour.Set(0.0f, 0.0f, 0.0f, 0.0f);
    RadialBlurParams.IntensityColour.Set(0.0f, 0.0f, 0.0f, 0.0f);
    RadialBlurPositionParams.AreaTrackPos = RADIAL_BLUR_TRACK_NONE;
    RadialBlurPositionParams.BlurTrackPos = RADIAL_BLUR_TRACK_NONE;
    PrevFrameRadialBlurParams = RadialBlurParams;
    PrevFrameRadialBlurPositionParams = RadialBlurPositionParams;
    DisplacementParams.MonochromeEffectColour.R = 0.0f;
    DisplacementParams.MonochromeEffectColour.G = 0.0f;
    DisplacementParams.MonochromeEffectColour.B = 0.0f;
    DisplacementParams.MonochromeEffectColour.A = 0.0f;
}
