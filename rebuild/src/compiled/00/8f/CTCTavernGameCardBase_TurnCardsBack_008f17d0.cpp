// CTCTavernGameCardBase::TurnCardsBack() -- clears every card's highlight and flips each card that is face up and not already turning back over (with the flip sound), then goes to state 0x19
// Leaf: ?TurnCardsBack@CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F17D0, 225 bytes (plain `ret`); vtable slot 55 (CTCTavernGameCardSorting
// 0x0126F5B4 inherits it). Ego_r.pdb `?TurnCardsBack@CTCTavernGameCardBase@@UAEXXZ`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetTC, interface 0x5B);
//          NGraphicAppearance::CTCGraphicAppearance::ClearMeshEffect(void* key) 0x004BFFC0 (landed spelling; key = this);
//          CCharString(const char*, long) 0x0099EBF0 ("TAVERN_GAME_CARD_FLIP" @ 0x0126F6C4); CTCTavernGame::PlaySound(const
//          CCharString&) 0x005EA950; ~CCharString 0x0099EAE0; CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20.
// Layout (retail, as the landed constructor 0x008F1CD0): Cards +0x128/+0x12c; CCard (Ego_r.pdb): Thing +0x00,
//         InitialRotation +0x0c, FinalRotation +0x10, TurnFraction +0x18, Turning +0x24, FaceDown +0x25.
// Shape: a pointer walk re-reading Cards.end() each pass; `T* p = 0; if (HasTC(k)) p = GetTC(k);` with GetTC's one-ternary
//        entry choice (component straight into ecx); the flip test compares both bools against the hoisted ebx zero;
//        the sound name is a named CCharString in its own block (its own frame word beside the lookup key); CCard::Turn
//        is the in-class inline member (out-of-line copy 0x008F0620) reading the old FaceDown once into cl.
struct CTCBase;
struct CCard;
struct CThingObject;

struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    CCard** begin() { return _M_start; }
    CCard** end() { return _M_finish; }
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    void ClearMeshEffect(void* key);            // 0x004BFFC0
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CCard {                                  // Ego_r.pdb CCard (0x58 bytes)
    CThing* Thing;                              // +0x00 CThingObject*
    long    Name;                               // +0x04
    long    Value;                              // +0x08
    float   InitialRotation;                    // +0x0c
    float   FinalRotation;                      // +0x10
    float   TurnSpeed;                          // +0x14
    float   TurnFraction;                       // +0x18
    float   MoveSpeed;                          // +0x1c
    float   MoveFraction;                       // +0x20
    bool    Turning;                            // +0x24
    bool    FaceDown;                           // +0x25
    void Turn()                                 // out-of-line copy 0x008F0620 (landed)
    {
        bool faceDown = FaceDown;
        Turning = true;
        TurnFraction = 0.0f;
        if (faceDown == true) {
            InitialRotation = 0.5f;
            FinalRotation = 0.0f;
        } else {
            InitialRotation = 0.0f;
            FinalRotation = 0.5f;
        }
        FaceDown = !faceDown;
    }
};

struct CTCTavernGame {
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950
    void SetState(long state);                  // 0x005E9B20
};

struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x38];
    CCardVector   Cards;                        // +0x128
    virtual void TurnCardsBack();
};

void CTCTavernGameCardBase::TurnCardsBack()
{
    for (CCard** it = Cards.begin(); it != Cards.end(); ++it) {
        CThing* thing = (*it)->Thing;
        NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
        if (thing->HasTC(0x5b))
            appearance = (NGraphicAppearance::CTCGraphicAppearance*)thing->GetTC(0x5b);
        appearance->ClearMeshEffect(this);
        if (!(*it)->Turning && !(*it)->FaceDown) {
            {
                CCharString sound("TAVERN_GAME_CARD_FLIP");
                PlaySound(sound);
            }
            (*it)->Turn();
        }
    }
    SetState(0x19);
}