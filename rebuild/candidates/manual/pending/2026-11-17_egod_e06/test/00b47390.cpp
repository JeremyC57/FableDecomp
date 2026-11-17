// Behaviour test for CEngineLightingManager::SetupShaderSpotLight(const CMatrix3x4&) (0x00B47390): falloff = transform * scene falloff
// (rotation rows times the scene rotation, translation row times the scene rotation plus the scene translation), attenuation squared =
// (SpotRadius / Radius)^2, constant = 1 / (1 / squared - 1); the shader gets (falloff, constant, -constant, colour).
#include <cstdio>
#include <cmath>
struct M { float e[4][3]; };
static void SetMultiply(M& r, const M& a, const M& b)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 3; ++j)
            r.e[i][j] = a.e[i][0] * b.e[0][j] + a.e[i][1] * b.e[1][j] + a.e[i][2] * b.e[2][j] + (i == 3 ? b.e[3][j] : 0.0f);
}
struct Out { M m; float scale, offset; const float* colour; };
static Out g;
static void Setup(const M& transform, const M& scene_falloff, float spot_radius, float radius, const float* colour)
{
    float light_attenuation_squared = (spot_radius / radius) * (spot_radius / radius);
    float light_attenuation_constant = 1.0f / (1.0f / light_attenuation_squared - 1.0f);
    M falloff_matrix = transform; M copy = falloff_matrix;
    SetMultiply(falloff_matrix, copy, scene_falloff);                    // PostMultiply
    g.m = falloff_matrix; g.scale = light_attenuation_constant; g.offset = -light_attenuation_constant; g.colour = colour;
}
static int fail(int n) { std::printf("00b47390_TEST FAIL %d\n", n); return 1; }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    M t = { { { 1, 2, 0 }, { 0, 1, 0 }, { 0, 0, 2 }, { 5, 6, 7 } } };
    M s = { { { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 1 }, { 1, 1, 1 } } };   // swap X/Y, translate by 1
    float colour[4] = { 1, 0.5f, 0.25f, 1 };
    Setup(t, s, 1.0f, 2.0f, colour);
    if (!near(g.m.e[0][0], 2) || !near(g.m.e[0][1], 1) || !near(g.m.e[0][2], 0)) return fail(1);   // row (1,2,0) swapped
    if (!near(g.m.e[2][2], 2)) return fail(2);
    if (!near(g.m.e[3][0], 7) || !near(g.m.e[3][1], 6) || !near(g.m.e[3][2], 8)) return fail(3);  // (6,5,7)+1
    if (!near(g.scale, 1.0f / 3.0f) || !near(g.offset, -1.0f / 3.0f) || g.colour != colour) return fail(4);   // ratio^2 = 0.25
    Setup(t, s, 3.0f, 4.0f, colour);                                    // 0.5625 -> 1/(1.7778-1) = 1.2857
    if (!near(g.scale, 1.0f / (1.0f / 0.5625f - 1.0f))) return fail(5);
    std::printf("00b47390_TEST PASS\n");
    return 0;
}
