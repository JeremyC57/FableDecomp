// Behaviour test for 0x0056ED74 CTCInventoryExperience::GetExperienceStatColour: stat bands 0..2 red
// (1, 0.3, 0.3, 1), 3..5 yellow (1, 1, 0.3, 1), anything else (negative too) blue (0.3, 0.3, 1, 1).
#include <cstdio>

struct CRGBFloatColour {
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
struct CTCInventoryExperience {
    CRGBFloatColour GetExperienceStatColour(EHeroTrainableStatType stat)
    {
        if (stat >= 0 && stat <= 2)
            return CRGBFloatColour(1.0f, 0.3f, 0.3f, 1.0f);
        if (stat >= 3 && stat <= 5)
            return CRGBFloatColour(1.0f, 1.0f, 0.3f, 1.0f);
        return CRGBFloatColour(0.3f, 0.3f, 1.0f, 1.0f);
    }
};

static bool Is(const CRGBFloatColour& c, float r, float g, float b, float a)
{
    return c.R == r && c.G == g && c.B == b && c.A == a;
}

int main()
{
    CTCInventoryExperience e;
    bool ok = true;
    for (int s = -2; s <= 8; ++s) {
        CRGBFloatColour c = e.GetExperienceStatColour((EHeroTrainableStatType)s);
        if (s >= 0 && s <= 2) ok = ok && Is(c, 1.0f, 0.3f, 0.3f, 1.0f);
        else if (s >= 3 && s <= 5) ok = ok && Is(c, 1.0f, 1.0f, 0.3f, 1.0f);
        else ok = ok && Is(c, 0.3f, 0.3f, 1.0f, 1.0f);
    }
    std::printf(ok ? "0056ed74_TEST PASS\n" : "0056ed74_TEST FAIL\n");
    return ok ? 0 : 1;
}