// Behaviour test for 0x006FDD60 CGameCameraBase::Update() (model copy; plain C++98, no engine headers).
// The model mirrors the row's logic: last-frame copies, spherical offset (theta / phi in turns, radius), hero alive test,
// target = hero position + (0,0,1), controlled-creature velocity (pos - last pos) * FPS, facing normalised, the fixed-camera
// release after 0.5 s of game time, the local-thing list (two-sided -> theta 0.08, one-sided -> look from behind the
// flattened sum), the radius / theta / phi easing, the thing-collision override and the line-of-sight fix-up (+1.5 Z).
#include <cstdio>
#include <cmath>
#include <list>

struct Vec { float X, Y, Z; Vec() : X(0), Y(0), Z(0) {} Vec(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static Vec operator+(const Vec& a, const Vec& b) { return Vec(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
static Vec operator-(const Vec& a, const Vec& b) { return Vec(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static Vec operator*(const Vec& a, float k) { return Vec(a.X * k, a.Y * k, a.Z * k); }
static Vec neg(const Vec& a) { return Vec(-a.X, -a.Y, -a.Z); }
static float dot(const Vec& a, const Vec& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static float mag(const Vec& a) { return (float)std::sqrt(dot(a, a)); }

// GFFastOneOverSqrt: table estimate (exponent (0x17C - e) / 2, 128-entry mantissa table) + one Newton step.
static unsigned int g_table[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        unsigned int e = 126 + (unsigned int)(i >> 6);
        float v = (float)std::ldexp(1.0 + ((i & 63) + 0.5) / 64.0, (int)e - 127);
        double y = 1.0 / std::sqrt((double)v);
        unsigned int E = (0x17c - e) >> 1;
        double m = y / std::ldexp(1.0, (int)E - 127) - 1.0;
        if (m < 0) m = 0; if (m > 0.996) m = 0.996;
        g_table[i] = (unsigned int)(m * 256.0 + 0.5) & 0xff;
    }
}
static float FastOneOverSqrt(float value)
{
    union { float f; unsigned int u; } in, est;
    in.f = value;
    est.u = (((0x17c - ((in.u >> 23) & 0xff)) >> 1) << 23) | (g_table[(in.u >> 17) & 0x7f] << 15);
    float r = est.f;
    return (3.0f - r * r * value) * r * 0.5f;
}
static void Normalise(Vec& v) { float k = FastOneOverSqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z); v.X *= k; v.Y *= k; v.Z *= k; }
static float RotationAngle(const Vec& from, const Vec& to)   // the landed 0x006FD4F0 logic
{
    float result = 0.0f;
    Vec flat(to.X, to.Y, 0.0f);
    float c = dot(flat, from) / (mag(from) * mag(flat));
    if (c <= 1.0f && c >= -1.0f) {
        float a = (float)std::acos(c) * 0.15915494f;
        float sign = 1.0f;
        if (flat.X * from.Y - flat.Y * from.X < 0.0f) sign = -1.0f;
        result = a * sign;
    }
    return result;
}

struct World {   // the collaborators Update reads
    long Frame, FPS; bool HeroAlive; Vec HeroPos, CreaturePos, CreatureLastPos, Facing;
    bool ThingCollision, LineOfSight; Vec Unblocked;
};
struct Camera {
    Vec Pos, LastPos, PointingAt, LastPointingAt; float Theta, Phi, Rad, ThetaFixed, RadFixed, PhiChanger;
    bool ActAsFixedCam; std::list<Vec> DrawCoordList, LocalThingPosList; long TriggerFrame; unsigned long BuildingUID;
    void Update(const World& w);
};

void Camera::Update(const World& w)
{
    LastPos = Pos;
    LastPointingAt = PointingAt;
    long frame = w.Frame;
    const double tp = 6.2831855f;
    Vec offset((float)(Rad * std::cos(Phi * tp) * std::sin(Theta * tp)), (float)(Rad * std::sin(Phi * tp) * std::sin(Theta * tp)),
               (float)(Rad * std::cos(Theta * tp)));
    if (!w.HeroAlive) return;
    Vec target = w.HeroPos + Vec(0.0f, 0.0f, 1.0f);
    Vec velocity = (w.CreaturePos - w.CreatureLastPos) * (float)w.FPS;
    Vec facing = w.Facing;
    if ((float)(frame - TriggerFrame) / (float)w.FPS > 0.5f && BuildingUID == 0 && ActAsFixedCam) {
        ActAsFixedCam = false;
        Rad = mag(PointingAt - Pos);
    }
    float theta_offset = 0.0f;
    Normalise(facing);
    Vec behind = neg(facing) * Rad;
    if (!LocalThingPosList.empty()) {
        Vec total;
        std::list<Vec>::const_iterator it = LocalThingPosList.begin();
        Vec last = *it - target;
        float d = 0.0f;
        for (; it != LocalThingPosList.end(); ++it) {
            Vec rel = *it - target;
            total = total + rel;
            if (d >= 0.0f) d = dot(rel, last);
            last = rel;
        }
        if (d >= 0.0f) { total.Z = 0.0f; Normalise(total); behind = neg(total) * Rad; }
        else theta_offset = (float)(0.08 - ThetaFixed);
    }
    if (!ActAsFixedCam) {
        Rad += (RadFixed - Rad) * 0.01f;
        Theta += (mag(velocity) * 0.0005f + ThetaFixed + theta_offset - Theta) * 0.05f;
        Phi += RotationAngle(behind, offset) * PhiChanger;
    }
    Vec new_pos = Pos;
    if (!ActAsFixedCam) new_pos = target + offset;
    PointingAt = target;
    if (w.ThingCollision) { new_pos = DrawCoordList.front(); ActAsFixedCam = true; }
    if (!w.LineOfSight) new_pos = w.Unblocked + Vec(0.0f, 0.0f, 1.5f);
    Pos = new_pos;
}

static bool near(float a, float b) { return std::fabs(a - b) <= 1e-3f * (1.0f + std::fabs(b)); }
static bool nearv(const Vec& a, const Vec& b) { return near(a.X, b.X) && near(a.Y, b.Y) && near(a.Z, b.Z); }

static Camera MakeCamera()
{
    Camera c;
    c.Pos = Vec(0, -10, 2); c.PointingAt = Vec(0, 0, 1); c.Theta = 0.2f; c.Phi = 1.25f; c.Rad = 10.0f;
    c.ThetaFixed = 0.2f; c.RadFixed = 10.0f; c.PhiChanger = 0.1f; c.ActAsFixedCam = false; c.TriggerFrame = 0; c.BuildingUID = 0;
    return c;
}
static World MakeWorld()
{
    World w;
    w.Frame = 100; w.FPS = 10; w.HeroAlive = true; w.HeroPos = Vec(1, 2, 0); w.CreaturePos = Vec(1, 2, 0);
    w.CreatureLastPos = Vec(0.5f, 2, 0); w.Facing = Vec(0, 2, 0); w.ThingCollision = false; w.LineOfSight = true;
    w.Unblocked = Vec(3, 3, 3);
    return w;
}

int main()
{
    BuildTable();
    int fails = 0;
    // fast inverse square root against the exact value
    for (float v = 0.01f; v < 1000.0f; v *= 1.7f)
        if (!near(FastOneOverSqrt(v), (float)(1.0 / std::sqrt((double)v)))) ++fails;
    // 1. dead hero: only the last-frame copies happen
    {
        Camera c = MakeCamera(); World w = MakeWorld(); w.HeroAlive = false;
        c.Update(w);
        if (!nearv(c.LastPos, Vec(0, -10, 2)) || !nearv(c.Pos, Vec(0, -10, 2)) || !near(c.Rad, 10.0f)) ++fails;
    }
    // 2. free camera, empty list, line of sight: easing + orbit position
    {
        Camera c = MakeCamera(); World w = MakeWorld();
        c.Update(w);
        // phi = 1.25 turns -> direction (0, 1); theta 0.2 turns
        Vec off(10.0f * (float)std::cos(1.25 * 6.2831855) * (float)std::sin(0.2 * 6.2831855),
                10.0f * (float)std::sin(1.25 * 6.2831855) * (float)std::sin(0.2 * 6.2831855), 10.0f * (float)std::cos(0.2 * 6.2831855));
        if (!nearv(c.PointingAt, Vec(1, 2, 1)) || !nearv(c.Pos, Vec(1, 2, 1) + off)) ++fails;
        // velocity (0.5,0,0)*10 = 5 -> theta += (5*0.0005 + 0.2 - 0.2) * 0.05
        if (!near(c.Theta, 0.2f + 0.0025f * 0.05f) || !near(c.Rad, 10.0f)) ++fails;
        // facing (0,1,0) -> behind (0,-10,0); offset points along +Y: half a turn apart (cross product 0: sign +)
        if (!near(c.Phi, 1.25f + 0.5f * 0.1f)) ++fails;
    }
    // 3. things on both sides of the hero: theta eases towards 0.08 + speed term
    {
        Camera c = MakeCamera(); World w = MakeWorld();
        c.LocalThingPosList.push_back(Vec(5, 2, 1)); c.LocalThingPosList.push_back(Vec(-3, 2, 1));
        c.Update(w);
        if (!near(c.Theta, 0.2f + (0.0025f + 0.08f - 0.2f) * 0.05f)) ++fails;
    }
    // 4. things on one side: look from behind the flattened sum (rotation from (-1,0,0)*10 towards +Y offset: quarter turn)
    {
        Camera c = MakeCamera(); World w = MakeWorld();
        c.LocalThingPosList.push_back(Vec(5, 2, 4)); c.LocalThingPosList.push_back(Vec(6, 2, -2));
        c.Update(w);
        float expect = RotationAngle(Vec(-10, 0, 0), Vec(0, 1, 0));
        if (!near(c.Phi, 1.25f + expect * 0.1f) || !near(c.Theta, 0.2f + 0.0025f * 0.05f)) ++fails;
    }
    // 5. fixed camera released after half a second, then blocked view: radius re-measured, unblocked point + 1.5
    {
        Camera c = MakeCamera(); World w = MakeWorld(); c.ActAsFixedCam = true; w.LineOfSight = false;
        c.Update(w);
        if (c.ActAsFixedCam || !near(c.Rad, (mag(Vec(0, 10, -1)) * 0.99f + 0.1f)) || !nearv(c.Pos, Vec(3, 3, 4.5f))) ++fails;
    }
    // 6. fixed camera not yet released (building UID set): position stays, then thing collision picks the first draw coord
    {
        Camera c = MakeCamera(); World w = MakeWorld(); c.ActAsFixedCam = true; c.BuildingUID = 7; w.ThingCollision = true;
        c.DrawCoordList.push_back(Vec(9, 8, 7)); c.DrawCoordList.push_back(Vec(1, 1, 1));
        c.Update(w);
        if (!c.ActAsFixedCam || !nearv(c.Pos, Vec(9, 8, 7)) || !near(c.Rad, 10.0f) || !near(c.Theta, 0.2f)) ++fails;
    }
    if (fails) { std::printf("006fdd60_TEST FAIL\n"); return 1; }
    std::printf("006fdd60_TEST PASS\n");
    return 0;
}
