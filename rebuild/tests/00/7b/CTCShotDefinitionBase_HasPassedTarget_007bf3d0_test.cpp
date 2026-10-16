#include <cstdio>
// Behaviour model of CTCShotDefinitionBase::HasPassedTarget (0x007BF3D0): dot(target - pos, pos - oldPos) < 0.
struct V { float x, y, z; };
static bool Passed(V pos, V old, V target)
{
    V m = { pos.x - old.x, pos.y - old.y, pos.z - old.z };
    V t = { target.x - pos.x, target.y - pos.y, target.z - pos.z };
    return t.x * m.x + t.y * m.y + t.z * m.z < 0.0f;
}
int main()
{
    V old = { 0, 0, 0 }, before = { 1, 0, 0 }, after = { 3, 0, 0 }, target = { 2, 0, 0 };
    if (Passed(before, old, target) || !Passed(after, before, target)) { std::printf("007bf3d0_TEST FAIL\n"); return 1; }
    std::printf("007bf3d0_TEST PASS\n");
    return 0;
}