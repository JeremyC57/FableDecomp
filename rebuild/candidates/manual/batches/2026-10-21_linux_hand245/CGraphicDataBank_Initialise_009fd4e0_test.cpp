// Behaviour test for 009fd4e0 CGraphicDataBank::Initialise: the eleven pixel formats and the blank textures created for valid ones.
#include <cstdio>
struct Init { long f[7]; };
static int Initialise(const Init& in, long* formats, int* created)
{
    static const int map[11] = { 0, 1, 3, 2, 0, 1, 4, 5, 4, 5, 6 };
    int n = 0;
    for (int i = 0; i < 11; ++i) formats[i] = in.f[map[i]];
    for (int i = 0; i < 11; ++i) { created[i] = 0; if (formats[i] != -1) { created[i] = 32; ++n; } }
    return n;
}
int main()
{
    Init in = { { 10, 11, 12, 13, -1, 15, 16 } };
    long formats[11]; int created[11];
    int n = Initialise(in, formats, created);
    if (n != 9 || formats[2] != 13 || formats[3] != 12 || formats[6] != -1 || created[6] || created[8] || formats[10] != 16) {
        std::printf("009fd4e0_TEST FAIL\n"); return 1; }
    std::printf("009fd4e0_TEST PASS\n");
    return 0;
}
