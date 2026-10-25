// 00714900 behaviour test: CActionMoveTo::Update -- refresh both handles, clear the navigation ignore-thing, fail (and
// optionally cancel the script) when a claimed action point's thing is dead or carried; an idle creature then asks the
// navigator for the next step towards the target thing's position or its own position (proximity 0 for an action
// point), records the input and activates navigation; a long enough step either sets the facing position directly or,
// when FaceMovement is set, looks there and faces it only when in view; a blocked route falls back to the backup
// proximity once or cancels the script; arrival/failure makes the hero stop looking and finishes.
#include <cstdio>

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
static const float kEpsilon = 0.0001f;

struct Nav {
    bool present; int ignoreCalls; const void* ignored;
    Vec dest; float proximity; long movementType; bool active; int inputs;
    long result; Vec step; bool avoid; bool ignorePref;
};
struct Look { bool present; int lookCalls; Vec lookedAt; bool inView; int stops; };
struct Thing {
    int type; Vec pos; bool busy; bool carried; bool dead;
    Nav nav; Look look; int facingCalls; Vec facing;
};
struct ScriptThing {
    Thing* thing; bool valid; int refreshes;
    bool IsValid() const { return valid; }
    void Refresh() { ++refreshes; valid = true; }
    bool IsDead() const { return thing->dead; }
    Thing* GetThing() const { return thing; }
};
struct Control { Thing* parent; int inlineClears; int cancelCalls; };

struct MoveTo {
    Control* script; bool Terminated;
    Vec Pos; long MovementType; bool FaceMovement; float Proximity;
    ScriptThing Target; bool UseTargetThing; ScriptThing Route;
    bool Avoid; bool UseActionPoint; bool GotActionPointIndex; bool UseBackup; bool CancelOnFailure; bool IgnorePref;

    void Update()
    {
        if (!Target.IsValid()) Target.Refresh();
        if (!Route.IsValid()) Route.Refresh();
        if (script->parent->nav.present) { ++script->parent->nav.ignoreCalls; script->parent->nav.ignored = 0; }
        if (GotActionPointIndex && (Target.IsDead() || Target.GetThing()->carried)) {
            Terminated = true;
            if (CancelOnFailure) ++script->inlineClears;
            return;
        }
        Thing* parent = script->parent;
        if (!parent->nav.present || parent->busy) return;
        Nav& nav = parent->nav;
        if (UseTargetThing) {
            if (Target.IsDead()) { Terminated = true; return; }
            nav.dest = Target.GetThing()->pos; nav.proximity = Proximity;
        } else {
            nav.proximity = UseActionPoint ? 0.0f : Proximity; nav.dest = Pos;
        }
        nav.movementType = MovementType; nav.avoid = Avoid; nav.ignorePref = IgnorePref;
        ++nav.inputs; nav.active = true;
        long result = nav.result;
        if (result == 2) {
            Vec s = nav.step;
            if (!(s.X * s.X + s.Y * s.Y + s.Z * s.Z > kEpsilon * kEpsilon)) return;
            Vec t = V(s.X + parent->pos.X, s.Y + parent->pos.Y, s.Z + parent->pos.Z);
            if (!FaceMovement) { ++parent->facingCalls; parent->facing = t; return; }
            if (parent->look.present) {
                ++parent->look.lookCalls; parent->look.lookedAt = t;
                if (parent->look.inView) { ++parent->facingCalls; parent->facing = t; }
            }
            return;
        }
        if (result == 1) {
            if (UseActionPoint && UseBackup) { UseActionPoint = false; return; }
            if (GotActionPointIndex && CancelOnFailure) ++script->cancelCalls;
        }
        if (parent->type == 2 && parent->look.present) ++parent->look.stops;
        Terminated = true;
    }
};

