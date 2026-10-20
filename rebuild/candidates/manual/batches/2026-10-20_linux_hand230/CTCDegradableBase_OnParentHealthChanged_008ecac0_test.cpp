// Behaviour test for CTCDegradableBase::OnParentHealthChanged (0x008ECAC0).
#include <cstdio>

struct Info { float hp; long smash; bool blocks; };
static Info g_stages[3] = { { 0.75f, 1, false }, { 0.5f, 2, false }, { 0.25f, 3, true } };
static int g_meshes = 0, g_smash = 0; static bool g_nav = false; static bool g_navSet = false;
struct Degradable {
    bool degradable; Info* cur; float health; bool hittable; bool hasNav;
    Info* InfoFor(float h) { Info* r = 0; for (int i = 0; i < 3; ++i) if (h <= g_stages[i].hp) r = &g_stages[i]; return r; }
    bool FullyDegraded() const { return cur && cur == &g_stages[2]; }
    void OnParentHealthChanged(bool fromHit)
    {
        if (!degradable || FullyDegraded()) return;
        Info* info = InfoFor(health);
        if (!info) return;
        if (cur && !(cur->hp - info->hp > 1e-4f)) return;
        cur = info; ++g_meshes;
        if (!fromHit) g_smash = cur->smash;
        if (FullyDegraded()) hittable = false;
        if (hasNav) { g_nav = cur->blocks; g_navSet = true; }
    }
};

int main()
{
    Degradable d = { true, 0, 0.9f, true, true };
    d.OnParentHealthChanged(false);
    if (d.cur || g_meshes) { std::printf("008ecac0_TEST FAIL\n"); return 1; }
    d.health = 0.6f; d.OnParentHealthChanged(true);
    if (d.cur != &g_stages[0] || g_meshes != 1 || g_smash != 0 || !g_navSet || g_nav) { std::printf("008ecac0_TEST FAIL\n"); return 1; }
    d.OnParentHealthChanged(false);
    if (g_meshes != 1) { std::printf("008ecac0_TEST FAIL\n"); return 1; }
    d.health = 0.1f; d.OnParentHealthChanged(false);
    if (d.cur != &g_stages[2] || g_smash != 3 || d.hittable || !g_nav) { std::printf("008ecac0_TEST FAIL\n"); return 1; }
    std::printf("008ecac0_TEST PASS\n");
    return 0;
}
