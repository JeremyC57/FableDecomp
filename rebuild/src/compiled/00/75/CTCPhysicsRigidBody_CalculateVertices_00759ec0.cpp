// CTCPhysicsRigidBody::CalculateVertices(CRigidBody::CRigidBodyConfig& config) -- rebuild a configuration's world-space bounding vertices: each body-space vertex rotated by the configuration's orientation and offset by its centre of mass
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759EC0, 183 bytes (`ret 4`; PDB ?CalculateVertices@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z).
// Callees: CMatrix3x4::operator*(const C3DVector&) const @ 0x00A56530 (landed; hidden-pointer C3DVector result).
// Layout: Ego_r.pdb CTCPhysicsRigidBody Body (CRigidBody) sits at retail +0xa0 (PDB +0xac: the three STLport vectors before it
//   are 12 bytes in retail, 16 in the PDB), so Body.BodyBoundingVertices is +0xe4; CRigidBodyConfig CMPosition +0x00,
//   Orientation +0x0c, BoundingVertices +0xa8. engine/CTCPhysicsRigidBody.h carries the PDB offsets and opaque members, so the
//   class is modelled locally with the retail offsets.
// Shape: `for (unsigned long i = 0; i < size(); ++i)` with size() re-evaluated each pass (signed /12 reciprocal multiply, `jb`)
//   and `*(begin() + i)` element access (the byte offset in ebx). The rotated vertex is a NAMED local: retail reads it back
//   from its frame slot (`fld [esp+1Ch]`); the unnamed temporary reads through the returned eax. The sum goes through the inline
//   value-constructing operator+ and the implicit assignment (three fstp/mov pairs).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CMatrix3x4 {
    C3DVector m_row0, m_row1, m_row2, m_translation;
    C3DVector operator*(const C3DVector& v) const;     // 0x00A56530
};
template <class T> struct vector {                     // retail STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CRigidBody {
    struct CRigidBodyConfig {                          // 0xb4 bytes in retail (PDB 0xb8)
        C3DVector  CMPosition;                         // +0x00
        CMatrix3x4 Orientation;                        // +0x0c
        C3DVector  CMVelocity;                         // +0x3c
        C3DVector  AngularMomentum;                    // +0x48
        C3DVector  CMForce;                            // +0x54
        C3DVector  Torque;                             // +0x60
        CMatrix3x4 InverseWorldInertiaTensor;          // +0x6c
        C3DVector  AngularVelocity;                    // +0x9c
        vector<C3DVector> BoundingVertices;            // +0xa8
    };
    float      OneOverMass;                            // +0x00
    float      CoefficientOfRestitution;               // +0x04
    CMatrix3x4 InverseBodyInertiaTensor;               // +0x08
    C3DVector  CMPosition;                             // +0x38
    vector<C3DVector> BodyBoundingVertices;            // +0x44
};
struct CTCPhysicsRigidBody {
    unsigned char _pad00[0xa0];
    CRigidBody Body;                                   // +0xa0
    void CalculateVertices(CRigidBody::CRigidBodyConfig& config);
};

void CTCPhysicsRigidBody::CalculateVertices(CRigidBody::CRigidBodyConfig& config)
{
    for (unsigned long i = 0; i < Body.BodyBoundingVertices.size(); ++i)
    {
        C3DVector rotated = config.Orientation * Body.BodyBoundingVertices[i];
        config.BoundingVertices[i] = rotated + config.CMPosition;
    }
}