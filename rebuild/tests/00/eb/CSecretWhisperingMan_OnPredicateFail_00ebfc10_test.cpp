// Behaviour test for CSecretWhisperingMan::OnPredicateFail (0x00EBFC10): only a kill report moves the quest to state 5, shows the digging spot and sets the Mayor's Invitation objective.
#include <cstdio>
#include <cstring>
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#define SCRIPT_SLOT(n) virtual void slot##n() {}
#define SCRIPT_SLOTS10(d) SCRIPT_SLOT(d##0); SCRIPT_SLOT(d##1); SCRIPT_SLOT(d##2); SCRIPT_SLOT(d##3); SCRIPT_SLOT(d##4); SCRIPT_SLOT(d##5); SCRIPT_SLOT(d##6); SCRIPT_SLOT(d##7); SCRIPT_SLOT(d##8); SCRIPT_SLOT(d##9)
struct CGameScriptInterface {                          // vtable 0x01260F0C (only the slot used is typed)
    SCRIPT_SLOTS10(0); SCRIPT_SLOTS10(1); SCRIPT_SLOTS10(2); SCRIPT_SLOTS10(3); SCRIPT_SLOTS10(4);
    SCRIPT_SLOTS10(5); SCRIPT_SLOTS10(6); SCRIPT_SLOTS10(7); SCRIPT_SLOTS10(8); SCRIPT_SLOTS10(9);
    SCRIPT_SLOTS10(10); SCRIPT_SLOTS10(11); SCRIPT_SLOTS10(12); SCRIPT_SLOTS10(13); SCRIPT_SLOTS10(14);
    SCRIPT_SLOTS10(15); SCRIPT_SLOTS10(16); SCRIPT_SLOTS10(17); SCRIPT_SLOTS10(18); SCRIPT_SLOTS10(19);
    SCRIPT_SLOTS10(20); SCRIPT_SLOTS10(21); SCRIPT_SLOTS10(22); SCRIPT_SLOTS10(23); SCRIPT_SLOTS10(24);
    SCRIPT_SLOTS10(25); SCRIPT_SLOTS10(26); SCRIPT_SLOTS10(27); SCRIPT_SLOTS10(28);
    SCRIPT_SLOT(290); SCRIPT_SLOT(291); SCRIPT_SLOT(292); SCRIPT_SLOT(293); SCRIPT_SLOT(294); SCRIPT_SLOT(295);
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region, const CCharString& regionAlt) const;   // slot 296 (+0x4a0)
};
struct CScriptThing {                                  // vtable 0x01238C8C (only the slot used is typed)
    SCRIPT_SLOTS10(0); SCRIPT_SLOT(10); SCRIPT_SLOT(11); SCRIPT_SLOT(12); SCRIPT_SLOT(13); SCRIPT_SLOT(14);
    SCRIPT_SLOT(15); SCRIPT_SLOT(16); SCRIPT_SLOT(17);
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48, 0x004AAAF0)
    long m_data[2];
};
struct CV_ParanoidWhispersScript {
    unsigned char _pad[0x48];
    long          ParanoidState;                       // +0x48 (PDB +0x5c)
    bool          HideDiggingSpot;                     // +0x4c (PDB +0x60)
};
struct CQ_SunnyvaleMasterData;
struct C3DVector { float X, Y, Z; };
class CSecretWhisperingMan {
public:
    void*                      __vftable;              // +0x00
    CGameScriptInterface*      Game;                   // +0x04
    CScriptThing               Me;                     // +0x08
    CV_ParanoidWhispersScript* ParentClass;            // +0x14
    CQ_SunnyvaleMasterData*    PMasterData;            // +0x18
    C3DVector                  StartPos;               // +0x1c
    void OnPredicateFail();
};
void CSecretWhisperingMan::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        ParentClass->ParanoidState = 5;
        ParentClass->HideDiggingSpot = false;
        Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05", "OakBay", "");
    }
}
static int g_live = 0; static char g_args[4][64]; static int g_calls = 0; static char g_killer[64]; static bool g_killed;
CCharString::CCharString(const char* text, long) { ++g_live; PStringData = (void*)text; }
CCharString::~CCharString() { --g_live; }
void CGameScriptInterface::SetQuestCardObjective(const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d) const
{
    ++g_calls; std::strcpy(g_args[0], (const char*)a.PStringData); std::strcpy(g_args[1], (const char*)b.PStringData);
    std::strcpy(g_args[2], (const char*)c.PStringData); std::strcpy(g_args[3], (const char*)d.PStringData);
}
bool CScriptThing::MsgIsKilledBy(const CCharString& k) const { std::strcpy(g_killer, (const char*)k.PStringData); return g_killed; }
int main()
{
    CGameScriptInterface game; CV_ParanoidWhispersScript quest;
    CSecretWhisperingMan man;
    quest.ParanoidState = 2; quest.HideDiggingSpot = true;
    man.Game = &game; man.ParentClass = &quest;
    bool ok = true;
    for (int pass = 0; pass < 2; ++pass) {
        g_killed = pass == 1;
        g_killer[0] = 'x'; g_killer[1] = 0;
        man.OnPredicateFail();
        ok = ok && g_live == 0 && g_killer[0] == 0 && g_calls == pass && quest.ParanoidState == (pass ? 5 : 2) && quest.HideDiggingSpot == !pass;
    }
    ok = ok && !std::strcmp(g_args[0], "V_MayorsInvitation") && !std::strcmp(g_args[1], "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_05")
        && !std::strcmp(g_args[2], "OakBay") && !std::strcmp(g_args[3], "");
    if (!ok) { std::printf("00ebfc10_TEST FAIL\n"); return 1; }
    std::printf("00ebfc10_TEST PASS\n");
    return 0;
}