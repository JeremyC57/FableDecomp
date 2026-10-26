#include <cstdio>
const float g_epsilon_0129ba3c = 1e-4f;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

static bool IsLongerThanEpsilon(const C3DVector& v)
{
    return v.GetLengthSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c;
}

class CPhysicsMeshObject {
public:
    bool IsRigidBodyBoundingPointsValid(vector<C3DVector>& points) const;
};

bool CPhysicsMeshObject::IsRigidBodyBoundingPointsValid(vector<C3DVector>& points) const
{
    C3DVector sum(0.0f, 0.0f, 0.0f);
    long count = points.size();
    for (unsigned long i = 0; i < count; ++i)
        sum += points[i];
    return !IsLongerThanEpsilon(sum);
}

int main()
{
    C3DVector pts[7] = { C3DVector(1, 2, 3), C3DVector(-1, -2, -3), C3DVector(4, 0, 0), C3DVector(-4, 0, 0), C3DVector(0, 5, 0),
                         C3DVector(0, -5, 0), C3DVector(0, 0, 0) };
    vector<C3DVector> v; v._M_start = pts; v._M_finish = pts + 7; v._M_end_of_storage = pts + 7;
    CPhysicsMeshObject o;
    if (!o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    pts[6].X = 0.5f;   /* off-centre: sum (0.5, 0, 0) */
    if (o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    v._M_finish = pts + 2;   /* two points, remainder loop only */
    if (!o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    v._M_finish = pts + 3;
    if (o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    v._M_finish = pts;   /* empty: zero sum is valid */
    if (!o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    pts[6].X = 0.00001f; v._M_finish = pts + 7;   /* within epsilon */
    if (!o.IsRigidBodyBoundingPointsValid(v)) { std::printf("00a11610_TEST FAIL\n"); return 1; }
    std::printf("00a11610_TEST PASS\n");
    return 0;
}