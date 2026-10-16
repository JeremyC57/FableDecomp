#include <cstdio>
#include <cmath>
// Behaviour model of CTCHeroPodium::OnInitialActivate (0x00771BD0).
struct V { float X, Y, Z; };
static void Activate(const V& facing, V& ex, V& ey)
{
    ey.X = facing.X * -1.0f; ey.Y = facing.Y * -1.0f; ey.Z = facing.Z * -1.0f;
    ey.Z = 0.0f;
    float l = std::sqrt(ey.X * ey.X + ey.Y * ey.Y + ey.Z * ey.Z);
    ey.X /= l; ey.Y /= l; ey.Z /= l;
    ex.X = ey.Y; ex.Y = -ey.X; ex.Z = 0.0f;
}
int main()
{
    V f = { 0.0f, 2.0f, 5.0f }, ex, ey;
    Activate(f, ex, ey);
    if (ey.X != 0.0f || ey.Y != -1.0f || ey.Z != 0.0f || ex.X != -1.0f || ex.Y != 0.0f || ex.Z != 0.0f) { std::printf("00771bd0_TEST FAIL\n"); return 1; }
    std::printf("00771bd0_TEST PASS\n");
    return 0;
}
