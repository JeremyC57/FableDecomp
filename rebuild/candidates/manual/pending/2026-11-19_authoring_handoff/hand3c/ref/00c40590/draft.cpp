// CRightHandedSet::NormalizeOrientationFrame() -- publish the frame's dirty bits, and when the orientation changed renormalise front/top and rebuild the frame
// Leaf: NormalizeOrientationFrame
// Grade: DIFFER(192v192) (wave-2 sec6, IMPROVED 8 -> 6 diff lines): SetCross in retail's source operand order and the
//   members passed directly (m_front/m_top/m_right, no front/top pointer locals) fix the first cross product; three
//   products still load b (m_right) first where retail loads a (front) first; flipping their spelled order changes
//   nothing (VC7.1 canonical order). Next: a separate inline cross helper taking the operands by value or a C3DVector
//   temporary.
// inside the inline Dot; left: four of the six inline cross-product multiplies load the `this`-relative right vector
// first where retail loads the front pointer local first (retail keeps the source order per product, f,f | r,f | f,r;
// the lane ranks `this` members above pointer locals whatever the spelling, and ranks by spelling only when both operands
// are `this` members -- the operand-order lane gap).
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
    float Dot(const C3DVector& v) const { float s = Z * v.Z; s += Y * v.Y; s += X * v.X; return s; }
    void SetCross(const C3DVector& a, const C3DVector& b)         // inline twin: this = a x b
    {
        X = a.Y * b.Z - a.Z * b.Y;
        Y = b.X * a.Z - a.X * b.Z;
        Z = a.X * b.Y - b.X * a.Y;
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
        m_front.Normalise();
        m_top.Normalise();
        float dot = m_front.Dot(m_top);
        if (dot > 0.05f || dot < -0.05f)
            m_top.SetCross(m_front, m_right);
        else
            m_right.SetCrossProduct(m_front, m_top);
    }
    m_changed = 0;
}
