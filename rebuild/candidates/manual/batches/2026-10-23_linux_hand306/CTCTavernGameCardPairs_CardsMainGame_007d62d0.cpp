// CTCTavernGameCardPairs::CardsMainGame() -- the pairs main loop: flip back or clear a mismatched selected pair, run the timed-game clock, and once every card is face up and settled play the win/lose sound and show the round-over OK message (state 0x18)
// Leaf: ?CardsMainGame@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D62D0, 656 bytes (+ int3 padding), plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCTavernGame::PlaySound 0x005EA950; CCard::Turn 0x008F0620;
//   CTCTavernGame::GetTavernGameDef 0x005EA000 / GetText 0x005E9E70 / SetStateAfterOK 0x005EB080. Globals: the frame rate
//   0x01375550 (int), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout (retail): GameWon +0x21, NumRounds +0x24, GameFinished +0x28, TargetTime +0x98, GameStarted +0xa2, Cards +0x128,
//   Counter +0x154, TimerStopped +0x158, GameType +0x300, SelectedCard +0x308, AlternateSelectedCard +0x318; CCard Turning
//   +0x24, FaceDown +0x25; CTavernGameDef win/lose text +0xa4/+0xa8, body text +0xf8.
struct CWideString {
    void* m_pData;
    CWideString(const CWideString& other);
    ~CWideString();
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0xa4 - 0x08];
    unsigned long WinText;                      // +0xa4
    unsigned long LoseText;                     // +0xa8
    unsigned char _padAC[0xf8 - 0xac];
    unsigned long OKBodyText;                   // +0xf8
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CCard {
    unsigned char _pad0[0x24];
    bool Turning;                               // +0x24
    bool FaceDown;                              // +0x25
    void Turn();                                // 0x008F0620
};
struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
};
extern long g_FrameRate_01375550;               // 0x01375550

struct CTCTavernGameCardPairs {
    virtual void v00();
    unsigned char _pad04[0x21 - 0x04];
    bool GameWon;                               // +0x21
    unsigned char _pad22[0x24 - 0x22];
    long NumRounds;                             // +0x24
    bool GameFinished;                          // +0x28
    unsigned char _pad29[0x98 - 0x29];
    float TargetTime;                           // +0x98
    unsigned char _pad9c[0xa2 - 0x9c];
    bool GameStarted;                           // +0xa2
    unsigned char _pada3[0x128 - 0xa3];
    CCardVector Cards;                          // +0x128
    unsigned char _pad134[0x154 - 0x134];
    float Counter;                              // +0x154
    bool TimerStopped;                          // +0x158
    unsigned char _pad159[0x300 - 0x159];
    long GameType;                              // +0x300
    unsigned char _pad304[0x308 - 0x304];
    CCard* SelectedCard;                        // +0x308
    unsigned char _pad30c[0x318 - 0x30c];
    CCard* AlternateSelectedCard;               // +0x318
    void GetTavernGameDef(CDefPointer* out);    // 0x005EA000
    CWideString GetText(unsigned long textID);  // 0x005E9E70
    void SetStateAfterOK(CWideString title, CWideString text, long state);   // 0x005EB080
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void CardsMainGame();
};

void CTCTavernGameCardPairs::CardsMainGame()
{
    if (SelectedCard && AlternateSelectedCard && !SelectedCard->Turning && !AlternateSelectedCard->Turning) {
        if (SelectedCard->FaceDown && AlternateSelectedCard->FaceDown) {
            SelectedCard = 0;
            AlternateSelectedCard = 0;
        } else {
            if (!SelectedCard->FaceDown) {
                {
                    CCharString sound("TAVERN_GAME_CARD_FLIP");
                    PlaySound(sound);
                }
                SelectedCard->Turn();
            }
            if (!AlternateSelectedCard->FaceDown) {
                {
                    CCharString sound("TAVERN_GAME_CARD_FLIP");
                    PlaySound(sound);
                }
                AlternateSelectedCard->Turn();
            }
        }
    }
    bool stopped = TimerStopped;
    if (!stopped) {
        switch (GameType) {
        case 1:
            if (GameStarted)
                Counter += 1.0f / (float)g_FrameRate_01375550;
            break;
        }
    }
    if (Counter < 0.0f)
        Counter = 0.0f;
    long type = GameType;
    switch (type) {
    case 1:
        if (!stopped && Counter > TargetTime)
            TimerStopped = true;
        break;
    }
    if (!TimerStopped) {
        for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it)
            if ((*it)->FaceDown)
                return;
    }
    TimerStopped = true;
    switch (type) {
    case 1:
        if (NumRounds <= 1)
            GameFinished = true;
        GameWon = Counter <= TargetTime;
        break;
    }
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it)
        if ((*it)->Turning == true)
            return;
    if (GameWon) {
        CCharString sound("TAVERN_GAME_WIN_GAME");
        PlaySound(sound);
    } else {
        CCharString sound("TAVERN_GAME_LOSE_GAME");
        PlaySound(sound);
    }
    CDefPointer def;
    GetTavernGameDef(&def);
    unsigned long text = GameWon ? def->WinText : def->LoseText;
    SetStateAfterOK(GetText(text), GetText(def->OKBodyText), 0x18);
}
