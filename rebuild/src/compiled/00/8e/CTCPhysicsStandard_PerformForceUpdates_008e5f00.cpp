// CTCPhysicsStandard::PerformForceUpdates() -- gravity-only twin: apply a -0.12 Z acceleration unless the thing rests still on near-flat ground (no air-resistance block)
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E5F00, 168 bytes (plain `ret`). No call or data reference to this body was found
//   (xref.py); it is byte-identical to the gravity half of 0x008E5670, so the class is the catalog's (CTCPhysicsStandard) until
//   a vtable shows otherwise.
// Callees: virtual GetFeelsGravity slot 48 (+0xc0), IsOnGround slot 56 (+0xe0); CTCBase::GetWorldMap() @ 0x006AF620,
//   CWorldMap::GetGroundFaceNormalAt(const C3DVector&) const @ 0x0063AB10, CTCPhysicsBase::ApplyAcceleration @ 0x006B0210.
//   Floats: 0.98f @ 0x01287364, g_epsilon_0129ba3c @ 0x0129BA3C (1e-4f).
// Shape: as 0x008E5670 (one `||` chain, inline GetLengthSquared() on the x87 stack).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C = 1e-4f
struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;   // 0x0063AB10
};
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual bool GetFeelsGravity() const;           // slot 48 (+0xc0)
    virtual bool GetFeelsAirResistance() const;     // slot 49 (+0xc4)
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual bool IsOnGround() const;                // slot 56 (+0xe0)
    void*     ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long      OldPositionLastFrameSet;              // +0x24
    C3DVector Velocity;                             // +0x28
    float     Radius;                               // +0x34
    void*     PhysicsDef;                           // +0x38 CDefPointer<CPhysicsDef const>::Object
    unsigned char m_flags3c;                        // +0x3c Moving / OldMoving
    float GetAirResistance();                       // 0x006AFDE0
    CWorldMap* GetWorldMap();                       // 0x006AF620 (CTCBase)
    void ApplyAcceleration(const C3DVector& acceleration);   // 0x006B0210
};
struct CTCPhysicsStandard : CTCPhysicsBase {
    void PerformForceUpdates();
};

void CTCPhysicsStandard::PerformForceUpdates()
{
    if (GetFeelsGravity()) {
        if (!IsOnGround() || GetWorldMap()->GetGroundFaceNormalAt(Position).Z < 0.98f
            || Velocity.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            ApplyAcceleration(C3DVector(0.0f, 0.0f, -0.12f));
    }
}