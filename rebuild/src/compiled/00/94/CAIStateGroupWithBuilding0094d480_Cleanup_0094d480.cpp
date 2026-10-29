// CAIStateGroupWithBuilding_0094d480::Cleanup() -- drop the building/thing pointer at +0xd0 (`PBuilding = 0`).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094D480, length 69 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem
// @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte frame temporary (vtable 0x0123E7E0).
//
// Shape: `PBuilding = 0` goes through a converting CIntelligentPointer temporary (base constructor, vtable
// store, SetPItem(0)) and the inline assignment `SetPItem(other.GetPItem())`, then the temporary's destructor.
// The manifest label "CAIStateGroup_MinionPutOutFires::Cleanup" is propagated from a byte-identical body:
// CAIStateGroup_MinionPutOutFires is 28 bytes (PDB) and keeps PBuildingOnFire at +0x14, while this body
// assigns the pointer at +0xd0, so the owner is a larger state group modelled locally and named descriptively.
class CThing;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x0123E7E0
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

struct CAIStateGroupWithBuilding_0094d480 {
    unsigned char _pad[0xd0];                  // vtable, CAIStateGroupBase members, earlier state
    CIntelligentPointer PBuilding;              // +0xd0
    void Cleanup();
};

void CAIStateGroupWithBuilding_0094d480::Cleanup()
{
    PBuilding = 0;
}