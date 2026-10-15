// Behaviour test for 0x0078BE70 CTCBerserk::GetSuperRumble: each output is the def's super value plus the
// component's own rumble value.
#include <cstdio>

struct CSpecialAbilitiesBerserkDef { float SuperRumbleLevel, SuperRumbleSmoothness, SuperRumbleAttack, SuperRumbleDecay; };
struct CTCBerserk {
    float RumbleLevel, RumbleSmoothness, RumbleAttack, RumbleDecay;
    CSpecialAbilitiesBerserkDef* PDef;
    void GetSuperRumble(float& level, float& smoothness, float& attack, float& decay)
    {
        level = PDef->SuperRumbleLevel + RumbleLevel;
        smoothness = PDef->SuperRumbleSmoothness + RumbleSmoothness;
        attack = PDef->SuperRumbleAttack + RumbleAttack;
        decay = PDef->SuperRumbleDecay + RumbleDecay;
    }
};

int main()
{
    CSpecialAbilitiesBerserkDef def = { 1.0f, 2.0f, 3.0f, 4.0f };
    CTCBerserk b;
    b.RumbleLevel = 0.5f; b.RumbleSmoothness = 0.25f; b.RumbleAttack = 8.0f; b.RumbleDecay = -1.0f; b.PDef = &def;
    float l = 0, s = 0, a = 0, d = 0;
    b.GetSuperRumble(l, s, a, d);
    if (l != 1.5f || s != 2.25f || a != 11.0f || d != 3.0f) { std::printf("0078be70_TEST FAIL\n"); return 1; }
    std::printf("0078be70_TEST PASS\n");
    return 0;
}