// NCameraModes::CPhysicsForceModelGotoOutsideSimple::DoVelocityDamping() -- the base force model's velocity damping (CPhysicsForceModelBase::DoVelocityDamping @ 0x008FA7E0) scaled by 6.0f
// Leaf: DoVelocityDamping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCC10, length 56 bytes.
// Identity: vtable 0x01275950 slot 7 (rtti-vtables.tsv); Ego_r.pdb has
// `?DoVelocityDamping@CPhysicsForceModelGotoOutsideSimple@NCameraModes@@UAE?AVC3DVector@@XZ` right
// after DoAttractToPoint. The catalog label "DoAttractToPoint" is propagated from 0x008FCBD0.
// Callees: the qualified (non-virtual) base call NCameraModes::CPhysicsForceModelBase::DoVelocityDamping
// @ 0x008FA7E0 (Ego_r 0x0091F9C0; it copies the controlled object's vector at +0x0C into the hidden
// return slot), the float pool constant 6.0f @ 0x0125A2A0.
// Shape: as DoAttractToPoint: the inline `C3DVector * float` is built through the value constructor
// straight into the hidden return slot, arguments evaluated right to left (z, y, x multiplied
// first, stored x, y, z).
class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator*(float s) const { return C3DVector(x * s, y * s, z * s); }
};

namespace NCameraModes {
class CPhysicsForceModelBase { public:                    // vtable 0x0127592C
    virtual C3DVector DoVelocityDamping();         // 0x008FA7E0
    virtual C3DVector DoAttractToPoint();          // 0x008FA800
};
class CPhysicsForceModelGotoOutsideSimple : public CPhysicsForceModelBase { public:   // vtable 0x01275950
    virtual C3DVector DoVelocityDamping();
};
}

C3DVector NCameraModes::CPhysicsForceModelGotoOutsideSimple::DoVelocityDamping()
{
    return CPhysicsForceModelBase::DoVelocityDamping() * 6.0f;
}