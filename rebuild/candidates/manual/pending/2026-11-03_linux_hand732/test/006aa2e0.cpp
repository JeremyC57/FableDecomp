#include <cstdio>
// Model of CThingPlayerCreature::DrawPreparePrimitivesForRendering: nothing is drawn (false) until the hero exists
// (g_HeroCreationState); then the creature base draw must succeed, each present hero component draws, the navigation-level
// debug text sits 0.7 above the focal point and the containing region's name 0.2 above that (scale 0.08), and the result is true.
struct Info { int drawn; float text_z[2]; int texts; };
static bool Draw(long creation_state, bool base_ok, bool debug_nav, bool inside_alive, int components, Info& info)
{
    if (!creation_state) return false;
    if (!base_ok) return false;
    info.drawn += components;
    if (debug_nav) {
        float z = 10.0f; z += 0.7f; info.text_z[info.texts++] = z;
        if (inside_alive) { z += 0.2f; info.text_z[info.texts++] = z; }
    }
    return true;
}
int main()
{
    Info a = { 0, { 0, 0 }, 0 };
    bool ok = !Draw(0, true, true, true, 6, a) && a.drawn == 0;
    ok = ok && !Draw(1, false, true, true, 6, a) && a.drawn == 0;
    ok = ok && Draw(1, true, true, true, 6, a) && a.drawn == 6 && a.texts == 2 && a.text_z[0] > 10.69f && a.text_z[0] < 10.71f
            && a.text_z[1] > 10.89f && a.text_z[1] < 10.91f;
    Info b = { 0, { 0, 0 }, 0 };
    ok = ok && Draw(1, true, true, false, 3, b) && b.texts == 1;
    std::printf(ok ? "006aa2e0_TEST PASS\n" : "006aa2e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
