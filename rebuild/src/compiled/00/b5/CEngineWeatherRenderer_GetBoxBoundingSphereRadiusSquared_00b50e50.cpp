// CEngineWeatherRenderer::GetBoxBoundingSphereRadiusSquared() const -- squared radius of the sphere bounding a weather box: the larger half-extents of the (X, Y) and (Y, Z) pairs, squared and summed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B50E50, length 114 bytes. No callees. Float pool 0.5f @ 0x0122F59C.
// Layout: the weather box size (C3DVector) at +0x1d4, inside the PDB's opaque `Settings` block (CEngineWeatherSettings,
// +0x114..+0x66c in engine/CEngineWeatherRenderer.h), so `this` is modelled locally.
// Shape: three float locals: Y and Z are copied through eax/ecx into frame words, X is loaded straight onto the FPU;
// the first maximum is an inline by-value helper whose if/else result local becomes the highest frame word (the X arm
// `fstp`s, the Y arm copies through edx; a plain if/else local in the body takes [esp] instead); the second maximum is a
// ternary kept on the FPU; `a *= 0.5f` goes through the frame before `b *= 0.5f`; `a*a + b*b` squares the FP top (b) first.
class C3DVector { public:
    float X, Y, Z;
};

class CEngineWeatherRenderer { public:
    unsigned char _pad[0x1d4];
    C3DVector     m_boxSize;                       // +0x1d4
    float GetBoxBoundingSphereRadiusSquared() const;
};

static float GetMax(float a, float b)
{
    float r;
    if (a > b)
        r = a;
    else
        r = b;
    return r;
}

float CEngineWeatherRenderer::GetBoxBoundingSphereRadiusSquared() const
{
    float x = m_boxSize.X;
    float y = m_boxSize.Y;
    float z = m_boxSize.Z;
    float a = GetMax(x, y);
    float b = y > z ? y : z;
    a *= 0.5f;
    b *= 0.5f;
    return a * a + b * b;
}