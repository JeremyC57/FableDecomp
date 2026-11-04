// Behaviour test for 0x00750110 CTCChest::DisplayLockedMessage() const: a model of the row over stub text bank / subtitle calls;
// checks nothing is shown without a chest sub-def or with no opener object / no openers required, the single-key text for one
// opener, the multiple-keys text with the count substituted otherwise, and the subtitle call's fixed arguments.
#include <cstdio>
#include <cstring>
#include <string>
struct ChestDef { long OpenerObject, OpenersRequired; };
static std::string g_shown; static long g_time; static int g_calls;
static std::string GetTextBySymbol(const char* s) { return std::string("[") + s + "]"; }
static void ConstructString(std::string& text, long n) { char b[16]; std::sprintf(b, "%ld", n); text += b; }
static void SetText(const std::string& t, long time, float delay, bool a, bool b) { g_shown = t; g_time = time; ++g_calls; (void)delay; (void)a; (void)b; }
static void DisplayLockedMessage(const ChestDef* chestDef)
{
    if (chestDef && chestDef->OpenerObject > 0 && chestDef->OpenersRequired > 0) {
        std::string text;
        if (chestDef->OpenersRequired == 1)
            text = GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_SINGLE_KEY");
        else {
            text = GetTextBySymbol("TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS");
            ConstructString(text, chestDef->OpenersRequired);
        }
        SetText(text, 0x122, 0.0f, false, true);
    }
}
int main()
{
    int ok = 1;
    DisplayLockedMessage(0); ok &= g_calls == 0;
    ChestDef d = { 0, 1 }; DisplayLockedMessage(&d); ok &= g_calls == 0;
    d.OpenerObject = 7; d.OpenersRequired = 0; DisplayLockedMessage(&d); ok &= g_calls == 0;
    d.OpenersRequired = 1; DisplayLockedMessage(&d);
    ok &= g_calls == 1 && g_shown == "[TEXT_GUI_CHEST_LOCKED_SINGLE_KEY]" && g_time == 0x122;
    d.OpenersRequired = 3; DisplayLockedMessage(&d);
    ok &= g_calls == 2 && g_shown == "[TEXT_GUI_CHEST_LOCKED_MULTIPLE_KEYS]3";
    std::printf(ok ? "00750110_TEST PASS\n" : "00750110_TEST FAIL\n");
    return ok ? 0 : 1;
}