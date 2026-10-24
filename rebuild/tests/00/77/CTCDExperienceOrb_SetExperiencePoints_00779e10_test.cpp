// Behaviour test for 0x00779E10 CTCDExperienceOrb::SetExperiencePoints(points) -- threshold-map scale lookup
#include <cstdio>

struct Entry { long Threshold; float Scale; };   // the def's map<long, float>, walked in key order

static float g_scaleSet = -1.0f;
struct Orb {
    long ExperiencePoints;
    const Entry* Scales;
    int ScaleCount;
    void SetExperiencePoints(long points)
    {
        ExperiencePoints = points;
        float scale = 1.0f;
        for (int i = 0; i != ScaleCount; ++i) {
            if (points < Scales[i].Threshold)
                break;
            scale = Scales[i].Scale;
        }
        g_scaleSet = scale;          // CTCGraphicAppearance::SetScale
    }
};

int main()
{
    const Entry table[3] = { { 10, 1.5f }, { 50, 2.0f }, { 200, 3.0f } };
    Orb orb = { 0, table, 3 };
    orb.SetExperiencePoints(5);
    if (orb.ExperiencePoints != 5 || g_scaleSet != 1.0f) { std::printf("00779e10_TEST FAIL\n"); return 1; }
    orb.SetExperiencePoints(10);
    if (g_scaleSet != 1.5f) { std::printf("00779e10_TEST FAIL\n"); return 1; }
    orb.SetExperiencePoints(199);
    if (g_scaleSet != 2.0f) { std::printf("00779e10_TEST FAIL\n"); return 1; }
    orb.SetExperiencePoints(5000);
    if (orb.ExperiencePoints != 5000 || g_scaleSet != 3.0f) { std::printf("00779e10_TEST FAIL\n"); return 1; }
    Orb bare = { 0, table, 0 };
    bare.SetExperiencePoints(99);
    if (g_scaleSet != 1.0f) { std::printf("00779e10_TEST FAIL\n"); return 1; }
    std::printf("00779e10_TEST PASS\n");
    return 0;
}