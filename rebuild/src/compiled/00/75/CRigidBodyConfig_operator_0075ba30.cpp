// CRigidBodyConfig::operator=(const CRigidBodyConfig& other) -- the compiler-generated memberwise assignment of a rigid body's physical state
// Leaf: ??4CRigidBodyConfig@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075BA30, length 196 bytes (`ret 4`, returns `this`).
// Callees: std::vector<C3DVector>::operator=(const vector&) @ 0x0075B8D0 (BoundingVertices; manifest "Std_Vector_Assign").
// Layout: Ego_r.pdb CRigidBodyConfig (0xb8) with retail's 12-byte STLport vector (0xb4): CMPosition +0x00,
// Orientation +0x0c (CMatrix3x4, 48 bytes), CMVelocity +0x3c, AngularMomentum +0x48, CMForce +0x54, Torque +0x60,
// InverseWorldInertiaTensor +0x6c, AngularVelocity +0x9c, BoundingVertices +0xa8.
// Shape: no user operator= -- the implicit one is emitted for the one-line `a = b` helper below. C3DVector has a
// user copy constructor but no operator=, so each vector member is copied through a pair of re-based pointer
// registers (`lea ecx,[eax+N] ; lea edx,[ebx+N]` then three dword moves); the plain 3x4 matrices are `rep movsd`
// blocks; the vector member goes through its out-of-line assignment last.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

struct CMatrix3x4 {
    float M[3][4];
};

namespace std {
template<class T> struct allocator { };
template<class T, class A = allocator<T> > struct vector {      // STLport: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector& operator=(const vector& other);
};
}

struct CRigidBodyConfig {
    C3DVector              CMPosition;                  // +0x00
    CMatrix3x4             Orientation;                 // +0x0c
    C3DVector              CMVelocity;                  // +0x3c
    C3DVector              AngularMomentum;             // +0x48
    C3DVector              CMForce;                     // +0x54
    C3DVector              Torque;                      // +0x60
    CMatrix3x4             InverseWorldInertiaTensor;   // +0x6c
    C3DVector              AngularVelocity;             // +0x9c
    std::vector<C3DVector> BoundingVertices;            // +0xa8
};

// 0x0075B8D0
template<> std::vector<C3DVector>& std::vector<C3DVector>::operator=(const std::vector<C3DVector>& other);

void AssignRigidBodyConfig(CRigidBodyConfig& to, const CRigidBodyConfig& from)
{
    to = from;
}