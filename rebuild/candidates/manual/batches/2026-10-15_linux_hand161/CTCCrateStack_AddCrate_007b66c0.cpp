// CTCCrateStack::AddCrate(CThing* crate) -- put the crate into the first free slot (FirstCrate, else SecondCrate); a full stack ignores it
//
// Retail 0x007B66C0, 144 bytes (`ret 4`). Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90 (the temporary,
// then the slot), ~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Layout = Ego_r.pdb CTCCrateStack (landed GetNextCrate 0x007B6750): FirstCrate +0xc, SecondCrate +0x14.
// Shape: `slot = CIntelligentPointer<CThing>(crate);` in each arm of an if / else-if (the landed
// CreateDressingSignPost 0x006C0F70 assignment); VC merges the two arms' tails (member SetPItem and
// the temporary's base destructor) behind `mov ecx,slot ; jmp`.
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
    void AddCrate(CThing* crate);
};

void CTCCrateStack::AddCrate(CThing* crate)
{
    if (!FirstCrate.GetPItem())
        FirstCrate = CIntelligentPointer<CThing>(crate);
    else if (!SecondCrate.GetPItem())
        SecondCrate = CIntelligentPointer<CThing>(crate);
}
