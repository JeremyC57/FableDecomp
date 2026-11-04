// Behaviour test for 0x00AB5030 CMouseDX::ResetNonExclusiveMouse: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
struct M { float ox, oy; bool b0; };
static void Reset(M& m, long cx, long cy, long ox, long oy, long w, long h, long bw, long bh, bool b0)
{
    if (cx >= ox && cx < w + ox && cy >= oy && cy < h + oy) {
        long x = cx - ox, y = cy - oy;
        if (x >= 0 && x < bw && y >= 0 && y < bh) { m.ox = (float)x; m.oy = (float)y; }
        m.b0 = b0;
    }
}
int main() {
    M m = { 0, 0, false }; Reset(m, 110, 120, 100, 100, 640, 480, 640, 480, true);
    int ok = m.ox == 10.0f && m.oy == 20.0f && m.b0;
    M n = { 0, 0, false }; Reset(n, 50, 120, 100, 100, 640, 480, 640, 480, true); ok &= n.ox == 0.0f && !n.b0;
    std::printf(ok ? "00ab5030_TEST PASS\n" : "00ab5030_TEST FAIL\n");
    return ok ? 0 : 1;
}
