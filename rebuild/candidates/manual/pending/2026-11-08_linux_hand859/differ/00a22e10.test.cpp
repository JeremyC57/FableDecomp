// Behaviour test for 0x00A22E10 CRightHandedSet::MakePerpendicular: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
struct V { float x, y, z; };
static void Norm(V& v) { float l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z); v.x /= l; v.y /= l; v.z /= l; }
int main() {
    V up = { 1.0f, 1.0f, 0.0f }, fw = { 2.0f, 0.0f, 0.0f };
    Norm(fw); float d = up.y * fw.y; d += up.z * fw.z; d += up.x * fw.x;
    up.x -= fw.x * d; up.y -= fw.y * d; up.z -= fw.z * d; Norm(up);
    int ok = fw.x == 1.0f && std::fabs(up.x) < 1e-6f && std::fabs(up.y - 1.0f) < 1e-6f;
    std::printf(ok ? "00a22e10_TEST PASS\n" : "00a22e10_TEST FAIL\n");
    return ok ? 0 : 1;
}
