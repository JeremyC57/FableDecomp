// Behaviour test for CTCPhysicsRigidBody::CalculateVertices (0x00759EC0): every body-space bounding vertex becomes
// Orientation * v + CMPosition in the configuration's vertex array; an empty body list leaves the output untouched.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CMatrix3x4 { public:
    C3DVector m_row0, m_row1, m_row2, m_translation;
    C3DVector operator*(const C3DVector& v) const
    {
        C3DVector result;
        result.X = m_row0.X * v.X + m_row0.Y * v.Y + m_row0.Z * v.Z;
        result.Y = m_row1.X * v.X + m_row1.Y * v.Y + m_row1.Z * v.Z;
        result.Z = m_row2.X * v.X + m_row2.Y * v.Y + m_row2.Z * v.Z;
        return result;
    }
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
struct CRigidBody {
    class CRigidBodyConfig { public:
        C3DVector  CMPosition;
        CMatrix3x4 Orientation;
        vector<C3DVector> BoundingVertices;
    };
    vector<C3DVector> BodyBoundingVertices;
};
struct CTCPhysicsRigidBody {
    CRigidBody Body;
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

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    int bad = 0;
    C3DVector local[3] = { C3DVector(1, 0, 0), C3DVector(0, 2, 0), C3DVector(1, 2, 3) };
    C3DVector world[4] = { C3DVector(9, 9, 9), C3DVector(9, 9, 9), C3DVector(9, 9, 9), C3DVector(7, 7, 7) };
    CTCPhysicsRigidBody rb;
    rb.Body.BodyBoundingVertices._M_start = local; rb.Body.BodyBoundingVertices._M_finish = local + 3;
    rb.Body.BodyBoundingVertices._M_end_of_storage = local + 3;
    CRigidBody::CRigidBodyConfig cfg;
    cfg.CMPosition = C3DVector(10, 20, 30);
    cfg.Orientation.m_row0 = C3DVector(0, -1, 0);      // 90 degrees about Z
    cfg.Orientation.m_row1 = C3DVector(1, 0, 0);
    cfg.Orientation.m_row2 = C3DVector(0, 0, 1);
    cfg.BoundingVertices._M_start = world; cfg.BoundingVertices._M_finish = world + 4; cfg.BoundingVertices._M_end_of_storage = world + 4;
    rb.CalculateVertices(cfg);
    if (!Near(world[0].X, 10) || !Near(world[0].Y, 21) || !Near(world[0].Z, 30)) bad |= 1;
    if (!Near(world[1].X, 8) || !Near(world[1].Y, 20) || !Near(world[1].Z, 30)) bad |= 2;
    if (!Near(world[2].X, 8) || !Near(world[2].Y, 21) || !Near(world[2].Z, 33)) bad |= 4;
    if (!Near(world[3].X, 7)) bad |= 8;               // past the body's count: untouched
    rb.Body.BodyBoundingVertices._M_finish = local;     // empty
    world[0] = C3DVector(5, 5, 5);
    rb.CalculateVertices(cfg);
    if (!Near(world[0].X, 5)) bad |= 16;
    if (bad) { std::printf("00759ec0_TEST FAIL\n"); return 1; }
    std::printf("00759ec0_TEST PASS\n");
    return 0;
}