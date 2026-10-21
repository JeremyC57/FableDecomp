// Behaviour test for CThingPhysical::SerialiseGameState (0x006A5AF0): reset outside modes 1/3, Health transferred with
// MaxHealth as the default and clamped for non-creatures, Scale transferred with default 1.
#include <cstdio>
#include <cstring>

struct CThingBaseDef { bool Damageable; float Health; };
struct CPersistContext {
    long Mode; bool UseDefaults;
    void Transfer(const char*, float& value, const float& def) { if (UseDefaults) value = def; }
};
struct CThingPhysical {
    unsigned char ThingClass; CThingBaseDef* PBaseDef; bool Drawable;
    float Scale; void* PLastCreator; void* POriginalCreator; float MaxHealth, Health; long CanCome;
    bool Damageable, Killable, RespondToHits, EvadeAllHits, NegateAllHits, AbleToBeEngagedInCombat;
    bool SerialiseGameState(CPersistContext& context)
    {
        if (context.Mode != 1 && context.Mode != 3) {
            MaxHealth = PBaseDef->Health;
            CanCome = -1;
            PLastCreator = 0; POriginalCreator = 0;
            Drawable = true;
            EvadeAllHits = false; NegateAllHits = false; Killable = true; RespondToHits = true;
            Damageable = PBaseDef->Damageable;
            AbleToBeEngagedInCombat = true;
        }
        context.Transfer("Health", Health, MaxHealth);
        if (ThingClass != 2 && Health > MaxHealth)
            Health = MaxHealth;
        float defaultScale = 1.0f;
        context.Transfer("Scale", Scale, defaultScale);
        return true;
    }
};

int main()
{
    CThingBaseDef def = { false, 10.0f };
    CThingPhysical t; std::memset(&t, 0, sizeof(t));
    t.PBaseDef = &def; t.ThingClass = 1; t.Health = 50.0f; t.MaxHealth = 80.0f; t.Scale = 3.0f; t.PLastCreator = &t;
    CPersistContext load = { 1, false };
    bool ok = t.SerialiseGameState(load) && t.MaxHealth == 80.0f && t.Health == 50.0f && t.PLastCreator == &t;
    CPersistContext save = { 2, false };
    ok = ok && t.SerialiseGameState(save) && t.MaxHealth == 10.0f && t.Health == 10.0f && t.PLastCreator == 0 && t.Killable;
    t.ThingClass = 2; t.Health = 50.0f;
    ok = ok && t.SerialiseGameState(save) && t.Health == 50.0f;
    CPersistContext defs = { 3, true };
    ok = ok && t.SerialiseGameState(defs) && t.Health == 10.0f && t.Scale == 1.0f;
    if (!ok) { std::printf("006a5af0_TEST FAIL\n"); return 1; }
    std::printf("006a5af0_TEST PASS\n");
    return 0;
}
