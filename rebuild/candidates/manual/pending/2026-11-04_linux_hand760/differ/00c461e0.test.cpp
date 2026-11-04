// Behaviour test for 0x00C461E0 (parked row): the row's logic over a flat model.
#include <cstdio>

static float Lerp(float a, float b, float t) { return a + (b - a) * t; }
static float Interp(const float c[8], float x, float y, float z) {
    return Lerp(Lerp(Lerp(c[0], c[4], x), Lerp(c[2], c[6], x), y), Lerp(Lerp(c[1], c[5], x), Lerp(c[3], c[7], x), y), z);
}
int main() {
    float c[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };   // value = 4x + 2y + z
    bool ok = Interp(c, 0, 0, 0) == 0 && Interp(c, 1, 1, 1) == 7 && Interp(c, 0.5f, 0.5f, 0.5f) == 3.5f && Interp(c, 1, 0, 0) == 4;
    std::printf(ok ? "00c461e0_TEST PASS\n" : "00c461e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
