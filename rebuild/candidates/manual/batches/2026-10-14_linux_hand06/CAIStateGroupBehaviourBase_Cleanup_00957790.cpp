// CAIStateGroupBehaviourBase::Cleanup() -- drop the target pointer, then the leaf cleanup hook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957790, length 73 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90,
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 (all on the 8-byte frame
// temporary), then the virtual at slot 17 (+0x44), CleanupLeaf.
// `PTargetThing = 0` goes through a converting CIntelligentPointer temporary (base constructor,
// vtable 0x01237F1C, SetPItem(0)) and the inline assignment `SetPItem(other.GetPItem())`
// (the same two-call shape as the landed copy constructors); the temporary is destroyed before
// the virtual call. PTargetThing sits at +0x14 as in the engine header.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01237F1C
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                 // slot 9 (+0x24)
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual void CleanupLeaf();             // slot 17 (+0x44)
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroupBehaviourBase : CAIStateGroupBase {
    CIntelligentPointer PTargetThing;       // +0x14 CIntelligentPointer<const CThing>
    virtual void Cleanup();
};

void CAIStateGroupBehaviourBase::Cleanup()
{
    PTargetThing = 0;
    CleanupLeaf();
}
