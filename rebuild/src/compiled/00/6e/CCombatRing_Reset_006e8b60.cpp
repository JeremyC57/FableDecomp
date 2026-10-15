// CCombatRing::Reset() -- zero the ring thickness and clear every prong's occupier pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8B60, 103 bytes (112 with padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, ::SetPItem 0x00A01B90,
//          ::GetPItem 0x00A01B50, ::~CBaseIntelligentPointer 0x00A01C10; CIntelligentPointer vtable 0x0123E7D8.
//
// Shape: PDB CCombatRing (RotationAngle +0, DeltaRotation +4, Index +8, RingThickness +0xc, Prongs +0x10:
// retail 12-byte vector of 28-byte CDefendingCombatantInfo::CProng, POccupier at +0x14). Each prong gets
// `POccupier = CIntelligentPointer(0)`: a frame temporary built inline (base constructor, vtable,
// SetPItem(0)), assigned through the inline operator= (`SetPItem(other.GetPItem())`) and destroyed
// through the base destructor, as the landed CThingGameObject::OnCreate 0x00838A40 and
// CTCShopKeeper::EmptyCustomerList 0x00798E10 (VC7.1 aligns that frame itself: `and esp,-8`). The end
// pointer is re-read every iteration (the calls may change it).
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    ~CBaseIntelligentPointer();            // 0x00A01C10
    CBaseObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct C3DVector { float X, Y, Z; };

struct CProng {                            // CDefendingCombatantInfo::CProng (28 bytes)
    bool Blocked;                          // +0x00
    C3DVector Prong;                       // +0x04
    long ProngBlockageTimer;               // +0x10
    CIntelligentPointer POccupier;         // +0x14
};

struct CCombatRing {
    float RotationAngle;                   // +0x00
    float DeltaRotation;                   // +0x04
    long Index;                            // +0x08
    float RingThickness;                   // +0x0c
    CProng* Prongs_first;                  // +0x10 (retail 12-byte vector)
    CProng* Prongs_last;                   // +0x14
    CProng* Prongs_end;                    // +0x18
    void Reset();
};

void CCombatRing::Reset()
{
    RingThickness = 0.0f;
    for (CProng* prong = Prongs_first; prong != Prongs_last; ++prong)
        prong->POccupier = CIntelligentPointer(0);
}