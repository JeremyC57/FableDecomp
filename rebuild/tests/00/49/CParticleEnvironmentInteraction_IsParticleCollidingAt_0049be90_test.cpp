// Behaviour test for CParticleEnvironmentInteraction::IsParticleCollidingAt 0x0049BE90 (model copy: ground as a flat height
// field sloped in X, things as meshes that report a fixed hit; same control flow as the row).
#include <cstdio>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Hit { V p, n; float d; bool ok; };
static float g_ground = 0.0f;
static float g_slope = 0.0f;
static float GroundAt(const V& p) { return g_ground + g_slope * p.X; }
static int g_nThings = 0;
static Hit g_hits[4];
static bool Collide(bool world, const V& pos, const V& vel, bool meshes, V& hitPos, V& hitNormal, bool& hitGround)
{
    if (world) {
        V end(pos.X + vel.X, pos.Y + vel.Y, pos.Z + vel.Z);
        if (meshes) {
            float nearest = 3.402823466e+38F; bool found = false;
            for (int i = 0; i < g_nThings; ++i)
                if (g_hits[i].ok)
                if (g_hits[i].d < nearest) {
                    nearest = g_hits[i].d; hitPos = g_hits[i].p;
                    hitNormal = V(-g_hits[i].n.X, -g_hits[i].n.Y, -g_hits[i].n.Z); found = true;
                }
            if (found) { hitGround = false; return true; }
        }
        float groundZ = GroundAt(pos);
        float endGroundZ = GroundAt(end);
        if (groundZ >= pos.Z) { hitGround = true; hitPos.X = pos.X; hitPos.Y = pos.Y; hitPos.Z = groundZ; hitNormal = V(0, 0, 1); return true; }
        if (endGroundZ >= end.Z) { hitGround = true; hitPos.X = end.X; hitPos.Y = end.Y; hitPos.Z = groundZ; hitNormal = V(0, 0, 1); return true; }
        hitGround = false; return false;
    }
    if (0.0f > pos.Z) { hitPos = pos; hitPos.Z = 0.0f; hitNormal = V(0.0f, 0.0f, 1.0f); hitGround = true; return true; }
    hitGround = false; return false;
}
int main()
{
    V hp, hn; bool g = true; int bad = 0;
    // no world, below plane
    if (!Collide(false, V(1, 2, -1), V(0, 0, 0), false, hp, hn, g) || !g || hp.Z != 0.0f || hp.X != 1 || hn.Z != 1.0f) bad = 1;
    // no world, above plane
    if (Collide(false, V(1, 2, 3), V(0, 0, 0), false, hp, hn, g) || g) bad = 2;
    // mesh: nearest of the reported hits wins (nearer one first, farther after it), normal reversed; a query that reports
    // no intersection is skipped even though its distance is the smallest
    g_nThings = 3;
    g_hits[0].p = V(7, 7, 7); g_hits[0].n = V(1, 0, 0); g_hits[0].d = 4.0f; g_hits[0].ok = true;
    g_hits[1].p = V(5, 5, 5); g_hits[1].n = V(0, 1, 0); g_hits[1].d = 9.0f; g_hits[1].ok = true;
    g_hits[2].p = V(3, 3, 3); g_hits[2].n = V(0, 0, 1); g_hits[2].d = 1.0f; g_hits[2].ok = false;
    g = true;
    if (!Collide(true, V(0, 0, 10), V(1, 1, 1), true, hp, hn, g) || g || hp.X != 7 || hn.X != -1.0f) bad = 3;
    // ground under start
    g_nThings = 0; g_ground = 5.0f;
    if (!Collide(true, V(1, 2, 4), V(0, 0, -1), true, hp, hn, g) || !g || hp.Z != 5.0f || hp.X != 1) bad = 4;
    // ground under end only (ground = 2.5 * X: start 2.5, end 5.0): end XY with the START's ground height
    g_ground = 0.0f; g_slope = 2.5f;
    if (!Collide(true, V(1, 2, 6), V(1, 1, -2), false, hp, hn, g) || !g || hp.X != 2 || hp.Y != 3 || hp.Z != 2.5f) bad = 5;
    // no hit
    if (Collide(true, V(1, 2, 9), V(0, 0, 1), false, hp, hn, g) || g) bad = 6;
    if (bad) { std::printf("0049be90_TEST FAIL\n"); return 1; }
    std::printf("0049be90_TEST PASS\n");
    return 0;
}