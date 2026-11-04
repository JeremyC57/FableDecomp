// Behaviour test for 0x00B451D0 CEngineLightingManager::CalculateFogTransformMatrix(float, float): the row's product (the fog matrix,
// z -> (z - start) / (end - start) in X, pre-multiplied by a view transform; first column returned) checked against a direct
// evaluation: transforming a view-space point by the view matrix and taking its normalised depth.
#include <cstdio>
#include <cmath>
struct M { float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43; };
struct V4 { float X, Y, Z, W; };
static V4 Calc(const M& l, float fogStart, float fogEnd)
{
    float scale = 1.0f / (fogEnd - fogStart);
    M a = { 0,0,0, 0,0,0, scale,0,0, -(scale * fogStart),0,0 };
    float e11 = a.E21 * l.E12 + a.E31 * l.E13 + a.E11 * l.E11;
    float e21 = a.E21 * l.E22 + a.E31 * l.E23 + a.E11 * l.E21;
    float e31 = a.E21 * l.E32 + a.E31 * l.E33 + a.E11 * l.E31;
    float e41 = a.E21 * l.E42 + a.E31 * l.E43 + a.E11 * l.E41 + a.E41;
    V4 r = { e11, e21, e31, e41 };
    return r;
}
int main()
{
    int ok = 1;
    M view = { 1,0,0.5f, 0,1,0.25f, 0,0,2, 3,4,5 };   // row-vector convention: p' = p * view
    V4 f = Calc(view, 10.0f, 50.0f);
    float px = 1, py = 2, pz = 3;
    float z = px * view.E13 + py * view.E23 + pz * view.E33 + view.E43;         // view-space depth
    float expect = (z - 10.0f) / 40.0f;
    float got = px * f.X + py * f.Y + pz * f.Z + f.W;
    ok &= std::fabs(got - expect) < 1e-5f;
    std::printf(ok ? "00B451D0_TEST PASS\n" : "00B451D0_TEST FAIL\n");
    return ok ? 0 : 1;
}