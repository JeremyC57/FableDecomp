// behaviour test for 0x0071D3B0 CTCHeroMorph::UpdateTattoos(CVectorMap<long, CTextureLayer>&): one layer per worn tattoo;
// a bank tattoo uses its bank texture, a valid custom tattoo reuses its cached texture while the saved file time is
// unchanged and otherwise loads the picture again (new texture id cached with the file time); invalid customs add nothing.
#include <cstdio>
#include <vector>
struct Tattoo { long bankIndex; bool invalidCustom; long fileTime; };
struct Cache { long textureID; long fileTime; };
static long g_nextTexture = 100;
static std::vector<long> UpdateTattoos(const std::vector<Tattoo>& tattoos, std::vector<Cache>& cache, int& loads)
{
    std::vector<long> layers;
    for (size_t i = 0; i < tattoos.size(); ++i) {
        const Tattoo& t = tattoos[i];
        if (t.bankIndex != -1) { layers.push_back(t.bankIndex); continue; }
        if (t.invalidCustom) continue;
        Cache& c = cache[i];
        if (c.textureID == -1 || c.fileTime != t.fileTime) {
            c.textureID = g_nextTexture++;
            c.fileTime = t.fileTime;
            ++loads;
        }
        layers.push_back(c.textureID);
    }
    return layers;
}
int main()
{
    std::vector<Tattoo> t;
    Tattoo bank = { 5, false, 0 }, custom = { -1, false, 11 }, bad = { -1, true, 0 };
    t.push_back(bank); t.push_back(custom); t.push_back(bad);
    std::vector<Cache> cache(3);
    for (int i = 0; i < 3; ++i) { cache[i].textureID = -1; cache[i].fileTime = 0; }
    int loads = 0;
    std::vector<long> a = UpdateTattoos(t, cache, loads);
    std::vector<long> b = UpdateTattoos(t, cache, loads);          // unchanged file: cached id reused
    t[1].fileTime = 12;
    std::vector<long> c = UpdateTattoos(t, cache, loads);          // changed file: reloaded
    bool ok = a.size() == 2 && a[0] == 5 && a[1] == 100 && b == a && loads == 2 && c.size() == 2 && c[1] == 101;
    if (!ok) { std::printf("0071d3b0_TEST FAIL\n"); return 1; }
    std::printf("0071d3b0_TEST PASS\n");
    return 0;
}