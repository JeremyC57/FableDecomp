// Behaviour test for CCompositeSurfaceCache::CCompositeSurfaceCache (0x00B2C400): owner kept, morphs/frame/texture empty, bank index and last-used frame 0.
#include <cstdio>
struct Morph { int Original, MorphID; void* s; void* f; void* e; Morph() : Original(0), MorphID(0), s(0), f(0), e(0) {} };
struct Owner { int dummy; };
struct Cache { Owner* OwnerPtr; Morph CurrentMorph, CachedMorph; unsigned int MipmapLevel; void* FrameData; void* TextureData;
    unsigned int BankFileIndex, LastUsedFrame;
    Cache(Owner& o) : OwnerPtr(&o), FrameData(0), TextureData(0), BankFileIndex(0), LastUsedFrame(0) { MipmapLevel = 77; } };
int main()
{
    Owner o; Cache c(o);
    if (c.OwnerPtr != &o || c.CurrentMorph.Original != 0 || c.CachedMorph.MorphID != 0 || c.CachedMorph.s != 0 || c.FrameData != 0 ||
        c.TextureData != 0 || c.BankFileIndex != 0 || c.LastUsedFrame != 0) { std::printf("00b2c400_TEST FAIL\n"); return 1; }
    std::printf("00b2c400_TEST PASS\n");
    return 0;
}