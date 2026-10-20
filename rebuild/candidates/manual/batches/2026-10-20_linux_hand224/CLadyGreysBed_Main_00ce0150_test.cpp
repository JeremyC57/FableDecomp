#include <cstdio>
#include <cstring>
struct Game {
    int frames; int usedAt; int sleptAt; int given;
    void NewScriptFrame() { ++frames; }
    bool MsgIsUsedBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && frames >= usedAt; }
    bool MsgOnHeroSlept(bool& s) const { s = frames >= sleptAt; return s; }
    void GiveHeroObject(const char* o, long n, bool b) { if (std::strcmp(o, "OBJECT_SILVER_KEY") == 0 && n == -1 && !b) ++given; }
};
struct Bed {
    Game* G; int doneAt;
    bool IsDone() { return G->frames >= doneAt; }
    void Main()
    {
        while (!G->MsgIsUsedBy("SCRIPT_NAME_HERO")) { G->NewScriptFrame(); if (IsDone()) return; }
        if (IsDone()) return;
        bool slept;
        while (!G->MsgOnHeroSlept(slept)) { G->NewScriptFrame(); if (IsDone()) return; }
        if (!IsDone()) G->GiveHeroObject("OBJECT_SILVER_KEY", -1, false);
    }
};
int main()
{
    Game g = { 0, 2, 5, 0 }; Bed a = { &g, 100 }; a.Main();
    Game h = { 0, 2, 50, 0 }; Bed b = { &h, 10 }; b.Main();
    if (g.given != 1 || g.frames != 5 || h.given != 0 || h.frames != 10) { std::printf("00ce0150_TEST FAIL\n"); return 1; }
    std::printf("00ce0150_TEST PASS\n");
    return 0;
}
