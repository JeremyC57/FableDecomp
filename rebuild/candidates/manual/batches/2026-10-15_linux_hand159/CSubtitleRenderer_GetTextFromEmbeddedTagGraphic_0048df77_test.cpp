// Behaviour test for 0x0048DF77: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: the GUI def's controller graphics, a text bank keyed by symbol, the empty wide string.
struct Str { char s[40]; Str() { s[0] = 0; } Str(const char* t) { std::strcpy(s, t); } bool eq(const char* t) const { return std::strcmp(s, t) == 0; } };
struct Gui { int A, B, X, Y, DPad, DPadDown, DPadLeft, DPadRight; };
static Gui g_gui = { 10, 11, 12, 13, 14, 15, 16, 17 };
static Str GetTextBySymbol(const Str& sym) { Str r("W:"); std::strcat(r.s, sym.s); return r; }
static Str Get(int graphic)
{
    Str tag;
    const Gui* gui = &g_gui;
    if (graphic == gui->A) tag = Str("TEXT_GUI_CONTROLLER_LEFT");
    else if (graphic == gui->B) tag = Str("TEXT_GUI_CONTROLLER_RIGHT");
    else if (graphic == gui->X) tag = Str("TEXT_GUI_CONTROLLER_LEFT");
    else if (graphic == gui->Y) tag = Str("TEXT_GUI_CONTROLLER_RIGHT");
    else if (graphic == gui->DPadLeft) tag = Str("TEXT_GUI_CONTROLLER_LEFT");
    else if (graphic == gui->DPadRight) tag = Str("TEXT_GUI_CONTROLLER_RIGHT");
    if (!tag.eq("")) return GetTextBySymbol(tag);
    return Str("");
}
static void Run()
{
    CHECK(Get(10).eq("W:TEXT_GUI_CONTROLLER_LEFT") && Get(12).eq("W:TEXT_GUI_CONTROLLER_LEFT") && Get(16).eq("W:TEXT_GUI_CONTROLLER_LEFT"));
    CHECK(Get(11).eq("W:TEXT_GUI_CONTROLLER_RIGHT") && Get(13).eq("W:TEXT_GUI_CONTROLLER_RIGHT") && Get(17).eq("W:TEXT_GUI_CONTROLLER_RIGHT"));
    CHECK(Get(14).eq("") && Get(15).eq("") && Get(99).eq(""));
}

int main()
{
    Run();
    if (g_fail) { std::printf("0048df77_TEST FAIL\n"); return 1; }
    std::printf("0048df77_TEST PASS\n");
    return 0;
}
