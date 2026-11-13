// Behaviour test for 0x00B3C5D0 CEngineResourceManager::SetupAlphaStippleTexture(bool rebuild): host model of the retail flow with
// a fake engine cache file. Checked: the dither textures get their 2x2 patterns (pitch honoured); a version-8 cache fills the stipple
// texture (pitch 0x80) and the 0x2000 hardware masks from the file and writes nothing; a missing / stale cache with rebuild=false
// fires the CAFile error callback and builds nothing; with rebuild=true the cache is rewritten (version 8, the 32x32 threshold bytes
// each widened to four bytes, then the 0x8000 bytes of masks) and the same data reaches the texture and the masks.
#include <cstdio>
#include <cstring>

static const unsigned int DitherPatterns[4][4] = {
    { 0x06060606, 0x02020202, 0x00000000, 0x04040404 },
    { 0x02020202, 0x06060606, 0x04040404, 0x00000000 },
    { 0x00000000, 0x04040404, 0x06060606, 0x02020202 },
    { 0x04040404, 0x00000000, 0x02020202, 0x06060606 },
};

struct FakeFile {                       // the engine cache file (StippleAlphaTexture.dat)
    bool Exists;
    unsigned char Data[4 + 0x1000 + 0x8000];
    long Size;
};
struct Manager {
    unsigned char Dither[4][2 * 8];     // four 2x2 textures, pitch 8
    bool DitherLocked[4];
    unsigned char Stipple[0x1000];      // the 32x32 ARGB stipple texture level 0
    long StipplePitch;
    unsigned int HW[0x2000];
    int ErrorCalls;
    int BuildCalls;
    FakeFile* File;

    void BuildAlphaStippleTexture(long size, unsigned char* pattern) { ++BuildCalls; for (long i = 0; i < size * size; ++i) pattern[i] = (unsigned char)(i * 7 + 1); }
    void BuildHWStipplePatterns(const unsigned char* pattern) { for (long i = 0; i < 0x2000; ++i) HW[i] = pattern[i & 0x3ff] * 0x01010101U + (unsigned int)i; }
    void CopyStipple(const unsigned char* data, long pitch) { StipplePitch = pitch; std::memcpy(Stipple, data, 0x1000); }

    void SetupAlphaStippleTexture(bool rebuild)     // 0x00B3C5D0
    {
        unsigned char stipple[0x1000];
        std::memset(stipple, 0, sizeof(stipple));
        for (unsigned int i = 0; i < 4; ++i) {
            unsigned char* mem = Dither[i]; long pitch = 8;
            DitherLocked[i] = true;
            ((unsigned int*)mem)[0] = DitherPatterns[i][0];
            ((unsigned int*)mem)[1] = DitherPatterns[i][1];
            ((unsigned int*)(mem + pitch))[0] = DitherPatterns[i][2];
            ((unsigned int*)(mem + pitch))[1] = DitherPatterns[i][3];
            DitherLocked[i] = false;
        }
        if (File->Exists) {
            int version; std::memcpy(&version, File->Data, 4);
            if (version == 8) {
                std::memcpy(stipple, File->Data + 4, 0x1000);
                CopyStipple(stipple, 0x80);
                std::memcpy(HW, File->Data + 4 + 0x1000, 0x8000);
                return;
            }
        }
        if (!rebuild) { ++ErrorCalls; return; }
        long size = 0;
        int eight = 8; std::memcpy(File->Data + size, &eight, 4); size += 4;
        unsigned char pattern[0x400];
        BuildAlphaStippleTexture(32, pattern);
        for (long i = 0; i < 0x400; ++i) {
            stipple[i * 4 + 0] = pattern[i]; stipple[i * 4 + 1] = pattern[i];
            stipple[i * 4 + 2] = pattern[i]; stipple[i * 4 + 3] = pattern[i];
        }
        std::memcpy(File->Data + size, stipple, 0x1000); size += 0x1000;
        CopyStipple(stipple, 0x80);
        BuildHWStipplePatterns(pattern);
        std::memcpy(File->Data + size, HW, 0x8000); size += 0x8000;
        File->Size = size; File->Exists = true;
    }
};

static FakeFile g_File;
static Manager g_M;

int main()
{
    int fails = 0;
    std::memset(&g_File, 0, sizeof(g_File)); std::memset(&g_M, 0, sizeof(g_M)); g_M.File = &g_File;
    // 1. no cache, no rebuild: error callback, nothing built.
    g_M.SetupAlphaStippleTexture(false);
    if (g_M.ErrorCalls != 1 || g_M.BuildCalls != 0 || g_File.Exists) ++fails;
    unsigned int t; std::memcpy(&t, g_M.Dither[2] + 8, 4);
    if (t != 0x06060606U || g_M.DitherLocked[2]) ++fails;
    std::memcpy(&t, g_M.Dither[3] + 12, 4);
    if (t != 0x06060606U) ++fails;
    // 2. rebuild: cache written, texture and masks filled.
    g_M.SetupAlphaStippleTexture(true);
    if (g_M.BuildCalls != 1 || !g_File.Exists || g_File.Size != 4 + 0x1000 + 0x8000) ++fails;
    int v; std::memcpy(&v, g_File.Data, 4);
    if (v != 8 || g_M.StipplePitch != 0x80) ++fails;
    if (g_M.Stipple[0] != 1 || g_M.Stipple[3] != 1 || g_M.Stipple[4] != 8 || g_File.Data[4 + 7] != 8) ++fails;
    if (g_M.HW[0] != 0x01010101U || g_M.HW[5] != 36 * 0x01010101U + 5) ++fails;
    // 3. version-8 cache: loaded, nothing rebuilt.
    std::memset(g_M.Stipple, 0, sizeof(g_M.Stipple)); std::memset(g_M.HW, 0, sizeof(g_M.HW));
    g_M.SetupAlphaStippleTexture(false);
    if (g_M.BuildCalls != 1 || g_M.ErrorCalls != 1 || g_M.Stipple[4] != 8 || g_M.HW[5] != 36 * 0x01010101U + 5) ++fails;
    // 4. stale cache (version 7) without rebuild: error callback again.
    int seven = 7; std::memcpy(g_File.Data, &seven, 4);
    g_M.SetupAlphaStippleTexture(false);
    if (g_M.ErrorCalls != 2 || g_M.BuildCalls != 1) ++fails;
    if (fails) { std::printf("00b3c5d0_TEST FAIL\n"); return 1; }
    std::printf("00b3c5d0_TEST PASS\n");
    return 0;
}