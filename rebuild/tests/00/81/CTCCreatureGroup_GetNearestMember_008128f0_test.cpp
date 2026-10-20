// Behaviour test for CTCCreatureGroup::GetNearest(Active)Member (008128f0): the first (active) member seeds the best
// distance-squared; later members replace it only when their distance-squared is below the SQUARE of the stored best.
#include <cstdio>

struct V { float x, y, z; };
struct Member { V pos; bool active; };
static const bool kActiveOnly = 0;
static int Nearest(const Member* m, int n, const V& p)
{
    int nearest = -1;
    float best = 0;
    for (int i = 0; i < n; ++i) {
        if (kActiveOnly && !m[i].active) continue;
        float dx = m[i].pos.x - p.x, dy = m[i].pos.y - p.y, dz = m[i].pos.z - p.z;
        float d = dz * dz; d += dy * dy; d += dx * dx;
        if (nearest >= 0) {
            if (d < best * best) { nearest = i; best = d; }
        } else { nearest = i; best = d; }
    }
    return nearest;
}

int main()
{
    V p = { 0, 0, 0 };
    Member m[3] = { { { 3, 0, 0 }, false }, { { 1, 0, 0 }, true }, { { 2, 0, 0 }, true } };
    int r = Nearest(m, 3, p);
    if (kActiveOnly ? r != 1 : r != 1) { std::printf("008128f0_TEST FAIL\n"); return 1; }
    Member far[2] = { { { 0.5f, 0, 0 }, true }, { { 0.4f, 0, 0 }, true } };   // 0.16 < 0.25^2 is false: first stays
    if (Nearest(far, 2, p) != 0) { std::printf("008128f0_TEST FAIL\n"); return 1; }
    if (Nearest(m, 0, p) != -1) { std::printf("008128f0_TEST FAIL\n"); return 1; }
    std::printf("008128f0_TEST PASS\n");
    return 0;
}