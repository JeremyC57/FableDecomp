#include <cstdio>
#include <cmath>
// Behaviour test for NCreatureMode::CModeDragonHover::FrameUpdate (0x008DB460): no target or a killed target does
// nothing; with a look TC the head turns at the target (head turning on) or 50 units along the forward vector; when idle
// and within 14 units of the target on the ground plane the dragon is moved back by 0.04 of its forward vector.
struct V3 { float X, Y, Z; };
static V3 mk(float x, float y, float z) { V3 v = { x, y, z }; return v; }
struct Driver { V3 forward; int moves; V3 moveTarget; V3 GetForward() { return forward; } void MoveTo(const V3& p) { ++moves; moveTarget = p; } };
struct Look { int calls; V3 at; long prio; void LookAtPos(const V3* p, long pr) { ++calls; at = *p; prio = pr; } };
struct Thing {
    V3 pos; bool killed; bool hasLook; Look look; Driver driver; bool busy;
    const V3& GetPosition() const { return pos; }
};
struct Hover {
    Thing* owner; Thing* target; bool useHeadTurning; int baseUpdates;
    Thing* GetPItem() { return target; }
    void FrameUpdate()
    {
        ++baseUpdates;
        if (!GetPItem() || GetPItem()->killed) return;
        Thing* o = owner;
        if (o->hasLook) {
            Look* look = &o->look;
            if (useHeadTurning) look->LookAtPos(&GetPItem()->GetPosition(), 10);
            else {
                V3 f = o->driver.GetForward();
                V3 ahead = mk(f.X * 50.0f, f.Y * 50.0f, f.Z * 50.0f);
                V3 p = owner->GetPosition();
                V3 at = mk(p.X + ahead.X, p.Y + ahead.Y, p.Z + ahead.Z);
                look->LookAtPos(&at, 10);
            }
        }
        if (owner->busy) return;
        Thing* me = owner; Thing* t = GetPItem();
        float dx = t->GetPosition().X - me->GetPosition().X, dy = t->GetPosition().Y - me->GetPosition().Y;
        if (std::sqrt(dx * dx + dy * dy) < 14.0f) {
            V3 f = owner->driver.GetForward();
            V3 p = owner->GetPosition();
            V3 dest = mk(p.X - f.X * 0.04f, p.Y - f.Y * 0.04f, p.Z - f.Z * 0.04f);
            owner->driver.MoveTo(dest);
        }
    }
};
int main()
{
    Thing dragon = Thing(); dragon.pos = mk(0, 0, 100); dragon.hasLook = true; dragon.driver.forward = mk(1, 0, 0);
    Thing hero = Thing(); hero.pos = mk(10, 0, 0);
    Hover h = { &dragon, 0, false, 0 };
    int bad = 0;
    h.FrameUpdate(); bad |= dragon.look.calls != 0 || h.baseUpdates != 1;
    h.target = &hero; hero.killed = true; h.FrameUpdate(); bad |= dragon.look.calls != 0;
    hero.killed = false; h.FrameUpdate();
    bad |= dragon.look.calls != 1 || dragon.look.at.X != 50.0f || dragon.look.at.Z != 100.0f || dragon.look.prio != 10;
    bad |= dragon.driver.moves != 1 || std::fabs(dragon.driver.moveTarget.X + 0.04f) > 1e-6f;
    h.useHeadTurning = true; hero.pos = mk(20, 0, 0); h.FrameUpdate();
    bad |= dragon.look.at.X != 20.0f || dragon.driver.moves != 1;            // 20 units away: no move
    dragon.busy = true; hero.pos = mk(1, 1, 0); h.FrameUpdate(); bad |= dragon.driver.moves != 1;
    if (bad) { std::printf("008db460_TEST FAIL\n"); return 1; }
    std::printf("008db460_TEST PASS\n");
    return 0;
}