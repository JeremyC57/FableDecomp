// Grade: DIFFER(861v709) -- 222 difflines, coarse pass: the axis lengths, mass (b*c*a*3.2), inverse mass, identity tensor with the box inverse-inertia diagonal, bounding points, centre of gravity, the two config vertex resizes, the current-config reset (position = centre + body position, orientation from the RHSet matrix by rep movsd), CalculateVertices and the config copy follow retail; the model is 152 bytes long: the config zeroing and the diagonal x87 expressions are not yet in retail form (retail shares a^2 through a stored temp and reuses the 1.0f/0 registers).
// CTCPhysicsRigidBody::InitialiseBody() -- from the physics mesh object's three axis half-extents set the mass (3.2 per unit volume), an identity restitution/inertia base with the box's inverse inertia diagonal, the body bounding points and centre of gravity, size the configs' vertex buffers, reset the current config (position = centre + body position, orientation from the right-handed set) and copy it to the last config
// Leaf: InitialiseBody
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B000, 709 bytes, plain `ret`.
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    __forceinline float Length() const { return sqrtf(X * X + Y * Y + Z * Z); }
};
struct CMatrix3x4 {
    float M[3][4];
    __forceinline void SetIdentity()
    {
        M[0][0] = 1.0f; M[0][1] = 0.0f; M[0][2] = 0.0f; M[0][3] = 0.0f;
        M[1][0] = 0.0f; M[1][1] = 1.0f; M[1][2] = 0.0f; M[1][3] = 0.0f;
        M[2][0] = 0.0f; M[2][1] = 0.0f; M[2][2] = 1.0f; M[2][3] = 0.0f;
    }
};
struct C3DMatrix { float M[12]; };
struct CRightHandedSet { C3DVector Forward, Up; };
const C3DMatrix& __fastcall MakeMatrix(C3DMatrix& out, const CRightHandedSet& rh);   // 0x00A358F0
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CC3DVectorVector : vector<C3DVector> {
    void resize(unsigned long n, const C3DVector& value);   // 0x0075C040
};
struct CPhysicsMeshObject {
    unsigned char _data[0x40];
    ~CPhysicsMeshObject();                            // 0x00A127D0
    const C3DVector& GetXAxis(C3DVector& out) const;  // 0x00A11C20
    const C3DVector& GetYAxis(C3DVector& out) const;  // 0x00A11C90
    const C3DVector& GetZAxis(C3DVector& out) const;  // 0x00A11BA0
    void GetRigidBodyBoundingPoints(CC3DVectorVector& points) const;   // 0x00A12840
    C3DVector GetCentreOfGravity() const;             // 0x00A11EC0
};
struct CRigidBodyConfig {
    C3DVector  CMPosition;                            // +0x00
    CMatrix3x4 Orientation;                           // +0x0c
    C3DVector  CMVelocity;                            // +0x3c
    C3DVector  AngularMomentum;                       // +0x48
    C3DVector  CMForce;                               // +0x54
    C3DVector  Torque;                                // +0x60
    CMatrix3x4 InverseWorldInertiaTensor;             // +0x6c
    C3DVector  AngularVelocity;                       // +0x9c
    CC3DVectorVector BoundingVertices;                // +0xa8
    CRigidBodyConfig& operator=(const CRigidBodyConfig& other);   // 0x0075BA30
};
struct CRigidBody {
    float      OneOverMass;                           // +0x00
    float      CoefficientOfRestitution;              // +0x04
    CMatrix3x4 InverseBodyInertiaTensor;              // +0x08
    C3DVector  CMPosition;                            // +0x38
    CC3DVectorVector BodyBoundingVertices;            // +0x44
    CRigidBodyConfig CurrentConfig;                   // +0x50
    CRigidBodyConfig LastConfig;                      // +0x104
};
#define FILL8(n) virtual void s##n##0(); virtual void s##n##1(); virtual void s##n##2(); virtual void s##n##3(); \
    virtual void s##n##4(); virtual void s##n##5(); virtual void s##n##6(); virtual void s##n##7();
struct CTCPhysicsRigidBody {
    FILL8(0) FILL8(1) FILL8(2) FILL8(3) FILL8(4) FILL8(5) FILL8(6) FILL8(7) FILL8(8)
    virtual const CRightHandedSet& GetRHSet();        // slot 72 (+0x120)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector     Position;                           // +0x0c
    unsigned char _pad18[0xa0 - 0x18];
    CRigidBody    Body;                               // +0xa0
    CPhysicsMeshObject GetPhysicsMeshObject();        // 0x006B0850
    void CalculateVertices(CRigidBodyConfig& config); // 0x00759EC0
    void InitialiseBody();
};

void CTCPhysicsRigidBody::InitialiseBody()
{
    CPhysicsMeshObject mesh = GetPhysicsMeshObject();
    C3DVector axis;
    float a = mesh.GetXAxis(axis).Length();
    float b = mesh.GetYAxis(axis).Length();
    float c = mesh.GetZAxis(axis).Length();
    float mass = b * c * a * 3.2f;
    Body.OneOverMass = 1.0f / mass;
    Body.CoefficientOfRestitution = 1.0f;
    Body.InverseBodyInertiaTensor.SetIdentity();
    float aa = a * a;
    float bb = b * b;
    float cc = c * c;
    Body.InverseBodyInertiaTensor.M[0][0] = 3.0f / ((aa + cc) * mass);
    Body.InverseBodyInertiaTensor.M[1][1] = 3.0f / ((bb + cc) * mass);
    Body.InverseBodyInertiaTensor.M[2][2] = 3.0f / ((bb + aa) * mass);
    mesh.GetRigidBodyBoundingPoints(Body.BodyBoundingVertices);
    Body.CMPosition = mesh.GetCentreOfGravity();
    C3DVector zero;
    Body.CurrentConfig.BoundingVertices.resize(Body.BodyBoundingVertices.size(), zero);
    Body.LastConfig.BoundingVertices.resize(Body.BodyBoundingVertices.size(), zero);
    CRigidBodyConfig& config = Body.CurrentConfig;
    config.AngularVelocity = C3DVector(0.0f, 0.0f, 0.0f);
    config.CMVelocity = C3DVector(0.0f, 0.0f, 0.0f);
    config.AngularMomentum = C3DVector(0.0f, 0.0f, 0.0f);
    config.CMForce = C3DVector(0.0f, 0.0f, 0.0f);
    config.Torque = C3DVector(0.0f, 0.0f, 0.0f);
    config.CMPosition = Body.CMPosition + Position;
    C3DMatrix m;
    *(C3DMatrix*)&config.Orientation = MakeMatrix(m, GetRHSet());
    CalculateVertices(config);
    Body.LastConfig = config;
}
