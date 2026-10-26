// Behaviour test for the quest-script constructor at 0x00E2AB60 (CVtable_012E2428).
#include <cstdio>

class CGameScriptInterface { public:
    long m_next;
    virtual long RegisterTimer() { return ++m_next; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

class CCharString { public:
    const char* m_pText;
    CCharString() : m_pText("") { }
};

class CTimer { public:
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
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

struct CVtable_012E2428 : CScriptObject {
    unsigned char _pad48[0x34];
    CTimer m_timer;
    CCharString m_texts[5];
    virtual void v00() { }
    CVtable_012E2428(void* owner, CGameScriptInterface* game);
};

CVtable_012E2428::CVtable_012E2428(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)
{
}

int main()
{
    CGameScriptInterface gsi; gsi.m_next = 40;
    g_pGameScriptInterface = &gsi;
    int owner = 0;
    CVtable_012E2428 s(&owner, &gsi);
    if (g_baseBuilt != 1 || s.m_pGame != &gsi || s.m_pOwner != &owner || s.m_timer.Index != 41 || s.m_texts[0].m_pText[0] != 0 || s.m_texts[4].m_pText[0] != 0) {
        std::printf("00e2ab60_TEST FAIL\n");
        return 1;
    }
    std::printf("00e2ab60_TEST PASS\n");
    return 0;
}