// Behaviour test for NParticleEngine::CPSCUpdateNormal::Update (0x00B0DC20): a standalone model of the system-life gate (alive = false
// once LifeTimer / tick rate reaches SystemLifeSecs, else the normalised time clamped to [0, 1]), the per-particle integration (first
// frame scaled by 1 - CreationTime / 255, air resistance pow(1 - AirResistance, dt)), the landscape bounce (X / Y scaled, Z scaled and
// negated) and the decal-emitter list pass (dead or null emitters erased, live ones updated).
#include <cstdio>
#include <cmath>
#include <list>
struct Vec { float X, Y, Z; };
static bool SystemLife(long lifeTimer, float oneOverTickRate, float systemLifeSecs, float& normalisedTime)
{
    float lifeTime = lifeTimer * oneOverTickRate;
    normalisedTime = 0.0f;
    if (lifeTime >= systemLifeSecs)
        return false;
    if (systemLifeSecs > 0.0f) {
        normalisedTime = lifeTime / systemLifeSecs;
        if (normalisedTime < 0.0f) normalisedTime = 0.0f;
        else if (normalisedTime > 1.0f) normalisedTime = 1.0f;
    }
    return true;
}
static void Integrate(Vec& position, Vec& velocity, Vec change, short lifetimer, unsigned char creationTime, float air)
{
    if (lifetimer >= 0) {
        velocity.X += change.X; velocity.Y += change.Y; velocity.Z += change.Z;
        position.X += velocity.X; position.Y += velocity.Y; position.Z += velocity.Z;
        velocity.X *= air; velocity.Y *= air; velocity.Z *= air;
    } else {
        float fraction = 1.0f - creationTime * (1.0f / 255.0f);
        velocity.X += change.X * fraction; velocity.Y += change.Y * fraction; velocity.Z += change.Z * fraction;
        position.X += velocity.X * fraction; position.Y += velocity.Y * fraction; position.Z += velocity.Z * fraction;
        float decay = std::pow(air, fraction);
        velocity.X *= decay; velocity.Y *= decay; velocity.Z *= decay;
    }
}
struct Emitter { bool Dead; int Updates; };
static void UpdateDecalEmitters(std::list<Emitter*>& emitters)
{
    std::list<Emitter*>::iterator it = emitters.begin();
    while (it != emitters.end()) {
        if (*it != 0 && !(*it)->Dead) { ++(*it)->Updates; ++it; }
        else it = emitters.erase(it);
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    float t;
    bool ok = !SystemLife(30, 1.0f / 15.0f, 2.0f, t);                         // 2 s reached: system ends
    ok = ok && SystemLife(15, 1.0f / 15.0f, 2.0f, t) && Near(t, 0.5f);
    Vec pos = { 0, 0, 0 }, vel = { 1, 0, 0 }, change = { 0, 0, -1 };
    Integrate(pos, vel, change, 3, 0, 0.5f);
    ok = ok && Near(pos.X, 1.0f) && Near(pos.Z, -1.0f) && Near(vel.X, 0.5f) && Near(vel.Z, -0.5f);
    Vec pos2 = { 0, 0, 0 }, vel2 = { 1, 0, 0 };
    Integrate(pos2, vel2, change, -1, 255, 0.5f);                              // created at the end of the frame: no movement
    ok = ok && Near(pos2.X, 0.0f) && Near(vel2.X, 1.0f);
    float bounce = 0.5f; Vec v = { 2.0f, 4.0f, -6.0f };
    v.X *= bounce; v.Y *= bounce; v.Z = -(v.Z * bounce);
    ok = ok && Near(v.X, 1.0f) && Near(v.Y, 2.0f) && Near(v.Z, 3.0f);
    Emitter live = { false, 0 }, dead = { true, 0 };
    std::list<Emitter*> emitters; emitters.push_back(&live); emitters.push_back(0); emitters.push_back(&dead);
    UpdateDecalEmitters(emitters);
    ok = ok && emitters.size() == 1 && live.Updates == 1;
    printf(ok ? "00b0dc20_TEST PASS\n" : "00b0dc20_TEST FAIL\n");
    return ok ? 0 : 1;
}
