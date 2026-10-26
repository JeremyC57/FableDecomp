// CTCBuyableHouse::GetMaxCollectableRentValue() const -- the most rent the house can hold: (Rent[level] + trophy value * 0.1) as a whole number, times the def's MaxRentBags
//
// Retail 0x006C0270, 73 bytes. Callees: CTCBuyableHouse::GetValueOfTrophies() const @ 0x006BFFD0,
// __ftol2 @ 0x00BFEA70, the def's virtual slot 1 (`call [edx+4]`, destroy) when the scoped
// reference drops the last count. Constant 0x0122EE58 = 0.1f.
// PDef (+0x0c) is the retail CDefPointer<const CBuyableHouseDef>: the pointee is intrusively counted
// (count at +4 behind its vtable) and `PDef->` hands out a scoped reference that AddRefs (null
// tested) and Releases (not null tested) around the member read -- the landed 0x005BD1CF CDefRef
// shape. CBuyableHouseDef in retail: Rent vector<long> {first, last, end} at +0x34, MaxRentBags +0x44
// (Ego_r.pdb +0x38 / +0x4c with 16-byte vectors). CurrentDressLevel is +0x1c.
// Shape: the Rent first pointer is read through the raw def pointer into a named local BEFORE the
// trophy call (edi) while the level is re-read after it; the trophy value is converted through the
// `push ecx` frame slot (`fild`), `* 0.1f`, then `fiadd` of the rent; one __ftol2; the def is then
// reached again through `PDef->MaxRentBags` (AddRef/Release pair) and multiplied (`imul esi,eax`).

class CBuyableHouseDef
{ public:
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long          m_refCount;                       // +0x04
    unsigned char _pad_0x08[0x2c];                  // +0x08
    long*         Rent_First;                       // +0x34 vector<long> Rent {first, last, end}
    long*         Rent_Last;                        // +0x38
    long*         Rent_End;                         // +0x3c
    long          DaysBetweenRentPayments;          // +0x40
    long          MaxRentBags;                      // +0x44

    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};

struct CDefRef                                      // scoped reference returned by CDefPointer::operator->
{
    CBuyableHouseDef* m_p;
    CDefRef(CBuyableHouseDef* p) : m_p(p) { if (m_p) m_p->AddRef(); }
    ~CDefRef() { m_p->Release(); }
    CBuyableHouseDef* operator->() const { return m_p; }
};

struct CDefPointer
{
    CBuyableHouseDef* Object;
    CDefRef operator->() const { return CDefRef(Object); }
};

#pragma pack(push, 1)
class CTCBuyableHouse
{ public:
    void*         __vftable;                        // +0x00 (CTCBase)
    void*         ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x4];                   // +0x08 (CTCBase flags)
    CDefPointer   PDef;                             // +0x0c CDefPointer<const CBuyableHouseDef>
    bool          IsScripted;                       // +0x10
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    unsigned char _pad_0x13[0x1];                   // +0x13
    long          DaySomeoneWillMoveIn;             // +0x14
    long          DayNextRentIsDue;                 // +0x18
    long          CurrentDressLevel;                // +0x1c

    long GetValueOfTrophies() const;                // 0x006BFFD0
    long GetMaxCollectableRentValue() const;
};
#pragma pack(pop)

long CTCBuyableHouse::GetMaxCollectableRentValue() const
{
    const long* rents = PDef.Object->Rent_First;
    long rent = (long)(GetValueOfTrophies() * 0.1f + rents[CurrentDressLevel]);
    return PDef->MaxRentBags * rent;
}