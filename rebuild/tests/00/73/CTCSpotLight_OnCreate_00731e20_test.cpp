#include <cstdio>
// Behaviour model of CTCSpotLight::OnCreate (0x00731E20): with a def and no override, the light info is created on demand and
// takes colour, radii, flicker, angle and width from the def.
struct Def { unsigned int colour; float inner, outer, width, flicker, angle; };
struct Info { float angle, width, inner, outer, flicker; unsigned int colour; };
static Info g_store;
static bool OnCreate(const Def* def, bool overridden, Info*& info)
{
    if (!overridden && def) {
        if (!info) info = &g_store;
        info->colour = def->colour; info->inner = def->inner; info->outer = def->outer;
        info->flicker = def->flicker; info->angle = def->angle; info->width = def->width;
        return true;
    }
    return false;
}
int main()
{
    Def d = { 0xff112233u, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
    Info* info = 0;
    bool ok = OnCreate(&d, false, info) && info == &g_store && g_store.colour == 0xff112233u && g_store.inner == 1.0f
           && g_store.outer == 2.0f && g_store.width == 3.0f && g_store.flicker == 4.0f && g_store.angle == 5.0f;
    Info* none = 0;
    ok = ok && !OnCreate(&d, true, none) && !none && !OnCreate(0, false, none);
    if (!ok) { std::printf("00731e20_TEST FAIL\n"); return 1; }
    std::printf("00731e20_TEST PASS\n");
    return 0;
}