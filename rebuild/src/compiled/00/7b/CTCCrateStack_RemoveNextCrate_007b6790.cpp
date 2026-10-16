// CTCCrateStack::RemoveNextCrate() -- take the top crate off the stack (SecondCrate, else FirstCrate): clear its slot and return the crate, or null when empty
//
// Retail 0x007B6790, 119 bytes (plain `ret`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90 (the null
// temporary, then the slot), ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C.
// Layout = Ego_r.pdb CTCCrateStack (landed GetNextCrate 0x007B6750): FirstCrate +0xc, SecondCrate +0x14.
// Shape: a zero-initialised result local assigned in the hit block and returned once at the end (VC
// keeps the zero in ebx and the hit value in edi, with two epilogues; an early `return 0` reuses the
// tested eax instead); the slot is a pointer local (esi) chosen by `slot->GetPItem() || (slot =
// &FirstCrate)->GetPItem()`, then reset with `*slot = CIntelligentPointer<CThing>(0);`.
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    CBaseObject* m_pItem;                           // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                  // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer   // vtable 0x01238C6C for <CThing>
{
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCCrateStack {
    void*                       __vftable;          // +0x00
    unsigned char               _pad04[8];          // +0x04 CTCBase
    CIntelligentPointer<CThing> FirstCrate;         // +0x0c
    CIntelligentPointer<CThing> SecondCrate;        // +0x14
    long                        CrateDefIndex;      // +0x1c
    CThing* RemoveNextCrate();
};

CThing* CTCCrateStack::RemoveNextCrate()
{
    CThing* crate = 0;
    CIntelligentPointer<CThing>* slot = &SecondCrate;
    if (slot->GetPItem() || (slot = &FirstCrate)->GetPItem())
    {
        crate = (CThing*)slot->GetPItem();
        *slot = CIntelligentPointer<CThing>(0);
    }
    return crate;
}