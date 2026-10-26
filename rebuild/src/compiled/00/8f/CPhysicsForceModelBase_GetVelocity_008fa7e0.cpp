// CPhysicsForceModelBase::GetVelocity() const -- returns a copy of the linked physics state's Velocity (C3DVector by hidden pointer)
// Leaf: GetVelocity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA7E0, 30 bytes (+2 int3 padding). No callees.
//
// The manifest names this `CPhysicsForceModelBase::DoVelocityDamping`; the bytes are the velocity
// getter that DoVelocityDamping (0x008FCC10, its only caller) scales: `this` stays in ecx there and
// the returned vector is multiplied component-wise by a constant. PDB layouts: CPhysicsForceModelBase
// { vptr; CPhysicsState* LinkedState (+0x4); ... } and CPhysicsState { C3DVector Position (+0x0);
// C3DVector Velocity (+0xC); C3DVector Acceleration (+0x18); ... }.
// Shape: the velocity is reached through an inline accessor returning `const C3DVector&`, and the
// vector has a user memberwise copy constructor that builds the hidden result: the first word is
// read at +0xC, then the member address is formed (`add ecx,0Ch`) for the other two dword copies.
// With a POD vector (implicit copy, by accessor or direct member) VC copies through esi/edx with a
// `mov ecx,eax` shuffle (35 bytes).
class C3DVector { public:
    float x;
    float y;
    float z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) {}
};

struct CPhysicsState {
    C3DVector Position;                    // +0x00
    C3DVector Velocity;                    // +0x0C
    C3DVector Acceleration;                // +0x18
    const C3DVector& GetVelocity() const { return Velocity; }
};

struct CPhysicsForceModelBase {
    virtual void Slot00();
    CPhysicsState* LinkedState;            // +0x04
    C3DVector GetVelocity() const;
};

C3DVector CPhysicsForceModelBase::GetVelocity() const
{
    return LinkedState->GetVelocity();
}