// Behaviour test for CShaderPointLight::ConvertToGenericLight (0x00F39D90).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderLightAttenuation { float Attenuation0, Attenuation1, Attenuation2, AttenuationBase; };
enum EParameterType { PARAMETER_TYPE_NONE = 0, PARAMETER_TYPE_DIRECTION = 1, PARAMETER_TYPE_POSITION = 2 };
struct CShaderGenericLight { C4DVector Parameters[2]; EParameterType Types[2]; float Attenuation2; float AttenuationBase; unsigned long Size; };
struct CShaderPointLight {
    C3DVector Direction, Position; CRGBFloatColour Colour; float Intensity; CShaderLightAttenuation Attenuation;
    virtual ~CShaderPointLight() {}
    void ConvertToGenericLight(CShaderGenericLight& light) const
    {
        light.Size = 2;
        light.Parameters[0].X = Position.X; light.Parameters[0].Y = Position.Y; light.Parameters[0].Z = Position.Z; light.Parameters[0].W = 0.0f;
        light.Parameters[1].X = Colour.R; light.Parameters[1].Y = Colour.G; light.Parameters[1].Z = Colour.B; light.Parameters[1].W = Colour.A;
        light.Attenuation2 = 1.0f / Attenuation.Attenuation2;
        light.AttenuationBase = Attenuation.AttenuationBase;
        light.Types[0] = PARAMETER_TYPE_POSITION; light.Types[1] = PARAMETER_TYPE_NONE;
    }
};
int main()
{
    CShaderPointLight p;
    p.Direction.X = p.Direction.Y = p.Direction.Z = 5.0f;
    p.Position.X = 1.0f; p.Position.Y = 2.0f; p.Position.Z = 3.0f;
    p.Colour.R = 0.25f; p.Colour.G = 0.5f; p.Colour.B = 0.75f; p.Colour.A = 1.0f;
    p.Attenuation.Attenuation0 = 0.0f; p.Attenuation.Attenuation1 = 0.0f; p.Attenuation.Attenuation2 = 4.0f; p.Attenuation.AttenuationBase = 0.125f;
    CShaderGenericLight g;
    p.ConvertToGenericLight(g);
    if (g.Size != 2 || g.Parameters[0].X != 1.0f || g.Parameters[0].Z != 3.0f || g.Parameters[0].W != 0.0f || g.Parameters[1].Z != 0.75f ||
        g.Attenuation2 != 0.25f || g.AttenuationBase != 0.125f || g.Types[0] != PARAMETER_TYPE_POSITION || g.Types[1] != PARAMETER_TYPE_NONE) { std::printf("00f39d90_TEST FAIL\n"); return 1; }
    std::printf("00f39d90_TEST PASS\n");
    return 0;
}