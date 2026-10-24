// Behaviour test for 0x0066F990 C2DVector::operator/=.
#include <cstdio>
class C2DVector {
public:
    float X, Y;
    C2DVector& operator/=(float divisor) { float inv = 1.0f / divisor; X *= inv; Y *= inv; return *this; }
};
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    C2DVector v; v.X = 6.0f; v.Y = -3.0f;
    C2DVector& r = (v /= 2.0f);
    if (&r != &v || !Near(v.X, 3.0f) || !Near(v.Y, -1.5f)) { std::printf("0066f990_TEST FAIL\n"); return 1; }
    v /= 0.5f;
    if (!Near(v.X, 6.0f) || !Near(v.Y, -3.0f)) { std::printf("0066f990_TEST FAIL\n"); return 1; }
    std::printf("0066f990_TEST PASS\n");
    return 0;
}
