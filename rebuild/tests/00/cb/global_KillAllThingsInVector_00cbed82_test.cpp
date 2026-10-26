// Behaviour test for KillAllThingsInVector (0x00CBED82): every alive thing is removed through the game
// interface with the caller's fade flag and `true`; dead things are skipped.
#include <cstdio>

class CScriptThing { public:
    int id;
    bool alive;
    bool IsAlive() const { return alive; }
};
struct CThingVector {
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    const CScriptThing* begin() const { return _M_start; }
    const CScriptThing* end() const { return _M_finish; }
};
class CGameScriptInterfaceBase { public:
    int removed[8];
    int count;
    int badFlags;
    void RemoveThing(const CScriptThing& thing, bool fade, bool flag, bool expectedFade)
    {
        removed[count++] = thing.id;
        if (fade != expectedFade || !flag) ++badFlags;
    }
};
static bool g_expectedFade = false;

void KillAllThingsInVector(CGameScriptInterfaceBase& game, const CThingVector& things, bool fade)
{
    for (const CScriptThing* it = things.begin(); it != things.end(); ++it)
        if (it->IsAlive())
            game.RemoveThing(*it, fade, true, g_expectedFade);
}

int main()
{
    CScriptThing t[4];
    for (int i = 0; i < 4; ++i) { t[i].id = 10 + i; t[i].alive = (i != 1); }
    CThingVector v;
    v._M_start = t; v._M_finish = t + 4;
    CGameScriptInterfaceBase game;
    game.count = 0; game.badFlags = 0;
    g_expectedFade = true;
    KillAllThingsInVector(game, v, true);
    if (game.count != 3 || game.removed[0] != 10 || game.removed[1] != 12 || game.removed[2] != 13 || game.badFlags) {
        std::printf("00cbed82_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbed82_TEST PASS\n");
    return 0;
}