// Behaviour test for 0x00435000 CGameScriptInterface::GiveHeroGold.
#include <cstdio>
struct CTCHeroMoney { int gold; void GiveGold(int a) { gold += a; } };
struct CThing {
    unsigned int m_presentBits[9]; CTCHeroMoney* money; unsigned char m_statusFlags;
    template<class T> bool GetInterface(T** ppOut, int type)
    {
        if (m_presentBits[type >> 5] & (1u << (type & 31))) { *ppOut = (T*)money; return true; }
        return false;
    }
};
struct CPlayer { CThing* c; CThing* GetControlledCreature() { return c; } };
struct CPlayerManager { CPlayer* p; CPlayer* GetMainPlayer() { return p; } };
struct CGameScriptInterface {
    CPlayerManager* PlayerManager;
    void GiveHeroGold(int amount)
    {
        CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
        if (hero && !(hero->m_statusFlags & 1)) { CTCHeroMoney* money; if (hero->GetInterface(&money, 0x8e)) money->GiveGold(amount); }
    }
};
static int Fail() { std::printf("00435000_TEST FAIL\n"); return 1; }
int main()
{
    CTCHeroMoney m; m.gold = 10;
    CThing hero; for (int i = 0; i < 9; ++i) hero.m_presentBits[i] = 0; hero.money = &m; hero.m_statusFlags = 0;
    CPlayer p; p.c = &hero; CPlayerManager pm; pm.p = &p;
    CGameScriptInterface s; s.PlayerManager = &pm;
    s.GiveHeroGold(5);
    if (m.gold != 10) return Fail();
    hero.m_presentBits[4] = 0x4000;
    s.GiveHeroGold(5);
    if (m.gold != 15) return Fail();
    hero.m_statusFlags = 1; s.GiveHeroGold(5);
    if (m.gold != 15) return Fail();
    p.c = 0; s.GiveHeroGold(5);
    if (m.gold != 15) return Fail();
    std::printf("00435000_TEST PASS\n");
    return 0;
}
