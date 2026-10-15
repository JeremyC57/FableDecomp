// NCameraModes::CPhysicsForceModelGotoOutsideSimple::DoAttractToPoint() -- the base force model's attraction (CPhysicsForceModelBase::DoAttractToPoint @ 0x008FA800) scaled by 3.0f
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCBD0, length 56 bytes (+ int3 padding).
// Identity: vtable 0x01275950 slot 6 (rtti-vtables.tsv); Ego_r.pdb has
// `?DoAttractToPoint@CPhysicsForceModelGotoOutsideSimple@NCameraModes@@UAE?AVC3DVector@@XZ` 0x40 bytes
// before the class's DoVelocityDamping, as here (0x008FCC10).
// Callees: the qualified (non-virtual) base call NCameraModes::CPhysicsForceModelBase::DoAttractToPoint
// @ 0x008FA800 (Ego_r 0x0091F9E0; C3DVector returned through a hidden pointer:
// `sub esp,0Ch ; lea eax,[esp] ; push eax`), the float pool constant 3.0f @ 0x0122DED4.
// Shape: the inline `C3DVector * float` builds the result straight into the caller's hidden return
// slot through the value constructor, whose arguments VC evaluates right to left (z, y, x loaded and
// multiplied first, stored x, y, z). `v.z * 3.0f` loads the component then `fmul [pool]`. `ret 4` =
// the hidden pointer only.
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator*(float s) const { return C3DVector(x * s, y * s, z * s); }
};

namespace NCameraModes {
struct CPhysicsForceModelBase {                    // vtable 0x0127592C
    virtual C3DVector DoVelocityDamping();         // 0x008FA7E0
    virtual C3DVector DoAttractToPoint();          // 0x008FA800
};
struct CPhysicsForceModelGotoOutsideSimple : CPhysicsForceModelBase {   // vtable 0x01275950
    virtual C3DVector DoAttractToPoint();
};
}

C3DVector NCameraModes::CPhysicsForceModelGotoOutsideSimple::DoAttractToPoint()
{
    return CPhysicsForceModelBase::DoAttractToPoint() * 3.0f;
}