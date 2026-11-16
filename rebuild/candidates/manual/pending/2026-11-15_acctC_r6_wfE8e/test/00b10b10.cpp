// Behaviour test for 0x00B10B10 NParticleEngine::CPSCEmitterGeneric::GenerateParticle (model of the row's logic):
// the emitter fields are decoded (unit * range [- offset], rounded to 2 decimals), the emitter shape picks position / normal / up
// (0 point at the origin, 1 random unit 2D ring point, 2 random unit 3D point with up = normal = position, 3 spline point at a random
// parameter), the radius (EmitterSize / 2, times GetRandomRadius when Solid) scales the shape point of types 1 and 2, direction /
// normal / up (never the position) are re-oriented (XZ: (x, -z, y), YZ: (-z, x, y)) for types 1..3, position / normal / up are scaled
// componentwise, normal normalised when non-zero; direction = random 3D with its up component projected out (Random2D), random 3D,
// outward (= normal), forward (the param callback, else zero), the named vector parameter when present (then moved by the INVERSE
// local transform) else the custom direction; OppositeDirection negates; angular perturbation adds random * 0.001 * n (the random
// point's component along direction removed) and keeps the length; speed = minSpeed + random in [0, max(max - min, 0)]; the position
// gets the emitter-position offset; direction / position go to world space unless the system stays with its emitter.
#include <cstdio>
#include <cmath>

