// CAIStateGroupWithBuilding_0091c080::Cleanup() -- clear two flags, then drop the building pointer at +0x68.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C080, length 74 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem
// @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte frame temporary (vtable 0x0123E7D8).
//
// Shape: the two byte flags at +0x58 / +0x59 are cleared first (scheduled between the temporary's address
// formation and its constructor call), then `PBuilding = 0` goes through a converting CIntelligentPointer
// temporary (base constructor, vtable store, SetPItem(0)), the inline `SetPItem(other.GetPItem())` assignment
// and the temporary's destructor. The manifest label "CAIStateGroup_MinionPutOutFires::Cleanup" is
// propagated (that class is 28 bytes, pointer at +0x14); the owner is modelled locally and named descriptively.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x0123E7D8
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroupWithBuilding_0091c080 {
    unsigned char _pad[0x58];                   // vtable, CAIStateGroupBase members, earlier state
    bool m_flag58;                              // +0x58
    bool m_flag59;                              // +0x59
    unsigned char _pad5a[0xe];
    CIntelligentPointer PBuilding;              // +0x68
    void Cleanup();
};

void CAIStateGroupWithBuilding_0091c080::Cleanup()
{
    m_flag58 = false;
    m_flag59 = false;
    PBuilding = 0;
}
