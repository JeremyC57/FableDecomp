// Behaviour test for CShaderDirectionalLight::ConvertToGenericLight (0x00F39D40).
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C4DVector { float X, Y, Z, W; };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderLightAttenuation { float Attenuation0, Attenuation1, Attenuation2, AttenuationBase; };
enum EParameterType { PARAMETER_TYPE_NONE = 0, PARAMETER_TYPE_DIRECTION = 1, PARAMETER_TYPE_POSITION = 2 };
struct CShaderGenericLight { C4DVector Parameters[2]; EParameterType Types[2]; float Attenuation2; float AttenuationBase; unsigned long Size; };
struct CShaderDirectionalLight {
    C3DVector Direction, Position; CRGBFloatColour Colour, BackColour;
    virtual ~CShaderDirectionalLight() {}
    void ConvertToGenericLight(CShaderGenericLight& light) const
    {
        light.Size = 2;
        light.Parameters[0].X = Direction.X; light.Parameters[0].Y = Direction.Y; light.Parameters[0].Z = Direction.Z; light.Parameters[0].W = 0.0f;
        light.Parameters[1].X = Colour.R; light.Parameters[1].Y = Colour.G; light.Parameters[1].Z = Colour.B; light.Parameters[1].W = Colour.A;
        light.Types[0] = PARAMETER_TYPE_DIRECTION; light.Types[1] = PARAMETER_TYPE_NONE;
    }
};
int main()
{
    CShaderDirectionalLight d;
    d.Direction.X = 0.0f; d.Direction.Y = -1.0f; d.Direction.Z = 0.5f;
    d.Position.X = d.Position.Y = d.Position.Z = 9.0f;
    d.Colour.R = 0.1f; d.Colour.G = 0.2f; d.Colour.B = 0.3f; d.Colour.A = 1.0f;
    CShaderGenericLight g; g.Attenuation2 = 7.0f; g.AttenuationBase = 8.0f;
    d.ConvertToGenericLight(g);
    if (g.Size != 2 || g.Parameters[0].Y != -1.0f || g.Parameters[0].Z != 0.5f || g.Parameters[0].W != 0.0f || g.Parameters[1].X != 0.1f || g.Parameters[1].W != 1.0f ||
        g.Types[0] != PARAMETER_TYPE_DIRECTION || g.Types[1] != PARAMETER_TYPE_NONE || g.Attenuation2 != 7.0f || g.AttenuationBase != 8.0f) { std::printf("00f39d40_TEST FAIL\n"); return 1; }
    std::printf("00f39d40_TEST PASS\n");
    return 0;
}