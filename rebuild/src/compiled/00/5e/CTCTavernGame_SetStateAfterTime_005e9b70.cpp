// CTCTavernGame::SetStateAfterTime(ETavernGameStates state, float time) -- a pending quit request leaves the game instead; otherwise enters the timed wait state 14 that moves on to `state` after `time`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9B70, length 68 bytes (ret 8). Name and signature: Ego_r.pdb
// `?SetStateAfterTime@CTCTavernGame@@QAEXW4ETavernGameStates@1@M@Z`.
// Callee: the virtual LeaveGame() (vtable 0x012560BC slot 30, +0x78 = 0x005EC650).
// Layout (retail offsets, see SetState 0x005E9B20: the PDB header sits 0x1c higher after its six 16/20-byte vectors):
// TavernGameState +0xc0, PreviousTavernGameState +0xc4, NextTavernGameState +0xc8, StateTimer +0xcc. Quit flag @ 0x013BAE1C.
// Shape: the float argument is copied with integer moves; the previous-state store precedes the wait-state immediate.
typedef long ETavernGameStates;

extern bool g_bTavernGameQuitRequested;      // 0x013BAE1C

class CTCTavernGame { public:
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
    void SetStateAfterTime(ETavernGameStates state, float time);
};

void CTCTavernGame::SetStateAfterTime(ETavernGameStates state, float time)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = 14;
    NextTavernGameState = state;
    StateTimer = time;
}