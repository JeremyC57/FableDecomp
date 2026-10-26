// Behaviour test for 0x0088E620 CGameScriptInterface::EntitySetFacingAngleTowardsThing: invalid things are ignored,
// the bearing is a turn fraction in [0, 1).
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public:
    C3DVector pos; bool valid;
    const C3DVector& GetPos() const { return pos; }
    bool IsValid() const { return valid; }
};
static int g_calls = 0; static float g_angle = -5.0f; static const CScriptThing* g_entity = 0; static bool g_imm = false;
class CGameScriptInterface { public:
    void EntitySetFacingAngle(const CScriptThing& entity, float angle, bool immediate) const { ++g_calls; g_angle = angle; g_entity = &entity; g_imm = immediate; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const;
};
void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target, bool immediate) const
{
    if (target.IsValid()) {
        if (entity.IsValid()) {
            const C3DVector& from = entity.GetPos();
            const C3DVector& to = target.GetPos();
            float angle = (float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f;
            if (angle < 0.0f || angle >= 1.0f) {
                angle = (float)fmod((double)angle, 1.0);
                if (angle < 0.0f)
                    angle += 1.0f;
            }
            EntitySetFacingAngle(entity, angle, immediate);
        }
    }
}
static bool near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }
int main()
{
    CGameScriptInterface gsi;
    CScriptThing a = { { 0.0f, 0.0f, 0.0f }, true }, b = { { 10.0f, 0.0f, 0.0f }, true };
    int fail = 0;
    b.valid = false; gsi.EntitySetFacingAngleTowardsThing(a, b, true); if (g_calls) fail |= 1;
    b.valid = true; a.valid = false; gsi.EntitySetFacingAngleTowardsThing(a, b, true); if (g_calls) fail |= 2;
    a.valid = true;
    gsi.EntitySetFacingAngleTowardsThing(a, b, true);           // due +X: atan2(10, 0) = pi/2 -> 0.25
    if (g_calls != 1 || !near(g_angle, 0.25f) || g_entity != &a || !g_imm) fail |= 4;
    b.pos.X = -10.0f;                                          // due -X: -0.25 wraps to 0.75
    gsi.EntitySetFacingAngleTowardsThing(a, b, false);
    if (!near(g_angle, 0.75f) || g_imm) fail |= 8;
    b.pos.X = 0.0f; b.pos.Y = -10.0f;                          // due -Y: 0.5
    gsi.EntitySetFacingAngleTowardsThing(a, b, false);
    if (!near(g_angle, 0.5f)) fail |= 16;
    if (fail) { std::printf("0088e620_TEST FAIL\n"); return 1; }
    std::printf("0088e620_TEST PASS\n");
    return 0;
}