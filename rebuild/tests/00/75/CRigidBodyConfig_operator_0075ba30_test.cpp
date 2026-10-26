// Behaviour test for 0x0075ba30 CRigidBodyConfig::operator= (compiler-generated memberwise assignment)
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CMatrix3x4 { public: float M[3][4]; };

static int g_vectorAssigns = 0;
struct CVectorC3DVector {
    C3DVector* _M_start; C3DVector* _M_finish; C3DVector* _M_end_of_storage;
    CVectorC3DVector& operator=(const CVectorC3DVector& other)
    {   // spy: share the other's storage, count the call
        ++g_vectorAssigns; _M_start = other._M_start; _M_finish = other._M_finish; _M_end_of_storage = other._M_end_of_storage; return *this;
    }
};
struct CRigidBodyConfig {
    C3DVector        CMPosition;
    CMatrix3x4       Orientation;
    C3DVector        CMVelocity;
    C3DVector        AngularMomentum;
    C3DVector        CMForce;
    C3DVector        Torque;
    CMatrix3x4       InverseWorldInertiaTensor;
    C3DVector        AngularVelocity;
    CVectorC3DVector BoundingVertices;
};
static void AssignRigidBodyConfig(CRigidBodyConfig& to, const CRigidBodyConfig& from) { to = from; }

static void Fill(C3DVector& v, float b) { v.X = b; v.Y = b + 1; v.Z = b + 2; }
static bool Same(const C3DVector& v, float b) { return v.X == b && v.Y == b + 1 && v.Z == b + 2; }

int main()
{
    CRigidBodyConfig a, b;
    C3DVector verts[2];
    Fill(b.CMPosition, 1); Fill(b.CMVelocity, 10); Fill(b.AngularMomentum, 20); Fill(b.CMForce, 30); Fill(b.Torque, 40); Fill(b.AngularVelocity, 50);
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 4; ++j) { b.Orientation.M[i][j] = (float)(i * 4 + j); b.InverseWorldInertiaTensor.M[i][j] = (float)(100 + i * 4 + j); }
    b.BoundingVertices._M_start = verts; b.BoundingVertices._M_finish = verts + 2; b.BoundingVertices._M_end_of_storage = verts + 2;
    a.BoundingVertices._M_start = a.BoundingVertices._M_finish = a.BoundingVertices._M_end_of_storage = 0;
    AssignRigidBodyConfig(a, b);
    bool ok = Same(a.CMPosition, 1) && Same(a.CMVelocity, 10) && Same(a.AngularMomentum, 20) && Same(a.CMForce, 30)
           && Same(a.Torque, 40) && Same(a.AngularVelocity, 50) && g_vectorAssigns == 1 && a.BoundingVertices._M_finish == verts + 2;
    for (int i = 0; ok && i < 3; ++i) for (int j = 0; j < 4; ++j)
        if (a.Orientation.M[i][j] != (float)(i * 4 + j) || a.InverseWorldInertiaTensor.M[i][j] != (float)(100 + i * 4 + j)) ok = false;
    if (!ok) { std::printf("0075ba30_TEST FAIL\n"); return 1; }
    std::printf("0075ba30_TEST PASS\n");
    return 0;
}