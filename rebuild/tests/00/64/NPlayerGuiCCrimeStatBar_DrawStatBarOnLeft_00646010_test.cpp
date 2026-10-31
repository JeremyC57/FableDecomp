// behaviour test for NPlayerGui::CCrimeStatBar::DrawStatBarOnLeft 0x00646010: the bar end is the lerp of the def's left
// offset to pos.Y by the fill fraction, floored by the fistp helper (round-to-nearest of value-0.5, +1 when the next integer
// equals the value), flipped against the render-target height; the text is centred: (width - textWidth) * 0.5
#include <cstdio>
#include <cmath>
static long RoundNearestEven(float x)
{
    float f = (float)floor(x); float d = x - f; long r = (long)f;
    if (d > 0.5f || (d == 0.5f && (r & 1))) ++r;
    return r;
}
static long FloorToLong(float v) { long r = RoundNearestEven(v - 0.5f); if ((float)r + 1.0f == v) ++r; return r; }
int main()
{
    int fail = 0;
    float vals[] = { 3.0f, 3.2f, 3.7f, -1.5f, 0.0f, 10.5f };
    for (int i = 0; i < 6; ++i) if (FloorToLong(vals[i]) != (long)floor(vals[i])) { std::printf("floor %f -> %ld\n", vals[i], FloorToLong(vals[i])); fail = 1; }
    float offset = 4.0f, posY = 104.0f, fraction = 0.5f;
    float left = -offset;
    float y = (posY - left) * fraction + left;                       // 50
    float barY = 480.0f - (float)FloorToLong(y);                     // 430
    float textX = (640.0f - 40.0f) * 0.5f;                           // 300
    if (y != 50.0f || barY != 430.0f || textX != 300.0f) fail = 1;
    std::printf(fail ? "00646010_TEST FAIL\n" : "00646010_TEST PASS\n");
    return fail;
}