// CTCBuyableHouse::CTCBuyableHouse(CThing* owner) -- CTCBase(owner), own vtable, empty def pointer, every flag and counter cleared (rent due day = LONG_MAX), the three sign/wife intelligent pointers default-built, IsResidential = true
// Leaf: ??0CTCBuyableHouse
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF8A0, length 120 bytes (+8 int3 padding). `ret 4`. Vtable 0x0125D6A4.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (x3, each
// followed by the CIntelligentPointer vtable store 0x01238C6C: the default ctor, no SetPItem).
// Shape: one init list in declaration order with a shared zero register (ebx); DaySomeoneWillMoveIn is left alone.
// Layout: engine/CTCBuyableHouse.h (Ego_r.pdb, retail-identical): PDef +0xC, IsScripted +0x10, OwnedByPlayer +0x11, Rented +0x12,
// DaySomeoneWillMoveIn +0x14, DayNextRentIsDue +0x18, CurrentDressLevel +0x1C, MaxDressLevel +0x20, MainSignPost +0x24,
// DressingSignPost +0x2C, FrameToNextCheckIfWeNeedToCreateAnything +0x34, WifeLivingHere +0x38, InitiallySetupDressLevel +0x40,
// VirtualMoneyBags +0x44, HasPreviouslyBeenBuyable +0x48, IsResidential +0x49. The header flattens the vptrs and the pointer
// members, so the class is restated polymorphically with the same names and offsets.
class CThing;
class CBuyableHouseDef;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {     // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() {}

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
};
class CTCBase {
public:
    CTCBase(CThing* owner);                     // 0x00686800
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBuyableHouse : public CTCBase {        // vtable 0x0125D6A4
public:
    CDefPointer<const CBuyableHouseDef> PDef;   // +0x0c
    bool IsScripted;                            // +0x10
    bool OwnedByPlayer;                         // +0x11
    bool Rented;                                // +0x12
    long DaySomeoneWillMoveIn;                  // +0x14
    long DayNextRentIsDue;                      // +0x18
    long CurrentDressLevel;                     // +0x1c
    long MaxDressLevel;                         // +0x20
    CIntelligentPointer<CThing> MainSignPost;           // +0x24
    CIntelligentPointer<CThing> DressingSignPost;       // +0x2c
    long FrameToNextCheckIfWeNeedToCreateAnything;  // +0x34
    CIntelligentPointer<CThing> WifeLivingHere;         // +0x38
    bool InitiallySetupDressLevel;              // +0x40
    long VirtualMoneyBags;                      // +0x44
    bool HasPreviouslyBeenBuyable;              // +0x48
    bool IsResidential;                         // +0x49
    CTCBuyableHouse(CThing* owner);
    virtual ~CTCBuyableHouse();
};

CTCBuyableHouse::CTCBuyableHouse(CThing* owner)
    : CTCBase(owner)
    , IsScripted(false)
    , OwnedByPlayer(false)
    , Rented(false)
    , DayNextRentIsDue(0x7fffffff)
    , CurrentDressLevel(0)
    , MaxDressLevel(0)
    , FrameToNextCheckIfWeNeedToCreateAnything(0)
    , InitiallySetupDressLevel(false)
    , VirtualMoneyBags(0)
    , HasPreviouslyBeenBuyable(false)
    , IsResidential(true)
{
}