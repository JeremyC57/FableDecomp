// behaviour test for 0x005839A5: only the first component holding the prefs list is used; null entries are skipped
#include <cstdio>
#include <cstring>
static int g_sliders = 0, g_text = 0, g_searched = 0;
struct Comp { bool has; Comp* find(const char* n) { ++g_searched; return (has && !std::strcmp(n, "PC_GAMEPLAY_PREFS_LIST")) ? this : 0; } };
static void Adjust(Comp** comps, unsigned n)
{
    for (unsigned i = 0; i < n; ++i) {
        Comp* p = comps[i]->find("PC_GAMEPLAY_PREFS_LIST");
        if (p) { int s[3] = { 1, 0, 1 }; for (int k = 0; k < 3; ++k) if (s[k]) ++g_sliders; ++g_text; return; }
    }
}
int main()
{
    Comp a = { false }, b = { true }, c = { true }; Comp* v[3] = { &a, &b, &c };
    Adjust(v, 3);
    Adjust(v, 0);
    if (g_searched != 2 || g_sliders != 2 || g_text != 1) { std::printf("005839a5_TEST FAIL\n"); return 1; }
    std::printf("005839a5_TEST PASS\n");
    return 0;
}