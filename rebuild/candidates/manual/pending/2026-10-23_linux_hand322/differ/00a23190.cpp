// CRightHandedSet::Interpolate(const CRightHandedSet& to, float t) -- when both the forward and up axes are within ~41 degrees of the target's (dot > 0.75) linearly blend each axis and re-orthonormalise; otherwise rotate through the quaternion between the two frames; return the blended frame
// Grade: DIFFER(366v1508) -- skeleton draft, parked on sight: x87 frame interpolation (dot gates, lerp + quaternion
//   slerp) with a 0xbc-byte spill frame; only the 0.75 gates and the linear blend are modelled.
// Leaf: Interpolate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A23190, 1506 bytes, `ret 0xc` (hidden return + 2 args), 0xbc-byte frame.
// Skeleton draft: only the 0.75 dot gates and the linear blend are modelled; the quaternion path is a stand-in.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& o) const { return X * o.X + Y * o.Y + Z * o.Z; }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
struct CRightHandedSet {
    C3DVector Forward;                              // +0x00
    C3DVector Up;                                   // +0x0c
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& forward, const C3DVector& up) : Forward(forward), Up(up) {}
    CRightHandedSet Interpolate(const CRightHandedSet& to, float t) const;
};
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& to, float t) const
{
    if (to.Forward.Dot(Forward) > 0.75f && Up.Dot(to.Up) > 0.75f) {
        C3DVector forward = Forward + (to.Forward - Forward) * t;
        C3DVector up = Up + (to.Up - Up) * t;
        return CRightHandedSet(forward, up);
    }
    return CRightHandedSet(t < 0.5f ? Forward : to.Forward, t < 0.5f ? Up : to.Up);
}
