// Behaviour test for CAIStateGroup_JackDragon::IntoHover (0x00951D60): a portable copy of the logic with stub markers, def and
// physics, checking the route, the rotated offset, the state, terminate path and the def release on every exit.
#include <cstdio>
#include <cmath>
#include <vector>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void RotateXY(float a) { float c = std::cos(a), s = std::sin(a); float x = X * c - Y * s, y = X * s + Y * c; X = x; Y = y; } };
struct Phys { float facingXY; float facingYZ; Phys() : facingXY(0), facingYZ(1) {} };
struct Thing { V3 pos; Phys* phys; bool dead; };
struct Def { long refs; float HoverBackOffOffset; bool deleted; };
struct Sim {
    Thing* m1; Thing* m2; Thing* dragon; Def* def; long GoLeft; long CurrentState; bool runnable; bool terminated;
    std::vector<V3> route; bool flew;
    static bool Alive(Thing* t) { return t != 0 && !t->dead; }
    void Release() { if (--def->refs == 0) def->deleted = true; }
    void IntoHover() {
        ++def->refs;                                     // GetSubDef hands out a counted def pointer
        if (Alive(m1) && Alive(m2) && m1->phys && m2->phys) {
            Thing* marker = GoLeft ? m1 : m2;
            V3 offset(0.0f, -10.07981f - def->HoverBackOffOffset, 30.148378f);
            V3 back(0.0f, -80.0f, 30.0f);
            offset.RotateXY(marker->phys->facingXY);
            back.RotateXY(marker->phys->facingXY);
            dragon->phys->facingYZ = 0.0f;
            route.clear();
            route.push_back(dragon->pos);
            V3 p; p.X = offset.X + marker->pos.X; p.Y = offset.Y + marker->pos.Y; p.Z = offset.Z + marker->pos.Z;
            route.push_back(p);
            flew = true;
            if (!runnable) { terminated = true; Release(); return; }
        }
        CurrentState = 3;                                // DRAGON_STATE_HOVER (descriptive enumerator, UpdateHover)
        Release();
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
int main()
{
    Phys p1, p2, pd; p1.facingXY = 0.0f; p2.facingXY = 3.14159265f;
    Thing m1 = { V3(100, 200, 10), &p1, false }, m2 = { V3(-100, 50, 0), &p2, false }, dragon = { V3(1, 2, 3), &pd, false };
    Def def = { 1, 5.0f, false };
    Sim s; s.m1 = &m1; s.m2 = &m2; s.dragon = &dragon; s.def = &def; s.GoLeft = 1; s.CurrentState = 0; s.runnable = true;
    s.terminated = false; s.flew = false;
    s.IntoHover();
    if (!s.flew || s.CurrentState != 3 || s.route.size() != 2 || pd.facingYZ != 0.0f || def.refs != 1) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    if (!Near(s.route[1].X, 100) || !Near(s.route[1].Y, 200 - 15.07981f) || !Near(s.route[1].Z, 40.148378f)) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    s.GoLeft = 0; s.CurrentState = 0; s.IntoHover();                 // right side, marker rotated by pi
    if (!Near(s.route[1].X, -100) || !Near(s.route[1].Y, 50 + 15.07981f) || s.CurrentState != 3) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    s.runnable = false; s.CurrentState = 0; s.IntoHover();           // not runnable: terminate, state untouched
    if (!s.terminated || s.CurrentState != 0 || def.refs != 1) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    m2.dead = true; s.flew = false; s.IntoHover();                   // dead marker: straight to hover state, no flight
    if (s.flew || s.CurrentState != 3) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    m2.dead = false; p1.facingXY = 0; m1.phys = 0; s.CurrentState = 0; s.IntoHover();   // no physics: no flight
    if (s.flew || s.CurrentState != 3 || def.refs != 1 || def.deleted) { std::printf("00951d60_TEST FAIL\n"); return 1; }
    std::printf("00951d60_TEST PASS\n");
    return 0;
}