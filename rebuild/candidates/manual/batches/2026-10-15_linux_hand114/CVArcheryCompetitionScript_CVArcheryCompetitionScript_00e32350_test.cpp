// Behaviour test for the quest-script constructor at 0x00E32350 (CV_ArcheryCompetitionScript).
#include <cstdio>

struct CGameScriptInterface {
    long m_next;
    virtual long RegisterTimer() { return ++m_next; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

struct CCharString {
    const char* m_pText;
    CCharString() : m_pText("") { }
};

struct CTimer {
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};

static int g_baseBuilt = 0;
struct CScriptBase {
    virtual void v00() { }
    unsigned char _body[0x3c];
    CScriptBase() { ++g_baseBuilt; }
};

struct CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;
    void* m_pOwner;
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CV_ArcheryCompetitionScript : CScriptObject {
    unsigned char _pad48[0x24];
    CTimer GameTimeLeft;
    CTimer RandomSpeechTimer;
    CCharString RandomSpeech[10];
    virtual void v00() { }
    CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game);
};

CV_ArcheryCompetitionScript::CV_ArcheryCompetitionScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}

int main()
{
    CGameScriptInterface gsi; gsi.m_next = 40;
    g_pGameScriptInterface = &gsi;
    int owner = 0;
    CV_ArcheryCompetitionScript s(&owner, &gsi);
    if (g_baseBuilt != 1 || s.m_pGame != &gsi || s.m_pOwner != &owner || s.GameTimeLeft.Index != 41 || s.RandomSpeechTimer.Index != 42 || s.RandomSpeech[0].m_pText[0] != 0 || s.RandomSpeech[9].m_pText[0] != 0) {
        std::printf("00e32350_TEST FAIL\n");
        return 1;
    }
    std::printf("00e32350_TEST PASS\n");
    return 0;
}
