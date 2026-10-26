#include <cstdio>
// Behaviour model of 0x00C40370: this = b x a, component by component.
class C3DVector { public:
    float X, Y, Z;
    void SetCrossProduct(const C3DVector& a, const C3DVector& pos)
    {
        X = pos.Y * a.Z - a.Y * pos.Z;
        Y = a.X * pos.Z - pos.X * a.Z;
        Z = a.Y * pos.X - a.X * pos.Y;
    }
};
int main()
{
    C3DVector x = { 1.0f, 0.0f, 0.0f }, y = { 0.0f, 1.0f, 0.0f }, r;
    r.SetCrossProduct(x, y);                   // y x x = -z
    if (r.X != 0.0f || r.Y != 0.0f || r.Z != -1.0f) { std::printf("00c40370_TEST FAIL\n"); return 1; }
    C3DVector a = { 2.0f, 3.0f, 4.0f }, pos = { 5.0f, 6.0f, 7.0f };
    r.SetCrossProduct(a, pos);                   // b x a = (6*4-7*3, 7*2-5*4, 5*3-6*2) = (3, -6, 3)
    if (r.X != 3.0f || r.Y != -6.0f || r.Z != 3.0f) { std::printf("00c40370_TEST FAIL\n"); return 1; }
    std::printf("00c40370_TEST PASS\n");
    return 0;
}