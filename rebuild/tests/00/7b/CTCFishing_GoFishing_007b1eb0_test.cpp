#include <cstdio>
class CPlayer;
class CThing { public: unsigned char m_flags; CPlayer* m_player; bool IsDead() const { return (m_flags & 1) != 0; } CPlayer* GetPlayer() { return m_player; } };
class CPlayer { public: CThing* m_creature; long m_mode; int m_modeCalls; CThing* GetPlayerCreature() { return m_creature; } void AddMode(long mode, bool) { m_mode = mode; ++m_modeCalls; } };
class CPlayerManager { public: CPlayer* m_main; CPlayer* GetMainPlayer() { return m_main; } };
static CPlayerManager g_manager;
class CTCFishing { public:
    void* vptrSlot; CThing* ParentThing; long Stage; bool StageInitialised; bool m_canCreate; bool m_lastArg;
    CPlayerManager* GetPlayerManager() { return &g_manager; }
    bool CreateFish(bool arg) { m_lastArg = arg; return m_canCreate; }
    void SetStage(long stage) { if (Stage != stage) { Stage = stage; StageInitialised = false; } }
    void GoFishing(bool arg)
    {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetPlayerCreature();
        if (hero && !hero->IsDead() && CreateFish(arg)) {
            hero->GetPlayer()->AddMode(0x1d, false);
            SetStage(1);
        }
    }
};
int main()
{
    CPlayer player; CThing hero; hero.m_flags = 0; hero.m_player = &player;
    player.m_creature = 0; player.m_mode = 0; player.m_modeCalls = 0; g_manager.m_main = &player;
    CTCFishing f; f.Stage = 0; f.StageInitialised = true; f.m_canCreate = true;
    f.GoFishing(true); bool noHero = f.Stage == 0 && player.m_modeCalls == 0;
    player.m_creature = &hero; hero.m_flags = 1; f.GoFishing(true); bool dead = f.Stage == 0;
    hero.m_flags = 0; f.m_canCreate = false; f.GoFishing(false); bool noFish = f.Stage == 0 && !f.m_lastArg;
    f.m_canCreate = true; f.GoFishing(true);
    bool started = f.Stage == 1 && !f.StageInitialised && player.m_mode == 0x1d && player.m_modeCalls == 1 && f.m_lastArg;
    f.StageInitialised = true; f.GoFishing(true); bool again = f.StageInitialised && player.m_modeCalls == 2;
    if (!noHero || !dead || !noFish || !started || !again) { std::printf("007b1eb0_TEST FAIL\n"); return 1; }
    std::printf("007b1eb0_TEST PASS\n");
    return 0;
}