// CBrainOutput::SetAsLookAtThing(const CThing* thing, long lookFlags) -- add the look-at type, point LookAtThing at the thing through a temporary intelligent pointer, record the flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093E760, length 85 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90
// (x2), ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte
// temporary in the frame (`sub esp,8`) and on LookAtThing (+0x40).
// Layout from Ego_r.pdb: Types +0 (bit 2 = look-at-thing), LookFlags +4, LookAtThing +0x40.
// Shape: `Types |= 4` is read/or/write; the temporary CIntelligentPointer is constructed (base
// call, then the derived vtable 0x01237F1C stored inline), set to the thing, and its item is
// copied into the member with SetPItem(GetPItem()); the temporary's destructor is the base
// destructor out of line (the derived class adds no store of its own) and runs BEFORE the
// LookFlags store: the temporary lives in a nested scope (a function-scope local would be
// destroyed after the store).
struct CThing;

struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    const CThing* GetPItem();                   // 0x00A01B50
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

struct CBrainOutput {
    unsigned long       Types;                  // +0x00
    long                LookFlags;              // +0x04
    unsigned char       _pad[0x38];             // +0x08..+0x3f
    CIntelligentPointer LookAtThing;            // +0x40
    void SetAsLookAtThing(const CThing* thing, long lookFlags);
};

void CBrainOutput::SetAsLookAtThing(const CThing* thing, long lookFlags)
{
    Types |= 4;
    {
        CIntelligentPointer pointer;
        pointer.SetPItem(thing);
        LookAtThing.SetPItem(pointer.GetPItem());
    }
    LookFlags = lookFlags;
}
