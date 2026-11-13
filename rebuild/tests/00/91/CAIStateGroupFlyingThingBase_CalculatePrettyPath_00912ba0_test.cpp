// Behaviour test for 0x00912BA0 CAIStateGroup_FlyingThingBase::CalculatePrettyPath: a host model of the row's path building
// (fixed-size curve list for the deque, deterministic stand-ins for the random / terrain callees), checking the curves built,
// the GFRandom hop count, the tangent halving with its 10-try limit, the minimum-height clamp and the failure exits.
#include <cstdio>
#include <cmath>

struct Vec { float X, Y, Z; };
static Vec V(float x, float y, float z) { Vec v; v.X = x; v.Y = y; v.Z = z; return v; }
static Vec Add(const Vec& a, const Vec& b) { return V(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static Vec Sub(const Vec& a, const Vec& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
struct Curve { Vec P1, P2, P3, P4; };
struct Path { Curve c[16]; int n; };

static unsigned int RotR13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int GFRandom(unsigned int range, unsigned int* seed)
{
    *seed = *seed * 0x24a1u + 0x24dfu; *seed = RotR13(*seed);
    return range ? *seed % range : 0;
}

struct Flyer {
    float MinHeightToKeepAboveGroundInFlight, MaxHeightAboveGround, FlightRange;
    Vec creature; unsigned int seed;
    int flightCalls, failFlightAt, failHopAt, hops;
    float groundBase; int groundCalls;           // terrain: ground = groundBase - groundCalls (rises as the probe moves back)
    float lastRandomMax;
    bool GetRandomFlightPos(const Vec& from, float range, Vec& pos)
    {
        ++flightCalls; if (flightCalls == failFlightAt) return false;
        pos = V(from.X + range * flightCalls, from.Y - flightCalls, from.Z);
        return true;
    }
    float GFFloatRandom(float max, unsigned int* seed) { lastRandomMax = max; return max * (float)(GFRandom(100, seed)) / 100.0f; }
    bool AddRandomBezierCurveToPath(Path& path)
    {
        ++hops; if (hops == failHopAt) return false;
        Curve c = path.c[path.n - 1]; c.P1 = c.P4; c.P3 = Add(c.P4, V(1, 0, 0)); c.P4 = Add(c.P4, V(2, 1, 0));
        path.c[path.n++] = c; return true;
    }
    float GetHeightZBelowAt(const Vec&) { ++groundCalls; return groundBase; }

    bool CalculatePrettyPath(Path& path, const Vec& destination)
    {
        path.n = 0;
        Curve curve; curve.P1 = creature;
        for (unsigned int point = 2; point <= 4; ++point) {
            Vec pos;
            if (!GetRandomFlightPos(curve.P1, FlightRange, pos)) return false;
            if (point == 2 || point == 4) pos.Z += GFFloatRandom(MaxHeightAboveGround, &seed) + MinHeightToKeepAboveGroundInFlight;
            else pos.Z += GFFloatRandom(MaxHeightAboveGround * 0.25f, &seed) + MinHeightToKeepAboveGroundInFlight;
            switch (point) { case 2: curve.P2 = pos; break; case 3: curve.P3 = pos; break; case 4: curve.P4 = pos; break; }
        }
        path.c[path.n++] = curve;
        unsigned int count = GFRandom(3, &seed);
        for (unsigned int i = 0; i < count; ++i)
            if (!AddRandomBezierCurveToPath(path)) return false;
        Curve last;
        Vec start = path.c[path.n - 1].P4; last.P1 = start;
        Vec direction = Sub(start, path.c[path.n - 1].P3);
        Vec control = Add(start, direction);
        float ground = GetHeightZBelowAt(control);
        long tries = 10;
        while (ground + MinHeightToKeepAboveGroundInFlight > control.Z && tries > 0) {
            direction.X *= 0.5f; direction.Y *= 0.5f; direction.Z *= 0.5f;
            control = Add(start, direction);
            ground = GetHeightZBelowAt(control);
            --tries;
        }
        float min_z = ground + MinHeightToKeepAboveGroundInFlight;
        if (control.Z < min_z) control.Z = min_z;
        last.P2 = control; last.P4 = destination;
        Vec mid;
        if (!GetRandomFlightPos(destination, FlightRange, mid)) return false;
        mid.Z += MinHeightToKeepAboveGroundInFlight;
        last.P3 = mid;
        path.c[path.n++] = last;
        return true;
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }
static Flyer Make()
{
    Flyer f; f.MinHeightToKeepAboveGroundInFlight = 2.0f; f.MaxHeightAboveGround = 8.0f; f.FlightRange = 10.0f;
    f.creature = V(5, 6, 7); f.seed = 1234u; f.flightCalls = 0; f.failFlightAt = -1; f.failHopAt = -1; f.hops = 0;
    f.groundBase = -100.0f; f.groundCalls = 0; f.lastRandomMax = 0; return f;
}

int main()
{
    int fail = 0;
    Path p; Vec dest = V(50, 60, 70);
    Flyer f = Make();
    unsigned int s = 1234u; GFRandom(100, &s); GFRandom(100, &s); GFRandom(100, &s);
    unsigned int expectHops = GFRandom(3, &s);
    if (!f.CalculatePrettyPath(p, dest)) fail |= 1;
    if (p.n != (int)(2 + expectHops) || f.hops != (int)expectHops) fail |= 2;
    const Curve& c0 = p.c[0];
    if (!Near(c0.P1.X, 5) || !Near(c0.P2.X, 15) || !Near(c0.P3.X, 25) || !Near(c0.P4.X, 35)) fail |= 4;   // range * call number
    if (c0.P3.Z < 7 + 2 || c0.P3.Z > 7 + 2 + 2.0f + 1e-3f) fail |= 8;                                    // point 3: max * 0.25
    const Curve& cl = p.c[p.n - 1]; const Curve& cp = p.c[p.n - 2];
    if (!Near(cl.P1.X, cp.P4.X) || !Near(cl.P2.X, cp.P4.X + (cp.P4.X - cp.P3.X)) || !Near(cl.P4.Z, 70)) fail |= 16;
    if (!Near(cl.P3.Z, 70 + 2) || f.groundCalls != 1) fail |= 32;                                         // low ground: no halving
    // high ground: the tangent is halved 10 times, then the control point is raised to ground + 2
    f = Make(); f.groundBase = 1000.0f;
    if (!f.CalculatePrettyPath(p, dest) || f.groundCalls != 11) fail |= 64;
    if (!Near(p.c[p.n - 1].P2.Z, 1002.0f)) fail |= 64;
    // failures: the first random position, a random hop
    f = Make(); f.failFlightAt = 1; if (f.CalculatePrettyPath(p, dest)) fail |= 128;
    f = Make(); f.failHopAt = 1; f.seed = 7u;
    { unsigned int t = 7u; GFRandom(100, &t); GFRandom(100, &t); GFRandom(100, &t); if (GFRandom(3, &t) > 0 && f.CalculatePrettyPath(p, dest)) fail |= 256; }
    if (fail) { std::printf("00912ba0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00912ba0_TEST PASS\n");
    return 0;
}