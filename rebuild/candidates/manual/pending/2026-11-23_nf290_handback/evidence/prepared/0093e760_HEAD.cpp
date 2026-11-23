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
class CThing;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CBrainOutput { public:
    unsigned long       Types;                  // +0x00
    long                LookFlags;              // +0x04
    unsigned char       _pad[0x38];             // +0x08..+0x3f
    CIntelligentPointer<const CThing> LookAtThing;            // +0x40
    void SetAsLookAtThing(const CThing* thing, long lookFlags);
};

void CBrainOutput::SetAsLookAtThing(const CThing* thing, long lookFlags)
{
    Types |= 4;
    {
        CIntelligentPointer<const CThing> pointer;
        pointer.SetPItem(thing);
        LookAtThing.SetPItem(pointer.GetPItem());
    }
    LookFlags = lookFlags;
}