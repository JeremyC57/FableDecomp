// Behaviour test for 0x0085C7D0 NCameraModes::CMoveOnCylinder::OnInit: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct V { float x, y, z; };
struct M { V ax, ay, az; float theta, height; bool cut; };
static void OnInit(M& m, V facing)
{
    m.ax = facing; V z = { 0.0f, 0.0f, 1.0f }; m.az = z;
    V y = { m.ax.y * z.z - m.ax.z * z.y, m.ax.z * z.x - m.ax.x * z.z, m.ax.x * z.y - m.ax.y * z.x }; m.ay = y;
    m.theta = 0.0f; m.height = 0.75f; m.cut = true;
}
int main() {
    M m; V f = { 1.0f, 0.0f, 0.0f }; OnInit(m, f);
    int ok = m.ay.x == 0.0f && m.ay.y == -1.0f && m.ay.z == 0.0f && m.height == 0.75f && m.cut && m.az.z == 1.0f;
    std::printf(ok ? "0085c7d0_TEST PASS\n" : "0085c7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
