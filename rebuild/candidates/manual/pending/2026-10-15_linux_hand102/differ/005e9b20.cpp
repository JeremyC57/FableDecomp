// CTCTavernGame::SetState(ETavernGameStates state) -- a pending quit request leaves the game instead; state 5 also marks the game played; state 47 leaves the game; otherwise the state changes (previous = current)
// Grade: DIFFER(74v74) -- one register: the state-47 arm's vtable fetch is `mov eax,[ecx] ; call [eax+78h]` in retail, `mov edx,[ecx] ; call [edx+78h]` in the lane (eax still holds the dead state). Tried: if/else-if with return, switch (reorders the arms), three-arm form with an inline ChangeState (duplicates the stores), two sequential ifs (same edx).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9B20, length 74 bytes (ret 4). Name and signature: Ego_r.pdb
// `?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z`.
// Callees: the virtual LeaveGame() (vtable 0x012560BC slot 30, +0x78 = 0x005EC650), twice.
// Layout: engine/CTCTavernGame.h (Ego_r.pdb) has TavernGameState +0xdc, PreviousTavernGameState +0xe0, GamePlayed +0xf4;
// the six PDB vectors (UIComponents 0x14, five handle vectors 0x10) are 12 bytes each in retail, so every member after
// them sits 0x1c lower: TavernGameState +0xc0, PreviousTavernGameState +0xc4, NextTavernGameState +0xc8, StateTimer +0xcc,
// GamePlayed +0xd8 (the landed NewGame 0x005E9A70 confirms the unshifted members below +0x4c). The header disagrees with
// the bytes, so the class is a local model with the PDB member names at the retail offsets. The quit-request flag is
// the global byte @ 0x013BAE1C.
// Shape: `if (state == 5) GamePlayed = true; else if (state == 47) { LeaveGame(); return; }` before the shared state
// change -- retail tail-duplicates the state change into the state-5 arm and the LeaveGame call into the 47 arm.
typedef long ETavernGameStates;

extern bool g_bTavernGameQuitRequested;      // 0x013BAE1C

struct CTCTavernGame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void LeaveGame();                // slot 30 (+0x78)
    unsigned char     _pad_0x04[0xbc];
    ETavernGameStates TavernGameState;           // +0xc0
    ETavernGameStates PreviousTavernGameState;   // +0xc4
    ETavernGameStates NextTavernGameState;       // +0xc8
    float             StateTimer;                // +0xcc
    unsigned char     _pad_0xd0[8];              // PGameMaster
    bool              GamePlayed;                // +0xd8
    void ChangeState(ETavernGameStates state)
    {
        PreviousTavernGameState = TavernGameState;
        TavernGameState = state;
    }
    void SetState(ETavernGameStates state);
};

void CTCTavernGame::SetState(ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
    if (state == 5)
        GamePlayed = true;
    else if (state == 47) {
        LeaveGame();
        return;
    }
    ChangeState(state);
}
