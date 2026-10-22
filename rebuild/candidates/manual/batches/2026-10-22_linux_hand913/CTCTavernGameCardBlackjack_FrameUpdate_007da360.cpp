// CTCTavernGameCardBlackjack::FrameUpdate() -- the card-game base update, then in the play states 28..37 the virtual per-state update (slot 56), in state 38 TurnPlayerCards
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA360, 50 bytes (plain `ret`). Vtable 0x0126F8F4 (CTCTavernGameCardBlackjack) slot 9.
// Callees: CTCTavernGameCardBase::FrameUpdate @ 0x008F1D40, CTCTavernGameCardBlackjack::TurnPlayerCards @ 0x007DA290 (a
// sibling-call `jmp`), this's virtual slot 56 (+0xe0, a sibling-call `jmp`).
// Layout: retail CTCTavernGame sits 0x1C below Ego_r.pdb (landed 0x005EC8A0 / 0x005E9D60): TavernGameState +0xC0. The header
// engine/CTCTavernGame.h carries the PDB offsets (STLport vectors 16 bytes there), so a local model.
// Shape: nested ifs -- the outer `>= 28` exits straight to `ret`; the inner test is written `> 37` first (state 38 arm), the
// virtual call as its else, which VC7.1 lays out last (`jle` to it); `<= 37` first puts the virtual call inline.
typedef long ETavernGameStates;

struct CTCTavernGameCardBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55();
    virtual void UpdateGameState();                     // slot 56 (+0xe0)
    unsigned char     _pad_0x04[0xc0 - 0x04];
    ETavernGameStates TavernGameState;                  // +0xc0
    void FrameUpdate();                                 // 0x008F1D40
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    void TurnPlayerCards();                             // 0x007DA290
    void FrameUpdate();
};

void CTCTavernGameCardBlackjack::FrameUpdate()
{
    CTCTavernGameCardBase::FrameUpdate();
    if (TavernGameState >= 28) {
        if (TavernGameState > 37) {
            if (TavernGameState == 38)
                TurnPlayerCards();
        } else
            UpdateGameState();
    }
}
