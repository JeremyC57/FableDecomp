// CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator(CThing*) -- CTCBase(owner), own vtable, cleared state, empty emitter pointer
// Leaf: ??0CTCAICreatureWillPowerIndicator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0E30, length 58 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is a
// propagated name), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, then the
// CIntelligentPointer vtable 0x01238C6C. Own vtable 0x0126FF2C (rtti-vtables.tsv); allocated with 0x24
// bytes by the landed factory 0x004D5E14.
//
// Layout = Ego_r.pdb CTCAICreatureWillPowerIndicator (0x24 bytes, byte-packed): CTCBase 0xc, PoweredUp
// +0x0c, Timer +0x10, ResetCount +0x14, ShowEffect +0x18 and the CIntelligentPointer<CThing> PEmitter at
// the UNALIGNED +0x19 -- the class is compiled under pack(1) (`lea edi,[esi+19h]`).
// Shape: base constructor, own vtable, the four init-list scalars from one zero register (ShowEffect =
// true), then the member pointer's base constructor and its derived vtable through edi. engine/CTCBase.h
// holds the layout only, so the base is modelled as `CTCBase_Calls` (as the landed CTCRockTrollShield).
class CThing;

#pragma pack(push, 1)
struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    unsigned char _pad_0x0a[2];
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCAICreatureWillPowerIndicator : public CTCBase_Calls { public:    // vtable 0x0126FF2C
    bool                PoweredUp;                          // +0x0c
    unsigned char       _pad_0x0d[3];
    long                Timer;                              // +0x10
    long                ResetCount;                         // +0x14
    bool                ShowEffect;                         // +0x18
    CIntelligentPointer PEmitter;                           // +0x19
    virtual void v00();
    CTCAICreatureWillPowerIndicator(CThing* owner);
};
#pragma pack(pop)

CTCAICreatureWillPowerIndicator::CTCAICreatureWillPowerIndicator(CThing* owner)
    : CTCBase_Calls(owner)
    , PoweredUp(false)
    , Timer(0)
    , ResetCount(0)
    , ShowEffect(true)
    , PEmitter()
{
}