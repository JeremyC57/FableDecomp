// CPlayerManager::GetPlayerDefIndexFromNumber(long) -- the global def index of the player def for a player number: PLAYER_NEUTRAL for 4, PLAYER_HERO for the main player, PLAYER_SPIRIT otherwise
// Grade: DIFFER(101v91) -- tail merging: retail re-addresses the string temporary with eax/edx/eax on the three paths, so the NEUTRAL and SPIRIT constructor blocks cross-jump into one; the lane picks eax/ecx/edx and merges none
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
        return DefinitionManager->GetDefGlobalIndexFromName("PLAYER_NEUTRAL");
    if (number == MainPlayer)
        return DefinitionManager->GetDefGlobalIndexFromName("PLAYER_HERO");
    return DefinitionManager->GetDefGlobalIndexFromName("PLAYER_SPIRIT");
}
