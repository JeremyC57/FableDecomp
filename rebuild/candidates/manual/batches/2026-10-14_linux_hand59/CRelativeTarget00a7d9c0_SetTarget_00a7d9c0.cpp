// CRelativeTarget_00a7d9c0::SetTarget(const C3DVector* position) -- hand the target's position relative to this object's origin (+4, +8; the Z passed through unchanged; the X/Y delta computed through an inline 2D subtraction returning a named local by its copy constructor, so both differences sit on the FPU stack before the stores) to the owner at +0 (virtual slot 4 with (&vector, 12, 0))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A7D9C0, length 57 bytes.
// Leaf: SetTarget
// The chunk label (CMurderTwistLeader::Init) is propagated; named by address. Shape: a 12-byte frame for the vector (X and Y through the FPU, Z through an integer register), `ret 4`.
struct C2DVector { float X, Y; C2DVector() { } C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { } };
static __forceinline C2DVector Sub(const C2DVector& a, const C2DVector& b) { C2DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; return r; }
struct C3DVector { float X, Y, Z; };
struct ITargetOwner_00a7d9c0 {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003();
    virtual void SetRelativeTarget(const C3DVector* position, long size, long flags);   // slot 4 (+0x10)
};

struct CRelativeTarget_00a7d9c0 {
    ITargetOwner_00a7d9c0* m_pOwner;               // +0x00
    C2DVector              m_origin;               // +0x04
    void SetTarget(const C3DVector* position);
};

void CRelativeTarget_00a7d9c0::SetTarget(const C3DVector* position)
{
    C2DVector delta = Sub(*(const C2DVector*)position, m_origin);
    C3DVector relative;
    relative.X = delta.X;
    relative.Y = delta.Y;
    relative.Z = position->Z;
    m_pOwner->SetRelativeTarget(&relative, 12, 0);
}
