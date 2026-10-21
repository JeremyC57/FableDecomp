// Grade: DIFFER(356v366) -- x87 scheduling: retail builds the first negated force as a temporary and copies its Z through the frame (`fstp ; mov edx ; mov`) before the next call, keeping X/Y on the FPU stack; ours constructs the sum directly (the assignment form copies all three words)
// CPhysicsForceModelBase::FrameUpdate() -- sum the force model's component forces (five reactive forces negated, one applied as is) and store the result as the linked physics state's acceleration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FA660, length 366 bytes, plain `ret`.
// Callees: the model's own virtual force getters, slots 2..7 (+0x08..+0x1C), each returning a C3DVector through a
// hidden pointer.
// Layout: Ego_r.pdb CPhysicsForceModelBase: LinkedState +4; the state's acceleration vector at +0x18 (descriptive).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
struct CPhysicsState {
    unsigned char _pad00[0x18];
    C3DVector     Acceleration;                         // +0x18 (descriptive)
};
struct CPhysicsForceModelBase {
    virtual void v00();
    virtual void v01();
    virtual C3DVector GetForce2() const;                // slot 2 (+0x08)
    virtual C3DVector GetForce3() const;                // slot 3 (+0x0c)
    virtual C3DVector GetForce4() const;                // slot 4 (+0x10)
    virtual C3DVector GetForce5() const;                // slot 5 (+0x14)
    virtual C3DVector GetForce6() const;                // slot 6 (+0x18)
    virtual C3DVector GetForce7() const;                // slot 7 (+0x1c)
    CPhysicsState* LinkedState;                         // +0x04
    void FrameUpdate();
};
void CPhysicsForceModelBase::FrameUpdate()
{
    C3DVector force = -GetForce2();
    force += -GetForce4();
    force += -GetForce3();
    force += -GetForce5();
    force += GetForce6();
    force += -GetForce7();
    LinkedState->Acceleration = force;
}
