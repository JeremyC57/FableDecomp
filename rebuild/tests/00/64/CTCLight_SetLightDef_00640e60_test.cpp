#include <cstdio>
// Behaviour model of CTCLight::SetLightDef (0x00640E60): force the overridden state and copy colour, radii, flicker
// and the inverted flag from the def.
struct Info { unsigned int colour; float inner, outer, flicker; unsigned long channels; bool inverted; };
struct Def { unsigned int colour; float inner, outer, flicker; bool inverted; };
struct Light {
    Info* info; bool overridden;
    void SetLightDef(const Def& d)
    {
        if (!overridden) { overridden = true; info = new Info(); }
        info->colour = d.colour; info->inner = d.inner; info->outer = d.outer; info->flicker = d.flicker; info->inverted = d.inverted;
    }
};
int main()
{
    Light l = { 0, false };
    Def d = { 0x11223344u, 1.5f, 4.0f, 0.25f, true };
    l.SetLightDef(d);
    if (!l.overridden || !l.info || l.info->colour != 0x11223344u || l.info->inner != 1.5f || l.info->outer != 4.0f || l.info->flicker != 0.25f || !l.info->inverted) { std::printf("00640e60_TEST FAIL\n"); return 1; }
    Info* first = l.info;
    l.SetLightDef(d);
    if (l.info != first) { std::printf("00640e60_TEST FAIL\n"); return 1; }
    delete l.info;
    std::printf("00640e60_TEST PASS\n");
    return 0;
}