// Behaviour test of CTCCreatureItemLevitationNymph::UpdateLevitationTarget (0x007F84C0, vtable slot 31).
#include <cstdio>
#include <cmath>
struct V3 { float X, Y, Z; void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } } };
struct Thing { V3 pos; bool dead; };
struct Nymph {
    Thing* parent; float DirX, DirY; float Height, Magnitude; Thing* target;
    void SetTarget(Thing* t, float mag, float h)
    {
        target = t; Height = h; Magnitude = mag;
        V3 d = { parent->pos.X - t->pos.X, parent->pos.Y - t->pos.Y, 0.0f }; d.Normalise(); DirX = d.X; DirY = d.Y;
    }
    void UpdateLevitationTarget()
    {
        if (target && !target->dead) {
            V3 d = { parent->pos.X - target->pos.X, parent->pos.Y - target->pos.Y, 0.0f }; d.Normalise(); DirX = d.X; DirY = d.Y;
        }
    }
};
int main()
{
    Thing me = { { 0, 5, 0 }, false }, t = { { 0, 0, 0 }, false };
    Nymph n; n.parent = &me; n.target = 0; n.DirX = n.DirY = 7;
    n.UpdateLevitationTarget();
    if (n.DirX != 7) { std::printf("007f84c0_TEST FAIL\n"); return 1; }
    n.target = &t; n.UpdateLevitationTarget();
    if (std::fabs(n.DirX) > 1e-5f || std::fabs(n.DirY - 1.0f) > 1e-5f) { std::printf("007f84c0_TEST FAIL\n"); return 1; }
    t.dead = true; me.pos.X = 5; n.UpdateLevitationTarget();
    if (std::fabs(n.DirY - 1.0f) > 1e-5f) { std::printf("007f84c0_TEST FAIL\n"); return 1; }
    std::printf("007f84c0_TEST PASS\n");
    return 0;
}
