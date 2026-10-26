#include <cstdio>
// Behaviour test for C3DVector::operator== (0x00c256f0): exact component-wise comparison.
class C3DVector { public:
    float x, y, z;
    int operator==(const C3DVector& o) const { return x == o.x && y == o.y && z == o.z; }
};
int main()
{
    C3DVector a = { 1.0f, 2.0f, 3.0f }, b = { 1.0f, 2.0f, 3.0f };
    if ((a == b) != 1) { std::printf("00c256f0_TEST FAIL\n"); return 1; }
    b.z = 3.5f;
    if ((a == b) != 0) { std::printf("00c256f0_TEST FAIL\n"); return 1; }
    b.z = 3.0f; b.x = -1.0f;
    if ((a == b) != 0) { std::printf("00c256f0_TEST FAIL\n"); return 1; }
    b.x = 1.0f; b.y = 0.0f;
    if ((a == b) != 0) { std::printf("00c256f0_TEST FAIL\n"); return 1; }
    std::printf("00c256f0_TEST PASS\n");
    return 0;
}