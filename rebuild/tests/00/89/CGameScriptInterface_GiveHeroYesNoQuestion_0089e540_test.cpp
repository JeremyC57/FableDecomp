// Behaviour test for CGameScriptInterface::GiveHeroYesNoQuestion @ 0x0089E540: a living hero with the boast UI has it
// closed for the question and reopened after; the optional pause is 1 second; the subtitle uses the yes/no form
// unless the extra answer text is non-empty (then the four-answer form), always colour 0x24 and no timeout.
#include <cstdio>
#include <string>
struct Result { bool closed, reopened; float pause; int form; unsigned colour; };
static Result Ask(bool heroAlive, bool hasBoastUI, const std::string& extra, bool pauseGame)
{
    Result r; r.closed = false; r.reopened = false; r.pause = 0.0f; r.form = 0; r.colour = 0;
    bool boast = false;
    if (heroAlive && hasBoastUI) { boast = true; r.closed = true; }
    if (pauseGame) r.pause = 1.0f;
    r.form = extra == "" ? 5 : 6;
    r.colour = 0x24;
    if (heroAlive && boast) r.reopened = true;
    return r;
}
int main()
{
    Result a = Ask(true, true, "", true);
    if (!a.closed || !a.reopened || a.pause != 1.0f || a.form != 5 || a.colour != 0x24) { std::printf("0089e540_TEST FAIL\n"); return 1; }
    Result b = Ask(true, false, "maybe", false);
    if (b.closed || b.reopened || b.pause != 0.0f || b.form != 6) { std::printf("0089e540_TEST FAIL\n"); return 1; }
    Result c = Ask(false, true, "", false);
    if (c.closed || c.reopened) { std::printf("0089e540_TEST FAIL\n"); return 1; }
    std::printf("0089e540_TEST PASS\n");
    return 0;
}