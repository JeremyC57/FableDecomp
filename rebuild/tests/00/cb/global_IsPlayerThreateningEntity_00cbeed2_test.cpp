// Behaviour test for IsPlayerThreateningEntity (0x00CBEED2): the hero threatens a thing when wielding a weapon
// and targeting exactly that thing; the targeted thing is only queried while wielding.
#include <cstdio>

class CScriptThing { public:
    int id;
    bool operator==(const CScriptThing& other) const { return id == other.id; }
};
class CGameScriptInterface { public:
    bool wielding;
    int targetId;
    int targetQueries;
    bool IsPlayerWieldingWeapon() const { return wielding; }
    CScriptThing GetHeroTargetedThing() { ++targetQueries; CScriptThing t; t.id = targetId; return t; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

int IsPlayerThreateningEntity(const CScriptThing& thing)
{
    return g_pGameScriptInterface->IsPlayerWieldingWeapon() && g_pGameScriptInterface->GetHeroTargetedThing() == thing;
}

int main()
{
    CGameScriptInterface gsi;
    gsi.targetQueries = 0;
    g_pGameScriptInterface = &gsi;
    CScriptThing villager;
    villager.id = 7;
    gsi.wielding = false; gsi.targetId = 7;
    int unarmed = IsPlayerThreateningEntity(villager);
    gsi.wielding = true;
    int threatening = IsPlayerThreateningEntity(villager);
    gsi.targetId = 8;
    int other = IsPlayerThreateningEntity(villager);
    if (unarmed != 0 || threatening != 1 || other != 0 || gsi.targetQueries != 2) {
        std::printf("00cbeed2_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbeed2_TEST PASS\n");
    return 0;
}