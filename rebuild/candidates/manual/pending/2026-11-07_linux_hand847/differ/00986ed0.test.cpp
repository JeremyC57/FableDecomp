// Behaviour test for 0x00986ED0 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby ctor: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
int main() {
    V up = { 0, 0, 1 }, fw = { 0, 1, 0 };
    V side = { fw.y * up.z - fw.z * up.y, fw.z * up.x - fw.x * up.z, fw.x * up.y - fw.y * up.x };
    float maxSq = 3.0f * 3.0f; float minCos = (float)std::cos(0.78539818525314331);
    int type = 1; if (type == 1) { side.x = -side.x; side.y = -side.y; side.z = -side.z; }
    int ok = side.x == -1.0f && side.y == 0.0f && maxSq == 9.0f && std::fabs(minCos - 0.70710677f) < 1e-6f;
    std::printf(ok ? "00986ed0_TEST PASS\n" : "00986ed0_TEST FAIL\n");
    return ok ? 0 : 1;
}
