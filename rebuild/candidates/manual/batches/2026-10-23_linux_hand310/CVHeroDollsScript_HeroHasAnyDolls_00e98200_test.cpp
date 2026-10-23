// Behaviour test for CV_HeroDollsScript::HeroHasAnyDolls (0x00e98200): the short-circuit chain over the seven dolls.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* m_pData;
    CCharString(const char* text) : m_pData(text) {}
};
struct CScriptThing { int id; };
struct CGameScriptInterface {
    CScriptThing hero;
    unsigned int owned;          // bit i set: doll i is in the hero's inventory
    int queries;
    virtual CScriptThing& GetHero() { return hero; }
    virtual bool IsObjectInThingsPossession(const CCharString& name, const CScriptThing& thing) const
    {
        static const char* const dolls[7] = { "OBJECT_DOLL_BRIAR_ROSE_01", "OBJECT_DOLL_MAZE_01",
            "OBJECT_DOLL_SCARLET_ROBE_01", "OBJECT_DOLL_THUNDER_01", "OBJECT_DOLL_WHISPER_01",
            "OBJECT_DOLL_TWINBLADE_01", "OBJECT_DOLL_SCYTHE_01" };
        ++const_cast<CGameScriptInterface*>(this)->queries;
        if (&thing != &hero) return false;
        for (int i = 0; i < 7; ++i)
            if (std::strcmp(name.m_pData, dolls[i]) == 0) return (owned >> i) & 1;
        return false;
    }
};
struct CV_HeroDollsScript {
    CGameScriptInterface* Game;
    bool HeroHasAnyDolls()
    {
        return Game->IsObjectInThingsPossession("OBJECT_DOLL_BRIAR_ROSE_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_MAZE_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_SCARLET_ROBE_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_THUNDER_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_WHISPER_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_TWINBLADE_01", Game->GetHero())
            || Game->IsObjectInThingsPossession("OBJECT_DOLL_SCYTHE_01", Game->GetHero());
    }
};
int main()
{
    CGameScriptInterface game;
    CV_HeroDollsScript script;
    script.Game = &game;
    for (unsigned int mask = 0; mask < 128; ++mask) {
        game.owned = mask;
        game.queries = 0;
        bool expect = (mask != 0); 
        int q = 0;
        for (int i = 0; i < 7; ++i) { ++q; if ((mask >> i) & 1) break; }
        if (script.HeroHasAnyDolls() != expect || game.queries != q) {
            std::printf("00e98200_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("00e98200_TEST PASS\n");
    return 0;
}
