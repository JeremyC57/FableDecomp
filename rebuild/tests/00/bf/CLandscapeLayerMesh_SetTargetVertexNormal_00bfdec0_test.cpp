#include <cstdio>
// behaviour copy of SetTargetVertexNormal @ 0x00bfdec0 (int instead of long: 64-bit host)
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CTVertex { unsigned short X, Y; float Z, NormalX, NormalY, NormalZ; unsigned char Pad[4]; };

static void SetTargetVertexNormal(CTVertex& vertex, unsigned int packedNormal)
{
    int x = packedNormal & 0x7ff;
    int y = (packedNormal >> 11) & 0x7ff;
    int z = packedNormal >> 22;
    x |= (x & 0x400) ? 0xfffff800 : 0;
    y |= (y & 0x400) ? 0xfffff800 : 0;
    z |= (z & 0x200) ? 0xfffffc00 : 0;
    C3DVector normal(x * (1.0f / 1023.0f), y * (1.0f / 1023.0f), z * (1.0f / 511.0f));
    vertex.NormalX = normal.X;
    vertex.NormalY = normal.Y;
    vertex.NormalZ = normal.Z;
}
static unsigned int Pack(int x, int y, int z) { return ((unsigned int)(x & 0x7ff)) | ((unsigned int)(y & 0x7ff) << 11) | ((unsigned int)(z & 0x3ff) << 22); }
static int Near(float a, float b) { float d = a - b; return d < 0.0005f && d > -0.0005f; }

int main()
{
    CTVertex v;
    v.X = 1; v.Y = 2; v.Z = 3.0f;
    int ok = 1;
    SetTargetVertexNormal(v, Pack(1023, 0, 0));
    if (!Near(v.NormalX, 1.0f) || !Near(v.NormalY, 0.0f) || !Near(v.NormalZ, 0.0f)) ok = 0;
    SetTargetVertexNormal(v, Pack(-1023, 511, -511));
    if (!Near(v.NormalX, -1.0f) || !Near(v.NormalY, 511.0f / 1023.0f) || !Near(v.NormalZ, -1.0f)) ok = 0;
    SetTargetVertexNormal(v, Pack(0, -1024, 511));
    if (!Near(v.NormalX, 0.0f) || !Near(v.NormalY, -1024.0f / 1023.0f) || !Near(v.NormalZ, 1.0f)) ok = 0;
    if (v.X != 1 || v.Y != 2 || !Near(v.Z, 3.0f)) ok = 0;    // position untouched
    if (!ok) { std::printf("00bfdec0_TEST FAIL\n"); return 1; }
    std::printf("00bfdec0_TEST PASS\n");
    return 0;
}