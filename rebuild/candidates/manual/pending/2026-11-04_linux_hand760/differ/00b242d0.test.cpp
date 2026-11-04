// Behaviour test for 0x00B242D0 (parked row): the row's logic over a flat model.
#include <cstdio>

struct V { float X, Y, Z; };
static bool ToScreen(bool ready, const V& view, float halfW, float halfH, float cx, float cy, float* sx, float* sy) {
    if (ready && view.Z > 0.0f) { float iz = 1.0f / view.Z; *sx = view.X * halfW * iz + cx; *sy = cy - iz * halfH * view.Y; return true; }
    return false;
}
int main() {
    float x = 0, y = 0; V p = { 1, 1, 2 }, behind = { 0, 0, -1 };
    bool ok = ToScreen(true, p, 320, 240, 320, 240, &x, &y) && x == 480 && y == 120;
    ok = ok && !ToScreen(true, behind, 320, 240, 320, 240, &x, &y) && !ToScreen(false, p, 320, 240, 320, 240, &x, &y);
    std::printf(ok ? "00b242d0_TEST PASS\n" : "00b242d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
