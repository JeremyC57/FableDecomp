// Behaviour test for NParticleEngine::CPSCUpdateNormal::UpdateAddParticle (0x00B0CAD0): a standalone model of the particle set-up --
// the emission fade gate (spawn only when random <= min + (1 - min) * fade), MaxLife = tick rate * life seconds (at least 1), the
// position history filled with the spawn position, Lifetimer -1, CreationTime = round(t * 255), AngleChange = round(speed * 63.5),
// the rotation speed lerped between the min and max angle speeds by a random fraction, and the fast-normalised rotation axis.
#include <cstdio>
#include <cmath>
struct Vec { float X, Y, Z; };
struct Particle { Vec Pos[4]; Vec Velocity; Vec Acceleration; short Lifetimer; unsigned char CreationTime; short MaxLife; signed char AngleChange; bool Active; };
static bool Spawn(Particle& p, float fadeMin, float fade, float random, float tickRate, float lifeSecs, Vec pos, Vec dir, float t,
                  float minSpeed, float maxSpeed, float speedRandom)
{
    if (random > fadeMin + (1.0f - fadeMin) * fade) return false;
    for (int i = 3; i > 0; --i) p.Pos[i] = p.Pos[i - 1];
    p.Pos[0] = pos; p.Acceleration.X = p.Acceleration.Y = p.Acceleration.Z = 0.0f;
    short life = (short)(tickRate * lifeSecs); p.MaxLife = life ? life : 1;
    p.Lifetimer = -1; p.CreationTime = (unsigned char)std::lrint(t * 255.0f);
    p.Active = true; p.Pos[1] = p.Pos[2] = p.Pos[3] = p.Pos[0]; p.Velocity = dir;
    float speed = minSpeed + (maxSpeed - minSpeed) * speedRandom;
    p.AngleChange = (signed char)std::lrint(speed * 63.5f);
    return true;
}
int main()
{
    Particle p = {}; Vec pos = { 1, 2, 3 }, dir = { 0, 0, 5 };
    bool ok = !Spawn(p, 0.25f, 0.0f, 0.5f, 15.0f, 2.0f, pos, dir, 0.5f, 0, 1, 0.5f);         // 0.5 > 0.25: no particle
    ok = ok && Spawn(p, 0.25f, 1.0f, 0.5f, 15.0f, 2.0f, pos, dir, 0.5f, 0.0f, 1.0f, 0.5f);
    ok = ok && p.MaxLife == 30 && p.Lifetimer == -1 && p.CreationTime == 128 && p.Active && p.Pos[3].Y == 2.0f && p.Velocity.Z == 5.0f
        && p.AngleChange == 32 && p.Acceleration.X == 0.0f;
    Particle q = {}; ok = ok && Spawn(q, 0, 1, 0, 15.0f, 0.01f, pos, dir, 0.0f, 0, 0, 0) && q.MaxLife == 1;
    printf(ok ? "00b0cad0_TEST PASS\n" : "00b0cad0_TEST FAIL\n");
    return ok ? 0 : 1;
}
