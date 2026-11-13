// Behaviour test for CAIStateGroup_Gaze::Update (0x00918B90): a copy of the control flow over a small mock world.
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    V3& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    V3& operator+=(const V3& v) { X += v.X; Y += v.Y; Z += v.Z; return *this; }
    V3 operator+(const V3& v) const { return V3(X + v.X, Y + v.Y, Z + v.Z); }
    V3 operator-(const V3& v) const { return V3(X - v.X, Y - v.Y, Z - v.Z); }
    float Mag() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    // C3DVector::RotateXY (landed 0x00A14040): angle in turns; heading atan2(X, Y), elevation and length kept.
    static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
    void RotateXY(float angle) {
        const double TWO_PI = 6.2831855f;
        float xy = Wrap(Wrap((float)atan2(X, Y) * 0.15915494f) + angle);
        float yz = Wrap((float)atan2(Z, sqrt(Y * Y + X * X)) * 0.15915494f);
        float len = Mag(); float cyz = (float)cos(yz * TWO_PI);
        X = (float)(sin(xy * TWO_PI) * (cyz * len)); Y = (float)(cos(xy * TWO_PI) * (cyz * len)); Z = (float)(sin(yz * TWO_PI) * len);
    } };
struct World {
    bool haveMarker, markerBusy, ownerIsWork; V3 markerPos, markerFacing, creaturePos, sphere; float roll, chance, minJourney;
    unsigned long seed; long frame, fps;
    int terminated, setUser, moveCalls, lookCalls, idleCalls, movement; V3 movedTo, lookedAt; float rotated; long endFrame;
};
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* s) { *s *= 0x24a1; *s += 0x24df; *s = Ror13(*s); return range ? *s % range : 0; }
static void Update(World& w)
{
    if (!w.haveMarker) { w.terminated++; return; }
    if (w.markerBusy) { w.terminated++; return; }
    w.setUser++;
    V3 pos = w.markerPos, facing = w.markerFacing;
    if (w.ownerIsWork) {
        w.rotated = 0.05f - 0.1f;                        // GFFloatRandom(0.2) - 0.1 with a fixed draw of 0.05
        facing.RotateXY(w.rotated);                      // facing.RotateXY(angle) before the look-at
        V3 jitter = w.sphere; jitter.Z = 0.0f; jitter *= 0.25f; pos += jitter;
    }
    int movement = 3;
    if (w.roll <= w.chance && (w.creaturePos - pos).Mag() > w.minJourney) movement = 5;
    w.movement = movement; w.movedTo = pos; w.moveCalls++;
    w.lookedAt = facing + pos; w.lookCalls++;
    long wait = (long)GFRandom(6, &w.seed);
    w.endFrame = w.frame + (wait + 3) * w.fps;
    while (w.frame < w.endFrame) { w.idleCalls++; w.frame++; }
    w.terminated++;
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    World w = {};
    Update(w);
    if (w.terminated != 1 || w.setUser != 0) { std::printf("00918b90_TEST FAIL\n"); return 1; }
    World b = {}; b.haveMarker = true; b.markerBusy = true; Update(b);
    if (b.terminated != 1 || b.setUser != 0 || b.moveCalls != 0) { std::printf("00918b90_TEST FAIL\n"); return 1; }
    World c = {}; c.haveMarker = true; c.ownerIsWork = true; c.markerPos = V3(10, 20, 5); c.markerFacing = V3(1, 0, 0);
    c.sphere = V3(0.4f, -0.8f, 0.9f); c.creaturePos = V3(100, 20, 5); c.roll = 0.1f; c.chance = 0.5f; c.minJourney = 50.0f;
    c.seed = 12345; c.fps = 30;
    unsigned long s = 12345; long expectWait = (long)GFRandom(6, &s);
    Update(c);
    if (!Near(c.movedTo.X, 10.1f) || !Near(c.movedTo.Y, 19.8f) || !Near(c.movedTo.Z, 5.0f) || c.movement != 5
        || !Near(c.rotated, -0.05f) || !Near(c.lookedAt.Z, 5.0f)
        // facing (1,0,0) is heading 0.25 turn; -0.05 turn gives heading 0.2: (sin 0.4pi, cos 0.4pi, 0) = (0.95106, 0.30902, 0)
        || fabs(c.lookedAt.X - (10.1f + 0.9510565f)) > 1e-4f || fabs(c.lookedAt.Y - (19.8f + 0.3090170f)) > 1e-4f || c.setUser != 1 || c.terminated != 1
        || c.idleCalls != (expectWait + 3) * 30 || expectWait < 0 || expectWait > 5) { std::printf("00918b90_TEST FAIL\n"); return 1; }
    World d = c; d.terminated = d.setUser = 0; d.frame = 0; d.ownerIsWork = false; d.minJourney = 1000.0f; d.idleCalls = 0;
    Update(d);
    if (!Near(d.movedTo.X, 10.0f) || !Near(d.movedTo.Y, 20.0f) || d.movement != 3
        || !Near(d.lookedAt.X, 11.0f) || !Near(d.lookedAt.Y, 20.0f) || !Near(d.lookedAt.Z, 5.0f)) { std::printf("00918b90_TEST FAIL\n"); return 1; }
    std::printf("00918b90_TEST PASS\n");
    return 0;
}