// behaviour test for 0x008D03E0 CGameScriptThing::GetFocalPos: the appearance's focal position (scale 1.0) of a live thing, else the stored position
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Appearance { Vec focal; float lastScale; };
struct Thing { Appearance* appearance; bool dead; };
struct ScriptThing { Thing* thing; Vec pos; };
static Vec FocalOf(Appearance* a, float scale) { a->lastScale = scale; Vec v = a->focal; v.X *= scale; return v; }
static Vec GetFocalPos(const ScriptThing& s)
{
    if (s.thing) {
        if (!s.thing->dead)
            return FocalOf(s.thing->appearance, 1.0f);
    }
    return s.pos;
}
int main()
{
    Appearance a = { { 1.0f, 2.0f, 3.0f }, 0.0f };
    Thing t = { &a, false };
    ScriptThing s = { &t, { 7.0f, 8.0f, 9.0f } };
    Vec v = GetFocalPos(s);
    if (v.X != 1.0f || v.Y != 2.0f || v.Z != 3.0f || a.lastScale != 1.0f) { std::printf("008d03e0_TEST FAIL\n"); return 1; }
    t.dead = true; v = GetFocalPos(s);
    if (v.X != 7.0f || v.Z != 9.0f) { std::printf("008d03e0_TEST FAIL\n"); return 1; }
    s.thing = 0; v = GetFocalPos(s);
    if (v.Y != 8.0f) { std::printf("008d03e0_TEST FAIL\n"); return 1; }
    std::printf("008d03e0_TEST PASS\n");
    return 0;
}