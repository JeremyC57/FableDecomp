// Behaviour test for CTCFlourishTarget::ProcessActionMovementDelta (0x007E8A40): a standalone model of the row's logic -- no attacker or a
// dead attacker: delta unchanged; x_axis = attacker - parent with Z forced to 0; unchanged unless |x_axis|^2 > DELTA_NOTIONAL_ZERO^2
// (exact boundary case); x_axis normalised, y_axis = x_axis x (0,0,1), components = the delta's dots with x / y / z; cull a negative x
// component (CullNegativeXAxisMovement), cull the y component (CullAllYAxisMovement); delta rebuilt from the axes. Every condition and
// stored value has a case (x component below / above 0, each flag on / off, Z of the attacker ignored, the parent's position used).
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static float g_eps = 1e-4f;                   // DELTA_NOTIONAL_ZERO, read from memory as retail does
struct Thing { V pos; bool dead; };
struct Flourish { Thing* parent; Thing* attacker; bool cullNegX; bool cullY; };

static float Dot(const V& a, const V& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static void Process(const Flourish& t, V& d)
{
    if (t.attacker && !t.attacker->dead) {
        V x = { t.attacker->pos.X - t.parent->pos.X, t.attacker->pos.Y - t.parent->pos.Y, t.attacker->pos.Z - t.parent->pos.Z };
        x.Z = 0.0f;
        if (x.X * x.X + x.Y * x.Y + x.Z * x.Z > g_eps * g_eps) {
            float m = std::sqrt(x.X * x.X + x.Y * x.Y + x.Z * x.Z); x.X /= m; x.Y /= m; x.Z /= m;
            V z = { 0.0f, 0.0f, 1.0f };
            V y = { x.Y * z.Z - x.Z * z.Y, x.Z * z.X - x.X * z.Z, x.X * z.Y - x.Y * z.X };
            V c = { Dot(x, d), Dot(y, d), Dot(z, d) };
            if (t.cullNegX && c.X < 0.0f) c.X = 0.0f;
            if (t.cullY) c.Y = 0.0f;
            V r = { x.X * c.X + y.X * c.Y + z.X * c.Z, x.Y * c.X + y.Y * c.Y + z.Y * c.Z, x.Z * c.X + y.Z * c.Y + z.Z * c.Z };
            d = r;
        }
    }
}

static int fails = 0;
static bool Near(const V& a, float x, float y, float z) { return std::fabs(a.X - x) < 1e-4f && std::fabs(a.Y - y) < 1e-4f && std::fabs(a.Z - z) < 1e-4f; }
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }

int main()
{
    Thing parent = { { 10.0f, 20.0f, 5.0f }, false };
    Thing att = { { 13.0f, 24.0f, 50.0f }, false };     // x_axis (0.6, 0.8, 0) despite the Z difference; y_axis (0.8, -0.6, 0)
    Flourish t = { &parent, &att, false, false };
    V d;
    // no flags: the delta rebuilt unchanged
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 1);
    // cull Y: delta = x * (x.d) + z * d.Z; x.d = 0.6 + 1.6 = 2.2
    t.cullY = true; d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 1.32f, 1.76f, 3.0f), 2);
    t.cullY = false;
    // cull negative X with x.d > 0: unchanged
    t.cullNegX = true; d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 3);
    // cull negative X with x.d < 0: d (-1,-2,3): x.d = -2.2, y.d = -0.8 + 1.2 = 0.4 -> y * 0.4 + z * 3
    d.X = -1.0f; d.Y = -2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 0.32f, -0.24f, 3.0f), 4);
    // the same with the flag off: unchanged
    t.cullNegX = false; d.X = -1.0f; d.Y = -2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, -1.0f, -2.0f, 3.0f), 5);
    // both flags, x.d < 0: only z survives
    t.cullNegX = true; t.cullY = true; d.X = -1.0f; d.Y = -2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 0.0f, 0.0f, 3.0f), 6);
    // no attacker / dead attacker: unchanged even with both culls
    Flourish none = { &parent, 0, true, true };
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(none, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 7);
    att.dead = true; d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(t, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 8);
    att.dead = false;
    // attacker straight above the parent (XY coincident): unchanged
    Thing above = { { 10.0f, 20.0f, 9.0f }, false }; Flourish ta = { &parent, &above, true, true };
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(ta, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 9);
    // exactly on the threshold (|x|^2 == eps^2): unchanged; just above: transformed (cull Y along +X: delta = (d.X, 0, d.Z))
    Thing p0 = { { 0.0f, 0.0f, 0.0f }, false };
    Thing onb = { { 1e-4f, 0.0f, 0.0f }, false }; Flourish tb = { &p0, &onb, false, true };
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(tb, d); Check(Near(d, 1.0f, 2.0f, 3.0f), 10);
    Thing abv = { { 2e-4f, 0.0f, 0.0f }, false }; tb.attacker = &abv;
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(tb, d); Check(Near(d, 1.0f, 0.0f, 3.0f), 11);
    // the parent position matters: parent moved so x_axis = (0, 1, 0), y_axis = (1, 0, 0); cull Y keeps (0, d.Y, d.Z)
    Thing p2 = { { 13.0f, 0.0f, 0.0f }, false }; Flourish tc = { &p2, &att, false, true };
    d.X = 1.0f; d.Y = 2.0f; d.Z = 3.0f; Process(tc, d); Check(Near(d, 0.0f, 2.0f, 3.0f), 12);
    if (fails) { std::printf("007e8a40_TEST FAIL\n"); return 1; }
    std::printf("007e8a40_TEST PASS\n");
    return 0;
}
