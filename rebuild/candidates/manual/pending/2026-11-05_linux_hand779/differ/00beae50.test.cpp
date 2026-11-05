// Behaviour test for CWaterPatchMesh::Load (0x00BEAE50): a standalone model of the stored patch -- the water type is kept for
// 2, 3, 4, 5, 6, 8 and forced to 1 otherwise; the 289 packed 66-byte vertex records (two shorts, a float, two shorts, a depth
// short, a float and the 48-byte shore table) unpack in order, the depth short scaled by 1/16384 and clamped to [0, 2].
#include <cstdio>
#include <cstring>
#include <vector>
struct Vertex { unsigned short X, Y; float Z; unsigned short Swell0, Swell1; float Depth; float ShoreU; unsigned char Shore[48]; };
static long WaterType(long type) { switch (type) { case 8: case 2: case 4: case 5: case 3: case 6: return type; default: return 1; } }
static float Depth(long packed) { float d = (float)packed * (1.0f / 16384.0f); if (d < 0.0f) d = 0.0f; if (d > 2.0f) d = 2.0f; return d; }
static void Unpack(const unsigned char* data, long count, Vertex* out)
{
    for (long i = 0; i < count; ++i) {
        const unsigned char* p = data + i * 66; short depth;
        std::memcpy(&out[i].X, p, 2); std::memcpy(&out[i].Y, p + 2, 2); std::memcpy(&out[i].Z, p + 4, 4);
        std::memcpy(&out[i].Swell0, p + 8, 2); std::memcpy(&out[i].Swell1, p + 10, 2); std::memcpy(&depth, p + 12, 2);
        out[i].Depth = Depth(depth); std::memcpy(&out[i].ShoreU, p + 14, 4); std::memcpy(out[i].Shore, p + 18, 48);
    }
}
int main()
{
    int fail = 0;
    long types[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1 }, want[] = { 1, 1, 2, 3, 4, 5, 6, 1, 8, 1, 1 };
    for (int i = 0; i < 11; ++i) if (WaterType(types[i]) != want[i]) fail = 1;
    if (Depth(16384) != 1.0f || Depth(-5) != 0.0f || Depth(32767) != 32767.0f / 16384.0f || Depth(16384 * 3) != 2.0f || Depth(8192) != 0.5f) fail = 2;
    std::vector<unsigned char> packed(289 * 66, 0);
    for (long i = 0; i < 289; ++i) {
        unsigned char* p = &packed[i * 66]; unsigned short x = (unsigned short)(i % 17), y = (unsigned short)(i / 17); float z = (float)i * 0.5f, u = 1.0f;
        short d = (short)(i * 100); unsigned short s0 = 3, s1 = 4;
        std::memcpy(p, &x, 2); std::memcpy(p + 2, &y, 2); std::memcpy(p + 4, &z, 4); std::memcpy(p + 8, &s0, 2); std::memcpy(p + 10, &s1, 2);
        std::memcpy(p + 12, &d, 2); std::memcpy(p + 14, &u, 4); for (int k = 0; k < 48; ++k) p[18 + k] = (unsigned char)(k + i);
    }
    std::vector<Vertex> v(289); Unpack(&packed[0], 289, &v[0]);
    if (v[0].X != 0 || v[0].Y != 0 || v[0].Depth != 0.0f || v[288].X != 16 || v[288].Y != 16 || v[288].Z != 144.0f || v[288].Depth != Depth(28800)) fail = 3;
    if (v[100].Depth != Depth(10000) || v[100].Swell1 != 4 || v[100].Shore[47] != (unsigned char)(147) || v[100].ShoreU != 1.0f) fail = 4;
    if (fail) { std::printf("00beae50_TEST FAIL %d\n", fail); return 1; }
    std::printf("00beae50_TEST PASS\n");
    return 0;
}
