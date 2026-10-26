// behaviour test for 0x004E0923 CHighlightItemDef::operator=: the base part is assigned first (once), then every member (ParticleEmitter, PickupParticleEmitter, RotationSpeed, DistanceFromGround, BobDistance, BobTime, StartAsRunning, StartAsRunningDelay)
// is copied, and *this is returned; the base's own bytes are left to the base operator=
#include <cstdio>
#include <cstring>
static int gBaseCalls = 0;
struct Vec2 {
    float X, Y;
    Vec2() : X(0.0f), Y(0.0f) { }
    Vec2(float x, float y) : X(x), Y(y) { }
    bool operator==(const Vec2& o) const { return X == o.X && Y == o.Y; }
};
struct Base {
    int Tag;
    Base() : Tag(0) { }
    Base& operator=(const Base&) { ++gBaseCalls; return *this; }
};
struct Def : public Base {
    int ParticleEmitter;
    int PickupParticleEmitter;
    float RotationSpeed;
    float DistanceFromGround;
    float BobDistance;
    float BobTime;
    bool StartAsRunning;
    float StartAsRunningDelay;
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    b.ParticleEmitter = 3;
    b.PickupParticleEmitter = 10;
    b.RotationSpeed = 1.5f + 2;
    b.DistanceFromGround = 1.5f + 3;
    b.BobDistance = 1.5f + 4;
    b.BobTime = 1.5f + 5;
    b.StartAsRunning = true;
    b.StartAsRunningDelay = 1.5f + 7;
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (!(a.ParticleEmitter == b.ParticleEmitter) || !(a.PickupParticleEmitter == b.PickupParticleEmitter) || !(a.RotationSpeed == b.RotationSpeed) || !(a.DistanceFromGround == b.DistanceFromGround) || !(a.BobDistance == b.BobDistance) || !(a.BobTime == b.BobTime) || !(a.StartAsRunning == b.StartAsRunning) || !(a.StartAsRunningDelay == b.StartAsRunningDelay)) fail |= 4;
    if (a.Tag != 11) fail |= 8;
    if (fail) { std::printf("004e0923_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e0923_TEST PASS\n");
    return 0;
}