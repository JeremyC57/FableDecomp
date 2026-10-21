// CTCTavernGameCardBase::CardsTurning() -- once no card is still turning, advance the game state; after a gather (state 0x19) stack every card on the averaged position, each higher by the def's step, and move to state 0x1a
// Grade: DIFFER(487v506) -- retail keeps a separate release+ret for the still-turning early return (ours merges it into the common tail), holds def in ebp and reuses def's frame slot for the height counter, and releases def via dec [ecx+4]; tried 5 shapes (b..e).
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CTavernGameCardBaseDef : CDefBase {
    unsigned char _pad08[0x5c - 0x08];
    float CardStackHeightStep;                          // +0x5c (descriptive)
    float CardStackBaseHeight;                          // +0x60 (descriptive)
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(push, 1)
struct CCard {                                          // descriptive card record (odd member offsets)
    unsigned char _pad00[0x20];
    long          TurnFrame;                            // +0x20
    bool          Turning;                              // +0x24
    unsigned char _pad25;
    bool          HasRestPosition;                      // +0x26
    C3DVector     RestPosition;                         // +0x27
    C3DVector     TargetPosition;                       // +0x33
    C3DVector     Position;                             // +0x3f
};
#pragma pack(pop)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CCardVector {                                    // retail STLport vector<CCard*> (12 bytes)
    CCard** first;
    CCard** last;
    CCard** endOfStorage;
    unsigned long size() const { return last - first; }
};
struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad04[0xc0 - 0x04];
    long State;                                         // +0xc0 (CTCTavernGame current state)
    unsigned char _padc4[0x128 - 0xc4];
    CCardVector Cards;                                  // +0x128
    void SetState(long state);                          // 0x005E9B20
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* def);   // 0x008F0B20
    void CardsTurning();
};

void CTCTavernGameCardBase::CardsTurning()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    for (CCard** it = Cards.first; it != Cards.last; ++it)
        if ((*it)->Turning == true)
            return;
    if (State == 0x16)
        SetState(0x17);
    else if (State == 0x19) {
        float sumX = 0.0f;
        float sumY = 0.0f;
        for (CCard** it = Cards.first; it != Cards.last; ++it) {
            sumX += (*it)->Position.X;
            sumY += (*it)->Position.Y;
        }
        float averageX = sumX / (float)Cards.size();
        float averageY = sumY / (float)Cards.size();
        C3DVector stackPos;
        stackPos.X = averageX;
        stackPos.Y = averageY;
        long height = 0x35;
        for (CCard** it = Cards.first; it != Cards.last; ++it) {
            PlaySound("TAVERN_GAME_CARD_DEAL");
            CCard* card = *it;
            stackPos.Z = (float)height * def->CardStackHeightStep + def->CardStackBaseHeight;
            if (card->HasRestPosition)
                card->RestPosition = card->Position;
            else {
                card->Position = card->RestPosition;
                card->HasRestPosition = true;
            }
            card->TargetPosition = stackPos;
            card->TurnFrame = 0;
            --height;
        }
        SetState(0x1a);
    } else
        SetState(0);
}
