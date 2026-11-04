// Behaviour test for 0x00878BE0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <cmath>
struct V { float X, Y, Z; };
static V PosFromParams(V observed, V xAxis, V yAxis, V zAxis, float angle, float extension) {
    extension = extension + extension + 0.5f;
    float t = (extension - 0.5f) * 0.5f; float scale = 1.0f + (1.0f - 1.0f) * t;
    double r = angle * 6.2831854820251465; float s = (float)std::sin(r), c = (float)std::cos(r);
    V off = { (xAxis.X * s + yAxis.X * c) * scale, (xAxis.Y * s + yAxis.Y * c) * scale, (xAxis.Z * s + yAxis.Z * c) * scale };
    V p = { observed.X + off.X + zAxis.X * extension, observed.Y + off.Y + zAxis.Y * extension, observed.Z + off.Z + zAxis.Z * extension };
    return p;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    V o = { 10, 20, 30 }, x = { 1, 0, 0 }, y = { 0, 1, 0 }, z = { 0, 0, 1 };
    V p = PosFromParams(o, x, y, z, 0.0f, 0.0f);
    bool ok = Near(p.X, 10) && Near(p.Y, 21) && Near(p.Z, 30.5f);
    p = PosFromParams(o, x, y, z, 0.25f, 1.0f);
    ok = ok && Near(p.X, 11) && Near(p.Y, 20) && Near(p.Z, 32.5f);
    std::printf(ok ? "00878be0_TEST PASS\n" : "00878be0_TEST FAIL\n");
    return ok ? 0 : 1;
}
