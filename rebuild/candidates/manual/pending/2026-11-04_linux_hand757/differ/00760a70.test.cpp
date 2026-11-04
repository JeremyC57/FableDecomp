// Behaviour test for 0x00760A70 NShape::CSubdivisionTriangle::GetUpDirection: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct V { float x, y, z; };
static V Cross(V a, V b) { V r = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x }; return r; }
static V Up(V a, V b, V c)
{
    V up = { 0.0f, 0.0f, 1.0f };
    V m = { b.x - (a.x + c.x) * 0.5f, b.y - (a.y + c.y) * 0.5f, b.z - (a.z + c.z) * 0.5f };
    V side = Cross(m, up);
    V cb = { c.x - b.x, c.y - b.y, c.z - b.z };
    if (cb.x * side.x + cb.y * side.y + cb.z * side.z < 0.0f) { up.z = -1.0f; }
    return up;
}
int main() {
    V a = { 0, 0, 0 }, b = { 1, 0, 0 }, c = { 0, 1, 0 };
    int ok = Up(a, b, c).z == -1.0f && Up(a, c, b).z == 1.0f;
    std::printf(ok ? "00760a70_TEST PASS\n" : "00760a70_TEST FAIL\n");
    return ok ? 0 : 1;
}
