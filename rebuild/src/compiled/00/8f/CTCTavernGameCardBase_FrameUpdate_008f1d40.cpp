// CTCTavernGameCardBase::FrameUpdate() -- per-frame card-game update: base update and pointer scale, the state's card phase (deal, move, finished moving, turn, turn back, main game), then while the game is being played every card in the pack follows the game position and the highlight is refreshed
// Leaf: ?FrameUpdate@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F1D40, 198 bytes of code plus its 10-entry jump table (row 240); vtable slot
// 9 (CTCTavernGameCardSorting 0x0126F5B4 inherits it). Ego_r.pdb `?FrameUpdate@CTCTavernGameCardBase@@UAEXXZ`.
// Callees: CTCTavernGame::FrameUpdate() 0x005EB9E0 (non-virtual base call); CTCTavernGameCardBase::UpdatePointerScale()
//          0x008F1BD0; the card-phase virtuals DealCards (slot 51), CardsMoving (52), CardsFinishedMoving (53),
//          CardsTurning (54), TurnCardsBack (55, 0x008F17D0), CardsMainGame (56) and UpdateHighlightedCard (61, tail jump);
//          CCard::FrameUpdate(const C3DVector&, const CRightHandedSet&) 0x008F06F0 (Ego_r.pdb; manifest
//          "CTCTavernGame_UpdateCardTilt" is a gap-scan name).
// Slot names: the slot order in the CardSorting / Blackjack / Pairs vtables against Ego_r.pdb's source order (Blackjack
//          0x007DA070 CardsMoving = 52, 0x007DA120 CardsFinishedMoving = 53, 0x007DA090 SelectCard = 59, 0x007DA0A0
//          UpdateHighlightedCard = 61; CardSorting slot 54 CardsTurning 0x008F0EB0, 55 TurnCardsBack 0x008F17D0).
// Layout (retail, as the landed constructor 0x008F1CD0): CTCTavernGame::IsBeingPlayed +0x20, TavernGameState +0xc0;
//         GamePositionRHSet +0xf0, GamePositionPos +0x108, Pack +0x134/+0x138.
// Shape: a dense switch over states 18..27 (`add eax,-12h ; cmp eax,9 ; ja` + jump table) with the case blocks in source
//        order; the pack walk re-reads Pack.end() and keeps both argument addresses in ebx/ebp.
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: float Up[3], Forward[3]; };

class CCard { public:
    void FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation);   // 0x008F06F0
};
struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    CCard** begin() { return _M_start; }
    CCard** end() { return _M_finish; }
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0x1c];
    bool          IsBeingPlayed;                // +0x20
    unsigned char _pad_0x21[0x9f];
    long          TavernGameState;              // +0xc0 ETavernGameStates
    unsigned char _pad_0xc4[0x2c];
    void FrameUpdate();                         // 0x005EB9E0
};

struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    CRightHandedSet GamePositionRHSet;          // +0xf0
    C3DVector     GamePositionPos;              // +0x108
    long          GamePositionActionPointIndex; // +0x114
    float         MinX, MaxX, MinY, MaxY;       // +0x118
    CCardVector   Cards;                        // +0x128
    CCardVector   Pack;                         // +0x134
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                 // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45();
    virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50();
    virtual void DealCards();                   // slot 51
    virtual void CardsMoving();                 // slot 52
    virtual void CardsFinishedMoving();         // slot 53
    virtual void CardsTurning();                // slot 54
    virtual void TurnCardsBack();               // slot 55
    virtual void CardsMainGame();               // slot 56
    virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void UpdateHighlightedCard();       // slot 61
    void UpdatePointerScale();                  // 0x008F1BD0
};

void CTCTavernGameCardBase::FrameUpdate()
{
    CTCTavernGame::FrameUpdate();
    UpdatePointerScale();
    switch (TavernGameState) {
    case 18:
    case 19:
        DealCards();
        break;
    case 20:
    case 26:
    case 27:
        CardsMoving();
        break;
    case 21:
        CardsFinishedMoving();
        break;
    case 24:
        TurnCardsBack();
        break;
    case 22:
    case 25:
        CardsTurning();
        break;
    case 23:
        CardsMainGame();
        break;
    }
    if (IsBeingPlayed && TavernGameState != 0) {
        for (CCard** it = Pack.begin(); it != Pack.end(); ++it)
            (*it)->FrameUpdate(GamePositionPos, GamePositionRHSet);
        UpdateHighlightedCard();
    }
}