struct V { double x, y, z; V(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) { } };
static V operator+(const V& a, const V& b) { return V(a.x + b.x, a.y + b.y, a.z + b.z); }
static V operator-(const V& a, const V& b) { return V(a.x - b.x, a.y - b.y, a.z - b.z); }
static V operator*(const V& a, double s) { return V(a.x * s, a.y * s, a.z * s); }
static double dot(const V& a, const V& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static V scaled(const V& a, const V& s) { return V(a.x * s.x, a.y * s.y, a.z * s.z); }
static bool nonzero(const V& v) { return dot(v, v) > 1e-6; }
static V normalised(const V& v) { return v * (1.0 / sqrt(dot(v, v))); }
static double round2(double v) { return floor(v * 100.0 + 0.5) / 100.0; }

struct Emitter {
    int type; bool solid, orientXZ, orientYZ, random2D, random3D, outward, forward, useParam, opposite;
    int perturbation; unsigned sizeField, minSpeedField, maxSpeedField; V custom, scaling;
};
struct System { bool stay; double worldScale; V worldOffset; double inverseScale; bool hasParam; V param; bool hasOffset; V offset; };

// deterministic stand-ins for the out-of-line callees
static int unit3Calls;
static V unit2() { return V(0.6, 0.8, 0.0); }
static V unit3() { return unit3Calls++ == 0 ? V(0.0, 0.0, 1.0) : V(0.6, 0.0, 0.8); }
static V pointInSphere() { return V(0.0, 1.0, 0.0); }
static double floatRandom(double range) { return range * 0.25; }
static double randomRadius() { return 0.5; }
static V spline(double t) { return V(t, 2 * t, 3 * t); }

static V orient(const Emitter& e, const V& v)
{
    if (e.orientXZ) return V(v.x, -v.z, v.y);
    if (e.orientYZ) return V(-v.z, v.x, v.y);
    return v;
}

static void Generate(const Emitter& e, const System& s, V& outPos, V& outDir)
{
    unit3Calls = 0;
    double size = round2(e.sizeField / 1023.0 * 10.0), minSpeed = round2(e.minSpeedField / 1023.0 * 10.0),
           maxSpeed = round2(e.maxSpeedField / 1023.0 * 10.0);
    V direction, position, normal, up(0, 0, 1);
    double radius = size * 0.5;
    switch (e.type) {
    case 0: break;
    case 1: { V r = unit2(); position = V(r.x, r.y, 0); normal = position;
              position = position * (e.solid ? randomRadius() * radius : radius);
              direction = orient(e, direction); normal = orient(e, normal); up = orient(e, up); break; }
    case 2: position = unit3(); normal = position; up = position;
            position = position * (e.solid ? randomRadius() * radius : radius);
            direction = orient(e, direction); normal = orient(e, normal); up = orient(e, up); break;
    case 3: position = spline(floatRandom(1.0)); normal = position; up = position;
            direction = orient(e, direction); normal = orient(e, normal); up = orient(e, up); break;
    }
    bool fromParam = false;
    position = scaled(position, e.scaling); normal = scaled(normal, e.scaling);
    if (nonzero(normal)) normal = normalised(normal);
    up = scaled(up, e.scaling);
    if (e.random2D) { direction = unit3(); direction = direction - up * dot(up, direction); if (nonzero(direction)) direction = normalised(direction); }
    else if (e.random3D) direction = unit3();
    else if (e.outward) direction = normal;
    else if (e.forward) direction = V();
    else if (e.useParam && s.hasParam) { direction = s.param; fromParam = true; }
    else direction = e.custom;
    if (e.opposite) direction = direction * -1.0;
    if (e.perturbation) {
        double length = sqrt(dot(direction, direction)); V r = pointInSphere();
        r = r - direction * dot(r, direction); direction = direction + r * (e.perturbation * 0.001);
        if (nonzero(direction)) direction = normalised(direction) * length;
    }
    double range = maxSpeed - minSpeed > 0 ? maxSpeed - minSpeed : 0;
    direction = direction * (floatRandom(range) + minSpeed);
    if (s.hasOffset) position = position + s.offset;
    if (fromParam) { if (s.stay) direction = direction * s.inverseScale; }
    else if (!s.stay) direction = direction * s.worldScale;
    if (!s.stay) position = position * s.worldScale + s.worldOffset;
    outPos = position; outDir = direction;
}

static bool near(const V& a, double x, double y, double z) { return fabs(a.x - x) < 1e-3 && fabs(a.y - y) < 1e-3 && fabs(a.z - z) < 1e-3; }

int main()
{
    int bad = 0; V p, d;
    // A: ring, XZ orientation (normal re-oriented, position not), scaling (1,2,1), outward direction, world transform x2 + (1,2,3)
    Emitter a = { 1, false, true, false, false, false, true, false, false, false, 0, 205, 102, 102, V(), V(1, 2, 1) };
    System sa = { false, 2.0, V(1, 2, 3), 0.5, false, V(), false, V() };
    Generate(a, sa, p, d);
    if (!near(p, 2.2, 5.2, 3.0) || !near(d, 1.2, 0.0, 1.6)) bad |= 1;
    // B: sphere, Solid (radius 0.5 * 2), Random2D direction with the up component projected out, opposite, speed 1 + 2/4, stays local
    Emitter b = { 2, true, false, false, true, false, false, false, false, true, 0, 409, 102, 307, V(), V(1, 1, 1) };
    System sb = { true, 2.0, V(), 0.5, false, V(), false, V() };
    Generate(b, sb, p, d);
    if (!near(p, 0, 0, 1.0)) bad |= 2;
    if (!near(d, -1.5, 0.0, 0.0)) bad |= 2;
    // C: point emitter, named parameter direction (inverse local transform x0.5), speed 2, position offset
    Emitter c = { 0, false, false, false, false, false, false, false, true, false, 0, 0, 205, 205, V(9, 9, 9), V(1, 1, 1) };
    System sc = { true, 2.0, V(), 0.5, true, V(0, 3, 4), true, V(1, 1, 1) };
    Generate(c, sc, p, d);
    if (!near(p, 1, 1, 1) || !near(d, 0, 3, 4)) bad |= 4;
    // D: spline point at t = 0.25, custom direction (1,0,0) perturbed by 1000 / 1000 towards (0,1,0), length kept
    Emitter e = { 3, false, false, false, false, false, false, false, false, false, 1000, 0, 102, 102, V(1, 0, 0), V(1, 1, 1) };
    System se = { true, 2.0, V(), 0.5, false, V(), false, V() };
    Generate(e, se, p, d);
    if (!near(p, 0.25, 0.5, 0.75) || !near(d, 0.7071, 0.7071, 0.0)) bad |= 8;
    // E: param direction requested but missing -> custom direction, world transform applied to it
    Emitter f = { 0, false, false, false, false, false, false, false, true, false, 0, 0, 102, 102, V(0, 0, 1), V(1, 1, 1) };
    System sf = { false, 3.0, V(), 0.5, false, V(), false, V() };
    Generate(f, sf, p, d);
    if (!near(d, 0, 0, 3.0)) bad |= 16;
    if (bad) { printf("00b10b10_TEST FAIL %d\n", bad); return 1; }
    printf("00b10b10_TEST PASS\n");
    return 0;
}
