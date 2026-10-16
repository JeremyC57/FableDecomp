// Behaviour test of CTCCreatureItemLevitationNymph::SetTarget (0x007F83F0).
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
    Thing me = { { 3, 4, 9 }, false }, t = { { 0, 0, 1 }, false };
    Nymph n; n.parent = &me; n.target = 0; n.DirX = n.DirY = 0;
    n.SetTarget(&t, 2.5f, 1.5f);
    if (n.target != &t || n.Magnitude != 2.5f || n.Height != 1.5f || std::fabs(n.DirX - 0.6f) > 1e-5f || std::fabs(n.DirY - 0.8f) > 1e-5f) { std::printf("007f83f0_TEST FAIL\n"); return 1; }
    std::printf("007f83f0_TEST PASS\n");
    return 0;
}
