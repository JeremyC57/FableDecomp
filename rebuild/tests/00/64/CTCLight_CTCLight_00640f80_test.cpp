#include <cstdio>
#include <cstdlib>
// Behaviour model of CTCLight::~CTCLight (0x00640F80): unregister unless dead, free vectors, release def and counted info.
static int g_unregistered = 0, g_defDeleted = 0, g_infoDeleted = 0;
struct Def { long refs; };
struct Info { unsigned int refs; void* data; };
static void DeleteData(void* d) { (void)d; ++g_infoDeleted; }
struct Light {
    bool dead; Info* info; void* data; Def* def; int* flare; int* lights;
    void Destroy()
    {
        if (!dead) ++g_unregistered;
        if (lights) std::free(lights);
        if (flare) std::free(flare);
        Def* d = def;
        if (d) { if (--d->refs == 0) ++g_defDeleted; def = 0; }
        if (info) { --info->refs; if (info->refs == 0) { DeleteData(info->data); delete info; } }
        data = 0; info = 0;
    }
};
int main()
{
    Def d; d.refs = 1;
    Info* i = new Info; i->refs = 1; i->data = 0;
    Light l; l.dead = false; l.info = i; l.data = 0; l.def = &d;
    l.flare = (int*)std::malloc(4); l.lights = 0;
    l.Destroy();
    if (g_unregistered != 1 || g_defDeleted != 1 || g_infoDeleted != 1 || l.def || l.info) { std::printf("00640f80_TEST FAIL\n"); return 1; }
    Def d2; d2.refs = 2;
    Light m; m.dead = true; m.info = 0; m.data = 0; m.def = &d2; m.flare = 0; m.lights = 0;
    m.Destroy();
    if (g_unregistered != 1 || d2.refs != 1 || g_defDeleted != 1) { std::printf("00640f80_TEST FAIL\n"); return 1; }
    std::printf("00640f80_TEST PASS\n");
    return 0;
}