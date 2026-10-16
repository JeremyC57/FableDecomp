#include <cstdio>
#include <cstring>
static int g_deleted = 0, g_unlinked = 0;
struct Thing { bool dead; const char* section; bool queued;
    bool IsAlive() const { return dead ? false : true; } };
struct Mgr {
    Thing* things[4]; int nThings; Thing* dead[4]; unsigned int nDead; Thing* hero;
    void Kill(Thing* t) { t->dead = true; if (!t->queued) { t->queued = true; dead[nDead++] = t; } }
    void UpdateDeaths() { for (unsigned int i = 0; i < nDead; ++i) { ++g_unlinked; if (dead[i]) ++g_deleted; } nDead = 0; }
    void DeleteAll() { Thing* h = hero; if (h && !h->dead) Kill(h); for (int i = 0; i < nThings; ++i) if (things[i]->IsAlive()) Kill(things[i]); UpdateDeaths(); nDead = 0; }
    void DeleteSection(const char* s) { for (int i = 0; i < nThings; ++i) { Thing* t = things[i]; if (t->IsAlive() && std::strcmp(t->section, s) == 0) Kill(t); } UpdateDeaths(); nDead = 0; } };
int main()
{
    Thing a = { false, "A", false }, b = { false, "B", false }, c = { true, "A", false };
    Mgr m = { { &a, &b, &c }, 3, { 0 }, 0, 0 };
    m.DeleteSection("A");
    bool ok = a.dead && !b.dead && g_deleted == 1 && m.nDead == 0;
    m.hero = &b; m.DeleteAll();
    ok = ok && b.dead && g_deleted == 2 && g_unlinked == 2;
    if (!ok) { std::printf("0051f070_TEST FAIL\n"); return 1; }
    std::printf("0051f070_TEST PASS\n");
    return 0;
}
