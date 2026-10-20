// Behaviour test for 008f1d40: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBase::FrameUpdate() -- per-frame card-game update: base update and pointer scale, the state's card phase (deal, move, finished moving, turn, turn back, main game), then while the game is being played every card in the pack follows the game position and the highlight is refreshed
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float Up[3], Forward[3]; };

struct CCard {
    void FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation);   // 0x008F06F0
};
struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    CCard** begin() { return _M_start; }
    CCard** end() { return _M_finish; }
};

struct CTCTavernGame {
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

// ---- stubs and driver ----
void CTCTavernGame::v00() {}
void CTCTavernGameCardBase::v01() {}
void CTCTavernGameCardBase::v02() {}
void CTCTavernGameCardBase::v03() {}
void CTCTavernGameCardBase::v04() {}
void CTCTavernGameCardBase::v05() {}
void CTCTavernGameCardBase::v06() {}
void CTCTavernGameCardBase::v07() {}
void CTCTavernGameCardBase::v08() {}
void CTCTavernGameCardBase::v10() {}
void CTCTavernGameCardBase::v11() {}
void CTCTavernGameCardBase::v12() {}
void CTCTavernGameCardBase::v13() {}
void CTCTavernGameCardBase::v14() {}
void CTCTavernGameCardBase::v15() {}
void CTCTavernGameCardBase::v16() {}
void CTCTavernGameCardBase::v17() {}
void CTCTavernGameCardBase::v18() {}
void CTCTavernGameCardBase::v19() {}
void CTCTavernGameCardBase::v20() {}
void CTCTavernGameCardBase::v21() {}
void CTCTavernGameCardBase::v22() {}
void CTCTavernGameCardBase::v23() {}
void CTCTavernGameCardBase::v24() {}
void CTCTavernGameCardBase::v25() {}
void CTCTavernGameCardBase::v26() {}
void CTCTavernGameCardBase::v27() {}
void CTCTavernGameCardBase::v28() {}
void CTCTavernGameCardBase::v29() {}
void CTCTavernGameCardBase::v30() {}
void CTCTavernGameCardBase::v31() {}
void CTCTavernGameCardBase::v32() {}
void CTCTavernGameCardBase::v33() {}
void CTCTavernGameCardBase::v34() {}
void CTCTavernGameCardBase::v35() {}
void CTCTavernGameCardBase::v36() {}
void CTCTavernGameCardBase::v37() {}
void CTCTavernGameCardBase::v38() {}
void CTCTavernGameCardBase::v39() {}
void CTCTavernGameCardBase::v40() {}
void CTCTavernGameCardBase::v41() {}
void CTCTavernGameCardBase::v42() {}
void CTCTavernGameCardBase::v43() {}
void CTCTavernGameCardBase::v44() {}
void CTCTavernGameCardBase::v45() {}
void CTCTavernGameCardBase::v46() {}
void CTCTavernGameCardBase::v47() {}
void CTCTavernGameCardBase::v48() {}
void CTCTavernGameCardBase::v49() {}
void CTCTavernGameCardBase::v50() {}
void CTCTavernGameCardBase::v57() {}
void CTCTavernGameCardBase::v58() {}
void CTCTavernGameCardBase::v59() {}
void CTCTavernGameCardBase::v60() {}

static char g_log[64]; static int g_n;
static void Log(char c) { if (g_n < 63) g_log[g_n++] = c; g_log[g_n] = 0; }
void CTCTavernGame::FrameUpdate() { Log('F'); }
void CTCTavernGameCardBase::UpdatePointerScale() { Log('P'); }
void CTCTavernGameCardBase::DealCards() { Log('1'); }
void CTCTavernGameCardBase::CardsMoving() { Log('2'); }
void CTCTavernGameCardBase::CardsFinishedMoving() { Log('3'); }
void CTCTavernGameCardBase::CardsTurning() { Log('4'); }
void CTCTavernGameCardBase::TurnCardsBack() { Log('5'); }
void CTCTavernGameCardBase::CardsMainGame() { Log('6'); }
void CTCTavernGameCardBase::UpdateHighlightedCard() { Log('H'); }
static CTCTavernGameCardBase* g_game;
void CCard::FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation)
{ if (&position == &g_game->GamePositionPos && &orientation == &g_game->GamePositionRHSet) Log('c'); else Log('?'); }

static bool Run(long state, bool played, const char* expected)
{
    g_n = 0; g_log[0] = 0;
    g_game->TavernGameState = state; g_game->IsBeingPlayed = played;
    g_game->FrameUpdate();
    if (std::strcmp(g_log, expected) != 0) { std::printf("008f1d40_TEST FAIL (state %ld: %s, want %s)\n", state, g_log, expected); return false; }
    return true;
}

int main()
{
    static CTCTavernGameCardBase game;
    static CCard cards[2];
    static CCard* pack[2] = { &cards[0], &cards[1] };
    g_game = &game;
    game.Pack._M_start = pack; game.Pack._M_finish = pack + 2; game.Pack._M_end_of_storage = pack + 2;
    const char* want[10] = { "FP1ccH", "FP1ccH", "FP2ccH", "FP3ccH", "FP4ccH", "FP6ccH", "FP5ccH", "FP4ccH", "FP2ccH", "FP2ccH" };
    for (long s = 18; s <= 27; ++s)
        if (!Run(s, true, want[s - 18])) return 1;
    if (!Run(17, true, "FPccH") || !Run(28, true, "FPccH") || !Run(0, true, "FP") || !Run(19, false, "FP1")) return 1;
    std::printf("008f1d40_TEST PASS\n");
    return 0;
}