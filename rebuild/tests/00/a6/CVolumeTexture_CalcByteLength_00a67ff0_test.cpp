#include <cstdio>
// Behaviour model of CVolumeTexture::CalcByteLength: the sum of every mip level's byte length (0 without a texture).
struct VolumeTexture {
    bool hasTexture; long levels; long levelBytes[8]; long ByteLength;
    long LevelBytes(long level) const { return levelBytes[level]; }
    void CalcByteLength()
    {
        ByteLength = 0;
        if (hasTexture) {
            long n = levels;
            for (long level = 0; level < n; ++level) ByteLength += LevelBytes(level);
        }
    }
};
int main()
{
    VolumeTexture v; v.hasTexture = true; v.levels = 3; v.levelBytes[0] = 512; v.levelBytes[1] = 64; v.levelBytes[2] = 8; v.ByteLength = 7;
    v.CalcByteLength();
    if (v.ByteLength != 584) { std::printf("00a67ff0_TEST FAIL\n"); return 1; }
    v.hasTexture = false; v.CalcByteLength();
    if (v.ByteLength != 0) { std::printf("00a67ff0_TEST FAIL\n"); return 1; }
    std::printf("00a67ff0_TEST PASS\n");
    return 0;
}