// behaviour test for 0x008C9A40 AttachEffectToStaff (anonymous namespace)
extern "C" int printf(const char*, ...);
static int g_fail;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static int Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
struct Thing { bool Dead; };
static bool Alive(const Thing* t) { return t != 0 && !t->Dead; }
static Thing g_effect; static Thing g_staff; static int g_creates, g_attaches; static const char* g_helper; static long g_attachType;
struct Creature { bool HasDef, KnowsEffect, HasCarrying; Thing* Staff; };
static Thing* Create(long) { ++g_creates; return &g_effect; }
static void AttachToThing(Thing& staff, long type, const char* helper) { ++g_attaches; g_attachType = type; g_helper = helper; CHECK(&staff == &g_staff); }
static Thing* AttachEffectToStaff(Creature& c)
{
    Thing* staff = 0; Thing* effect_thing = 0;
    long effect = 0;
    if (c.HasDef && c.KnowsEffect && c.HasCarrying) {
        staff = c.Staff;
        if (Alive(staff)) effect_thing = Create(effect);
    }
    if (Alive(effect_thing)) { AttachToThing(*staff, 10, "SHOT_CREATION_DUMMY"); return effect_thing; }
    return 0;
}
int main()
{
    Creature c = { true, true, true, &g_staff };
    CHECK(AttachEffectToStaff(c) == &g_effect && g_attaches == 1 && g_attachType == 10 && Same(g_helper, "SHOT_CREATION_DUMMY"));
    c.KnowsEffect = false; CHECK(AttachEffectToStaff(c) == 0 && g_creates == 1);
    c.KnowsEffect = true; g_staff.Dead = true; CHECK(AttachEffectToStaff(c) == 0 && g_creates == 1);
    g_staff.Dead = false; g_effect.Dead = true; CHECK(AttachEffectToStaff(c) == 0 && g_creates == 2 && g_attaches == 1);
    if (g_fail == 0) printf("008c9a40_TEST PASS\n");
    return g_fail;
}