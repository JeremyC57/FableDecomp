// CTCDiscovery::CTCDiscovery(CThing* parent) -- CTCBase(parent), own vtable, Discovered = false, default-constructed Discoverer pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007509A0, length 47 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Vtable 0x012695F4 = CTCDiscovery, 0x01237F1C = CIntelligentPointer (rtti-vtables.tsv).
// PDB CTCDiscovery (0x18 bytes): Discovered +0x0c, Discoverer (CIntelligentPointer<const CThing>) +0x0d --
// the member really is unaligned (`lea edi,[esi+0Dh]`), so the class is modelled under pack(1).
// Shape: speed TU; the bool initialiser is scheduled right after the derived vtable store and before the
// member's base constructor call; the default CIntelligentPointer constructor is the base call plus
// its own vtable store (no SetPItem, unlike the CTCDeadShotInThingTracker member at 0x007E3450).
class CThing;

struct CTCBase_Calls {
    virtual void v00();
    unsigned char _body[0x8];                               // ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9
    CTCBase_Calls(CThing* parent);                          // 0x00686800
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // sizeof 8: vptr + PData
    virtual void v00();
    void* PData;                                            // +0x04 CBaseObjectPointer*
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() { }
};

class CTCDiscovery : public CTCBase_Calls { public:
    bool                Discovered;                         // +0x0c
    CIntelligentPointer Discoverer;                         // +0x0d CIntelligentPointer<const CThing>
    virtual void v00();                                     // own vtable 0x012695F4
    CTCDiscovery(CThing* parent);
};
#pragma pack(pop)

CTCDiscovery::CTCDiscovery(CThing* parent)
    : CTCBase_Calls(parent)
    , Discovered(false)
    , Discoverer()
{
}