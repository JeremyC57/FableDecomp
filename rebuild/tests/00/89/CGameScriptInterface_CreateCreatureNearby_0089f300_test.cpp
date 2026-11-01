// Behaviour test for CGameScriptInterface::CreateCreatureNearby (0x0089F300): a copy of the model's logic over stubs.
#include <stdio.h>
class C3DVector { public: float X, Y, Z; };
static float g_lastRange; static int g_resets; static int g_navOk;
static float GFFloatRandom(float range, unsigned long*) { return range * 0.5f; }
class CTCCreatureNavigation { public:
    bool GetNearbyWanderPosition(const C3DVector& p, float range, C3DVector& out, bool) const
    { g_lastRange = range; out.X = p.X + range; out.Y = p.Y; out.Z = p.Z; return g_navOk != 0; }
};
class CThing { public:
    bool alive; bool hasNav; C3DVector pos; CTCCreatureNavigation nav;
    bool HasTC(long) const { return hasNav; }
    const CTCCreatureNavigation* GetTC(long) const { return &nav; }
    const C3DVector& GetPos() const { return pos; }
    void ResetPosition(const C3DVector& p) { pos = p; ++g_resets; }
};
class CScriptThing { public:
    CThing* thing;
    bool IsAlive() const { return thing && thing->alive; }
    CThing* GetPThing() const { return thing; }
};
static CScriptThing CreateCreatureNearby(CThing* created, float radius)
{
    CScriptThing creature = { created };
    if (creature.IsAlive()) {
        CThing* thing = creature.GetPThing();
        if (thing->HasTC(7)) {
            const CTCCreatureNavigation* navigation = thing->GetTC(7);
            float range = 0.8f;
            unsigned long seed = 0;
            if (radius > 0.8f)
                range = GFFloatRandom(radius - 0.8f, &seed) + 0.8f;
            C3DVector wander;
            if (navigation->GetNearbyWanderPosition(creature.GetPThing()->GetPos(), range, wander, false))
                creature.GetPThing()->ResetPosition(wander);
        }
    }
    return creature;
}
int main()
{
    int ok = 1;
    CThing t; t.alive = true; t.hasNav = true; t.pos.X = 1.0f; t.pos.Y = 2.0f; t.pos.Z = 3.0f;
    g_navOk = 1; g_resets = 0;
    CScriptThing r = CreateCreatureNearby(&t, 0.5f);
    ok &= r.thing == &t && g_lastRange == 0.8f && g_resets == 1 && t.pos.X == 1.8f;
    CreateCreatureNearby(&t, 2.8f);
    ok &= g_lastRange > 1.79f && g_lastRange < 1.81f && g_resets == 2;
    g_navOk = 0; CreateCreatureNearby(&t, 2.8f); ok &= g_resets == 2;
    g_navOk = 1; t.hasNav = false; CreateCreatureNearby(&t, 2.8f); ok &= g_resets == 2;
    t.hasNav = true; t.alive = false; CreateCreatureNearby(&t, 2.8f); ok &= g_resets == 2;
    printf(ok ? "0089f300_TEST PASS\n" : "0089f300_TEST FAIL\n");
    return ok ? 0 : 1;
}