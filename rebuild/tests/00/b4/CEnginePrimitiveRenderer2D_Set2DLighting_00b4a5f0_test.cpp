// Behaviour test for CEnginePrimitiveRenderer2D::Set2DLighting @ 0x00B4A5F0
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); float r = 1.0f / l; X *= r; Y *= r; Z *= r; }
};
class CRGBFloatColour { public: float R, G, B, A; };
class CEnginePrimitiveRenderer2D { public:
    CRGBFloatColour AmbientLightColour, DiffuseLightColour, BackLightColour;
    C3DVector LightVector;
    void Set2DLighting(const C3DVector& lightVector, const CRGBFloatColour& ambient, const CRGBFloatColour& diffuse, const CRGBFloatColour& back)
    {
        AmbientLightColour = ambient; DiffuseLightColour = diffuse; BackLightColour = back;
        LightVector = lightVector; LightVector.Normalise();
    }
};
int main()
{
    CEnginePrimitiveRenderer2D r;
    C3DVector dir = { 0, 3, 4 };
    CRGBFloatColour a = { 0.1f, 0.2f, 0.3f, 1 }, d = { 1, 1, 1, 1 }, b = { 0.5f, 0, 0, 0.25f };
    r.Set2DLighting(dir, a, d, b);
    if (r.AmbientLightColour.G != 0.2f || r.DiffuseLightColour.A != 1 || r.BackLightColour.A != 0.25f ||
        std::fabs(r.LightVector.Y - 0.6f) > 1e-6f || std::fabs(r.LightVector.Z - 0.8f) > 1e-6f || r.LightVector.X != 0 || dir.Y != 3) {
        std::printf("00b4a5f0_TEST FAIL\n"); return 1;
    }
    std::printf("00b4a5f0_TEST PASS\n");
    return 0;
}