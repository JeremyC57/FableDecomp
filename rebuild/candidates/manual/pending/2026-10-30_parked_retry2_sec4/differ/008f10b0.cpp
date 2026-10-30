// CTCTavernGameCardBase::ShuffleCards() -- shuffle the deck order, then hide every card and stack it face down at the deck position, each card one def step lower than the previous, offset by the scaled card-layout vector from the owner's sub def
// Grade: DIFFER(438v410) nd=172 (was 466v410 nd=177) -- card offset added in place (pos.X += off.X; pos.Y += off.Y); residue: the layout sub-def copy/temp refcount in a register (retail load/inc/dec/store pairs, ours inc [mem] + reloads), def frame word shared with the height counter, frame 0x34 v 0x20
// Retail: Fable.exe @ 0x008F10B0, 416 bytes (plain `ret`, no EH frame).
// Callees: GetCardDef 0x008F0B20, Algorithm_FisherYatesShuffle 0x007D7860 (__fastcall first,last),
//   CDefClassBase::GetSubDef<T> 0x008F27A0, CTCTavernGame::HideMesh 0x005E9B10 / PositionMesh 0x005EABC0 (static __fastcall)
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct C2DVector {
    float X;
    float Y;
};
struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct CCardEntry { unsigned char _pad[0x44]; };        // 68-byte deck entry
struct CTavernGameCardBaseDef : CDefBase {
    unsigned char _pad08[0x5c - 0x08];
    float CardStackHeightStep;                          // +0x5c (descriptive)
    float CardStackBaseHeight;                          // +0x60 (descriptive)
    unsigned char _pad64[0x78 - 0x64];
    CCardEntry* DeckFirst;                              // +0x78 (STLport vector<68-byte entry>)
    CCardEntry* DeckLast;                               // +0x7c
    unsigned long DeckSize() const { return DeckLast - DeckFirst; }
};
struct CTavernGameLayoutDef : CDefBase {                // owner sub def (descriptive)
    unsigned char _pad08[0x2c - 0x08];
    C2DVector CardSpacing;                              // +0x2c
    unsigned char _pad34[0x64 - 0x34];
    C2DVector CardScale;                                // +0x64
    C2DVector GetCardOffset() const { C2DVector r; r.X = CardScale.X * CardSpacing.X; r.Y = CardScale.Y * CardSpacing.Y; return r; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CTavernGameLayoutDef>* def) const;   // 0x008F27A0
    CDefPointer<CTavernGameLayoutDef> GetLayoutDef() const { CDefPointer<CTavernGameLayoutDef> d; GetSubDef(&d); return d; }
};
class CThing;
struct CThingHead {
    unsigned char _pad00[0x70];
    CDefClassBase* DefClass;                            // +0x70
};
#pragma pack(push, 1)
struct CCard {
    CThing*       Mesh;                                 // +0x00
    unsigned char _pad04[0x10 - 0x04];
    float         Alpha;                                // +0x10 (descriptive)
    unsigned char _pad14[0x25 - 0x14];
    bool          FaceDown;                             // +0x25 (descriptive)
    unsigned char _pad26;
    C3DVector     RestPosition;                         // +0x27
    C3DVector     TargetPosition;                       // +0x33
};
#pragma pack(pop)
void __fastcall Algorithm_FisherYatesShuffle(CCard** first, CCard** last);   // 0x007D7860
struct CTCTavernGame {
    static void __fastcall HideMesh(CThing* thing);    // 0x005E9B10
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& pos, const CRightHandedSet& rh,
                                        const C3DVector& offset, float a, float b);   // 0x005EABC0
};
struct CTCTavernGameCardBase {
    virtual void v00();
    CThingHead* Owner;                                  // +0x04
    unsigned char _pad08[0xf0 - 0x08];
    CRightHandedSet DeckRH;                             // +0xf0
    C3DVector DeckPosition;                             // +0x108
    unsigned char _pad114[0x134 - 0x114];
    CCard** ShuffledFirst;                              // +0x134
    CCard** ShuffledLast;                               // +0x138
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* def);   // 0x008F0B20
    void ShuffleCards();
};

void CTCTavernGameCardBase::ShuffleCards()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    Algorithm_FisherYatesShuffle(ShuffledFirst, ShuffledLast);
    if (def->DeckSize() != 0) {
        long height = 0x35;
        unsigned long i = 0;
        do {
            C3DVector pos(0.0f, 0.0f, (float)height * def->CardStackHeightStep + def->CardStackBaseHeight);
            C2DVector off = Owner->DefClass->GetLayoutDef()->GetCardOffset();
            pos.X += off.X;
            pos.Y += off.Y;
            CCard* card = ShuffledFirst[i];
            CTCTavernGame::HideMesh(card->Mesh);
            CTCTavernGame::PositionMesh(card->Mesh, DeckPosition, DeckRH, pos, 0.5f, 0.5f);
            card->RestPosition = pos;
            card->TargetPosition = pos;
            card->Alpha = 0.5f;
            card->FaceDown = true;
            ++i;
            --height;
        } while (i < def->DeckSize());
    }
}
