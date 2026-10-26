#include <cstdio>
#include <cstring>
// behaviour test: CV_BeardyBaldyScript::IsHeroWearingAnyOddHairdo asks the interface, hairstyle by hairstyle and in retail order, whether the
// hero wears it, stops at the first hit and destroys every string temporary it built
static int g_live = 0;
struct CCharString {
    const char* Text;
    CCharString(const char* text, long length = -1) : Text(text) { (void)length; ++g_live; }
    ~CCharString() { --g_live; }
};
struct CScriptThing { int Id; };
struct CGameScriptInterface {
    CScriptThing Hero;
    const char* Asked[16];
    int Count;
    int Hit;                                   // index answering true, -1 none
    virtual CScriptThing& GetHero() { return Hero; }
    virtual bool IsWearingHairstyle(CScriptThing& thing, const CCharString& hairstyle) const
    {
        CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this);
        if (&thing != &Hero) return false;
        self->Asked[self->Count] = hairstyle.Text;
        return self->Count++ == Hit;
    }
};
struct CV_BeardyBaldyScript {
    CGameScriptInterface* Game;
    bool IsHeroWearingAnyOddHairdo();
};
bool CV_BeardyBaldyScript::IsHeroWearingAnyOddHairdo()
{
    bool wearing = Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_BUZZ_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_BASIN_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_MOHAWK_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_PONYTAIL_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_HAIR_PLATS_01");
    return wearing;
}
static const char* const kNames[5] = { "OBJECT_HERO_HAIR_BUZZ_01", "OBJECT_HERO_HAIR_BASIN_01", "OBJECT_HERO_HAIR_MOHAWK_01", "OBJECT_HERO_HAIR_PONYTAIL_01", "OBJECT_HERO_HAIR_PLATS_01" };
int main()
{
    CGameScriptInterface game;
    CV_BeardyBaldyScript script;
    script.Game = &game;
    for (int hit = -1; hit < 5; ++hit) {
        game.Count = 0; game.Hit = hit;
        bool r = script.IsHeroWearingAnyOddHairdo();
        int expectCount = hit < 0 ? 5 : hit + 1;
        bool ok = r == (hit >= 0) && game.Count == expectCount && g_live == 0;
        for (int i = 0; ok && i < game.Count; ++i)
            ok = std::strcmp(game.Asked[i], kNames[i]) == 0;
        if (!ok) { std::printf("00e53ad0_TEST FAIL\n"); return 1; }
    }
    std::printf("00e53ad0_TEST PASS\n");
    return 0;
}