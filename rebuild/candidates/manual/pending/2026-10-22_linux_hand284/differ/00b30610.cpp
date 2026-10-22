// CEngineCamera::ClipBox(const CMatrix3x4& transform) const -- frustum-cull an oriented unit box: transform the eight corners (+-1 on each axis) by the matrix and return false when all eight lie on the outer side of any one of the four side planes
// Grade: DIFFER(710v710) -- VC7.1 reassociates the 24 corner sums to share partial sums (CSE); retail pairs the Y components as (x + translation) + (y + z) for corners 2-7 while every source form tried (13 association forms per component, explicit per-corner trees, out-param vs by-value transform) pairs them differently; the dot product loads one Y operand in the other order; loop and early-out are exact
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B30610, length 710 bytes (`ret 4`, ?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z). Constants -1.0f 0x0122DEE0, 0.0f 0x0122DEDC.
// Layout: Ego_r.pdb CEngineCamera FrustrumPlanes CPlane[4] {Normal, Distance} at +0x1c0; modelled locally.
// Leaf: ClipBox
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
};
struct CMatrix3x4 { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void TransformPoint(C3DVector& out, const C3DVector& v) const
    {
        out.X = v.X * E11 + v.Y * E21 + v.Z * E31 + E41;
        out.Y = v.X * E12 + v.Y * E22 + v.Z * E32 + E42;
        out.Z = v.X * E13 + v.Y * E23 + v.Z * E33 + E43;
    }
    C3DVector Transform(const C3DVector& v) const
    {
        return C3DVector(v.X * E11 + v.Y * E21 + v.Z * E31 + E41,
                         v.X * E12 + v.Y * E22 + v.Z * E32 + E42,
                         v.X * E13 + v.Y * E23 + v.Z * E33 + E43);
    }
};
inline C3DVector operator*(const C3DVector& v, const CMatrix3x4& m)
{
    return C3DVector(m.E41 + (v.X * m.E11 + v.Y * m.E21 + v.Z * m.E31),
                     m.E42 + (v.X * m.E12 + v.Y * m.E22 + v.Z * m.E32),
                     m.E43 + (v.X * m.E13 + v.Y * m.E23 + v.Z * m.E33));
}
struct CPlane { C3DVector Normal; float Distance; };
class CEngineCamera {
public:
    unsigned char _pad00[0x1c0];
    CPlane        FrustrumPlanes[4];                    // +0x1c0
    bool ClipBox(const CMatrix3x4& transform) const;
};
bool CEngineCamera::ClipBox(const CMatrix3x4& transform) const
{
    C3DVector points[8] = {
        C3DVector(-1.0f, -1.0f, -1.0f) * transform,
        C3DVector(1.0f, -1.0f, -1.0f) * transform,
        C3DVector(-1.0f, 1.0f, -1.0f) * transform,
        C3DVector(1.0f, 1.0f, -1.0f) * transform,
        C3DVector(-1.0f, -1.0f, 1.0f) * transform,
        C3DVector(1.0f, -1.0f, 1.0f) * transform,
        C3DVector(-1.0f, 1.0f, 1.0f) * transform,
        C3DVector(1.0f, 1.0f, 1.0f) * transform
    };
    for (unsigned long i = 0; i < 4; ++i) {
        bool outside = true;
        for (unsigned long j = 0; j < 8; ++j) {
            if (FrustrumPlanes[i].Normal.Dot(points[j]) - FrustrumPlanes[i].Distance < 0.0f) {
                outside = false;
                break;
            }
        }
        if (outside)
            return false;
    }
    return true;
}
