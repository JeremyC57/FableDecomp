// Behaviour test for 0x0049E270 CWorld::Unload: reset the main player's modes, kill a still-alive controlled creature (not immediately),
// leave a dead or missing one alone, then process the thing manager's deaths.
#include <cstdio>
class CThing { public:
    unsigned char Dead : 1;
    int killed; int killImmediate;
    CThing() : Dead(0), killed(0), killImmediate(-1) { }
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { killed++; killImmediate = immediately ? 1 : 0; }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    int resets; CThingPlayerCreature* creature;
    CPlayer() : resets(0), creature(0) { }
    void ResetModes() { resets++; }
    CThingPlayerCreature* GetPControlledCreature() const { return creature; }
};
class CPlayerManager { public:
    CPlayer main;
    CPlayer& GetMainPlayer() { return main; }
};
class CThingManager { public:
    int updates; CThingManager() : updates(0) { }
    void UpdateDeaths() { updates++; }
};
template <class T> inline bool IS_THING_ALIVE(const T& thing) { return &*thing && !thing->IsDead(); }
class CWorld { public:
    CPlayerManager* PlayerManager;
    CThingManager things;
    virtual ~CWorld() { }
    virtual CThingManager& GetThingManager() { return things; }
    void Unload()
    {
        CPlayer& player = PlayerManager->GetMainPlayer();
        player.ResetModes();
        CThingPlayerCreature* creature = player.GetPControlledCreature();
        if (IS_THING_ALIVE(creature))
            creature->Kill(false);
        GetThingManager().UpdateDeaths();
    }
};
int main()
{
    CPlayerManager pm; CWorld w; w.PlayerManager = &pm;
    int ok = 1;
    w.Unload();                                                  // no creature
    if (pm.main.resets != 1 || w.things.updates != 1) ok = 0;
    CThingPlayerCreature c; pm.main.creature = &c;
    w.Unload();                                                  // alive: killed, not immediately
    if (c.killed != 1 || c.killImmediate != 0 || w.things.updates != 2) ok = 0;
    c.Dead = 1;
    w.Unload();                                                  // dead: left alone
    if (c.killed != 1 || pm.main.resets != 3 || w.things.updates != 3) ok = 0;
    if (!ok) { std::printf("0049e270_TEST FAIL\n"); return 1; }
    std::printf("0049e270_TEST PASS\n");
    return 0;
}