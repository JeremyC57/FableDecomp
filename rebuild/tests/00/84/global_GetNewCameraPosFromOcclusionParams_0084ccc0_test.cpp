// Behaviour test for GetNewCameraPosFromOcclusionParams (0x0084CCC0): start + direction * distance.
#include <cstdio>
class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};
static C3DVector GetNewCameraPosFromOcclusionParams(const C3DVector& start, const C3DVector& direction, float distance, const C3DVector&)
{
    C3DVector pos = start + direction * distance;
    return pos;
}
int main()
{
    C3DVector s(1.0f, 2.0f, 3.0f), d(0.5f, -1.0f, 2.0f), u(9.0f, 9.0f, 9.0f);
    C3DVector r = GetNewCameraPosFromOcclusionParams(s, d, 4.0f, u);
    if (r.x != 3.0f || r.y != -2.0f || r.z != 11.0f) { std::printf("0084ccc0_TEST FAIL\n"); return 1; }
    std::printf("0084ccc0_TEST PASS\n");
    return 0;
}