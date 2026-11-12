// Behaviour test for CAIStateGroup_MinionKidGroupTag::TagTarget (0x009341C0): a copy of the decision logic over a small model.
#include <cstdio>

struct Pos { float X, Y, Z; };
struct Creature { Pos pos; float radius; bool dead; };
struct World {
    Creature self;
    Creature* nearest;
    int moveResult;      // AIAction_MoveToThingProximity result (2 = finished)
    int lookResult;      // AIAction_LookAtThing result (1 = terminate process)
    int performResult;   // AIAction_PerformActionAndWait result
    unsigned long frame, taggedFrame;
    long fps;
    // effects
    bool moved, looked, tagged, played, terminated;
    bool moveRelocates; Pos newTargetPos;   // target moves during the look
};

static bool InReach(const Creature& self, const Creature& target)
{
    float dx = target.pos.X - self.pos.X;
    float dy = target.pos.Y - self.pos.Y;
    float reach = self.radius + 0.5f;
    return dy * dy + dx * dx <= reach * reach;
}

static void TagTarget(World& w)
{
    Creature* target = w.nearest;
    if (target != 0 && !target->dead) {
        w.moved = true;
        if (w.moveResult == 2) {
            if (InReach(w.self, *target)) {
                w.looked = true;
                if (w.moveRelocates) target->pos = w.newTargetPos;
                if (w.lookResult == 1) {
                    w.terminated = true;
                } else if (InReach(w.self, *target)) {
                    if ((float)(unsigned long)(w.frame - w.taggedFrame) > 2.0f * w.fps) {
                        w.tagged = true;
                        w.played = true;
                        if (w.performResult == 1) w.terminated = true;
                    }
                }
            }
        }
    }
}

static World Base(Creature* t)
{
    World w = {};
    w.self.pos.X = 0; w.self.pos.Y = 0; w.self.pos.Z = 0; w.self.radius = 0.5f; w.self.dead = false;
    w.nearest = t; w.moveResult = 2; w.lookResult = 2; w.performResult = 1;
    w.frame = 1000; w.taggedFrame = 900; w.fps = 30;
    return w;
}

int main()
{
    int fails = 0;
    Creature t = { { 0.6f, 0.6f, 50.0f }, 0.5f, false };   // planar distance ~0.85 <= 1.0 (Z ignored)
    World w = Base(&t); TagTarget(w);
    if (!(w.tagged && w.played && w.terminated)) ++fails;
    w = Base(0); TagTarget(w); if (w.moved || w.terminated) ++fails;                       // no member
    Creature d = t; d.dead = true; w = Base(&d); TagTarget(w); if (w.moved) ++fails;        // dead
    w = Base(&t); w.moveResult = 4; TagTarget(w); if (w.looked || w.terminated) ++fails;   // move not finished
    Creature far = { { 0.8f, 0.7f, 0.0f }, 0.5f, false }; w = Base(&far); TagTarget(w);    // out of reach
    if (w.looked || w.tagged) ++fails;
    w = Base(&t); w.lookResult = 1; TagTarget(w); if (!w.terminated || w.tagged) ++fails;  // look abandoned
    Creature t2 = t; w = Base(&t2); w.moveRelocates = true; w.newTargetPos.X = 3; w.newTargetPos.Y = 0; w.newTargetPos.Z = 0;
    TagTarget(w); if (!w.looked || w.tagged || w.terminated) ++fails;                       // ran away during the look
    w = Base(&t); w.taggedFrame = 950; TagTarget(w); if (w.tagged) ++fails;                 // 50 frames < 60
    w = Base(&t); w.taggedFrame = 940; TagTarget(w); if (w.tagged) ++fails;                 // exactly 60: not greater
    w = Base(&t); w.taggedFrame = 1100; TagTarget(w); if (!w.tagged) ++fails;              // unsigned wrap: huge elapsed
    w = Base(&t); w.performResult = 2; TagTarget(w); if (!w.played || w.terminated) ++fails;
    if (fails) { std::printf("009341c0_TEST FAIL\n"); return 1; }
    std::printf("009341c0_TEST PASS\n");
    return 0;
}