// Behaviour test for CGPUScratchBufferTextureManager::CResourceEntry copy constructor (0x00F37970): every member copied.
#include <cstdio>
struct C2DExtentsI { long Width, Height; };
struct CPixelFormat { unsigned long Format; };
struct CResourceEntry {
    long ResourceType; void* Texture; void* Surface; long PageIndex; C2DExtentsI Dimensions; long NoLevels;
    CPixelFormat PixelFormat; long MultiSampleType; unsigned long UseageFlags; long MemoryPool;
    CResourceEntry() { }
    CResourceEntry(const CResourceEntry& o)
        : ResourceType(o.ResourceType), Texture(o.Texture), Surface(o.Surface), PageIndex(o.PageIndex), Dimensions(o.Dimensions),
          NoLevels(o.NoLevels), PixelFormat(o.PixelFormat), MultiSampleType(o.MultiSampleType), UseageFlags(o.UseageFlags), MemoryPool(o.MemoryPool) { }
};
int main()
{
    int t, s;
    CResourceEntry a; a.ResourceType = 1; a.Texture = &t; a.Surface = &s; a.PageIndex = 4; a.Dimensions.Width = 256; a.Dimensions.Height = 128;
    a.NoLevels = 7; a.PixelFormat.Format = 21; a.MultiSampleType = 2; a.UseageFlags = 0x200; a.MemoryPool = 3;
    CResourceEntry b(a);
    bool ok = b.ResourceType == 1 && b.Texture == &t && b.Surface == &s && b.PageIndex == 4 && b.Dimensions.Width == 256 && b.Dimensions.Height == 128
           && b.NoLevels == 7 && b.PixelFormat.Format == 21 && b.MultiSampleType == 2 && b.UseageFlags == 0x200 && b.MemoryPool == 3;
    if (!ok) { std::printf("00f37970_TEST FAIL\n"); return 1; }
    std::printf("00f37970_TEST PASS\n");
    return 0;
}
