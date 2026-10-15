// NScreenEffect::CScreenEffectPriorityStack::Reset() -- put the current radial blur, displacement colour and screen filter parameters back to their neutral values
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880A40, length 145 bytes (+15 int3 padding).
// Called at the end of the stack's constructor (0x008814A9) and by its rebuild (tail jump at 0x00881365).
// Layout (PDB member order, retail offsets: the three-element list arrays are 12 bytes and the key sets 12 bytes in
// retail): CurrentRadialBlurParams +0x1C (CRadialBlurParams, 48 bytes), CurrentRadialBlurPosParams +0x4C
// (CRadialBlurPositionParams: two C3DVectors, two track-position enums), CurrentDisplacementParam +0x84
// (CRGBFloatColour), CurrentScreenFilterParam +0xAC (CScreenFilterParams: Alpha, Saturation, Contrast, Brightness,
// CRGBFloatColour Tint).
// Shape: one zero register and one 1.0f register; float constants store as integer moves; the high-to-low
// Brightness/Contrast/Alpha stores are one chained assignment (evaluated right to left); the two track-position
// enums go through an inline two-argument setter (written as two statements VC sinks them below the zero stores).
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };

enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { ERBTP_NONE = 0, ERBTP_CAMERA = 1 };

struct CRadialBlurParams {
    CRGBFloatColour ThresholdColour;    // +0x00
    CRGBFloatColour IntensityColour;    // +0x10
    float Length;                       // +0x20
    float Fade;                         // +0x24
    float Range;                        // +0x28
    float Twist;                        // +0x2C
};

struct CRadialBlurPositionParams {
    C3DVector AreaCenter;               // +0x00
    C3DVector BlurCenter;               // +0x0C
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;   // +0x18
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;   // +0x1C
    void SetTrackPositions(ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION area, ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION blur)
    {
        AreaTrackPos = area;
        BlurTrackPos = blur;
    }
};

struct CScreenFilterParams {
    float Alpha;                        // +0x00
    float Saturation;                   // +0x04
    float Contrast;                     // +0x08
    float Brightness;                   // +0x0C
    CRGBFloatColour Tint;               // +0x10
};

namespace NScreenEffect {

class CScreenEffectPriorityStack {
public:
    void Reset();
private:
    void*                     __vftable;                     // +0x00
    unsigned char             RadialParamStack[0x0C];        // +0x04 list<CRadialBlurEntry>[3]
    unsigned char             RadialBlurKeys[0x0C];          // +0x10 set<CStackEntry*>
    CRadialBlurParams         CurrentRadialBlurParams;       // +0x1C
    CRadialBlurPositionParams CurrentRadialBlurPosParams;    // +0x4C
    unsigned char             DisplacementParamStack[0x0C];  // +0x6C
    unsigned char             DisplacementKeys[0x0C];        // +0x78
    CRGBFloatColour           CurrentDisplacementParam;      // +0x84
    unsigned char             ScreenFilterParamStack[0x0C];  // +0x94
    unsigned char             ScreenFilterKeys[0x0C];        // +0xA0
    CScreenFilterParams       CurrentScreenFilterParam;      // +0xAC
};

void CScreenEffectPriorityStack::Reset()
{
    CurrentRadialBlurParams.ThresholdColour.R = 0.0f;
    CurrentRadialBlurParams.ThresholdColour.G = 0.0f;
    CurrentRadialBlurParams.ThresholdColour.B = 0.0f;
    CurrentRadialBlurParams.ThresholdColour.A = 0.0f;
    CurrentRadialBlurParams.IntensityColour.R = 0.0f;
    CurrentRadialBlurParams.IntensityColour.G = 0.0f;
    CurrentRadialBlurParams.IntensityColour.B = 0.0f;
    CurrentRadialBlurParams.IntensityColour.A = 0.0f;
    CurrentRadialBlurParams.Length = 0.0f;
    CurrentRadialBlurParams.Fade = 0.0f;
    CurrentRadialBlurParams.Range = 0.0f;
    CurrentRadialBlurParams.Twist = 0.0f;
    CurrentRadialBlurPosParams.AreaCenter.X = 0.0f;
    CurrentRadialBlurPosParams.AreaCenter.Y = 0.0f;
    CurrentRadialBlurPosParams.AreaCenter.Z = 0.0f;
    CurrentRadialBlurPosParams.BlurCenter.X = 0.0f;
    CurrentRadialBlurPosParams.BlurCenter.Y = 0.0f;
    CurrentRadialBlurPosParams.BlurCenter.Z = 0.0f;
    CurrentRadialBlurPosParams.SetTrackPositions(ERBTP_CAMERA, ERBTP_CAMERA);
    CurrentDisplacementParam.R = 0.0f;
    CurrentDisplacementParam.G = 0.0f;
    CurrentDisplacementParam.B = 0.0f;
    CurrentDisplacementParam.A = 0.0f;
    CurrentScreenFilterParam.Alpha = CurrentScreenFilterParam.Contrast = CurrentScreenFilterParam.Brightness = 0.0f;
    CurrentScreenFilterParam.Saturation = 1.0f;
    CurrentScreenFilterParam.Tint.R = 1.0f;
    CurrentScreenFilterParam.Tint.G = 1.0f;
    CurrentScreenFilterParam.Tint.B = 1.0f;
    CurrentScreenFilterParam.Tint.A = 1.0f;
}

} // namespace NScreenEffect