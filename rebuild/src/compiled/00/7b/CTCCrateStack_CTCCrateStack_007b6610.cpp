// CTCCrateStack::CTCCrateStack(CThing* thing) -- CTCBase(thing), own vtable, the FirstCrate and SecondCrate intelligent pointers constructed, CrateDefIndex = 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6610, length 66 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (x2). Vtables: 0x0126DA14 = CTCCrateStack, 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Identity: the manifest label "CTCAvoidRegionExit::CTCAvoidRegionExit" is propagated; the vtable names the class.
// Layout = Ego_r.pdb CTCCrateStack (32 bytes): FirstCrate +0xc, SecondCrate +0x14 (CIntelligentPointer<CThing>),
// CrateDefIndex +0x1c. Shape: the vtable store precedes the member constructors (each an inline
// CIntelligentPointer() over the out-of-line base, its vtable stored through the member's address
// register), and the scalar initialiser is stored last.
class CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCCrateStack : public CTCBase_Calls { public:                      // vtable 0x0126DA14
    CIntelligentPointer FirstCrate;                         // +0x0c
    CIntelligentPointer SecondCrate;                        // +0x14
    long                CrateDefIndex;                      // +0x1c
    virtual void v00();
    CTCCrateStack(CThing* thing);
};

CTCCrateStack::CTCCrateStack(CThing* thing)
    : CTCBase_Calls(thing)
    , CrateDefIndex(0)
{
}