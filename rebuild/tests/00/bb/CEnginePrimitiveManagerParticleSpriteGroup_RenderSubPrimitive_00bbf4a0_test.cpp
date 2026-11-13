// Behaviour test for 0x00BBF4A0 CEnginePrimitiveManagerParticleSpriteGroup::RenderSubPrimitive: a copy of the row's sprite-packing logic
// (offset selectors, blend-mode additive flag, per-sprite constant rows with fast cos/sin, doubled / premultiplied colour, texel offset,
// batches of ten sprites with a tail flush) driven with a stub frame lookup and draw recorder.
#include <cstdio>
#include <cmath>
static float GFastCosTable[1025];
static long ToLongNear(float v) { return (long)floor(v + 0.5f); }
static float GFFastCos(float angle) { angle = angle * 1024.0f; long i = ToLongNear(angle - 0.5f); float frac = angle - (float)i; i &= 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1]; }
static float GFFastSin(float angle) { angle = angle * 1024.0f; long i = ToLongNear(angle - 0.5f); float frac = angle - (float)i; i = (i - 256) & 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1]; }
struct Block { float X, Y, Z, W; };
struct Sprite { float Pos[3]; float SizeX, SizeY, Angle; unsigned char B, G, R, A; unsigned long Frame; };
struct Frame { unsigned short TLX, TLY; };
struct Draw { unsigned long first, count, verts, tris; };
static Block ConstantBlock[40];
static Draw draws[16]; static int ndraws;
static Frame frames[4] = { {0, 0}, {32767, 0}, {0, 32767}, {16383, 8191} };
static bool GetFrame(Frame& f, unsigned long index) { if (index > 3) return false; f = frames[index]; return true; }
static bool SetupAlphaBlending(int mode) { return mode == 3 || mode == 4; }
static void Offsets(unsigned short flags, float& z, float& w)
{
    z = 0.0f; w = 0.0f;
    if (flags & 2) z = 1.0f; else if (flags & 4) z = -1.0f; else if (flags & 8) w = -1.0f; else if (flags & 0x10) w = 1.0f;
}
static unsigned long Pack(const Sprite* sprites, int n, bool additive)
{
    ndraws = 0;
    unsigned long count = 0;
    Frame frame;
    for (int k = 0; k < n; ++k) {
        const Sprite& sprite = sprites[k];
        if (!(sprite.SizeX > 0.0f && sprite.SizeY > 0.0f) || !GetFrame(frame, sprite.Frame))
            continue;
        float r = sprite.R * (1.0f / 255.0f), g = sprite.G * (1.0f / 255.0f), b = sprite.B * (1.0f / 255.0f), a = sprite.A * (1.0f / 255.0f);
        a *= 2.0f;
        if (additive) { r *= a; g *= a; b *= a; }
        Block& position = ConstantBlock[count++];
        position.X = sprite.Pos[0]; position.Y = sprite.Pos[1]; position.Z = sprite.Pos[2];
        float angle = sprite.Angle;
        Block& shape = ConstantBlock[count++];
        shape.X = GFFastCos(angle); shape.Y = GFFastSin(angle); shape.Z = sprite.SizeX; shape.W = sprite.SizeY;
        Block& col = ConstantBlock[count++];
        col.X = r; col.Y = g; col.Z = b; col.W = a;
        Block& offset = ConstantBlock[count++];
        float tlx = frame.TLX; offset.X = tlx * (1.0f / 32767.0f);
        float tly = frame.TLY; offset.Y = tly * (1.0f / 32767.0f);
        if (count == 40) { Draw d = { 4, count, 40, 20 }; draws[ndraws++] = d; count = 0; }
    }
    if (count) { unsigned long s = count >> 2; Draw d = { 4, s * 4, s * 4, s * 2 }; draws[ndraws++] = d; }
    return count;
}
static bool Near(float a, float b) { return fabs(a - b) < 2e-3f; }
int main()
{
    for (int k = 0; k <= 1024; ++k) GFastCosTable[k] = (float)cos(6.283185307179586 * k / 1024.0);
    bool ok = true;
    float z, w;
    Offsets(2, z, w); ok = ok && z == 1.0f && w == 0.0f;
    Offsets(4, z, w); ok = ok && z == -1.0f && w == 0.0f;
    Offsets(8, z, w); ok = ok && z == 0.0f && w == -1.0f;
    Offsets(0x10, z, w); ok = ok && z == 0.0f && w == 1.0f;
    Offsets(6, z, w); ok = ok && z == 1.0f;
    ok = ok && SetupAlphaBlending(3) && SetupAlphaBlending(4) && !SetupAlphaBlending(2) && !SetupAlphaBlending(0);
    Sprite s[13];
    for (int k = 0; k < 13; ++k) {
        Sprite t = { { (float)k, 2.0f * k, -1.0f }, 1.0f + k, 0.5f, 0.25f, 0, 0, 255, 128, (unsigned long)(k & 3) };
        s[k] = t;
    }
    s[3].SizeX = 0.0f;      // skipped: zero width
    s[5].SizeY = -1.0f;     // skipped: negative height
    s[7].Frame = 9;         // skipped: frame not loaded
    unsigned long left = Pack(s, 13, true);
    // 10 visible sprites -> exactly one full batch, nothing left over
    ok = ok && left == 0 && ndraws == 1 && draws[0].count == 40 && draws[0].tris == 20;
    // first sprite (k=0): position, cos/sin of a quarter turn, size, red premultiplied by doubled alpha, frame 0 offset
    ok = ok && ConstantBlock[0].X == 0.0f && ConstantBlock[0].Z == -1.0f;
    ok = ok && Near(ConstantBlock[1].X, 0.0f) && Near(ConstantBlock[1].Y, 1.0f) && ConstantBlock[1].Z == 1.0f && ConstantBlock[1].W == 0.5f;
    float a2 = 128.0f / 255.0f * 2.0f;
    ok = ok && Near(ConstantBlock[2].X, a2) && Near(ConstantBlock[2].W, a2) && ConstantBlock[2].Y == 0.0f;
    // second packed sprite is k=1 with frame 1: TLX 32767 -> 1.0
    ok = ok && Near(ConstantBlock[7].X, 1.0f) && Near(ConstantBlock[7].Y, 0.0f) && ConstantBlock[4].X == 1.0f;
    // non-additive: colour kept, alpha doubled; 3 sprites -> tail flush of 12 constants, 6 triangles
    left = Pack(s, 4, false);
    ok = ok && left == 12 && ndraws == 1 && draws[0].count == 12 && draws[0].verts == 12 && draws[0].tris == 6;
    ok = ok && Near(ConstantBlock[2].X, 1.0f) && Near(ConstantBlock[2].W, a2);
    if (!ok) { std::printf("00bbf4a0_TEST FAIL\n"); return 1; }
    std::printf("00bbf4a0_TEST PASS\n");
    return 0;
}