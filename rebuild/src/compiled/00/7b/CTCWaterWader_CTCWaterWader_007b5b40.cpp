// CTCWaterWader::CTCWaterWader(CThing*) -- CTCBase(thing), own vtable, both particle pointers built null, not wading
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B5B40, length 81 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Vtables: CTCWaterWader 0x0126D95C, CIntelligentPointer<CThing> 0x01238C6C.
// Layout (Ego_r.pdb CTCWaterWader, 32 bytes): CTCBase 0xc, PParticle +0x0c, PWaterfallParticle +0x14
// (CIntelligentPointer<CThing>, 8 bytes each), OldWade +0x1c.
// Shape: the derived vtable is stored before the member constructors (VC installs the vptr first); each
// pointer is constructed from a null CThing* (base constructor, its vtable, SetPItem(0)); OldWade = false last.
struct CThing;
struct CBaseObject;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_CThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CTCWaterWader : CTCBase_Calls {                      // vtable 0x0126D95C
    CIntelligentPointer_CThing PParticle;                   // +0x0c
    CIntelligentPointer_CThing PWaterfallParticle;          // +0x14
    bool                       OldWade;                     // +0x1c
    virtual void v00();
    CTCWaterWader(CThing* thing);
};

CTCWaterWader::CTCWaterWader(CThing* thing)
    : CTCBase_Calls(thing)
    , PParticle(0)
    , PWaterfallParticle(0)
    , OldWade(false)
{
}