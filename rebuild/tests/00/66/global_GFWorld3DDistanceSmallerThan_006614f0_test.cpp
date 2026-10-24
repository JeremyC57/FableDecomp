// Behaviour test for 0x006614F0 GFWorld3DDistanceSmallerThan (model copy)
#include <cstdio>
struct C3DVector { float X, Y, Z; };
bool SmallerThan(const C3DVector& a, const C3DVector& b, float distance)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return dx * dx + dy * dy + dz * dz < distance * distance;
}
int main()
{
    C3DVector a = { 1.0f, 2.0f, 3.0f }, b = { 4.0f, 6.0f, 3.0f };    // distance 5
    if (!SmallerThan(a, b, 5.5f) || SmallerThan(a, b, 5.0f) || SmallerThan(a, b, 4.0f) || !SmallerThan(a, a, 0.1f) || SmallerThan(a, a, 0.0f))
        { std::printf("006614f0_TEST FAIL\n"); return 1; }
    std::printf("006614f0_TEST PASS\n");
    return 0;
}