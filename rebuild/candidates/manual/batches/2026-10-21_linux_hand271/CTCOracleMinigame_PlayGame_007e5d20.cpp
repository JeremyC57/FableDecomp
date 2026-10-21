// CTCOracleMinigame::PlayGame() -- start the oracle minigame at level 1: take the output interval from the def, find the four symbol objects and the gamesmaster by track name, clear the firing symbols and put the main player into the minigame mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5D20, length 475 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 / ~CCharString @ 0x0099EAE0, the CTCBase accessors
// GetThingSearchTools @ 0x00686D90 and GetPlayerManager @ 0x00686D40 (propagated manifest labels), CThingSearchTools::
// GetPFirstThing<CThingFilter_HasTrackName, ...> @ 0x007E6EC0 (symbols) and the second instantiation @ 0x0056D050 (the
// gamesmaster; manifest names both "<CThingFilter_HasDefGlobalIndex,CThing>"), CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetCurrentMode @ 0x00633BE0 / AddMode(EPlayerMode, bool) @ 0x006345C0 (mode 0x29).
// Strings: "OBJECT_SS_ORACLE_SYMBOL_YELLOW_01" @ 0x0127078C, "..._RED_01" @ 0x0127076C, "..._GREEN_01" @ 0x01270748,
// "..._BLUE_01" @ 0x01270728, "OracleGamesmaster" @ 0x01270714.
// Shape: each track name is a NAMED CCharString in its own block (an unnamed temporary pushes the ctor's eax); the filter
// copies it as a temporary, so the two frame slots alternate between the searches.
// Layout (local model, as the landed Quit 0x007E62D0: engine/CTCOracleMinigame.h is the PDB with a 12-byte list; retail's
// STLport list is one sentinel pointer, so SpeechMarker is +0xe4 and PDef +0xe8): Level +0x10, Stage +0x14,
// TimeBetweenOutputs +0x20, Symbols +0xb0, SymbolsFiring +0xc0; the def's output interval +0x2c (descriptive).
struct CThing;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CThingFilter_HasTrackName {
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
struct CThingSearchTools {
    bool GetPFirstThing(const CThingFilter_HasTrackName& filter, CThing** out);         // 0x007E6EC0
    bool GetPFirstSpeechThing(const CThingFilter_HasTrackName& filter, CThing** out);   // 0x0056D050 (second instantiation)
};
struct CPlayer {
    long GetCurrentMode();                              // 0x00633BE0
    void AddMode(long mode, bool exclusive);            // 0x006345C0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct COracleMinigameDef {
    unsigned char _pad00[0x2c];
    float TimeBetweenOutputs;                           // +0x2c (descriptive)
};
struct CTCOracleMinigame {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _base08[4];
    unsigned long CurrentText;                          // +0x0c
    long Level;                                         // +0x10
    long Stage;                                         // +0x14 EOracleMinigameState
    long PostWaitStage;                                 // +0x18
    long WaitTime;                                      // +0x1c
    float TimeBetweenOutputs;                           // +0x20
    long NumOutputsFired;                               // +0x24
    long Output[32];                                    // +0x28
    bool Success;                                       // +0xa8
    long NextOutputFrame;                               // +0xac
    CThing* Symbols[4];                                 // +0xb0
    long SymbolsFiring[4];                              // +0xc0
    long FramesFired[4];                                // +0xd0
    void* InputsPending;                                // +0xe0 (STLport list: one sentinel pointer)
    CThing* SpeechMarker;                               // +0xe4
    const COracleMinigameDef* PDef;                     // +0xe8
    CPlayerManager*    GetPlayerManager();              // 0x00686D40 (CTCBase)
    CThingSearchTools* GetThingSearchTools() const;     // 0x00686D90 (CTCBase)
    void PlayGame();
};

void CTCOracleMinigame::PlayGame()
{
    Level = 1;
    Stage = 1;
    TimeBetweenOutputs = PDef->TimeBetweenOutputs;
    {
        CCharString name("OBJECT_SS_ORACLE_SYMBOL_YELLOW_01");
        GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTrackName(name), &Symbols[0]);
    }
    {
        CCharString name("OBJECT_SS_ORACLE_SYMBOL_RED_01");
        GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTrackName(name), &Symbols[1]);
    }
    {
        CCharString name("OBJECT_SS_ORACLE_SYMBOL_GREEN_01");
        GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTrackName(name), &Symbols[2]);
    }
    {
        CCharString name("OBJECT_SS_ORACLE_SYMBOL_BLUE_01");
        GetThingSearchTools()->GetPFirstThing(CThingFilter_HasTrackName(name), &Symbols[3]);
    }
    {
        CCharString name("OracleGamesmaster");
        GetThingSearchTools()->GetPFirstSpeechThing(CThingFilter_HasTrackName(name), &SpeechMarker);
    }
    SymbolsFiring[0] = 0;
    SymbolsFiring[1] = 0;
    SymbolsFiring[2] = 0;
    SymbolsFiring[3] = 0;
    CPlayer* player = GetPlayerManager()->GetMainPlayer();
    if (player->GetCurrentMode() != 0x29)
        player->AddMode(0x29, false);
    CurrentText = 0;
}
