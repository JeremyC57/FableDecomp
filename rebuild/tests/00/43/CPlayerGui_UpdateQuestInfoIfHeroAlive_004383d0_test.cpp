// test for 004383d0 UpdateQuestInfoIfHeroAlive
#include <stdio.h>
#include <string.h>
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* PInfo;
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                              // +0x91 bit 0
};
class CPlayer {
public:
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PlayerCharacter;             // +0x34 CIntelligentPointer<CThing>
    CThing* GetPlayerCharacter() { return (CThing*)PlayerCharacter.GetPItem(); }
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                            // 0x00449970
};
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    void Update();                                       // 0x0064A32E
};
}
class CPlayerGui {
public:
    unsigned char               _pad000[8];
    CPlayerManager*             PlayerManager;           // +0x08
    unsigned char               _pad00c[0x1c8 - 0xc];
    NPlayerGui::CDrawQuestInfo* PDrawQuestInfo;          // +0x1c8 CCountedPointer data
    void UpdateQuestInfoIfHeroAlive();
};
void CPlayerGui::UpdateQuestInfoIfHeroAlive()
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetPlayerCharacter();
    if (hero && !hero->Dead)
        PDrawQuestInfo->Update();
}

static CThing g_hero; static CThing* g_item; static CPlayer g_player; static int g_upd; static void* g_updthis;
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return (CBaseObject*)g_item; }
CPlayer* CPlayerManager::GetMainPlayer() { return &g_player; }
void NPlayerGui::CDrawQuestInfo::Update() { ++g_upd; g_updthis = this; }
int main()
{
    static CPlayerGui g; CPlayerManager pm; NPlayerGui::CDrawQuestInfo qi;
    g.PlayerManager = &pm; g.PDrawQuestInfo = &qi;
    memset(&g_hero, 0, sizeof g_hero);
    g_item = &g_hero; g.UpdateQuestInfoIfHeroAlive();
    int ok = g_upd == 1 && g_updthis == &qi;
    g_hero.Dead = 1; g.UpdateQuestInfoIfHeroAlive();
    ok = ok && g_upd == 1;
    g_item = 0; g.UpdateQuestInfoIfHeroAlive();
    ok = ok && g_upd == 1;
    printf(ok ? "004383d0_TEST PASS\n" : "004383d0_TEST FAIL\n");
    return 0;
}