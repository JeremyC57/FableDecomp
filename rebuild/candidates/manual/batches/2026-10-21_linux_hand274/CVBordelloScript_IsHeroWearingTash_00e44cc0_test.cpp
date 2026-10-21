#include <cstdio>
#include <cstring>
// behaviour test: CV_BordelloScript::IsHeroWearingTash asks the interface, hairstyle by hairstyle and in retail order, whether the
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
struct CV_BordelloScript {
    CGameScriptInterface* Game;
    bool IsHeroWearingTash();
};
bool CV_BordelloScript::IsHeroWearingTash()
{
    bool wearing = Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMITH_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHTRADER_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHKHG_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSHERIFF_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHCHINESE_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMALL_01");
    return wearing;
}
static const char* const kNames[6] = { "OBJECT_HERO_TASHSMITH_01", "OBJECT_HERO_TASHTRADER_01", "OBJECT_HERO_TASHKHG_01", "OBJECT_HERO_TASHSHERIFF_01", "OBJECT_HERO_TASHCHINESE_01", "OBJECT_HERO_TASHSMALL_01" };
int main()
{
    CGameScriptInterface game;
    CV_BordelloScript script;
    script.Game = &game;
    for (int hit = -1; hit < 6; ++hit) {
        game.Count = 0; game.Hit = hit;
        bool r = script.IsHeroWearingTash();
        int expectCount = hit < 0 ? 6 : hit + 1;
        bool ok = r == (hit >= 0) && game.Count == expectCount && g_live == 0;
        for (int i = 0; ok && i < game.Count; ++i)
            ok = std::strcmp(game.Asked[i], kNames[i]) == 0;
        if (!ok) { std::printf("00e44cc0_TEST FAIL\n"); return 1; }
    }
    std::printf("00e44cc0_TEST PASS\n");
    return 0;
}
