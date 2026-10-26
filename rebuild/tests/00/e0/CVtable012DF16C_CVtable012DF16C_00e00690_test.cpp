// Behaviour test for the quest-script constructor at 0x00E00690 (CVtable_012DF16C).
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

struct CVtable_012DF16C : CScriptObject {
    CTimer m_timer;
    unsigned char _pad4c[0x2c];
    CCharString m_text;
    virtual void v00() { }
    CVtable_012DF16C(void* owner, CGameScriptInterface* game);
};

CVtable_012DF16C::CVtable_012DF16C(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game), m_text()
{
}

int main()
{
    CGameScriptInterface gsi; gsi.m_next = 40;
    g_pGameScriptInterface = &gsi;
    int owner = 0;
    CVtable_012DF16C s(&owner, &gsi);
    if (g_baseBuilt != 1 || s.m_pGame != &gsi || s.m_pOwner != &owner || s.m_timer.Index != 41 || s.m_text.m_pText == 0 || s.m_text.m_pText[0] != 0) {
        std::printf("00e00690_TEST FAIL\n");
        return 1;
    }
    std::printf("00e00690_TEST PASS\n");
    return 0;
}