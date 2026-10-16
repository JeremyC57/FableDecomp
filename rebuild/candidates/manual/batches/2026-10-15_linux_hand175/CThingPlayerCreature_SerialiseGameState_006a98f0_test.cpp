#include <cstdio>
struct CThingCreatureDef { float DefaultMaxHealth; };
struct CPersistContext { long m_mode; };
static bool g_baseResult = true;
struct CThingCreatureBase {
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;
    unsigned char Drawable : 1;
    unsigned char _bits : 6;
    float MaxHealth;
    float Health;
    const CThingCreatureDef* PDef;
    bool SerialiseGameState(CPersistContext&) { return g_baseResult; }
};
static const float HEALTH_UNSET = 0.0f;
struct CThingPlayerCreature : CThingCreatureBase {
    bool SerialiseGameState(CPersistContext& context);
};
bool CThingPlayerCreature::SerialiseGameState(CPersistContext& context)
{
    if (CThingCreatureBase::SerialiseGameState(context)) {
        if (context.m_mode != 1 && context.m_mode != 3) {
            float maxHealth = PDef->DefaultMaxHealth;
            MaxHealth = maxHealth;
            if (Health == HEALTH_UNSET)
                Health = maxHealth;
            Drawable = 1;
        }
        return true;
    }
    return false;
}
static int Fail() { std::printf("006a98f0_TEST FAIL\n"); return 1; }
int main()
{
    CThingCreatureDef def; def.DefaultMaxHealth = 50.0f;
    CThingPlayerCreature p; p.PDef = &def; p.Drawable = 0; p.DeletedOnOwnerLevelOutOfScope = 0; p._bits = 0;
    CPersistContext c;
    p.MaxHealth = 1.0f; p.Health = 0.0f; c.m_mode = 0;
    if (!p.SerialiseGameState(c) || p.MaxHealth != 50.0f || p.Health != 50.0f || p.Drawable != 1) return Fail();
    p.MaxHealth = 1.0f; p.Health = 7.0f; p.Drawable = 0;
    if (!p.SerialiseGameState(c) || p.MaxHealth != 50.0f || p.Health != 7.0f || p.Drawable != 1) return Fail();
    p.MaxHealth = 1.0f; p.Drawable = 0; c.m_mode = 3;
    if (!p.SerialiseGameState(c) || p.MaxHealth != 1.0f || p.Drawable != 0) return Fail();
    c.m_mode = 1;
    if (!p.SerialiseGameState(c) || p.MaxHealth != 1.0f) return Fail();
    g_baseResult = false; c.m_mode = 0;
    if (p.SerialiseGameState(c) || p.MaxHealth != 1.0f) return Fail();
    std::printf("006a98f0_TEST PASS\n");
    return 0;
}
