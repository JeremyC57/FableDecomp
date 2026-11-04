// Behaviour test for CTCPhysicsBase::ImpulseRespondToCollisonWithImpassableObject (0x006AFAF0), g++ model.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; };
static void Respond(const V& collideNormal, V& velocity)
{
    V n = collideNormal;
    if (n.Y * n.Y + n.X * n.X > 1e-4f * 1e-4f) {
        n.Z = 0.0f;
        float l = std::sqrt(n.X * n.X + n.Y * n.Y);
        n.X /= l; n.Y /= l;
    }
    float dot = n.X * velocity.X + n.Y * velocity.Y + n.Z * velocity.Z;
    velocity.X -= n.X * dot; velocity.Y -= n.Y * dot; velocity.Z -= n.Z * dot;
}
int main()
{
    V n = { 2.0f, 0.0f, 5.0f }, v = { -3.0f, 4.0f, -1.0f };
    Respond(n, v);
    int ok = std::fabs(v.X) < 1e-5f && std::fabs(v.Y - 4.0f) < 1e-5f && std::fabs(v.Z + 1.0f) < 1e-5f;
    V up = { 0.0f, 0.0f, 1.0f }, w = { 1.0f, 2.0f, -3.0f };
    Respond(up, w);   // vertical normal: not flattened, the Z component is removed
    ok &= std::fabs(w.X - 1.0f) < 1e-6f && std::fabs(w.Y - 2.0f) < 1e-6f && std::fabs(w.Z) < 1e-6f;
    std::printf(ok ? "006afaf0_TEST PASS\n" : "006afaf0_TEST FAIL\n");
    return ok ? 0 : 1;
}
