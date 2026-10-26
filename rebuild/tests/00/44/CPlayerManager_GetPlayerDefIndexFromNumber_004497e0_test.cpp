// Behaviour test for 004497e0: src/004497e0.cpp copied verbatim; the definition lookup and CCharString stubbed so the
// neutral (4), hero (main player) and spirit (anything else) names are checked, with each temporary destroyed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayerManager::GetPlayerDefIndexFromNumber(long) -- the global def index of the player def for a player number: PLAYER_NEUTRAL for 4, PLAYER_HERO for the main player, PLAYER_SPIRIT otherwise
// Retry 10b (accessor sweep, FIXES B-4/B-5): the definition manager is read through an inline accessor; the plain member read gives the parked register difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004497E0, length 91 bytes.
// Callees: CCharString::CCharString(const char*, long = -1) @ 0x0099EBF0, CGameDefinitionManager::
// GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410 (manifest: CDefinitionManager), CCharString::
// ~CCharString @ 0x0099EAE0 (landed). Strings 0x01231C6C "PLAYER_NEUTRAL", 0x01231C60 "PLAYER_HERO",
// 0x01231C50 "PLAYER_SPIRIT". Caller CPlayerManager_AddPlayerOrAI 0x0044A1A0.
// Layout: retail std::vector is 12 bytes, so MainPlayer is +0x1c (Ego_r.pdb +0x20); modelled locally.
// Shape: the CCharString temporary lives in the dead argument slot ([esp+10h] after three pushes) and is
// re-addressed with `lea` after its constructor (an implicit conversion, GOTCHAS waves 11/15); one
// GetDefGlobalIndexFromName call and one destructor after the paths join; `push -1` hoisted above the first test.
// Tried: explicit CCharString("...") temporaries (73 bytes: all three paths merge and the constructor's eax is
// pushed), implicit conversions in three returns (this file, 101), an if/else chain into a `long` local (101),
// `if (number != 4 && number == MainPlayer)` plus a literal ternary (94: the literal goes through eax).
class CCharStringData;
class CCharString { public:
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};

class CMainGameComponent;
class CPlayer;

class CPlayerManager { public:
    void*                   __vftable;                      // +0x00
    CMainGameComponent*     Component;                      // +0x04
    CGameDefinitionManager* DefinitionManager;              // +0x08
    CGameDefinitionManager* GetDefinitionManager() const { return DefinitionManager; }
    CPlayer**               Players_First;                  // +0x0c retail std::vector<CPlayer*> (12 bytes)
    CPlayer**               Players_Last;                   // +0x10
    CPlayer**               Players_End;                    // +0x14
    long                    PlayerNeutral;                  // +0x18
    long                    MainPlayer;                     // +0x1c
    long GetPlayerDefIndexFromNumber(long number);
};

long CPlayerManager::GetPlayerDefIndexFromNumber(long number)
{
    if (number == 4)
        return GetDefinitionManager()->GetDefGlobalIndexFromName("PLAYER_NEUTRAL");
    if (number == MainPlayer)
        return GetDefinitionManager()->GetDefGlobalIndexFromName("PLAYER_HERO");
    return GetDefinitionManager()->GetDefGlobalIndexFromName("PLAYER_SPIRIT");
}
// ---- end source ----
static int g_ctors = 0, g_dtors = 0;
CCharString::CCharString(const char* text, long) : m_pStringData((CCharStringData*)text) { ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
long CGameDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name)
{
    const char* s = (const char*)name.m_pStringData;
    if (strcmp(s, "PLAYER_NEUTRAL") == 0) return 10;
    if (strcmp(s, "PLAYER_HERO") == 0) return 20;
    if (strcmp(s, "PLAYER_SPIRIT") == 0) return 30;
    return -1;
}
int main()
{
    static CGameDefinitionManager dm;
    CPlayerManager pm; memset(&pm, 0, sizeof(pm)); pm.DefinitionManager = &dm; pm.MainPlayer = 1;
    CHECK(pm.GetPlayerDefIndexFromNumber(4) == 10);
    CHECK(pm.GetPlayerDefIndexFromNumber(1) == 20);
    CHECK(pm.GetPlayerDefIndexFromNumber(2) == 30);
    pm.MainPlayer = 4;
    CHECK(pm.GetPlayerDefIndexFromNumber(4) == 10);
    CHECK(g_ctors == 4 && g_dtors == 4);
    if (fails == 0) printf("004497e0_TEST PASS\n"); else printf("004497e0_TEST FAIL\n");
    return fails;
}