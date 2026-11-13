// Behaviour test for 0x00B4AC10 CEnginePrimitiveRenderer2D::CEnginePrimitiveRenderer2D: a copy of the ctor's data logic
// (FOV2D from the def, light vector, colours, 2D layer tables) over a plain model, with a fake counted-pointer registration list.
#include <cstdio>
#include <cmath>
struct Vec { float X, Y, Z; void AccurateNormalise() { float m = std::sqrt(X*X + Y*Y + Z*Z); X /= m; Y /= m; Z /= m; } };
struct Col { float R, G, B, A; };
struct Def { float FOV_2D; };
static const float g_TwoPi = 6.2831855f;
struct Renderer2D {
    int managers[5]; int count;
    float FOV2D; Col Ambient, Diffuse, Back; Vec LightVector;
    float Layer2DDistances[83]; float Layer2DSortDistances[83]; bool Enable2DLayer[83];
    void Add(int m) { managers[count++] = m; }
    void Init(const Def* def)
    {
        count = 0;
        for (int m = 0; m < 5; ++m) Add(m);
        FOV2D = g_TwoPi * (def->FOV_2D * (1.0f / 360.0f));
        if (FOV2D <= 0.0f) FOV2D = 0.78539819f;
        Vec& light = LightVector; light.X = 1.0f; light.Y = 1.0f; light.Z = 1.0f; light.AccurateNormalise();
        Col& a = Ambient; a.R = 0.5f; a.G = 0.5f; a.B = 0.5f; a.A = 1.0f;
        Col& d = Diffuse; d.R = 0.5f; d.G = 0.5f; d.B = 0.5f; d.A = 1.0f;
        Col& b = Back; b.R = 0.0f; b.G = 0.0f; b.B = 0.0f; b.A = 1.0f;
        float depth = 0.1f;
        for (long i = 0; i < 83; ++i) {
            if (i == 72) { Layer2DDistances[i] = depth + 0.40499997f; depth += 0.80999994f; }
            else { Layer2DDistances[i] = depth + 0.00054878066f; depth += 0.0010975613f; }
            Enable2DLayer[i] = true;
            Layer2DSortDistances[i] = (float)i;
        }
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Renderer2D r; Def d; d.FOV_2D = 90.0f; r.Init(&d);
    bool ok = r.count == 5 && near(r.FOV2D, 1.5707964f) && near(r.LightVector.X, 0.57735f) && near(r.LightVector.Z, 0.57735f)
        && r.Ambient.R == 0.5f && r.Diffuse.A == 1.0f && r.Back.G == 0.0f && r.Back.A == 1.0f
        && near(r.Layer2DDistances[0], 0.10054878f) && near(r.Layer2DDistances[1], 0.10164634f)
        && near(r.Layer2DDistances[72], 0.1f + 72 * 0.0010975613f + 0.40499997f)
        && near(r.Layer2DDistances[73], 0.1f + 72 * 0.0010975613f + 0.80999994f + 0.00054878066f)
        && r.Layer2DSortDistances[82] == 82.0f && r.Enable2DLayer[0] && r.Enable2DLayer[82];
    Def z; z.FOV_2D = 0.0f; r.Init(&z);
    ok = ok && r.FOV2D == 0.78539819f;
    Def n; n.FOV_2D = -10.0f; r.Init(&n);
    ok = ok && r.FOV2D == 0.78539819f;
    if (!ok) { std::printf("00b4ac10_TEST FAIL\n"); return 1; }
    std::printf("00b4ac10_TEST PASS\n");
    return 0;
}