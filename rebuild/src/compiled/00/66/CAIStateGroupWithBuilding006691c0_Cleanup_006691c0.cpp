// CAIStateGroupWithBuilding_006691c0::Cleanup() -- drop the building/thing pointer at +0xf4 (`PBuilding = 0`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006691C0, length 69 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem
// @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte frame temporary (vtable 0x0123E7D8).
//
// Shape: `PBuilding = 0` goes through a converting CIntelligentPointer temporary (base constructor, vtable
// store, SetPItem(0)) and the inline assignment `SetPItem(other.GetPItem())`, then the temporary's destructor.
// The manifest label "CAIStateGroup_MinionPutOutFires::Cleanup" is propagated from a byte-identical body:
// CAIStateGroup_MinionPutOutFires is 28 bytes (PDB) and keeps PBuildingOnFire at +0x14, while this body
// assigns the pointer at +0xf4, so the owner is a larger state group modelled locally and named descriptively.
class CThing;

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

struct CAIStateGroupWithBuilding_006691c0 {
    unsigned char _pad[0xf4];                  // vtable, CAIStateGroupBase members, earlier state
    CIntelligentPointer PBuilding;              // +0xf4
    void Cleanup();
};

void CAIStateGroupWithBuilding_006691c0::Cleanup()
{
    PBuilding = 0;
}