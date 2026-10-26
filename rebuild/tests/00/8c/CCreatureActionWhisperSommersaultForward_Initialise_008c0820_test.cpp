// Behaviour test for 0x008C0820 CCreatureAction_WhisperSommersaultForward::Initialise: animation named, base run, Landed
// cleared, a CTCSolidToThing added to the creature and told not to be solid to the combat target.
#include <cstdio>
#include <cstring>
static int g_base = 0; static char g_anim[64]; static char g_added[64];
class CThing;
struct CSolid { CThing* m_notSolidTo; void AddThingNotSolidTo(CThing* t) { m_notSolidTo = t; } };
static CSolid g_solid;
class CThing { public: CSolid* AddTC(const char* n, bool, void*) { std::strcpy(g_added, n); return &g_solid; } };
struct CAction {
    CThing* m_creature; CThing* m_target; bool Landed;
    void SetAnimationName(const char* n) { std::strcpy(g_anim, n); }
    void BaseInitialise() { ++g_base; }
    void Initialise()
    {
        SetAnimationName("MOVEMENT_LONG_FORWARD");
        BaseInitialise();
        Landed = false;
        CSolid* solid = m_creature->AddTC("CTCSolidToThing", false, 0);
        solid->AddThingNotSolidTo(m_target);
    }
};
int main()
{
    CThing creature, target;
    CAction a; a.m_creature = &creature; a.m_target = &target; a.Landed = true; g_solid.m_notSolidTo = 0;
    a.Initialise();
    if (g_base != 1 || a.Landed || std::strcmp(g_anim, "MOVEMENT_LONG_FORWARD") || std::strcmp(g_added, "CTCSolidToThing") || g_solid.m_notSolidTo != &target)
        { std::printf("008c0820_TEST FAIL\n"); return 1; }
    std::printf("008c0820_TEST PASS\n");
    return 0;
}