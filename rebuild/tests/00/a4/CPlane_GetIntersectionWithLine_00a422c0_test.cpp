// Behaviour test for CPlane::GetIntersectionWithLine (0x00A422C0).
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
inline float DotProduct(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
inline float DotProductByValue(C3DVector a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
class CPlane { public:
    C3DVector Normal; float Distance;
    bool GetIntersectionWithLine(const C3DVector& point, const C3DVector& direction, C3DVector& intersection) const
    {
        float denominator = DotProduct(Normal, direction);
        if (denominator != 0.0f) {
            float t = (Distance - DotProductByValue(point, Normal)) / denominator;
            intersection = direction * t + point;
            return true;
        }
        return false;
    }
};
int main()
{
    CPlane p; p.Normal = C3DVector(0, 0, 1); p.Distance = 5;           /* plane z = 5 */
    C3DVector hit(-1, -1, -1);
    if (!p.GetIntersectionWithLine(C3DVector(1, 2, 1), C3DVector(1, 0, 2), hit)) { std::printf("00a422c0_TEST FAIL\n"); return 1; }
    if (hit.X != 3.0f || hit.Y != 2.0f || hit.Z != 5.0f) { std::printf("00a422c0_TEST FAIL\n"); return 1; }
    C3DVector keep(9, 9, 9);
    if (p.GetIntersectionWithLine(C3DVector(1, 2, 1), C3DVector(1, 1, 0), keep) || keep.X != 9.0f) { std::printf("00a422c0_TEST FAIL\n"); return 1; }
    std::printf("00a422c0_TEST PASS\n");
    return 0;
}