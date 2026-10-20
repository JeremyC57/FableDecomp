// Behaviour test for 0x00A68130 CVolumeTexture::CopyFromVolumeTexture: copies min(levels) levels pairwise with the
// filter and whole-volume boxes; every level temporary is released.
#include <cstdio>
static int g_copies = 0, g_released = 0, g_bad = 0;
struct C3DBoxI;
struct CVolume {
    long owner, level;
    CVolume(long o, long l) : owner(o), level(l) { }
    CVolume(const CVolume& v) : owner(v.owner), level(v.level) { }
    ~CVolume() { ++g_released; }
    long CopyFromVolume(const C3DBoxI* destBox, const CVolume& source, const C3DBoxI* sourceBox, unsigned long filter)
    {
        if (destBox || sourceBox || filter != 7 || owner != 1 || source.owner != 2 || source.level != level || level != g_copies) ++g_bad;
        ++g_copies; return 0;
    }
};
template <class T> inline const T& (min)(const T& a, const T& b) { return a < b ? a : b; }
struct CVolumeTexture {
    long id, levelCount;
    long GetNoLevels() const { return levelCount; }
    CVolume GetVolumeLevel(long level) const { return CVolume(id, level); }
    void CopyFromVolumeTexture(const CVolumeTexture& source, unsigned long filter)
    {
        long sourceLevels = source.GetNoLevels();
        long levels = GetNoLevels();
        long count = (min)(sourceLevels, levels);
        for (long level = 0; level < count; ++level)
            GetVolumeLevel(level).CopyFromVolume(0, source.GetVolumeLevel(level), 0, filter);
    }
};
int main()
{
    CVolumeTexture dst = { 1, 5 }, src = { 2, 3 };
    dst.CopyFromVolumeTexture(src, 7);
    bool ok = g_copies == 3 && g_bad == 0 && g_released >= 6;
    g_copies = 0; dst.levelCount = 2; src.levelCount = 4;
    dst.CopyFromVolumeTexture(src, 7);
    ok = ok && g_copies == 2 && g_bad == 0;
    if (!ok) { std::printf("00a68130_TEST FAIL\n"); return 1; }
    std::printf("00a68130_TEST PASS\n");
    return 0;
}
