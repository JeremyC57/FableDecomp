#include <cstdio>
// CModeDragonGetHit::OnActivate model: base activation, PTarget = the main player's hero, a player creature suspended.
struct CThing { unsigned char type; int suspends; void SetSuspendAnimation(bool s, void*) { if (s) ++suspends; } };
static CThing g_hero = { 0, 0 };
struct CPlayer { CThing* GetHeroThing() { return &g_hero; } };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
struct CEngine { CPlayerManager* m_pPlayerManager; };
static CPlayerManager g_pm;
static CEngine g_engine = { &g_pm };
static CEngine* g_pEngine = &g_engine;
struct CIntelligentPointer {
    CThing* m_pItem;
    CIntelligentPointer() : m_pItem(0) { }
    CIntelligentPointer(CThing* item) : m_pItem(item) { }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; return *this; }
};
struct CModeDragonGetHit {
    int activations; CThing* m_pThing; CIntelligentPointer PTarget;
    void BaseOnActivate() { ++activations; }
    void OnActivate()
    {
        BaseOnActivate();
        PTarget = CIntelligentPointer(g_pEngine->m_pPlayerManager->GetMainPlayer()->GetHeroThing());
        if (m_pThing->type == 1)
            m_pThing->SetSuspendAnimation(true, this);
    }
};
int main()
{
    CThing dragon = { 1, 0 };
    CModeDragonGetHit m; m.activations = 0; m.m_pThing = &dragon;
    m.OnActivate();
    if (!(m.activations == 1 && m.PTarget.m_pItem == &g_hero && dragon.suspends == 1)) { std::printf("008d7510_TEST FAIL\n"); return 1; }
    std::printf("008d7510_TEST PASS\n");
    return 0;
}
