// CEngineInternalPrimitiveLine::GetWorldPosition(C3DVector& position) -- the line's midpoint in world space, start + (end - start) * 0.5; always true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC7D40, length 110 bytes (`ret 4`). Constant 0.5f @ 0x0122F59C.
// Layout: no PDB layout for the line primitive; its start and end C3DVectors sit at +0x48 / +0x54 (after the 0x48-byte
// CEngineInternalPrimitiveBase), modelled locally.
// Shape: the inline C3DVector operators each return a constructed temporary: the difference (Z parked in the frame), the scaled
// half (X parked), then the sum built in the frame and copied into the argument through edx/ecx (implicit operator=).
// `(end - start) * 0.5f + start` gives retail's `fld half ; fadd start` order.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
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

bool CEngineInternalPrimitiveLine::GetWorldPosition(C3DVector& position)
{
    position = (m_end - m_start) * 0.5f + m_start;
    return true;
}