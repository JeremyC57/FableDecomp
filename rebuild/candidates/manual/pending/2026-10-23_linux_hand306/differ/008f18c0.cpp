// Grade: DIFFER(772v770) -- x87 cross-term operand order (F.X vs Up.Z), def/old reg alloc
// CTCTavernGameCardBase::UpdateHighlightedCard() -- clear the old highlight, then while the game waits for a pick (state 0x17) find the card under the pointer in table space and highlight it (with the pick sound when it changes)
// Leaf: ?UpdateHighlightedCard@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F18C0, 770 bytes (+ int3 padding), plain `ret`; vtable slot 61.
// Callees: CThing::GetPosition() const 0x004C73D0; CTCTavernGameCardBase::GetCardDef 0x008F0B20 (same-TU stand-in, as the
//   landed GetPackPosition 0x008F0B80); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x5B);
//   NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect(void*) 0x004BFFC0 / SetAsHighlighted 0x004C04C0;
//   CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCTavernGame::PlaySound 0x005EA950. 0.5f 0x0122F59C.
// Layout (retail, as the landed FrameUpdate 0x008F1D40 / UpdatePointerScale 0x008F1BD0): TavernGameState +0xc0,
//   GamePositionRHSet +0xf0, GamePositionPos +0x108, Cards +0x128, Pointer +0x140, HighlightedCard +0x30c;
//   CTavernGameCardBaseDef CardWidth/CardHeight +0x2c/+0x30, highlight width +0x50, colour +0x54; CCard thing +0x00.
// Shape: the table-space position is the offset dotted with the set's right (forward x up) and forward axes.
struct CTCBase;
struct CRGBColour { unsigned char B, G, R, A; };

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DVector { float X, Y; };
__forceinline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
__forceinline float Dot(const C3DVector& a, const C3DVector& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    __forceinline void ToLocal(const C3DVector& d, float& x, float& y) const
    {
        C3DVector right = CrossProduct(Forward, Up);
        x = Dot(d, right);
        y = Dot(d, Forward);
    }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { PRIORITY_3 = 3 };
struct CTCGraphicAppearance {
    void ClearMeshEffect(void* key);                    // 0x004BFFC0
    void SetAsHighlighted(float width, float start, const CRGBColour& colour, bool includeCarried, bool flag,
                          EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
};
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

struct CCard {
    CThing* Thing;                                      // +0x00
};
struct CCardVector {
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_refCount;                                    // +0x04
};
struct CTavernGameCardBaseDef : CDefPointee {
    unsigned char _pad08[0x2c - 0x08];
    float CardWidth;                                    // +0x2c
    float CardHeight;                                   // +0x30
    unsigned char _pad34[0x50 - 0x34];
    float HighlightWidth;                               // +0x50
    CRGBColour HighlightColour;                         // +0x54
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

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad04[0xc0 - 0x04];
    long TavernGameState;                               // +0xc0
    unsigned char _padc4[0xf0 - 0xc4];
    CRightHandedSet GamePositionRHSet;                  // +0xf0
    C3DVector GamePositionPos;                          // +0x108
    unsigned char _pad114[0x128 - 0x114];
    CCardVector Cards;                                  // +0x128
    unsigned char _pad134[0x140 - 0x134];
    CThing* Pointer;                                    // +0x140
    unsigned char _pad144[0x30c - 0x144];
    CCard* HighlightedCard;                             // +0x30c
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* out);   // 0x008F0B20
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void UpdateHighlightedCard();
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

void CTCTavernGameCardBase::UpdateHighlightedCard()
{
    const C3DVector& pointerPos = Pointer->GetPosition();
    C2DVector pointer;
    GamePositionRHSet.ToLocal(C3DVector(pointerPos.X - GamePositionPos.X, pointerPos.Y - GamePositionPos.Y,
                                        pointerPos.Z - GamePositionPos.Z), pointer.X, pointer.Y);
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    float halfWidth = def->CardWidth * 0.5f;
    float halfHeight = def->CardHeight * 0.5f;
    CCard* old = HighlightedCard;
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (old) {
        old->Thing->GetInterface(&appearance, 0x5b);
        appearance->ClearMeshEffect(this);
        HighlightedCard = 0;
    }
    if (TavernGameState == 0x17) {
        for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it) {
            const C3DVector& cardPos = (*it)->Thing->GetPosition();
            C2DVector card;
            GamePositionRHSet.ToLocal(C3DVector(cardPos.X - GamePositionPos.X, cardPos.Y - GamePositionPos.Y,
                                                cardPos.Z - GamePositionPos.Z), card.X, card.Y);
            if (card.X - halfWidth <= pointer.X && card.X + halfWidth >= pointer.X &&
                card.Y - halfHeight <= pointer.Y && card.Y + halfHeight >= pointer.Y) {
                HighlightedCard = *it;
                break;
            }
        }
        if (HighlightedCard) {
            if (HighlightedCard != old)
                PlaySound(CCharString("SOUND_CARD_HIGHLIGHT"));
            HighlightedCard->Thing->GetInterface(&appearance, 0x5b);
            appearance->SetAsHighlighted(def->HighlightWidth, 0.0f, def->HighlightColour, false, false,
                                         NGraphicAppearance::PRIORITY_3, this);
        }
    }
}
