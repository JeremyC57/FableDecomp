#include <cstdio>
struct CThing { bool dead; int kills; void Kill(bool) { ++kills; dead = true; } };
struct CIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
static bool IS_THING_ALIVE(const CIntelligentPointer* p) { return p->GetPItem() != 0 && !p->GetPItem()->dead; }
static int g_baseKills = 0;
struct CTCBase { virtual ~CTCBase() {} virtual void OnKill() { ++g_baseKills; } };
struct CTCTrapBase : CTCBase {
    CIntelligentPointer PObstruction;
    virtual void OnKill()
    {
        CTCBase::OnKill();
        if (IS_THING_ALIVE(&PObstruction))
            PObstruction.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing obs; obs.dead = false; obs.kills = 0;
    CTCTrapBase t; t.PObstruction.m_pItem = &obs;
    t.OnKill();
    if (g_baseKills != 1 || obs.kills != 1 || !obs.dead) { std::printf("00787d94_TEST FAIL\n"); return 1; }
    t.OnKill();                                  // already dead: not killed again
    if (g_baseKills != 2 || obs.kills != 1) { std::printf("00787d94_TEST FAIL\n"); return 1; }
    t.PObstruction.m_pItem = 0; t.OnKill();      // null pointer
    if (g_baseKills != 3) { std::printf("00787d94_TEST FAIL\n"); return 1; }
    std::printf("00787d94_TEST PASS\n");
    return 0;
}
