#include <cstdio>
#include <vector>
// Behaviour model of CBlendedEnvironmentTheme::BlendTextureSets: result = from's textures then to's, blends scaled by
// (1 - t) and t; CompactTextureSet receives the original count.
struct Set { std::vector<long> Textures; std::vector<float> Blends; };
static long g_compactCount = -1;
struct Theme {
    void CompactTextureSet(Set&, long count) { g_compactCount = count; }
    void BlendTextureSets(const Set& from, const Set& to, Set& result, float blend)
    {
        long count = (long)from.Textures.size();
        result.Textures.resize(count * 2, 0);
        result.Blends.resize(count * 2, 0.0f);
        for (long i = 0; i < count; ++i) {
            result.Textures[count + i] = to.Textures[i];
            result.Textures[i] = from.Textures[i];
            result.Blends[count + i] = to.Blends[i] * blend;
            result.Blends[i] = from.Blends[i] * (1.0f - blend);
        }
        CompactTextureSet(result, count);
    }
};
int main()
{
    Set a, b, r; Theme th;
    for (int i = 0; i < 5; ++i) { a.Textures.push_back(10 + i); a.Blends.push_back(1.0f); b.Textures.push_back(20 + i); b.Blends.push_back(0.5f); }
    th.BlendTextureSets(a, b, r, 0.25f);
    bool ok = r.Textures.size() == 10 && r.Blends.size() == 10 && g_compactCount == 5;
    for (int i = 0; ok && i < 5; ++i)
        ok = r.Textures[i] == 10 + i && r.Textures[5 + i] == 20 + i && r.Blends[i] == 0.75f && r.Blends[5 + i] == 0.125f;
    if (!ok) { std::printf("00887830_TEST FAIL\n"); return 1; }
    std::printf("00887830_TEST PASS\n");
    return 0;
}
