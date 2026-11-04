// Behaviour test for the quest-script constructor at 0x00E2AB60 (CV_AmbushScamScript).
#include <cstdio>

class CGameScriptInterface { public:
    long m_next;
    virtual long RegisterTimer() { return ++m_next; }
};
CGameScriptInterface* pDSTGame = 0;

class CCharString { public:
    const char* m_pText;
    CCharString() : m_pText("") { }
};

class CTimer { public:
    long Index;
    CTimer() : Index(pDSTGame->RegisterTimer()) { }
};

static int g_baseBuilt = 0;
class CScriptBase { public:
    virtual void v00() { }
    unsigned char _body[0x3c];
    CScriptBase() { ++g_baseBuilt; }
};

struct CScriptObject : CScriptBase {
    CGameScriptInterface* m_pGame;
    void* m_pOwner;
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }
};

struct CV_AmbushScamScript : CScriptObject {
    unsigned char _pad48[0x34];
    CTimer m_timer;
    CCharString m_texts[5];
    virtual void v00() { }
    CV_AmbushScamScript(void* owner, CGameScriptInterface* game);
};

CV_AmbushScamScript::CV_AmbushScamScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}

int main()
{
    CGameScriptInterface gsi; gsi.m_next = 40;
    pDSTGame = &gsi;
    int owner = 0;
    CV_AmbushScamScript s(&owner, &gsi);
    if (g_baseBuilt != 1 || s.m_pGame != &gsi || s.m_pOwner != &owner || s.m_timer.Index != 41 || s.m_texts[0].m_pText[0] != 0 || s.m_texts[4].m_pText[0] != 0) {
        std::printf("00e2ab60_TEST FAIL\n");
        return 1;
    }
    std::printf("00e2ab60_TEST PASS\n");
    return 0;
}