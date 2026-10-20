// Behaviour test for CThingAICreature::InitialiseMovementSpeeds (0x00830DD0): the flying speed is initialised from
// STANDARD_FLY when the creature has that animation in any group, else zeroed; all four speeds are randomised only
// when the def's speed variation is above the epsilon.
#include <cstdio>
#include <cstring>
static int g_strings = 0;
class CCharString {
public:
    char* PStringData;
    CCharString(const char* text, long length) { ++g_strings; PStringData = new char[std::strlen(text) + 1]; std::strcpy(PStringData, text); (void)length; }
    ~CCharString() { --g_strings; delete[] PStringData; }
};
static int g_filtersAlive = 0;
struct CAnimFilterBase { virtual ~CAnimFilterBase() { --g_filtersAlive; } virtual int Kind() const { return 0; } CAnimFilterBase() { ++g_filtersAlive; } };
struct CAnimFilterAnyGroup : CAnimFilterBase { virtual int Kind() const { return 1; } };
static const float g_epsilon_0129ba3c = 1e-4f;
struct CCreatureSpeedSource { unsigned char _pad00[0xb0]; float SpeedVariation; };
static bool g_hasFly = false; static int g_hasAnimCalls = 0, g_initCalls = 0, g_randomCalls = 0; static float* g_randomised[8]; static int g_hasAnimFilterKind = -1;
struct CThingAICreature {
    unsigned char _pad00[0xe0]; CCreatureSpeedSource* m_pSpeedSource; unsigned char _pade4[0x1b8 - 0xe4];
    float MaxWalkingSpeed; float MaxJoggingSpeed; float MaxRunningSpeed; float MaxFlyingSpeed;
    bool HasAnim(const CCharString& name, const CAnimFilterBase& filter) { ++g_hasAnimCalls; g_hasAnimFilterKind = filter.Kind(); return std::strcmp(name.PStringData, "STANDARD_FLY") == 0 && g_hasFly; }
    void InitialiseMovementSpeed(float* speed, const CCharString& name) { ++g_initCalls; if (!std::strcmp(name.PStringData, "STANDARD_FLY")) *speed = 7.5f; }
    void RandomiseMovementSpeed(float* speed) { g_randomised[g_randomCalls++] = speed; *speed += 1.0f; }
    void InitialiseMovementSpeeds();
};
void CThingAICreature::InitialiseMovementSpeeds()
{
    bool hasFlyAnim;
    {
        CCharString flyAnim("STANDARD_FLY", -1);
        CAnimFilterAnyGroup anyGroup;
        hasFlyAnim = HasAnim(flyAnim, anyGroup);
    }
    if (hasFlyAnim) {
        CCharString flyAnim("STANDARD_FLY", -1);
        InitialiseMovementSpeed(&MaxFlyingSpeed, flyAnim);
    } else {
        MaxFlyingSpeed = 0.0f;
    }
    if (m_pSpeedSource->SpeedVariation > g_epsilon_0129ba3c) {
        RandomiseMovementSpeed(&MaxWalkingSpeed);
        RandomiseMovementSpeed(&MaxJoggingSpeed);
        RandomiseMovementSpeed(&MaxRunningSpeed);
        RandomiseMovementSpeed(&MaxFlyingSpeed);
    }
}
static int fail(const char* why) { std::printf("00830dd0_TEST FAIL: %s\n", why); return 1; }
int main()
{
    static CCreatureSpeedSource source; static CThingAICreature creature; creature.m_pSpeedSource = &source;
    creature.MaxWalkingSpeed = 1.0f; creature.MaxJoggingSpeed = 2.0f; creature.MaxRunningSpeed = 3.0f; creature.MaxFlyingSpeed = 99.0f;
    // 1. no fly animation, no variation: flying speed zeroed, nothing randomised
    source.SpeedVariation = 0.0f; creature.InitialiseMovementSpeeds();
    if (creature.MaxFlyingSpeed != 0.0f || g_hasAnimCalls != 1 || g_hasAnimFilterKind != 1 || g_initCalls != 0 || g_randomCalls != 0) return fail("no fly, no variation");
    if (creature.MaxWalkingSpeed != 1.0f || g_strings != 0 || g_filtersAlive != 0) return fail("untouched speeds / temporaries");
    // 2. fly animation present with variation: initialised then all four randomised (flying last)
    g_hasFly = true; source.SpeedVariation = 0.5f; creature.InitialiseMovementSpeeds();
    if (g_initCalls != 1 || creature.MaxFlyingSpeed != 8.5f || g_randomCalls != 4) return fail("fly + variation");
    if (g_randomised[0] != &creature.MaxWalkingSpeed || g_randomised[1] != &creature.MaxJoggingSpeed || g_randomised[2] != &creature.MaxRunningSpeed || g_randomised[3] != &creature.MaxFlyingSpeed) return fail("randomise order");
    if (creature.MaxWalkingSpeed != 2.0f || creature.MaxJoggingSpeed != 3.0f || creature.MaxRunningSpeed != 4.0f) return fail("randomised values");
    // 3. variation exactly at the epsilon: no randomising
    source.SpeedVariation = 1e-4f; creature.InitialiseMovementSpeeds();
    if (g_randomCalls != 4 || creature.MaxFlyingSpeed != 7.5f) return fail("epsilon boundary");
    if (g_strings != 0 || g_filtersAlive != 0) return fail("leak");
    std::printf("00830dd0_TEST PASS\n");
    return 0;
}