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
struct CCharStringData;
struct CCharString {
    CCharStringData* m_pStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);    // 0x009AD410
};

struct CMainGameComponent;
struct CPlayer;

struct CPlayerManager {
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