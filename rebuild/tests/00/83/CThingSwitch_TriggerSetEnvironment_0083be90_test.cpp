// Behaviour test for CThingSwitch::TriggerSetEnvironment (0x0083BE90): the theme transition fires only for a living hero
// inside the half-open trigger square and a resolvable theme def.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class C2DVector { public: float X, Y; C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { } };
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};
static long g_theme; static float g_time; static int g_transitions;
class CThing { public: C3DVector Position; bool Dead; };
class CThingSwitch { public:
    C3DVector Pos; float TriggerRadius; long ThemeIndex; float TimeToChangeEnvironmentDef; CThing* Hero;
    const C3DVector& GetPosition() const { return Pos; }
    void TriggerSetEnvironment()
    {
        C2DBoxF area;
        area.TLX = GetPosition().X - TriggerRadius;
        area.TLY = GetPosition().Y - TriggerRadius;
        area.BRX = GetPosition().X + TriggerRadius;
        area.BRY = GetPosition().Y + TriggerRadius;
        CThing* hero = Hero;
        if (hero && !hero->Dead && area.ContainsPoint(C2DVector(hero->Position))) {
            long theme = ThemeIndex;
            if (theme) { g_theme = theme; g_time = TimeToChangeEnvironmentDef; ++g_transitions; }
        }
    }
};

int main()
{
    CThing hero = { { 10.5f, 20.0f, 0 }, false };
    CThingSwitch s = { { 10.0f, 20.0f, 5.0f }, 1.0f, 7, 2.5f, &hero };
    s.TriggerSetEnvironment();
    bool ok = g_transitions == 1 && g_theme == 7 && g_time == 2.5f;
    hero.Position.X = 11.0f; s.TriggerSetEnvironment(); ok = ok && g_transitions == 1;   // max edge excluded
    hero.Position.X = 9.0f; s.TriggerSetEnvironment(); ok = ok && g_transitions == 2;    // min edge included
    hero.Dead = true; s.TriggerSetEnvironment(); ok = ok && g_transitions == 2;
    hero.Dead = false; s.ThemeIndex = 0; s.TriggerSetEnvironment(); ok = ok && g_transitions == 2;
    s.ThemeIndex = 3; s.Hero = 0; s.TriggerSetEnvironment(); ok = ok && g_transitions == 2;
    if (!ok) { std::printf("0083be90_TEST FAIL\n"); return 1; }
    std::printf("0083be90_TEST PASS\n");
    return 0;
}