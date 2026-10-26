// NScreenEffect::CScreenEffectPriorityStack::CRadialBlurProcess::Process(float t, const CStackEntry* entry) -- blend the stack's current radial-blur parameters and position towards a radial-blur entry: current = current * (1 - t) + entry * t
// Leaf: Process@CRadialBlurProcess
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880AE0, length 611 bytes (virtual, ret 8); PDB
// ?Process@CRadialBlurProcess@CScreenEffectPriorityStack@NScreenEffect@@UAEXMPBUCStackEntry@23@@Z. No callees.
// Layout: the retail CScreenEffectPriorityStack / CRadialBlurEntry model of the landed SetBlurTrackPos 0x008811D0:
// CurrentRadialBlurParams +0x1c (ThresholdColour, IntensityColour, Length, Fade, Range, Twist), CurrentRadialBlurPosParams
// +0x4c (AreaCenter, BlurCenter); the entry's param +0x10 and posparam +0x40. The process holds the stack at +4.
// Shape: `1 - t` once; each colour/vector member is `cur = cur * (1 - t) + entry * t` through by-value operator* and
// operator+ temporaries (VC evaluates the right operand first, so entry * t is formed first, and the sum temporary is
// copied with integer moves); the four scalars are blended in place. The stack pointer is re-read per member.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() {}
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
struct CRadialBlurParams {                     // 48 bytes
    CRGBFloatColour ThresholdColour;           // +0x00
    CRGBFloatColour IntensityColour;           // +0x10
    float Length;                              // +0x20
    float Fade;                                // +0x24
    float Range;                               // +0x28
    float Twist;                               // +0x2c
};
enum ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION { ENVIRONMENT_RADIAL_BLUR_TRACK_NONE = 0 };
struct CRadialBlurPositionParams {             // 32 bytes
    C3DVector AreaCenter;                      // +0x00
    C3DVector BlurCenter;                      // +0x0c
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION AreaTrackPos;   // +0x18
    ENVIRONMENT_RADIAL_BLUR_TRACK_POSITION BlurTrackPos;   // +0x1c
};

namespace NScreenEffect {

class CScreenEffectPriorityStack { public:
    struct CStackEntry {
        virtual void v00();
        long curdurationcount;                 // +0x04
        long durationcount;                    // +0x08
        bool fadingout;                        // +0x0c
    };
    struct CRadialBlurEntry : CStackEntry {
        CRadialBlurParams         param;       // +0x10
        CRadialBlurPositionParams posparam;    // +0x40
    };
    struct CRadialBlurProcess {
        virtual void Process(float t, const CStackEntry* entry);
        CScreenEffectPriorityStack* Stack;     // +0x04
    };
    void*                     __vftable;                   // +0x00
    unsigned char             RadialParamStack[0x0c];      // +0x04 three 4-byte STLport lists
    unsigned char             RadialBlurKeys[0x0c];        // +0x10 STLport set<CStackEntry*>
    CRadialBlurParams         CurrentRadialBlurParams;     // +0x1c
    CRadialBlurPositionParams CurrentRadialBlurPosParams;  // +0x4c
};

void CScreenEffectPriorityStack::CRadialBlurProcess::Process(float t, const CStackEntry* entry)
{
    const CRadialBlurEntry* blur = static_cast<const CRadialBlurEntry*>(entry);
    float inverse = 1.0f - t;
    Stack->CurrentRadialBlurParams.ThresholdColour =
        Stack->CurrentRadialBlurParams.ThresholdColour * inverse + blur->param.ThresholdColour * t;
    Stack->CurrentRadialBlurParams.IntensityColour =
        Stack->CurrentRadialBlurParams.IntensityColour * inverse + blur->param.IntensityColour * t;
    Stack->CurrentRadialBlurParams.Length = Stack->CurrentRadialBlurParams.Length * inverse + blur->param.Length * t;
    Stack->CurrentRadialBlurParams.Fade = Stack->CurrentRadialBlurParams.Fade * inverse + blur->param.Fade * t;
    Stack->CurrentRadialBlurParams.Range = Stack->CurrentRadialBlurParams.Range * inverse + blur->param.Range * t;
    Stack->CurrentRadialBlurParams.Twist = Stack->CurrentRadialBlurParams.Twist * inverse + blur->param.Twist * t;
    Stack->CurrentRadialBlurPosParams.AreaCenter =
        Stack->CurrentRadialBlurPosParams.AreaCenter * inverse + blur->posparam.AreaCenter * t;
    Stack->CurrentRadialBlurPosParams.BlurCenter =
        Stack->CurrentRadialBlurPosParams.BlurCenter * inverse + blur->posparam.BlurCenter * t;
}

}