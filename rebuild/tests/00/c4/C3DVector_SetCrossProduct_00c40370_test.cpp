#include <cstdio>
// Behaviour model of 0x00C40370: this = b x a, component by component.
struct C3DVector {
    float X, Y, Z;
    void SetCrossProduct(const C3DVector& a, const C3DVector& b)
    {
        X = b.Y * a.Z - a.Y * b.Z;
        Y = a.X * b.Z - b.X * a.Z;
        Z = a.Y * b.X - a.X * b.Y;
    }
};
int main()
{
    C3DVector x = { 1.0f, 0.0f, 0.0f }, y = { 0.0f, 1.0f, 0.0f }, r;
    r.SetCrossProduct(x, y);                   // y x x = -z
    if (r.X != 0.0f || r.Y != 0.0f || r.Z != -1.0f) { std::printf("00c40370_TEST FAIL\n"); return 1; }
    C3DVector a = { 2.0f, 3.0f, 4.0f }, b = { 5.0f, 6.0f, 7.0f };
    r.SetCrossProduct(a, b);                   // b x a = (6*4-7*3, 7*2-5*4, 5*3-6*2) = (3, -6, 3)
    if (r.X != 3.0f || r.Y != -6.0f || r.Z != 3.0f) { std::printf("00c40370_TEST FAIL\n"); return 1; }
    std::printf("00c40370_TEST PASS\n");
    return 0;
}