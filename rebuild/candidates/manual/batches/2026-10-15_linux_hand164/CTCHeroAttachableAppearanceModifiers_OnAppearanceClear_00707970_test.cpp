#include <cstdio>
// Behaviour model of CTCHeroAttachableAppearanceModifiers::OnAppearanceClear (0x00707970).
struct Mods {
    bool loaded; bool reload; int updates; unsigned long changedMask; int count;
    void Clear()
    {
        if (loaded) { loaded = false; updates += 3; changedMask = 0xf; }
        count = 0;
        reload = true;
    }
};
int main()
{
    Mods m; m.loaded = true; m.reload = false; m.updates = 0; m.changedMask = 0; m.count = 5;
    m.Clear();
    if (m.loaded || !m.reload || m.updates != 3 || m.changedMask != 0xf || m.count) { std::printf("00707970_TEST FAIL\n"); return 1; }
    m.reload = false; m.count = 2; m.Clear();
    if (m.updates != 3 || !m.reload || m.count) { std::printf("00707970_TEST FAIL\n"); return 1; }
    std::printf("00707970_TEST PASS\n");
    return 0;
}
