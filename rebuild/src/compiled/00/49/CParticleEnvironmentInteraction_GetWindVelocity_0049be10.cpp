// CParticleEnvironmentInteraction::GetWindVelocity() const -- with a world: the world wind's direction scaled by its speed; otherwise the stored WindVelocity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049BE10, length 123 bytes (padded to 128). CParticleEnvironmentInteraction
// vtable 0x0122ED8C slot 3.
// Leaf: ?GetWindVelocity@CParticleEnvironmentInteraction@@UBE?AVC3DVector@@XZ
// Callees: the world wind object's (CWorld +0x18) virtual slot 2 (+0x08, returns the direction by reference) and slot 3
// (+0x0c, returns the speed as a float).
// Layout: PDB, byte-packed (as the landed GetCameraPos 0x0049BDC0): PWorld +4, WindVelocity +8, WindActive +0x14, CameraPos +0x15.
// Shape: the direction is copied into a NAMED frame vector; `direction * speed` goes through an inline C3DVector::operator*(float)
// that builds a named result field by field (the by-value float parameter is homed with `fst`, so x*s comes off the call
// result and y, z re-read it) and returns it through the memberwise user copy ctor (fxch st(2) + three fstp); both virtual
// calls reach the wind object through an inline CWorld::GetWind() accessor (the direct member chain loads the first vtable
// into edx instead of eax); the fallback returns WindVelocity by the user copy ctor.
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float s) const
    {
        C3DVector r;
        r.X = X * s;
        r.Y = Y * s;
        r.Z = Z * s;
        return r;
    }
};
struct CWind {
    virtual void v00(); virtual void v01();
    virtual const C3DVector& GetDirection() const;   // slot 2 (+0x08)
    virtual float GetSpeed() const;                  // slot 3 (+0x0c)
};
struct CWorld {
    unsigned char _world[0x18];
    CWind*        PWind;                             // +0x18
    CWind* GetWind() const { return PWind; }
};
#pragma pack(push, 1)
struct CParticleEnvironmentInteraction {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual C3DVector GetWindVelocity() const;       // slot 3
    CWorld*   PWorld;                                // +0x04
    C3DVector WindVelocity;                          // +0x08
    bool      WindActive;                            // +0x14
    C3DVector CameraPos;                             // +0x15
};
#pragma pack(pop)
C3DVector CParticleEnvironmentInteraction::GetWindVelocity() const
{
    if (PWorld) {
        C3DVector direction = PWorld->GetWind()->GetDirection();
        return direction * PWorld->GetWind()->GetSpeed();
    }
    return WindVelocity;
}