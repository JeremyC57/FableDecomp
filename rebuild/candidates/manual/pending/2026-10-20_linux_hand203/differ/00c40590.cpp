// CRightHandedSet::NormalizeOrientationFrame() -- publish the frame's dirty bits, and when the orientation changed renormalise front/top and rebuild the frame
// Leaf: NormalizeOrientationFrame
// Grade: DIFFER(192v192) -- x87 operand order only: the dot product's first two terms come out Y,Z where retail has Z,Y, and four of
// the six cross-product multiplies load the member operand first where retail loads the front vector first; swapping the
// source operands, a pointer/inline-member form and an inline Dot/SetCross pair did not move them (canonicalised by the lane).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40590, length 192 bytes (`ret`).
// Callees: C3DVector::Normalise() @ 0x00C402E0 (returns the old length, dropped: `fstp st(0)`), C3DVector::SetCrossProduct
// (const C3DVector&, const C3DVector&) @ 0x00C40370 (landed; manifest label "CRightHandedSet::GetRight" is propagated).
// Globals: 0x0143BC24 the accumulated dirty mask of the audio-library 3D state; float pool 0x0125A15C = 0.05f,
// 0x012B7B78 = -0.05f.
// Layout: NOT a CRightHandedSet (Ego_r.pdb: 0x18 bytes, Up/Forward): `this` owns three vectors at +0x24/+0x30/+0x3c and a
// change mask at +0x48 (bit 0x20000 = orientation changed), an object of the 0x00C3xxxx-0x00C4xxxx audio library whose
// class is not identified; the vectors are named after their role (front, top, right = front x top).
// Shape: the mask is OR-ed into the global first (the member is re-read for the bit test after the global store);
// both vectors are renormalised, and if front.top is outside +-0.05 the top vector is rebuilt in place as front x
// right (inline, component by component; in this TU `fld [a] ; fmul [b]` is the source product `b * a`, and the
// dot product's first two terms are emitted second-first), else right = SetCrossProduct(front, top). Both arms clear the mask.
struct C3DVector {
    float X, Y, Z;
    float Normalise();                                           // 0x00C402E0
    void SetCrossProduct(const C3DVector& a, const C3DVector& b);   // 0x00C40370
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    void SetCross(const C3DVector& a, const C3DVector& b)         // inline twin: this = a x b
    {
        X = b.Z * a.Y - b.Y * a.Z;
        Y = a.Z * b.X - a.X * b.Z;
        Z = b.Y * a.X - b.X * a.Y;
    }
};

extern unsigned long g_audio3DDirtyMask;                        // 0x0143BC24

struct COrientationFrame_00c40590 {
    unsigned char _pad00[0x24];
    C3DVector     m_front;                                      // +0x24
    C3DVector     m_top;                                        // +0x30
    C3DVector     m_right;                                      // +0x3c
    unsigned long m_changed;                                    // +0x48
    void NormalizeOrientationFrame();
};

void COrientationFrame_00c40590::NormalizeOrientationFrame()
{
    g_audio3DDirtyMask |= m_changed & 0x38000;
    if (m_changed & 0x20000) {
        C3DVector* front = &m_front;
        front->Normalise();
        C3DVector* top = &m_top;
        top->Normalise();
        float dot = front->Dot(*top);
        if (dot > 0.05f || dot < -0.05f)
            top->SetCross(*front, m_right);
        else
            m_right.SetCrossProduct(*front, *top);
    }
    m_changed = 0;
}
