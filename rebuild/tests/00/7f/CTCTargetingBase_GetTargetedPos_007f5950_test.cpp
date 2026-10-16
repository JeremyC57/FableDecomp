#include <cstdio>
// Behaviour model of CTCTargetingBase::GetTargetedPos (0x007F5950): a live targeted thing yields its fire point and true; a missing
// or dead target leaves the output untouched and returns false.
struct V { float x, y, z; };
struct Thing { bool dead; V pos; };
static bool GetPos(const Thing* t, V& out, float radius)
{
    if (t && !t->dead) { V p = { t->pos.x, t->pos.y, t->pos.z + radius }; out = p; return true; }
    return false;
}
int main()
{
    Thing live = { false, { 1, 2, 3 } }, dead = { true, { 4, 5, 6 } };
    V out = { 0, 0, 0 };
    bool a = GetPos(&live, out, 1.0f);
    V keep = out;
    bool b = GetPos(&dead, out, 1.0f), c = GetPos(0, out, 1.0f);
    if (!a || b || c || keep.z != 4.0f || out.x != 1.0f) { std::printf("007f5950_TEST FAIL\n"); return 1; }
    std::printf("007f5950_TEST PASS\n");
    return 0;
}