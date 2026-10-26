// CTCTavernGameCardBase::TurnAllCards() -- for every card: play the flip sound and start a half turn (0 <-> 0.5) towards the other face, then go to tavern-game state 22
// Leaf: TurnAllCards
// Retail: Fable.exe @ 0x008F09E0, 136 bytes; CTCTavernGameCardSorting vtable 0x0126F5B4 slot 53 (the catalog's "vfunc_53";
//         the body sits in the CTCTavernGameCardBase TU and is inherited: CTCTavernGameCardPairs' vtable 0x0126F6FC overrides
//         slot 53 with 0x007D6260, so this is CTCTavernGameCardBase's implementation). Real name unknown, descriptive name used.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CTCTavernGame::PlaySound(const CCharString&) 0x005EA950,
//          CCharString::~CCharString() 0x0099EAE0, CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20. String
//          0x0126F6C4 "TAVERN_GAME_CARD_FLIP".
// Layout: retail CTCTavernGameCardBase::Cards (vector<CCard*>, 12 bytes) +0x128 (PDB +0x140); Ego_r.pdb CCard (same in retail):
//         InitialRotation +0x0c, FinalRotation +0x10, TurnFraction +0x18, Turning +0x24, FaceDown +0x25.
// Shape: the sound name is a named local (an unnamed temporary reuses the ctor's eax); `cmp cl,1 ; jne` is the explicit `== true` test of the bool; the zero and 0.5 live in ebx/ebp across the loop.
typedef long ETavernGameStates;

class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CCard { public:
    void* Thing;                  // +0x00
    long  Name;                   // +0x04
    long  Value;                  // +0x08
    float InitialRotation;        // +0x0c
    float FinalRotation;          // +0x10
    float TurnSpeed;              // +0x14
    float TurnFraction;           // +0x18
    float MoveSpeed;              // +0x1c
    float MoveFraction;           // +0x20
    bool  Turning;                // +0x24
    bool  FaceDown;               // +0x25
};

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(ETavernGameStates state);             // 0x005E9B20
    unsigned char _pad_0x04[0x124];
    CCard** CardsBegin;           // +0x128
    CCard** CardsEnd;             // +0x12c
    CCard** CardsEndOfStorage;    // +0x130
    void TurnAllCards();
};

void CTCTavernGameCardBase::TurnAllCards()
{
    for (CCard** it = CardsBegin; it != CardsEnd; ++it) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP");
            PlaySound(sound);
        }
        CCard* card = *it;
        card->Turning = true;
        card->TurnFraction = 0.0f;
        if (card->FaceDown == true) {
            card->InitialRotation = 0.5f;
            card->FinalRotation = 0.0f;
        } else {
            card->InitialRotation = 0.0f;
            card->FinalRotation = 0.5f;
        }
        card->FaceDown = !card->FaceDown;
    }
    SetState(22);
}