// Behaviour test for NUISystem::CText::GetDataFromDef (0x0054ED90): the font bank named by the def's font string
// replaces the component's font (old one released, new one add-ref'd, same block left alone), the text window and
// scale-text flag are copied, the def's text value is set with (true, line-break flag), the alignment ORs bit 3/4/5
// into the flag byte (other values leave it), and the def is released.
#include <cstdio>
#include <string>
#include <map>

struct Block { long refs; bool destroyed; };
static std::map<std::string, Block*> g_banks;
static int g_defReleases = 0;
struct Box { float a, b, c, d; };
struct Def { long refs; std::string font; std::string text; Box window; bool lineBreak, scale; int align; };
struct Text {
    unsigned char flags; Block* font; Box window; bool scale; std::string value; bool v1, v2;
    void Release() { if (font && --font->refs == 0) font->destroyed = true; }
    void GetDataFromDef(Def* def)
    {
        Block* b = g_banks[def->font];
        if (b) ++b->refs;                                    // the temporary
        if (font != b) { Release(); font = b; if (b) ++b->refs; }
        if (b && --b->refs == 0) b->destroyed = true;       // temporary released
        window = def->window; scale = def->scale;
        value = def->text; v1 = true; v2 = def->lineBreak;
        switch (def->align) { case 0: flags |= 8; break; case 1: flags |= 0x10; break; case 2: flags |= 0x20; break; }
        if (--def->refs == 0) ++g_defReleases;
    }
};

int main()
{
    int ok = 1;
    Block arial = { 1, false }, serif = { 1, false };
    g_banks["arial"] = &arial; g_banks["serif"] = &serif;
    Def d = { 2, "arial", "hello", { 1, 2, 3, 4 }, true, true, 1 };
    Text t = { 0, 0, { 0, 0, 0, 0 }, false, "", false, false };
    t.GetDataFromDef(&d);
    ok &= t.font == &arial && arial.refs == 2 && t.flags == 0x10 && t.window.c == 3 && t.scale && t.value == "hello";
    ok &= t.v1 && t.v2 && d.refs == 1 && g_defReleases == 0;
    d.refs = 1; d.font = "serif"; d.align = 2; d.lineBreak = false; d.scale = false;
    arial.refs = 1;                                          // only the text holds it now
    t.GetDataFromDef(&d);
    ok &= t.font == &serif && serif.refs == 2 && arial.destroyed && t.flags == 0x30 && !t.v2 && !t.scale && g_defReleases == 1;
    d.refs = 1; d.align = 5;
    t.GetDataFromDef(&d);                                    // same font, unknown alignment
    ok &= t.font == &serif && serif.refs == 2 && t.flags == 0x30;
    std::printf(ok ? "0054ed90_TEST PASS\n" : "0054ed90_TEST FAIL\n");
    return ok ? 0 : 1;
}