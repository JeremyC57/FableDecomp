// Behaviour test for 0x00B49320 CShaderPointLight constructor.
#include <cstdio>
struct C3DVector { float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
struct CRGBFloatColour { float R, G, B, A; };
struct CShaderLightAttenuation { float Attenuation0, Attenuation1, Attenuation2, AttenuationBase; };
struct CShaderPointLight {
    C3DVector Direction; C3DVector Position; CRGBFloatColour Colour; float Intensity; CShaderLightAttenuation Attenuation;
    CShaderPointLight(const C3DVector& p, const CRGBFloatColour& c, float i, const CShaderLightAttenuation& a) : Position(p), Colour(c), Intensity(i), Attenuation(a) {}
};
int main()
{
    CRGBFloatColour c = { 0.1f, 0.2f, 0.3f, 1.0f }; CShaderLightAttenuation a = { 1.0f, 0.5f, 0.25f, 2.0f };
    CShaderPointLight l(C3DVector(1, 2, 3), c, 4.0f, a);
    if (l.Position.Y != 2 || l.Colour.B != 0.3f || l.Intensity != 4.0f || l.Attenuation.Attenuation2 != 0.25f || l.Attenuation.AttenuationBase != 2.0f) {
        std::printf("00b49320_TEST FAIL\n"); return 1;
    }
    std::printf("00b49320_TEST PASS\n");
    return 0;
}
