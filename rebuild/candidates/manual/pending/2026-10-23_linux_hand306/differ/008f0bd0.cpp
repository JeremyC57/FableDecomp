// Grade: DIFFER(722v722) -- 4 lines: Move target X store before push 0
// CTCTavernGameCardBase::DealCards() -- deal the cards one at a time: place the current card at its layout slot (growing the dealt-area bounds and playing the deal sound), wait for it to land, move a stray card back, and enter state 0x15 once every card is dealt
// Leaf: ?DealCards@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F0BD0, 722 bytes (+ int3 padding), plain `ret`; vtable slot 51.
// Callees: CTCTavernGameCardBase::GetCardDef 0x008F0B20 (same-TU stand-in below, as the landed GetPackPosition
//   0x008F0B80); virtual slot 57 (the layout index); CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   CTCTavernGame::PlaySound 0x005EA950; CTCTavernGame::SetState 0x005E9B20; CTCTavernGameCardBase::GetPackPosition
//   0x008F0B80; CCard::Move(const C3DVector&, bool) 0x008F0660. Globals: the table-height factor 0x0138927C, 1e-4f
//   0x0129BA3C, 0.0f 0x0122DEDC.
// Layout (retail CTCTavernGameCardBase, as the landed FrameUpdate 0x008F1D40): MinX/MaxX/MinY/MaxY +0x118, Cards +0x128,
//   DealIndex +0x310; CTavernGameCardBaseDef CardWidth/CardHeight +0x2c/+0x30, height scale/offset +0x5c/+0x60, layouts
//   +0x6c (0x48-byte entries, slot positions vector at +0x3c); CCard (pack(1)) +0x20 state, +0x26 placed, +0x27 old
//   position, +0x33 target, +0x3f position.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
};
struct C2DVector {
    float X, Y;
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
struct CCardLayout {                                    // 0x48 bytes
    unsigned char _pad0[0x3c];
    C2DVector* Positions;                               // +0x3c (vector _M_start)
    unsigned char _pad40[8];
};
struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    float CardWidth;                                    // +0x2c
    float CardHeight;                                   // +0x30
    unsigned char _pad34[0x5c - 0x34];
    float HeightScale;                                  // +0x5c
    float HeightOffset;                                 // +0x60
    unsigned char _pad64[0x6c - 0x64];
    CCardLayout* Layouts;                               // +0x6c (vector _M_start)
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

#pragma pack(push, 1)
struct CCard {
    unsigned char _pad0[0x20];
    long State;                                         // +0x20
    unsigned char _pad24[2];
    bool Placed;                                        // +0x26
    C3DVector OldPosition;                              // +0x27
    C3DVector Target;                                   // +0x33
    C3DVector Position;                                 // +0x3f
    void Move(const C3DVector& to, bool snap);          // 0x008F0660
};
#pragma pack(pop)

struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};

extern float g_CardTableHeight;                         // 0x0138927C
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct CTCTavernGameCardBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual long GetLayoutIndex();                      // slot 57
    unsigned char _pad04[0x118 - 0x04];
    float MinX, MaxX, MinY, MaxY;                       // +0x118
    CCardVector Cards;                                  // +0x128
    unsigned char _pad134[0x310 - 0x134];
    unsigned long DealIndex;                            // +0x310
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    C3DVector GetPackPosition();                        // 0x008F0B80
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                          // 0x005E9B20
    void DealCards();
    void Deal(CDefPointer<CTavernGameCardBaseDef>& def, long layout);
};
void* FindRef_CTCTavernGameCardBase_GetCardDef_0();
#pragma auto_inline(off)
__declspec(noinline) void CTCTavernGameCardBase::GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out)
{
    void* p = FindRef_CTCTavernGameCardBase_GetCardDef_0();
    if (!p)
        return;
    *(void**)out = p;
}
#pragma auto_inline(on)

#include <math.h>

void CTCTavernGameCardBase::DealCards()
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    long layout = GetLayoutIndex();
    if (MinX == 0.0f && MaxX == 0.0f && MinY == 0.0f && MaxY == 0.0f) {
        DealIndex = 0;
    } else if (!Cards._M_start[DealIndex]->Placed) {
        if (++DealIndex == Cards.size()) {
            SetState(0x15);
            return;
        }
    } else {
        CCard* card = Cards._M_start[DealIndex];
        if (fabs(card->Target.Z - (def->HeightScale * g_CardTableHeight + def->HeightOffset)) > g_epsilon_0129ba3c) {
            C2DVector pack(GetPackPosition());
            card = Cards._M_start[DealIndex];
            C2DVector cardPos(card->Position);
            if (fabs(pack.X - cardPos.X) > def->CardWidth || fabs(pack.Y - cardPos.Y) > def->CardHeight) {
                card->Move(C3DVector(card->Target.X, card->Target.Y, def->HeightScale * g_CardTableHeight + def->HeightOffset),
                           false);
            }
        }
        goto done;
    }
    {
    const C2DVector& slot = def->Layouts[layout].Positions[DealIndex];
    C3DVector position;
    position.X = slot.X * def->CardWidth;
    position.Y = def->CardHeight * slot.Y;
    position.Z = Cards._M_start[DealIndex]->OldPosition.Z;
    if (position.X < MinX)
        MinX = position.X;
    if (position.X > MaxX)
        MaxX = position.X;
    if (position.Y < MinY)
        MinY = position.Y;
    if (position.Y > MaxY)
        MaxY = position.Y;
    {
        CCharString sound("SOUND_CARD_DEAL");
        PlaySound(sound);
    }
    CCard* card = Cards._M_start[DealIndex];
    if (card->Placed) {
        card->OldPosition = card->Position;
    } else {
        card->Position = card->OldPosition;
        card->Placed = true;
    }
    card->Target = position;
    card->State = 0;
    }
done:
    ;
}
