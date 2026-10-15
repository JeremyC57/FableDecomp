// Behaviour test for the quest-script constructor at 0x00D7C700 (CVtable_012D3994).
#include <cstdio>

struct CGameScriptInterface {
    long m_next;
    virtual long RegisterTimer() { return ++m_next; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

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

struct CVtable_012D3994 : CScriptObject {
    unsigned char _pad48[0x10];
    CTimer m_timer;
    long m_value5c, m_value60, m_value64;
    virtual void v00() { }
    CVtable_012D3994(void* owner, CGameScriptInterface* game);
};

CVtable_012D3994::CVtable_012D3994(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game), m_value5c(0), m_value60(0), m_value64(0)
{
}

int main()
{
    CGameScriptInterface gsi; gsi.m_next = 40;
    g_pGameScriptInterface = &gsi;
    int owner = 0;
    CVtable_012D3994 s(&owner, &gsi);
    if (g_baseBuilt != 1 || s.m_pGame != &gsi || s.m_pOwner != &owner || s.m_timer.Index != 41
        || s.m_value5c != 0 || s.m_value60 != 0 || s.m_value64 != 0) {
        std::printf("00d7c700_TEST FAIL\n");
        return 1;
    }
    std::printf("00d7c700_TEST PASS\n");
    return 0;
}
