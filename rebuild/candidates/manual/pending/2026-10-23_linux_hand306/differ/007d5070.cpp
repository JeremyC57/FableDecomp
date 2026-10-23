// Grade: DIFFER(539v539) -- 2 lines: Move target X=0 store scheduled before fstp Z (retail after); tried ctor init-list/body, named local, member assigns, z local
// CTCTavernGameCardSorting::CardsMainGame() -- the sorting main loop: run the timed-game clock (a timed game past its target time is lost: lose sound and the round-over OK message, state 0x18); once every card is face up and settled, stack them back on the table with the deal sound and enter state 0x1a
// Leaf: ?CardsMainGame@CTCTavernGameCardSorting
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D5070, 544 bytes (+ int3 padding), plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCTavernGame::PlaySound 0x005EA950; CCard::Move 0x008F0660; CTCTavernGameCardBase::GetCardDef 0x008F0B20; CTCTavernGame::SetState 0x005E9B20;
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
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x5c - 0x08];
    float HeightScale;                          // +0x5c
    float HeightOffset;                         // +0x60
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(push, 1)
struct CCard {
    unsigned char _pad0[0x24];
    bool Turning;                               // +0x24
    bool FaceDown;                              // +0x25
    void Move(const C3DVector& to, bool snap);  // 0x008F0660
};
#pragma pack(pop)
struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
extern long g_FrameRate_01375550;               // 0x01375550

struct CTCTavernGameCardSorting {
    virtual void v00();
    unsigned char _pad04[0x21 - 0x04];
    bool GameWon;                               // +0x21
    unsigned char _pad22[0x28 - 0x22];
    bool GameFinished;                          // +0x28
    unsigned char _pad29[0x98 - 0x29];
    float TargetTime;                           // +0x98
    unsigned char _pad9c[0x128 - 0x9c];
    CCardVector Cards;                          // +0x128
    unsigned char _pad134[0x154 - 0x134];
    float Counter;                              // +0x154
    bool TimerStopped;                          // +0x158
    unsigned char _pad159[0x300 - 0x159];
    long GameType;                              // +0x300
    void GetTavernGameDef(CDefPointer<CTavernGameDef>* out);          // 0x005EA000
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);        // 0x008F0B20
    CWideString GetText(unsigned long textID);  // 0x005E9E70
    void SetStateAfterOK(CWideString title, CWideString text, long state);   // 0x005EB080
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                  // 0x005E9B20
    void CardsMainGame();
};

void CTCTavernGameCardSorting::CardsMainGame()
{
    if (!TimerStopped) {
        switch (GameType) {
        case 1:
            Counter += 1.0f / (float)g_FrameRate_01375550;
            break;
        }
    }
    if (Counter < 0.0f)
        Counter = 0.0f;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it) {
        if (!(*it)->FaceDown) {
            switch (GameType) {
            case 1:
                if (Counter > TargetTime) {
                    TimerStopped = true;
                    GameFinished = true;
                    GameWon = false;
                    {
                        CCharString sound("TAVERN_GAME_LOSE_GAME");
                        PlaySound(sound);
                    }
                    CDefPointer<CTavernGameDef> def;
                    GetTavernGameDef(&def);
                    unsigned long text = GameWon ? def->WinText : def->LoseText;
                    SetStateAfterOK(GetText(text), GetText(def->OKBodyText), 0x18);
                }
                break;
            }
            return;
        }
    }
    TimerStopped = true;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it)
        if ((*it)->Turning == true)
            return;
    unsigned long i = 0;
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it, ++i) {
        {
            CCharString sound("TAVERN_GAME_CARD_DEAL");
            PlaySound(sound);
        }
        (*it)->Move(C3DVector(0.0f, 0.0f, (float)(Cards.size() - i) * def->HeightScale + def->HeightOffset), false);
    }
    SetState(0x1a);
}
