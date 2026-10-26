// Behaviour test for CTCInventoryExperience::GetExperienceAbilityColour 0x0056EDD2 (model copy)
#include <cstdio>

class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

class CTCInventoryExperience { public:
    CRGBFloatColour GetExperienceAbilityColour(long ability);
};

CRGBFloatColour CTCInventoryExperience::GetExperienceAbilityColour(long ability)
{
    long strength[7] = { 3, 16, 12, 9, 11, 18, 19 };
    long skill[5] = { 1, 5, 2, 6, 10 };
    long will[6] = { 4, 8, 13, 14, 15, 17 };
    long i;
    for (i = 0; i != 5; ++i)
        if (strength[i] == ability)
            return CRGBFloatColour(0.5f, 0.0f, 1.0f, 1.0f);
    for (i = 0; i != 5; ++i)
        if (skill[i] == ability)
            return CRGBFloatColour(0.1f, 0.1f, 0.7f, 1.0f);
    for (i = 0; i != 6; ++i)
        if (will[i] == ability)
            return CRGBFloatColour(0.5f, 0.5f, 1.0f, 1.0f);
    return CRGBFloatColour(0.0f, 0.0f, 1.0f, 1.0f);
}

static bool Is(const CRGBFloatColour& c, float r, float g, float b, float a)
{
    return c.R == r && c.G == g && c.B == b && c.A == a;
}

int main()
{
    CTCInventoryExperience e;
    if (!Is(e.GetExperienceAbilityColour(3), 0.5f, 0.0f, 1.0f, 1.0f) ||
        !Is(e.GetExperienceAbilityColour(11), 0.5f, 0.0f, 1.0f, 1.0f) ||
        !Is(e.GetExperienceAbilityColour(18), 0.0f, 0.0f, 1.0f, 1.0f) ||   // past the five-entry strength scan
        !Is(e.GetExperienceAbilityColour(10), 0.1f, 0.1f, 0.7f, 1.0f) ||
        !Is(e.GetExperienceAbilityColour(17), 0.5f, 0.5f, 1.0f, 1.0f) ||
        !Is(e.GetExperienceAbilityColour(0), 0.0f, 0.0f, 1.0f, 1.0f)) {
        std::printf("0056edd2_TEST FAIL\n"); return 1;
    }
    std::printf("0056edd2_TEST PASS\n");
    return 0;
}