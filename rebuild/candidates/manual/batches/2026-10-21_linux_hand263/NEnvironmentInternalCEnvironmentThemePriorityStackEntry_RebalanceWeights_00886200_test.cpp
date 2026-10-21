// behaviour test for 00886200 RebalanceWeights: locked weights keep their share, unlocked share the rest, tiny themes are
// dropped, survivors unlock and the base gets what is left
#include <cstdio>
#include <vector>
struct TW { int id; float w; bool lock; };
struct Entry { float base; bool baseLocked; std::vector<TW> themes; };
static void Rebalance(Entry& e)
{
    float total = e.base;
    float locked = e.baseLocked ? e.base : 0.0f;
    size_t i;
    for (i = 0; i < e.themes.size(); ++i) { total += e.themes[i].w; if (e.themes[i].lock) locked += e.themes[i].w; }
    if (locked > 0.9990234375f) {
        for (i = 0; i < e.themes.size(); ++i) { if (e.themes[i].lock) e.themes[i].w /= locked; else e.themes[i].w = 0.0f; }
    } else {
        for (i = 0; i < e.themes.size(); ++i) if (!e.themes[i].lock) e.themes[i].w *= (1.0f - locked) / (total - locked);
    }
    e.base = 1.0f;
    for (i = 0; i < e.themes.size();) {
        if (e.themes[i].w < 0.0009765625f) e.themes.erase(e.themes.begin() + i);
        else { e.themes[i].lock = false; e.base -= e.themes[i].w; ++i; }
    }
    e.baseLocked = false;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    Entry e; e.base = 0.5f; e.baseLocked = false;
    TW a = { 1, 0.25f, true }; TW b = { 2, 0.5f, false }; TW c = { 3, 0.0f, false };
    e.themes.push_back(a); e.themes.push_back(b); e.themes.push_back(c);
    Rebalance(e);
    // locked 0.25, total 1.25: unlocked scale 0.75/1.0 -> b = 0.375, c dropped
    if (e.themes.size() != 2 || !Near(e.themes[1].w, 0.375f) || e.themes[0].lock || !Near(e.base, 0.375f) || e.baseLocked) {
        std::printf("00886200_TEST FAIL\n"); return 1;
    }
    Entry f; f.base = 1.0f; f.baseLocked = true;
    TW d = { 4, 1.0f, true }; TW g = { 5, 0.3f, false };
    f.themes.push_back(d); f.themes.push_back(g);
    Rebalance(f);
    // locked 2.0 > 1: d = 0.5, g = 0 (dropped)
    if (f.themes.size() != 1 || !Near(f.themes[0].w, 0.5f) || !Near(f.base, 0.5f)) { std::printf("00886200_TEST FAIL\n"); return 1; }
    std::printf("00886200_TEST PASS\n");
    return 0;
}
