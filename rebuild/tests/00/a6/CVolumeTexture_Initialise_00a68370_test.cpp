// Behaviour test for CVolumeTexture::Initialise (0x00A68370): power-of-two rounding of the three extents, the forced
// single mip level, failure when the D3DX create fails, and the byte-counter update through Sizeof().
#include <stdio.h>

static unsigned int gW, gH, gD, gLevels, gUsage; static long gFormat, gPool; static long gResult;
static int gUninit, gCalc;
static unsigned int gVolumeBytes;

static unsigned int GFGetTopBit(unsigned int value)
{
    if (value == 0)
        return (unsigned int)-1;
    if (value >= 0x80000000u)
        return 31;
    ++value;
    int r = ((int)((1u << 16) - value) >> 31) & 16;
    r += ((int)((1u << (r + 8)) - value) >> 31) & 8;
    r += ((int)((1u << (r + 4)) - value) >> 31) & 4;
    r += ((int)((1u << (r + 2)) - value) >> 31) & 2;
    r += ((int)((1u << (r + 1)) - value) >> 31) & 1;
    return (unsigned int)r;
}
static unsigned int GFGetNextPowerOfTwo(unsigned int value)
{
    if (value == 0)
        return 0;
    return 1u << GFGetTopBit(value * 2 - 1);
}

struct Extents { int X, Y, Z; };
struct FormatInfo { int D3DFormat; };
static const FormatInfo gFormats[3] = { { 21 }, { 22 }, { 50 } };

struct VolumeTexture {
    void* PD3DVolumeTexture;
    int   ByteLength;
    void* PData;
    void Uninitialise() { ++gUninit; PD3DVolumeTexture = 0; }
    void CalcByteLength() { ++gCalc; ByteLength = 1000; }
    unsigned int Sizeof() const
    {
        unsigned int size = 0;
        if (PD3DVolumeTexture) {
            if (PData)
                size = 77 + 4;
            else
                return ByteLength + 4;
        }
        return size;
    }
    bool CreateTexture(const Extents& size, int no_levels, unsigned int usage, int format, int pool)
    {
        gW = size.X; gH = size.Y; gD = size.Z; gLevels = no_levels; gUsage = usage;
        gFormat = gFormats[format].D3DFormat; gPool = pool;
        if (gResult >= 0) PD3DVolumeTexture = (void*)this;
        return gResult >= 0 ? true : false;
    }
    bool Initialise(const Extents& extents, int no_levels, int format, unsigned int usage, int pool)
    {
        Uninitialise();
        Extents size = extents;
        size.X = GFGetNextPowerOfTwo(size.X);
        size.Y = GFGetNextPowerOfTwo(size.Y);
        size.Z = GFGetNextPowerOfTwo(size.Z);
        if (no_levels != 1)
            no_levels = 1;
        if (!CreateTexture(size, no_levels, usage, format, pool))
            return false;
        CalcByteLength();
        gVolumeBytes += Sizeof();
        return true;
    }
};

int main()
{
    int ok = 1;
    ok &= GFGetNextPowerOfTwo(0) == 0 && GFGetNextPowerOfTwo(1) == 1 && GFGetNextPowerOfTwo(2) == 2;
    ok &= GFGetNextPowerOfTwo(3) == 4 && GFGetNextPowerOfTwo(5) == 8 && GFGetNextPowerOfTwo(64) == 64;
    ok &= GFGetNextPowerOfTwo(65) == 128 && GFGetNextPowerOfTwo(1000) == 1024 && GFGetNextPowerOfTwo(70000) == 131072;
    ok &= GFGetTopBit(0) == (unsigned int)-1 && GFGetTopBit(0x80000001u) == 31 && GFGetTopBit(1) == 0;

    VolumeTexture t; t.PD3DVolumeTexture = 0; t.ByteLength = 0; t.PData = 0;
    Extents e = { 100, 3, 0 };
    gResult = 0;
    ok &= t.Initialise(e, 5, 2, 8, 1);
    ok &= gW == 128 && gH == 4 && gD == 0 && gLevels == 1 && gUsage == 8 && gFormat == 50 && gPool == 1;
    ok &= gUninit == 1 && gCalc == 1 && gVolumeBytes == 1004;

    gResult = -1;
    ok &= !t.Initialise(e, 1, 0, 0, 0);
    ok &= gUninit == 2 && gCalc == 1 && gVolumeBytes == 1004 && gFormat == 21;

    if (!ok) { printf("00a68370_TEST FAIL\n"); return 1; }
    printf("00a68370_TEST PASS\n");
    return 0;
}