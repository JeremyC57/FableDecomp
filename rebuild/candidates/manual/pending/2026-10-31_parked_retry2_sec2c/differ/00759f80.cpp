// Grade: DIFFER(1213v975) 335 diff lines (parked 1416/385): in-place scaled spin matrix (InitialiseSkewedSymmetric + operator*=) (retry wave 2 sec2c)
// Was: DIFFER(1416v975) -- 457 difflines, coarse pass: inline CMatrix3x4 operators build far more temporaries / rep movsd copies than retail; frame 0x10c v 0x108
// CTCPhysicsRigidBody::Integrate(float dt) -- one Euler step of the rigid body from the last configuration: position by velocity, orientation by the skew-symmetric angular velocity, velocity by force over mass, angular momentum by torque; then re-orthonormalise, rebuild the world inverse inertia tensor (R * Ibody^-1 * R^T) and the angular velocity from it
// Leaf: Integrate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759F80, 973 bytes, `ret 4`.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};
__forceinline C3DVector operator*(float f, const C3DVector& v) { return C3DVector(f * v.X, f * v.Y, f * v.Z); }
struct CMatrix3x4 {
    C3DVector m_row0;                              // +0x00
    C3DVector m_row1;                              // +0x0c
    C3DVector m_row2;                              // +0x18
    C3DVector m_translation;                       // +0x24
    void InitialiseSkewedSymmetric(const C3DVector& v);   // 0x00A55D80
    CMatrix3x4 operator*(const CMatrix3x4& o) const;      // 0x00A55DF0
    C3DVector operator*(const C3DVector& v) const;        // 0x00A56530
    void Orthonormalise();                                // 0x00A56270
    __forceinline CMatrix3x4& operator*=(float f)
    {
        m_row0.X *= f; m_row1.X *= f; m_row2.X *= f; m_translation.X *= f;
        m_row0.Y *= f; m_row1.Y *= f; m_row2.Y *= f; m_translation.Y *= f;
        m_row0.Z *= f; m_row1.Z *= f; m_row2.Z *= f; m_translation.Z *= f;
        return *this;
    }
    __forceinline CMatrix3x4 operator*(float f) const
    {
        CMatrix3x4 r = *this;
        r.m_row0.X *= f; r.m_row1.X *= f; r.m_row2.X *= f; r.m_translation.X *= f;
        r.m_row0.Y *= f; r.m_row1.Y *= f; r.m_row2.Y *= f; r.m_translation.Y *= f;
        r.m_row0.Z *= f; r.m_row1.Z *= f; r.m_row2.Z *= f; r.m_translation.Z *= f;
        return r;
    }
    __forceinline CMatrix3x4 operator+(const CMatrix3x4& o) const
    {
        CMatrix3x4 r = *this;
        r.m_row0.X += o.m_row0.X; r.m_row0.Y += o.m_row0.Y; r.m_row0.Z += o.m_row0.Z;
        r.m_row1.X += o.m_row1.X; r.m_row1.Y += o.m_row1.Y; r.m_row1.Z += o.m_row1.Z;
        r.m_row2.X += o.m_row2.X; r.m_row2.Y += o.m_row2.Y; r.m_row2.Z += o.m_row2.Z;
        r.m_translation.X += o.m_translation.X; r.m_translation.Y += o.m_translation.Y; r.m_translation.Z += o.m_translation.Z;
        return r;
    }
    __forceinline CMatrix3x4 Transpose() const
    {
        CMatrix3x4 r = *this;
        r.m_row0.Y = m_row1.X; r.m_row1.X = m_row0.Y;
        r.m_row0.Z = m_row2.X; r.m_row2.X = m_row0.Z;
        r.m_row1.Z = m_row2.Y; r.m_row2.Y = m_row1.Z;
        return r;
    }
};
__forceinline CMatrix3x4 Star(const C3DVector& v) { CMatrix3x4 m; m.InitialiseSkewedSymmetric(v); return m; }
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CRigidBodyConfig {
    C3DVector  CMPosition;                            // +0x00
    CMatrix3x4 Orientation;                           // +0x0c
    C3DVector  CMVelocity;                            // +0x3c
    C3DVector  AngularMomentum;                       // +0x48
    C3DVector  CMForce;                               // +0x54
    C3DVector  Torque;                                // +0x60
    CMatrix3x4 InverseWorldInertiaTensor;             // +0x6c
    C3DVector  AngularVelocity;                       // +0x9c
    vector<C3DVector> BoundingVertices;               // +0xa8
};
struct CRigidBody {
    float      OneOverMass;                           // +0x00
    float      CoefficientOfRestitution;              // +0x04
    CMatrix3x4 InverseBodyInertiaTensor;              // +0x08
    C3DVector  CMPosition;                            // +0x38
    vector<C3DVector> BodyBoundingVertices;           // +0x44
    CRigidBodyConfig CurrentConfig;                   // +0x50
    CRigidBodyConfig LastConfig;                      // +0x104
};
struct CTCPhysicsRigidBody {
    virtual void v00();
    unsigned char _pad04[0xa0 - 0x04];
    CRigidBody    Body;                               // +0xa0
    void Integrate(float dt);
};

void CTCPhysicsRigidBody::Integrate(float dt)
{
    CRigidBodyConfig& current = Body.CurrentConfig;
    const CRigidBodyConfig& last = Body.LastConfig;
    current.CMPosition = last.CMPosition + dt * last.CMVelocity;
    CMatrix3x4 spin;
    spin.InitialiseSkewedSymmetric(last.AngularVelocity);
    spin *= dt;
    current.Orientation = last.Orientation + spin * last.Orientation;
    current.CMVelocity = last.CMVelocity + last.CMForce * (dt * Body.OneOverMass);
    current.AngularMomentum = last.AngularMomentum + last.Torque * dt;
    current.Orientation.Orthonormalise();
    current.InverseWorldInertiaTensor = current.Orientation * Body.InverseBodyInertiaTensor * current.Orientation.Transpose();
    current.AngularVelocity = current.InverseWorldInertiaTensor * current.AngularMomentum;
}
