// Grade: DIFFER(672v726) -- 312 difflines, coarse: retail keeps this in edi / config in esi across the loop with a separate byte-offset counter for CollisionNormal (frame 0x7c v 0x74); ours spills this and reloads ecx; vertex Z copied through an int spill slot in retail
// CTCPhysicsRigidBody::ResolveCollisions(CRigidBodyConfig& config) -- impulse response for each colliding vertex (r = vertex - CM, v = vCM + w x r, j = -(1+e) v.n / (1/m + ((I^-1 (r x n)) x r).n)), applied to CM velocity, angular momentum and angular velocity; then clears both collision lists
// Leaf: ResolveCollisions
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075A7A0, 736 bytes, `ret 4`.
#include <string.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
};
__forceinline C3DVector operator*(float f, const C3DVector& v) { return C3DVector(f * v.X, f * v.Y, f * v.Z); }
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{ return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
__forceinline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + a.Y * b.Y + a.X * b.X; }
struct CMatrix3x4 {
    C3DVector m_row0, m_row1, m_row2, m_translation;
    C3DVector operator*(const C3DVector& v) const;        // 0x00A56530
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](long i) { return _M_start[i]; }
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
    vector<C3DVector> BoundingVertices;               // +0xa8
};
struct CTCPhysicsRigidBody {
    virtual void v00();
    unsigned char _pad04[0x40 - 0x04];
    vector<C3DVector> CollisionNormal;                // +0x40
    vector<long>      CollidingVertexIndex;           // +0x4c
    unsigned char _pad58[0xa0 - 0x58];
    float OneOverMass;                                // +0xa0
    float CoefficientOfRestitution;                   // +0xa4
    void ResolveCollisions(CRigidBodyConfig& config);
};

void CTCPhysicsRigidBody::ResolveCollisions(CRigidBodyConfig& config)
{
    long count = CollidingVertexIndex.size();
    for (long i = 0; i < count; ++i) {
        C3DVector r = config.BoundingVertices[CollidingVertexIndex[i]] - config.CMPosition;
        C3DVector velocity = config.CMVelocity + CrossProduct(config.AngularVelocity, r);
        const C3DVector& normal = CollisionNormal[i];
        float numerator = -(DotProduct(velocity, normal) * (CoefficientOfRestitution + 1.0f));
        float denominator = OneOverMass + DotProduct(CrossProduct(config.InverseWorldInertiaTensor * CrossProduct(r, normal), r), normal);
        C3DVector impulse = (numerator / denominator) * normal;
        config.CMVelocity += OneOverMass * impulse;
        config.AngularMomentum += CrossProduct(r, impulse);
        config.AngularVelocity = config.InverseWorldInertiaTensor * config.AngularMomentum;
    }
    {
        C3DVector* dst = CollisionNormal._M_start;
        C3DVector* src = CollisionNormal._M_finish;
        for (long n = CollisionNormal._M_finish - src; n > 0; --n) { *dst = *src; ++dst; ++src; }
        CollisionNormal._M_finish = dst;
    }
    {
        long* first = CollidingVertexIndex._M_start;
        long* last = CollidingVertexIndex._M_finish;
        if (last == last) CollidingVertexIndex._M_finish = first;
        else {
            long bytes = (char*)last - (char*)last;
            memmove(first, last, bytes);
            CollidingVertexIndex._M_finish = (long*)((char*)first + bytes);
        }
    }
}
