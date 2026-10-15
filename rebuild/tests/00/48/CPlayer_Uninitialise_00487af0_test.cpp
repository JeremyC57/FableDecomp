#include <cstdio>
static int g_killed = 0;
struct Creature { void Kill(bool) { ++g_killed; } };
struct Ptr { Creature* p; Ptr(Creature* c) : p(c) {} Creature* GetPItem() const { return p; } void SetPItem(Creature* c) { p = c; }
    Ptr& operator=(const Ptr& o) { SetPItem(o.GetPItem()); return *this; } };
struct Player {
    bool init; int baseCalls; Ptr PlayerCharacter;
    Player() : init(true), baseCalls(0), PlayerCharacter(0) {}
    bool IsInitialised() const { return init; }
    void BaseUninitialise() { ++baseCalls; init = false; }
    void Uninitialise()
    {
        if (IsInitialised() == true) {
            if (PlayerCharacter.GetPItem()) { PlayerCharacter.GetPItem()->Kill(false); PlayerCharacter = Ptr(0); }
        }
        BaseUninitialise();
    }
};
int main()
{
    Creature c; Player p; p.PlayerCharacter.SetPItem(&c);
    p.Uninitialise();
    int ok = g_killed == 1 && p.PlayerCharacter.GetPItem() == 0 && p.baseCalls == 1;
    p.PlayerCharacter.SetPItem(&c); p.Uninitialise();
    ok = ok && g_killed == 1 && p.baseCalls == 2;
    if (!ok) { std::printf("00487af0_TEST FAIL\n"); return 1; }
    std::printf("00487af0_TEST PASS\n");
    return 0;
}