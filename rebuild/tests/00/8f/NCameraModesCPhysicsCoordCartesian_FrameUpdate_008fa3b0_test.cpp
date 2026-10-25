// Behaviour test for NCameraModes::CPhysicsCoordCartesian::FrameUpdate (0x008FA3B0): Euler step with the +-100 velocity
// clamp, and the culled snap to the first attractor with zero velocity.
#include <cstdio>
#include <cmath>

static long g_framesPerSecond = 10;
struct V { float X, Y, Z; };
struct Node { Node* next; Node* prev; V pos; float weight; };
struct State { V Position, Velocity, Acceleration; Node* Attractors; Node* Incidental; bool CullPhysics; };
static void Clamp(float& v) { if (v < -100.0f) v = -100.0f; else if (v > 100.0f) v = 100.0f; }
static void FrameUpdate(State* state)
{
    if (!state->CullPhysics) {
        float dt = 1.0f / (float)g_framesPerSecond;
        V vel = state->Velocity, pos = state->Position, acc = state->Acceleration;
        vel.X += acc.X * dt; vel.Y += acc.Y * dt; vel.Z += acc.Z * dt;
        Clamp(vel.X); Clamp(vel.Y); Clamp(vel.Z);
        pos.X += vel.X * dt; pos.Y += vel.Y * dt; pos.Z += vel.Z * dt;
        state->Position = pos; state->Velocity = vel;
    }
    else {
        state->Position = state->Attractors->next->pos;
        V zero = { 0.0f, 0.0f, 0.0f }; state->Velocity = zero;
    }
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-3f; }

int main()
{
    State s = { { 1, 2, 3 }, { 10, 95, -99 }, { 20, 100, -50 }, 0, 0, false };
    FrameUpdate(&s);
    bool ok = Near(s.Velocity.X, 12) && Near(s.Velocity.Y, 100) && Near(s.Velocity.Z, -100)
        && Near(s.Position.X, 2.2f) && Near(s.Position.Y, 12) && Near(s.Position.Z, -7);
    Node head, first; head.next = &first; first.pos.X = 7; first.pos.Y = 8; first.pos.Z = 9;
    s.Attractors = &head; s.CullPhysics = true;
    FrameUpdate(&s);
    ok = ok && s.Position.X == 7 && s.Position.Z == 9 && s.Velocity.X == 0 && s.Velocity.Y == 0 && s.Velocity.Z == 0;
    if (!ok) { std::printf("008fa3b0_TEST FAIL\n"); return 1; }
    std::printf("008fa3b0_TEST PASS\n");
    return 0;
}