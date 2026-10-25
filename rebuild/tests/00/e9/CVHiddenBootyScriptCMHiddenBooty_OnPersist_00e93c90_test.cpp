// behaviour test for 0x00e93c90 CV_HiddenBootyScript::CM_HiddenBooty::OnPersist(CPersistContext&): the flags HasHeroGotAllClues, HasHeroGotAnyClues, PlayedIntroduction are transferred, in
// that order, under their own names with default false; no other member is touched
#include <cstdio>
#include <cstring>
struct Ctx {
    int n; const char* names[4]; bool* slots[4]; bool defaults[4];
    void Transfer(const char* name, bool& v, const bool& d) { names[n] = name; slots[n] = &v; defaults[n] = d; ++n; v = !v; }
};
struct Script {
    bool flags[8];
    void OnPersist(Ctx& c)
    {
        c.Transfer("HasHeroGotAllClues", flags[0], false);
        c.Transfer("HasHeroGotAnyClues", flags[1], false);
        c.Transfer("PlayedIntroduction", flags[2], false);
    }
};
int main()
{
    int fails = 0;
    Script s; for (int i = 0; i < 8; ++i) s.flags[i] = false;
    Ctx c; c.n = 0;
    s.OnPersist(c);
    if (c.n != 3) fails |= 1;
    if (std::strcmp(c.names[0], "HasHeroGotAllClues") || std::strcmp(c.names[1], "HasHeroGotAnyClues") || std::strcmp(c.names[2], "PlayedIntroduction")) fails |= 2;
    if (c.slots[0] != &s.flags[0] || c.slots[1] != &s.flags[1] || c.slots[2] != &s.flags[2]) fails |= 4;
    if (c.defaults[0] || c.defaults[1] || c.defaults[2]) fails |= 8;
    int touched = 0; for (int i = 0; i < 8; ++i) touched += s.flags[i];
    if (touched != 3) fails |= 16;
    if (fails) { std::printf("00e93c90_TEST FAIL %d\n", fails); return 1; }
    std::printf("00e93c90_TEST PASS\n");
    return 0;
}