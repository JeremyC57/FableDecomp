// CTCTavernGameCardPairs::SelectCard() -- pairs selection: remember the first card, run the card-base selection, drop a selection that is already face up or repeats the first card, flip a face-down card (flip sound), and once two cards are up compare them (correct: clear both; incorrect sound otherwise)
// Leaf: ?SelectCard@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5FB0, 272 bytes (plain `ret`, returns true); CTCTavernGameCardPairs
// vtable 0x0126F6FC slot 59 (Ego_r.pdb `?SelectCard@CTCTavernGameCardPairs@@UAE_NXZ`; the manifest says "vfunc_59").
// Callees: CTCTavernGameCardBase::SelectCard() 0x008F09C0 (non-virtual base call); CCharString(const char*, long)
//          0x0099EBF0 ("TAVERN_GAME_CARD_FLIP" 0x0126F6C4, "TAVERN_GAME_CORRECT" 0x0126F414, "TAVERN_GAME_INCORRECT"
//          0x0126F6AC); CTCTavernGame::PlaySound(const CCharString&) 0x005EA950; ~CCharString 0x0099EAE0; CCard::Turn()
//          0x008F0620.
// Layout (retail): CTCTavernGame::GameStarted +0xa2 (PDB +0xbe); CTCTavernGameCardBase::SelectedCard +0x308 (PDB +0x32c);
//          CTCTavernGameCardPairs::AlternateSelectedCard +0x318 (PDB +0x338); CCard Value +0x08, Turning +0x24,
//          FaceDown +0x25 (as the landed GetHandValue 0x007DA230).
// Shape: the zero lives in ebx for every test and clear; each selection test re-reads SelectedCard; the sound names
//        are block-scoped named strings; the match/mismatch arms share the hoisted `push -1 ; lea ecx` and cross-jump
//        into one PlaySound + destructor.
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CCard {
    void*         Thing;                        // +0x00 CThingObject*
    long          Name;                         // +0x04
    long          Value;                        // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                      // +0x24
    bool          FaceDown;                     // +0x25
    void Turn();                                // 0x008F0620
};

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0x9e];
    bool          GameStarted;                  // +0xa2
    unsigned char _pad_0xa3[0x265];
    CCard*        SelectedCard;                 // +0x308
    CCard*        HighlightedCard;              // +0x30c
    long          CurrentMovingCard;            // +0x310
    unsigned char _pad_0x314[4];
    bool SelectCard();                          // 0x008F09C0 (virtual slot 59, called non-virtually)
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    CCard*        AlternateSelectedCard;        // +0x318
    bool SelectCard();
};

bool CTCTavernGameCardPairs::SelectCard()
{
    GameStarted = true;
    if (SelectedCard && AlternateSelectedCard)
        return true;
    if (!AlternateSelectedCard)
        AlternateSelectedCard = SelectedCard;
    CTCTavernGameCardBase::SelectCard();
    if (SelectedCard && (SelectedCard->Turning || !SelectedCard->FaceDown))
        SelectedCard = 0;
    if (SelectedCard == AlternateSelectedCard)
        SelectedCard = 0;
    if (SelectedCard && SelectedCard->FaceDown == true && !SelectedCard->Turning) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP", -1);
            PlaySound(sound);
        }
        SelectedCard->Turn();
    }
    if (SelectedCard && AlternateSelectedCard) {
        if (SelectedCard->Value == AlternateSelectedCard->Value) {
            SelectedCard = 0;
            AlternateSelectedCard = 0;
            CCharString sound("TAVERN_GAME_CORRECT", -1);
            PlaySound(sound);
        } else {
            CCharString sound("TAVERN_GAME_INCORRECT", -1);
            PlaySound(sound);
        }
    }
    return true;
}
