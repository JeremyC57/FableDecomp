// Behaviour test for 0x00E33660 CAC_Owner::OnPredicateFail
#include <cstdio>
#include <cstring>
static int g_strings = 0;
class CCharString { public:
    char t[32];
    CCharString(const char* s) { std::strcpy(t, s); ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CScriptThing { public: bool accept; virtual bool Accepts(const CCharString& n) const { return accept && n.t[0] == 0; } virtual ~CScriptThing() {} };
class CGameScriptInterface { public:
    bool loaded; int queries;
    virtual bool IsRegionLoaded(const CCharString& r) const { const_cast<CGameScriptInterface*>(this)->queries++; return loaded && std::strcmp(r.t, "KnotholeGlade") == 0; }
    virtual ~CGameScriptInterface() {}
};
struct SOwnerRecord { bool m_flag4c; };
struct CAC_Owner {
    CGameScriptInterface* m_pGame; CScriptThing m_thing; SOwnerRecord* m_pOwnerRecord;
    void OnPredicateFail() { if (m_pGame->IsRegionLoaded("KnotholeGlade") && m_thing.Accepts("")) m_pOwnerRecord->m_flag4c = true; }
};
int main()
{
    CGameScriptInterface game; game.loaded = false; game.queries = 0;
    SOwnerRecord rec; rec.m_flag4c = false;
    CAC_Owner o; o.m_pGame = &game; o.m_thing.accept = true; o.m_pOwnerRecord = &rec;
    o.OnPredicateFail();
    bool ok = !rec.m_flag4c && game.queries == 1 && g_strings == 0;
    game.loaded = true; o.m_thing.accept = false; o.OnPredicateFail();
    ok = ok && !rec.m_flag4c && g_strings == 0;
    o.m_thing.accept = true; o.OnPredicateFail();
    ok = ok && rec.m_flag4c && g_strings == 0 && game.queries == 3;
    if (!ok) { std::printf("00e33660_TEST FAIL\n"); return 1; }
    std::printf("00e33660_TEST PASS\n");
    return 0;
}