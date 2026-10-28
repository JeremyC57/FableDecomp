#include <cstdio>
#include <cstring>

// Test model of CVolumeTexture::GetAsTextureList: a mock volume (width x height x depth, bpp bytes per pixel,
// padded row/slice pitches) is split into one mock 2D texture per slice, copied row by row into the texture's
// own pitch; swizzle/unswizzle are PC no-ops and are only counted.
struct SBox { int RowPitch; int SlicePitch; unsigned char* PMem; };
struct SRect { int Pitch; unsigned char* PMem; };

static int g_swizzles = 0, g_unswizzles = 0, g_locks = 0, g_unlocks = 0;

struct CMockTexture {
    int Width, Height, Bpp;
    unsigned char Bits[8 * 16];                  // pitch 16
    bool Initialise(int w, int h, int bpp) { Width = w; Height = h; Bpp = bpp; std::memset(Bits, 0xEE, sizeof(Bits)); return true; }
    SRect Lock() { ++g_locks; SRect r; r.Pitch = 16; r.PMem = Bits; return r; }
    void Unlock() { ++g_unlocks; }
    void Swizzle(SRect&) { ++g_swizzles; }
    void Unswizzle(SRect&) { ++g_unswizzles; }
};

struct CMockVolume {
    int Width, Height, Depth, Bpp;
    unsigned char Mem[4 * 8 * 20];               // row pitch 20, slice pitch 160
    SBox Lock() { SBox b; b.RowPitch = 20; b.SlicePitch = 160; b.PMem = Mem; return b; }
    void GetAsTextureList(CMockTexture* textures)
    {
        long rowBytes = Bpp * Width;
        SBox box = Lock();
        ++g_unswizzles;
        unsigned char* src = box.PMem;
        for (long z = 0; z < Depth; ++z) {
            textures[z].Initialise(Width, Height, Bpp);
            SRect rect = textures[z].Lock();
            textures[z].Unswizzle(rect);
            unsigned char* dst = rect.PMem;
            unsigned char* sliceStart = src;
            for (long y = 0; y < Height; ++y) {
                std::memcpy(dst, src, rowBytes);
                dst += rect.Pitch;
                src += box.RowPitch;
            }
            src = sliceStart + box.SlicePitch;
            textures[z].Swizzle(rect);
            textures[z].Unlock();
        }
        ++g_swizzles;
    }
};

int main()
{
    CMockVolume v;
    v.Width = 3; v.Height = 5; v.Depth = 4; v.Bpp = 4;
    for (int i = 0; i < (int)sizeof(v.Mem); ++i) v.Mem[i] = (unsigned char)(i * 7);
    CMockTexture t[4];
    v.GetAsTextureList(t);
    int ok = 1;
    for (int z = 0; z < 4; ++z)
        for (int y = 0; y < 5; ++y)
            for (int x = 0; x < 12; ++x)
                if (t[z].Bits[y * 16 + x] != v.Mem[z * 160 + y * 20 + x]) ok = 0;
    if (t[0].Bits[12] != 0xEE) ok = 0;           // past the row bytes stays untouched
    if (g_locks != 4 || g_unlocks != 4 || g_swizzles != 5 || g_unswizzles != 5) ok = 0;
    if (!ok) { std::printf("00a68910_TEST FAIL\n"); return 1; }
    std::printf("00a68910_TEST PASS\n");
    return 0;
}