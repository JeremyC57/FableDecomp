// CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere) -- sphere around the line: centre = (start + end) * 0.5, radius = |centre - start|; always true
// Grade: DIFFER(141v154) -- (1) X of the sum: retail `fld start.X ; fadd end.X` (Y/Z end first), ours end first for all three; (2) the radius: retail re-reads the centre from the argument with Z copied through edx into a frame word and spills dX to [esp] (`fld [esp] ; fmul [esp]`), ours keeps all three differences on the x87 stack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7DB0, length 154 bytes (`ret 4`). Constant 0.5f @ 0x0122F59C; fsqrt inline (sqrtf, <math.h>).
// Layout: start +0x48, end +0x54 (see 0x00BC7D40, same model).
// Tried: (a) member operators + Length() [best, this file]; (b) `(end + start)` + a named copy of the centre (copies forwarded from
// registers, 161 bytes); (c) a by-value first operand for operator- (161 bytes); (d) a named offset local (same as a).
// Next: the centre copy looks like a by-value C3DVector taken from `sphere.Center` after its stores -- try a non-inline-able
// accessor or a `const C3DBoundingSphere&` re-read; the X add order may be a QFE x87 scheduling artefact.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float Length() const { return sqrtf(X * X + Y * Y + Z * Z); }
};

struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
};

class CEngineInternalPrimitiveLine {
public:
    virtual void v000();
    unsigned char _pad04[0x44];
    C3DVector     m_start;                         // +0x48
    C3DVector     m_end;                           // +0x54
    virtual bool GetWorldPosition(C3DVector& position);
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);
};

bool CEngineInternalPrimitiveLine::GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere)
{
    sphere.Center = (m_start + m_end) * 0.5f;
    sphere.Radius = (sphere.Center - m_start).Length();
    return true;
}
