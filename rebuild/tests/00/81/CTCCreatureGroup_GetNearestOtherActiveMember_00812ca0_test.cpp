#include <cstdio>
// behaviour test: GetNearestOtherActiveMember -- null in, null out; inactive members and the creature itself (same unique id)
// are skipped; the first candidate seeds the best squared distance and later ones replace it when their squared distance is
// below the best squared distance squared (retail quirk kept)
struct V { float X, Y, Z; };
struct Member { int Id; V Pos; bool Active; };
static float Sq(const V& a, const V& b) { float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z; return x * x + y * y + z * z; }
static const Member* Nearest(const Member* creature, const Member* m, int n)
{
    if (creature == 0) return 0;
    const Member* nearest = 0; float best = 0;
    for (int i = 0; i < n; ++i) {
        if (!m[i].Active || m[i].Id == creature->Id) continue;
        float d = Sq(m[i].Pos, creature->Pos);
        if (nearest != 0) { if (d < best * best) { nearest = &m[i]; best = d; } }
        else { nearest = &m[i]; best = d; }
    }
    return nearest;
}
int main()
{
    Member m[4] = { { 1, { 0, 0, 0 }, true }, { 2, { 9, 0, 0 }, true }, { 3, { 2, 0, 0 }, true }, { 4, { 1, 0, 0 }, false } };
    bool ok = Nearest(0, m, 4) == 0;
    ok = ok && Nearest(&m[0], m, 4) == &m[2];          // 81 then 4 < 81*81; the inactive one at 1 is skipped
    ok = ok && Nearest(&m[1], m, 2) == &m[0];          // itself skipped
    Member solo[1] = { { 1, { 0, 0, 0 }, true } };
    ok = ok && Nearest(&solo[0], solo, 1) == 0;
    if (!ok) { std::printf("00812ca0_TEST FAIL\n"); return 1; }
    std::printf("00812ca0_TEST PASS\n");
    return 0;
}