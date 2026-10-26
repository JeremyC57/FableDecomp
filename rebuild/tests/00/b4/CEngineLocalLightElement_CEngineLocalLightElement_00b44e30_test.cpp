// Behaviour test for CEngineLocalLightElement::CEngineLocalLightElement @ 0x00B44E30.
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
class CRGBFloatColour { public: float R, G, B, A; };
class CEngineLocalLightElement { public:
    C3DVector WorldPos; float Radius, RadiusSquared, SpotRadius; CRGBFloatColour Colour;
    float AttenuationDistSquared, AttenuationConstant; unsigned int ChannelMask;
    CEngineLocalLightElement(const C3DVector& p, float radius, float spotRadius, const CRGBFloatColour& c, unsigned int mask)
        : WorldPos(p), Radius(radius), RadiusSquared(radius * radius), SpotRadius(spotRadius), Colour(c), ChannelMask(mask)
    {
        float invRadiusSquared = 1.0f / RadiusSquared;
        AttenuationDistSquared = 1.0f / (spotRadius * spotRadius) - invRadiusSquared;
        AttenuationConstant = invRadiusSquared / AttenuationDistSquared * -1.0f;
    }
};
int main()
{
    C3DVector p = { 1, 2, 3 }; CRGBFloatColour c = { 0.1f, 0.2f, 0.3f, 1.0f };
    CEngineLocalLightElement e(p, 2.0f, 1.0f, c, 0x55);
    // 1/1 - 1/4 = 0.75 ; (1/4) / 0.75 * -1 = -1/3. Attenuation k*(1/d^2) + C is 0 at d = radius, 1 at d = spotRadius when k = 1/0.75
    int ok = e.WorldPos.X == 1 && e.WorldPos.Y == 2 && e.WorldPos.Z == 3 && e.Radius == 2 && e.RadiusSquared == 4 && e.SpotRadius == 1
          && e.Colour.G == 0.2f && e.Colour.A == 1.0f && e.ChannelMask == 0x55
          && std::fabs(e.AttenuationDistSquared - 0.75f) < 1e-6f && std::fabs(e.AttenuationConstant + 1.0f / 3.0f) < 1e-6f;
    if (!ok) { std::printf("00b44e30_TEST FAIL\n"); return 1; }
    std::printf("00b44e30_TEST PASS\n");
    return 0;
}