static Thing MakeThing(int type)
{
    Thing t;
    t.type = type; t.pos = V(1.0f, 2.0f, 3.0f); t.busy = false; t.carried = false; t.dead = false;
    t.nav.present = true; t.nav.ignoreCalls = 0; t.nav.ignored = (const void*)1; t.nav.dest = V(0, 0, 0); t.nav.proximity = -1.0f;
    t.nav.movementType = -1; t.nav.active = false; t.nav.inputs = 0; t.nav.result = 0; t.nav.step = V(0, 0, 0);
    t.nav.avoid = false; t.nav.ignorePref = false;
    t.look.present = true; t.look.lookCalls = 0; t.look.lookedAt = V(0, 0, 0); t.look.inView = false; t.look.stops = 0;
    t.facingCalls = 0; t.facing = V(0, 0, 0);
    return t;
}
static MoveTo MakeAction(Control* c, Thing* target)
{
    MoveTo m;
    m.script = c; m.Terminated = false; m.Pos = V(10.0f, 20.0f, 30.0f); m.MovementType = 3; m.FaceMovement = false;
    m.Proximity = 1.5f; m.Target.thing = target; m.Target.valid = false; m.Target.refreshes = 0; m.UseTargetThing = false;
    m.Route.thing = target; m.Route.valid = false; m.Route.refreshes = 0; m.Avoid = true; m.UseActionPoint = false;
    m.GotActionPointIndex = false; m.UseBackup = false; m.CancelOnFailure = false; m.IgnorePref = true;
    return m;
}
static bool Eq(const Vec& a, const Vec& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }

int main()
{
    int fails = 0;
    {   // refresh, ignore-thing reset, arrival of a non-hero: finished, no stop-looking
        Thing me = MakeThing(1), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        m.Update();
        if (m.Target.refreshes != 1 || m.Route.refreshes != 1 || me.nav.ignoreCalls != 1 || me.nav.ignored != 0) ++fails;
        if (!m.Terminated || me.look.stops != 0 || !me.nav.active || me.nav.proximity != 1.5f || !Eq(me.nav.dest, m.Pos)) ++fails;
        if (me.nav.movementType != 3 || !me.nav.avoid || !me.nav.ignorePref) ++fails;
    }
    {   // action point thing carried: fail and cancel inline
        Thing me = MakeThing(2), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        target.carried = true; m.GotActionPointIndex = true; m.CancelOnFailure = true;
        m.Update();
        if (!m.Terminated || c.inlineClears != 1 || me.nav.inputs != 0) ++fails;
    }
    {   // busy creature: nothing
        Thing me = MakeThing(1), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        me.busy = true; m.Update();
        if (m.Terminated || me.nav.inputs != 0) ++fails;
    }
    {   // dead target thing: finished without a nav step
        Thing me = MakeThing(1), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        m.UseTargetThing = true; target.dead = true; m.Update();
        if (!m.Terminated || me.nav.inputs != 0) ++fails;
    }
    {   // moving towards the target thing, tiny step: nothing faced; then a real step faces pos + step
        Thing me = MakeThing(1), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        m.UseTargetThing = true; target.pos = V(7.0f, 8.0f, 9.0f); me.nav.result = 2; me.nav.step = V(0.00001f, 0, 0);
        m.Update();
        if (m.Terminated || me.facingCalls != 0 || !Eq(me.nav.dest, target.pos)) ++fails;
        me.nav.step = V(1.0f, 0.0f, -1.0f); m.Update();
        if (m.Terminated || me.facingCalls != 1 || !Eq(me.facing, V(2.0f, 2.0f, 2.0f))) ++fails;
    }
    {   // FaceMovement: look first, face only when in view
        Thing me = MakeThing(1), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        m.FaceMovement = true; me.nav.result = 2; me.nav.step = V(0.5f, 0.5f, 0.0f);
        m.Update();
        if (me.look.lookCalls != 1 || me.facingCalls != 0 || !Eq(me.look.lookedAt, V(1.5f, 2.5f, 3.0f))) ++fails;
        me.look.inView = true; m.Update();
        if (me.look.lookCalls != 2 || me.facingCalls != 1 || m.Terminated) ++fails;
    }
    {   // blocked route with an action point: fall back once (proximity 0 first), then cancel and finish as the hero
        Thing me = MakeThing(2), target = MakeThing(1); Control c = { &me, 0, 0 }; MoveTo m = MakeAction(&c, &target);
        m.UseActionPoint = true; m.UseBackup = true; m.GotActionPointIndex = true; m.CancelOnFailure = true; me.nav.result = 1;
        m.Update();
        if (me.nav.proximity != 0.0f || m.UseActionPoint || m.Terminated) ++fails;
        m.Update();
        if (me.nav.proximity != 1.5f || c.cancelCalls != 1 || me.look.stops != 1 || !m.Terminated) ++fails;
    }
    if (fails) { std::printf("00714900_TEST FAIL (%d)\n", fails); return 1; }
    std::printf("00714900_TEST PASS\n");
    return 0;
}