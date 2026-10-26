// CTCTavernGameCardSorting::SelectCard() -- after the card-base selection, highlight every face-up card lower than the selected one (a sorting mistake: the round ends, a timed game is lost); with no mistake, a face-up highlighted card plays the "correct" sound
// Leaf: ?SelectCard@CTCTavernGameCardSorting
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D5890, 366 bytes (plain `ret`); CTCTavernGameCardSorting vtable 0x0126F5B4
// slot 59 (SelectCard, as CardPairs 0x007D5FB0, landed).
// Callees: CTCTavernGameCardBase::SelectCard() 0x008F09C0 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020 (interface 0x5B = word 2 bit 27); NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float,
//          float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*) 0x004C04C0; CCharString(const char*,
//          long) 0x0099EBF0 ("TAVERN_GAME_CORRECT" 0x0126F414); CTCTavernGame::PlaySound(const CCharString&) 0x005EA950;
//          ~CCharString 0x0099EAE0; the def's slot 1 on release.
// Layout (retail, as the landed CardBase/CardPairs sources): GameWon +0x21, GameFinished +0x28, Cards +0x128,
//          RoundFinished +0x158, GameType +0x300, SelectedCard +0x308, HighlightedCard +0x30c; CCard Thing +0x00,
//          Value +0x08, Turning +0x24, FaceDown +0x25; CTavernGameCardBaseDef highlight time +0x50, colour +0x58.
// Shape: the appearance pointer is declared (unset) outside the loop, so VC keeps it in edi across iterations and the
//        absent-interface arm reuses it (its home is the lookup key's frame word); the game-type test is a one-case
//        `switch` (`dec eax ; jne`).
class CTCBase;
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CRGBColour { public:
    unsigned char B, G, R, A;
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_3 = 3 };
class CTCGraphicAppearance { public:
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool c, bool d, EAppearanceMeshEffectPriority priority,
                          void* owner);       // 0x004C04C0
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CCard { public:
    CThing*       Thing;                        // +0x00 CThingObject*
    long          Name;                         // +0x04
    long          Value;                        // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                      // +0x24
    bool          FaceDown;                     // +0x25
};
struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
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
class CTavernGameCardBaseDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0x48];
    float         HighlightTime;                // +0x50
    unsigned char _pad_0x54[4];
    CRGBColour    HighlightColour;              // +0x58
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CCardDefPointer {                        // CDefPointer<const CTavernGameCardBaseDef>
    CTavernGameCardBaseDef* Object;
    CCardDefPointer() : Object(0) {}
    ~CCardDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0x1d];
    bool          GameWon;                      // +0x21
    unsigned char _pad_0x22[6];
    bool          GameFinished;                 // +0x28
    unsigned char _pad_0x29[0xff];
    CCardVector   Cards;                        // +0x128
    unsigned char _pad_0x134[0x24];
    bool          RoundFinished;                // +0x158
    unsigned char _pad_0x159[0x1a7];
    long          GameType;                     // +0x300
    unsigned char _pad_0x304[4];
    CCard*        SelectedCard;                 // +0x308
    CCard*        HighlightedCard;              // +0x30c
    bool SelectCard();                          // 0x008F09C0 (virtual slot 59, called non-virtually)
    void GetCardDef(CCardDefPointer* out);      // 0x008F0B20
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
};
struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    bool SelectCard();
};

bool CTCTavernGameCardSorting::SelectCard()
{
    CTCTavernGameCardBase::SelectCard();
    if (!SelectedCard)
        return true;
    bool mistake = false;
    CCardDefPointer def;
    GetCardDef(&def);
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    for (CCard** it = Cards._M_start; it != Cards._M_finish; ++it) {
        CCard* card = *it;
        if (!card->FaceDown && card->Value < SelectedCard->Value) {
            card->Thing->GetInterface(&appearance, 0x5b);
            appearance->SetAsHighlighted(def->HighlightTime, 0.0f, def->HighlightColour, false, false,
                                         NGraphicAppearance::APPEARANCE_MESH_EFFECT_PRIORITY_3, this);
            mistake = true;
        }
    }
    if (mistake) {
        switch (GameType) {
        case 1:
            GameWon = false;
            break;
        }
        RoundFinished = true;
        GameFinished = true;
        return false;
    }
    if (HighlightedCard && !HighlightedCard->FaceDown && !HighlightedCard->Turning) {
        CCharString sound("TAVERN_GAME_CORRECT", -1);
        PlaySound(sound);
    }
    return true;
}