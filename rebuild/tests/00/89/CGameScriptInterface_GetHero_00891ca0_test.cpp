// Behaviour test for 0x00891CA0 CGameScriptInterface::GetHero: the cached script thing follows the hero thing, is reset
// to null without one, and the counted info blocks are released (deleter run once the last reference goes).
#include <cstdio>
static int g_deleted = 0, g_freed = 0;
class CCPPointerInfo { public: unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void*) { ++g_deleted; }
class CBaseClass { public: virtual ~CBaseClass() {} };
struct CCountedScriptImp {
    void* Data; CCPPointerInfo* Info;
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; ++g_freed; }
        }
        Data = 0; Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:
    CCountedScriptImp PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing(void* data) { PImp.Data = data; PImp.Info = new CCPPointerInfo; PImp.Info->RefCount = 1; PImp.Info->DeleteFunc = DeleteData; PImp.Info->Data = data; }
    CScriptThing(const CScriptThing& o) : CBaseClass() { PImp.Data = o.PImp.Data; PImp.Info = o.PImp.Info; if (PImp.Info) ++PImp.Info->RefCount; }
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& o)
    {
        if (o.PImp.Info) ++o.PImp.Info->RefCount;
        CCountedScriptImp old; old.Data = PImp.Data; old.Info = PImp.Info;
        PImp.Data = o.PImp.Data; PImp.Info = o.PImp.Info;
        return *this;
    }
};
class CThing { public: int id; CScriptThing GetScriptThing() { return CScriptThing(this); } };
class CPlayer { public: CThing* hero; CThing* GetHeroThing() { return hero; } };
class CPlayerManager { public: CPlayer* player; CPlayer* GetMainPlayer() { return player; } };
class CGameScriptInterface { public:
    CPlayerManager* PlayerManager;
    CScriptThing Hero;
    CScriptThing& GetHero();
};
CScriptThing& CGameScriptInterface::GetHero()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (hero)
        Hero = hero->GetScriptThing();
    else
        Hero = CScriptThing();
    return Hero;
}
int main()
{
    CThing h = { 1 }; CPlayer player = { &h }; CPlayerManager pm = { &player };
    CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    int fail = 0;
    CScriptThing& r = gsi.GetHero();
    if (&r != &gsi.Hero || gsi.Hero.PImp.Data != &h || !gsi.Hero.PImp.Info || gsi.Hero.PImp.Info->RefCount != 1) fail |= 1;
    gsi.GetHero();                                   // refresh: the old block is released and deleted
    if (gsi.Hero.PImp.Data != &h || gsi.Hero.PImp.Info->RefCount != 1 || g_freed != 1 || g_deleted != 1) fail |= 2;
    player.hero = 0;
    gsi.GetHero();
    if (gsi.Hero.PImp.Data != 0 || gsi.Hero.PImp.Info != 0 || g_freed != 2) fail |= 4;
    if (fail) { std::printf("00891ca0_TEST FAIL\n"); return 1; }
    std::printf("00891ca0_TEST PASS\n");
    return 0;
}