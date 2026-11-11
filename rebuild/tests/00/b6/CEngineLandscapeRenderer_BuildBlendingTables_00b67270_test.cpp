// Behaviour test for 0x00B67270 CEngineLandscapeRenderer::BuildBlendingTables: each of the five 128x128 tables gets, at texel (x, y),
// every byte of the pixel set to floor(blend(direction, normal) * 255), where normal is the unit hemisphere normal over
// ((x-64)/64, (y-64)/64, sqrt(max(0, 1 - X*X - Y*Y))); row stride is the locked pitch, texel stride the bytes per pixel.
#include <cstdio>
#include <cmath>
#include <vector>

class C3DVector { public:
    float X, Y, Z;
    void AccurateNormalise() { float l = (float)std::sqrt(X * X + Y * Y + Z * Z); if (l > 0) { X /= l; Y /= l; Z /= l; } }
};
static long FloorToLong(float v) { return (long)std::floor(v); }
// stand-in blend: direction 0 weights Z, 1..4 weight +X, -X, +Y, -Y (clamped at 0)
static float Blend(long direction, const C3DVector& n)
{
    float v = 0;
    switch (direction) { case 0: v = n.Z; break; case 1: v = n.X; break; case 2: v = -n.X; break; case 3: v = n.Y; break; default: v = -n.Y; break; }
    return v > 0 ? v : 0;
}

static void Build(std::vector<unsigned char>* tables, long pitch, long bytes_per_pixel)
{
    unsigned char* table_bits[5];
    for (long i = 0; i < 5; ++i) { tables[i].assign(pitch * 128, 0xCD); table_bits[i] = &tables[i][0]; }
    for (long y = 0; y < 128; ++y) {
        for (long x = 0; x < 128; ++x) {
            C3DVector normal;
            normal.X = (x - 64) * (1.0f / 64.0f);
            normal.Y = (y - 64) * (1.0f / 64.0f);
            float z_squared = 1.0f - normal.X * normal.X - normal.Y * normal.Y;
            if (!(z_squared > 0.0f)) z_squared = 0.0f;
            normal.Z = (float)std::sqrt(z_squared);
            normal.AccurateNormalise();
            for (long direction = 0; direction < 5; ++direction) {
                long value = FloorToLong(Blend(direction, normal) * 255.0f);
                for (long b = 0; b < bytes_per_pixel; ++b)
                    table_bits[direction][y * pitch + x * bytes_per_pixel + b] = (unsigned char)value;
            }
        }
    }
}

#define FAIL { std::printf("00b67270_TEST FAIL\n"); return 1; }
int main()
{
    std::vector<unsigned char> tables[5];
    Build(tables, 128 * 4 + 16, 4);
    long pitch = 128 * 4 + 16;
    // centre texel: normal (0, 0, 1) -> Z table 255, side tables 0
    if (tables[0][64 * pitch + 64 * 4] != 255 || tables[1][64 * pitch + 64 * 4] != 0) FAIL;
    // all four bytes of a pixel are equal
    for (int b = 1; b < 4; ++b) if (tables[0][64 * pitch + 64 * 4 + b] != 255) FAIL;
    // x = 0 is the -X edge: the -X table is full there, the +X table empty
    if (tables[2][64 * pitch + 0] != 255 || tables[1][64 * pitch + 0] != 0) FAIL;
    // pitch padding is not written
    if (tables[0][64 * pitch + 128 * 4] != 0xCD) FAIL;
    // (96, 64): normal (0.5, 0, sqrt(0.75)) -> +X table floor(0.5 * 255) = 127
    if (tables[1][64 * pitch + 96 * 4] != 127) FAIL;
    std::printf("00b67270_TEST PASS\n");
    return 0;
}