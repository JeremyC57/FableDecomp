#include <cstdio>

// Model of CRadialBlurProcess::Process: every radial-blur parameter of the stack moves towards the entry's value,
// current = current * (1 - t) + entry * t; the track-position enums are left alone.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour() {}
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
    CRGBFloatColour operator*(float s) const { return CRGBFloatColour(R * s, G * s, B * s, A * s); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
struct CRadialBlurParams { CRGBFloatColour ThresholdColour, IntensityColour; float Length, Fade, Range, Twist; };
struct CRadialBlurPositionParams { C3DVector AreaCenter, BlurCenter; int AreaTrackPos, BlurTrackPos; };
struct CStack { CRadialBlurParams CurrentRadialBlurParams; CRadialBlurPositionParams CurrentRadialBlurPosParams; };
struct CRadialBlurEntry { CRadialBlurParams param; CRadialBlurPositionParams posparam; };

static void Process(CStack* Stack, float t, const CRadialBlurEntry* blur)
{
    float inverse = 1.0f - t;
    CRadialBlurParams& p = Stack->CurrentRadialBlurParams;
    p.ThresholdColour = p.ThresholdColour * inverse + blur->param.ThresholdColour * t;
    p.IntensityColour = p.IntensityColour * inverse + blur->param.IntensityColour * t;
    p.Length = p.Length * inverse + blur->param.Length * t;
    p.Fade = p.Fade * inverse + blur->param.Fade * t;
    p.Range = p.Range * inverse + blur->param.Range * t;
    p.Twist = p.Twist * inverse + blur->param.Twist * t;
    CRadialBlurPositionParams& q = Stack->CurrentRadialBlurPosParams;
    q.AreaCenter = q.AreaCenter * inverse + blur->posparam.AreaCenter * t;
    q.BlurCenter = q.BlurCenter * inverse + blur->posparam.BlurCenter * t;
}

int main()
{
    CStack s; CRadialBlurEntry e;
    s.CurrentRadialBlurParams.ThresholdColour = CRGBFloatColour(0, 0, 0, 0);
    s.CurrentRadialBlurParams.IntensityColour = CRGBFloatColour(1, 1, 1, 1);
    s.CurrentRadialBlurParams.Length = 2; s.CurrentRadialBlurParams.Fade = 4; s.CurrentRadialBlurParams.Range = 8; s.CurrentRadialBlurParams.Twist = 0;
    s.CurrentRadialBlurPosParams.AreaCenter = C3DVector(0, 0, 0); s.CurrentRadialBlurPosParams.BlurCenter = C3DVector(4, 4, 4);
    s.CurrentRadialBlurPosParams.AreaTrackPos = 3; s.CurrentRadialBlurPosParams.BlurTrackPos = 5;
    e.param.ThresholdColour = CRGBFloatColour(4, 8, 2, 1); e.param.IntensityColour = CRGBFloatColour(3, 5, 1, 1);
    e.param.Length = 6; e.param.Fade = 0; e.param.Range = 8; e.param.Twist = 2;
    e.posparam.AreaCenter = C3DVector(2, -2, 4); e.posparam.BlurCenter = C3DVector(0, 8, 4);
    e.posparam.AreaTrackPos = 0; e.posparam.BlurTrackPos = 0;
    Process(&s, 0.25f, &e);
    const CRadialBlurParams& p = s.CurrentRadialBlurParams; const CRadialBlurPositionParams& q = s.CurrentRadialBlurPosParams;
    if (p.ThresholdColour.R != 1.0f || p.ThresholdColour.G != 2.0f || p.ThresholdColour.B != 0.5f || p.ThresholdColour.A != 0.25f
        || p.IntensityColour.R != 1.5f || p.IntensityColour.G != 2.0f || p.IntensityColour.B != 1.0f
        || p.Length != 3.0f || p.Fade != 3.0f || p.Range != 8.0f || p.Twist != 0.5f
        || q.AreaCenter.X != 0.5f || q.AreaCenter.Y != -0.5f || q.AreaCenter.Z != 1.0f
        || q.BlurCenter.X != 3.0f || q.BlurCenter.Y != 5.0f || q.BlurCenter.Z != 4.0f
        || q.AreaTrackPos != 3 || q.BlurTrackPos != 5)
        { std::printf("00880ae0_TEST FAIL\n"); return 1; }
    std::printf("00880ae0_TEST PASS\n");
    return 0;
}