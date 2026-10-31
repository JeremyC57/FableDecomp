// behaviour test for 0x0075FE30 NShape::CIsPointFinalLinkInTriangleWithMatchingCirculation::operator()
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static V mk(float x, float y, float z) { V v; v.x = x; v.y = y; v.z = z; return v; }
static V norm(V v) { float l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); if (l > 0) { v.x /= l; v.y /= l; v.z /= l; } return v; }
static bool Test(V a, V b, V up, V p) {
    V l1 = norm(mk(b.x - a.x, b.y - a.y, 0)); V l2 = norm(mk(p.x - b.x, p.y - b.y, 0));
    V c = mk(l1.y * up.z - l1.z * up.y, l1.z * up.x - l1.x * up.z, l1.x * up.y - l1.y * up.x);
    float r = c.x * l2.x; r += c.z * l2.z; r += c.y * l2.y;
    return r > 0.0f;
}
int main() {
    V a = {0, 0, 0}, b = {1, 0, 0}, up = {0, 0, 1};
    bool right = Test(a, b, up, mk(2, -1, 0));   // clockwise turn seen from +Z
    bool left = Test(a, b, up, mk(2, 1, 0));
    bool flipped = Test(a, b, mk(0, 0, -1), mk(2, 1, 0));
    if (!right || left || !flipped) { std::printf("0075fe30_TEST FAIL\n"); return 1; }
    std::printf("0075fe30_TEST PASS\n");
    return 0;
}