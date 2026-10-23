// Behaviour test for 008955c0: body copied from src/008955c0.cpp (CGameScriptInterface::IsEntityFollowingHero), the thing / player / component callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CThing_Retail;
class CCharString { public: void* PStringData; };
struct CTCBase_Retail;
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CBase { virtual ~CBase() {} };
struct CScriptThing : CBase {
    virtual CThing_Retail* GetThing() const { return m_thing; }
    virtual bool IsValid() const { return m_thing != 0; }
    CThing_Retail* m_thing;
    void* m_pItem;
    CScriptThing() : m_thing(0), m_pItem(0) { }
};
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];
    CInterfaceMap_Retail  m_interfaces;
    unsigned char         _pad4c[0x45];
    unsigned char         m_dead;
    unsigned char         _pad92[0x40];
    int                   m_id;
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    CScriptThing GetScriptThing() { CScriptThing s; s.m_thing = this; return s; }
};

static int g_isAI = 0; static CThing_Retail* g_isAIArg = 0; static void* g_isAIThis = 0; static bool g_isAIResult = false;
struct CTCFollowed_Retail { bool IsAIFollower(CThing_Retail* thing) { ++g_isAI; g_isAIArg = thing; g_isAIThis = this; return g_isAIResult; } };
static CThing_Retail* g_hero = 0;
struct CPlayer_Retail { CThing_Retail* GetHeroThing() { return g_hero; } };
struct CPlayerManager_Retail { CPlayer_Retail m_player; CPlayer_Retail* GetPlayer() { return &m_player; } };
struct CGameScriptInterface {
    virtual void v000() {}
    void* World; void* Component; void* DisplayEngine; void* DefinitionManager;
    CPlayerManager_Retail*     PlayerManager;
    bool IsEntityFollowingHero(const CScriptThing& thing) const;
};
bool CGameScriptInterface::IsEntityFollowingHero(const CScriptThing& thing) const
{
    if (thing.IsValid()) {
        CThing_Retail* follower = thing.GetThing();
        CThing_Retail* hero = PlayerManager->GetPlayer()->GetHeroThing();
        if (hero && !hero->m_dead) {
            if (hero->HasInterface(0xbc)) {
                CTCFollowed_Retail* followed = hero->GetTC<CTCFollowed_Retail>(0xbc);
                if (followed)
                    return followed->IsAIFollower(follower);
            }
        }
    }
    return false;
}
int main()
{
    CPlayerManager_Retail pm; CGameScriptInterface gsi; gsi.PlayerManager = &pm;
    CThing_Retail hero; memset(&hero, 0, sizeof(hero)); CThing_Retail follower; memset(&follower, 0, sizeof(follower));
    CTCFollowed_Retail followed; CTCBase_Retail* other = (CTCBase_Retail*)0x99;
    CInterfaceMap_Retail::SEntry entries[3] = { { 0x10, other }, { 0xbc, (CTCBase_Retail*)&followed }, { 0xc0, other } };
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 3;
    hero.m_presentBits[0xbc >> 5] = 1UL << (0xbc & 31);
    CScriptThing st; st.m_thing = &follower;
    // 1. everything present: the component answers
    g_hero = &hero; g_isAIResult = true;
    CHECK(gsi.IsEntityFollowingHero(st) == true && g_isAI == 1 && g_isAIArg == &follower && g_isAIThis == &followed);
    g_isAIResult = false; CHECK(gsi.IsEntityFollowingHero(st) == false && g_isAI == 2);
    // 2. invalid script thing: nothing asked
    CScriptThing bad; CHECK(gsi.IsEntityFollowingHero(bad) == false && g_isAI == 2);
    // 3. no hero / dead hero
    g_hero = 0; CHECK(gsi.IsEntityFollowingHero(st) == false && g_isAI == 2);
    g_hero = &hero; hero.m_dead = 1; CHECK(gsi.IsEntityFollowingHero(st) == false && g_isAI == 2); hero.m_dead = 0;
    // 4. interface bit clear
    hero.m_presentBits[0xbc >> 5] = 0; CHECK(gsi.IsEntityFollowingHero(st) == false && g_isAI == 2); hero.m_presentBits[0xbc >> 5] = 1UL << (0xbc & 31);
    // 5. bit set but the map has no 0xbc entry: the end record's component (null) is taken
    CInterfaceMap_Retail::SEntry entries2[3] = { { 0x10, other }, { 0xc0, other }, { 0, 0 } };
    hero.m_interfaces.first = entries2; hero.m_interfaces.last = entries2 + 2;
    CHECK(gsi.IsEntityFollowingHero(st) == false && g_isAI == 2);
    if (fails == 0) printf("008955c0_TEST PASS\n"); else printf("008955c0_TEST FAIL\n");
    return fails;
}