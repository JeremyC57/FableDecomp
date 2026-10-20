// Behaviour test for 006d6870: body copied from src/006d6870.cpp (CTCHeroExperience::GetMeleeStrikeAnimationSpeedMultiplier), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_destroyed = 0;
struct CDefObject { virtual void Unused() { } virtual void Destroy() { ++g_destroyed; } long m_count; };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_count; }
    ~CDefPointer() { if (Object) if (--Object->m_count == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CFloatRange { float HighValue; float LowValue; };
class CTCHeroExperience;
static int g_param = 0; static const CTCHeroExperience* g_paramThis = 0; static float g_fraction = 0.25f;
struct CHeroAbilityStatWeight {
    unsigned char StatWeights[0xc];
    float GetParameter(const CTCHeroExperience& experience) const { ++g_param; g_paramThis = &experience; return g_fraction; }
};
struct CHeroCombatDef : CDefObject {
    unsigned char _pad[0xf0 - 0xc];
    CFloatRange MeleeStrikeAnimationSpeedFractionWithStats;   // +0xf0
    CHeroAbilityStatWeight MeleeStrikeAnimationSpeedWeighting; // +0xf8
};
class CTCHeroExperience {
public:
    unsigned char _pad[0x58];
    CDefPointer<CHeroCombatDef> PHeroCombatDef;
    float GetMeleeStrikeAnimationSpeedMultiplier() const;
};
float CTCHeroExperience::GetMeleeStrikeAnimationSpeedMultiplier() const
{
    CDefPointer<CHeroCombatDef> combat = PHeroCombatDef;
    float fraction = combat->MeleeStrikeAnimationSpeedWeighting.GetParameter(*this);
    float low = PHeroCombatDef->MeleeStrikeAnimationSpeedFractionWithStats.LowValue;
    return low + (PHeroCombatDef->MeleeStrikeAnimationSpeedFractionWithStats.HighValue - low) * fraction;
}
int main()
{
    CHeroCombatDef def; def.m_count = 1; def.MeleeStrikeAnimationSpeedFractionWithStats.LowValue = 1.0f; def.MeleeStrikeAnimationSpeedFractionWithStats.HighValue = 3.0f;
    CTCHeroExperience exp; exp.PHeroCombatDef.Object = &def;
    float r = exp.GetMeleeStrikeAnimationSpeedMultiplier();
    CHECK(g_param == 1 && g_paramThis == &exp && r == 1.5f && def.m_count == 1 && g_destroyed == 0);
    g_fraction = 1.0f; r = exp.GetMeleeStrikeAnimationSpeedMultiplier();
    CHECK(r == 3.0f && def.m_count == 1);
    def.m_count = 0; g_fraction = 0.0f;                   // the temporary copy's release reaches zero -> Destroy
    exp.PHeroCombatDef.Object = &def; r = exp.GetMeleeStrikeAnimationSpeedMultiplier();
    CHECK(r == 1.0f && g_destroyed == 1);
    exp.PHeroCombatDef.Object = 0;
    if (fails == 0) printf("006d6870_TEST PASS\n"); else printf("006d6870_TEST FAIL (%d)\n", fails);
    return fails;
}