// Behaviour test for 0x006AFE60 CTCPhysicsBase::RespondToCollision: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static void Respond(float defFriction, float defElasticity, V n, float surfaceFriction, V& v)
{
    float friction = -(defFriction * surfaceFriction);
    float k = (n.x * v.x + n.y * v.y + n.z * v.z) * (-1.0f - friction - defElasticity);
    v.x += v.x * friction + n.x * k; v.y += v.y * friction + n.y * k; v.z += v.z * friction + n.z * k;
}
int main() {
    V n = { 0.0f, 0.0f, 1.0f }; V v = { 2.0f, 0.0f, -4.0f };
    Respond(0.5f, 0.5f, n, 0.5f, v);
    // friction = -0.25: tangential 2 -> 1.5; normal -4: k = -4 * (-1 + 0.25 - 0.5) = 5 -> -4 + 1 + 5 = 2 (elastic bounce at 0.5)
    int ok = std::fabs(v.x - 1.5f) < 1e-6f && v.y == 0.0f && std::fabs(v.z - 2.0f) < 1e-6f;
    std::printf(ok ? "006afe60_TEST PASS\n" : "006afe60_TEST FAIL\n");
    return ok ? 0 : 1;
}
