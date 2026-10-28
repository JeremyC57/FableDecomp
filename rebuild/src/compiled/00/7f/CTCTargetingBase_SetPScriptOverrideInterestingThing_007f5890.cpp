// CTCTargetingBase::SetPScriptOverrideInterestingThing(CThing* thing) -- unless it is the parent itself, wraps the thing in a temporary intelligent pointer and stores its item in PScriptOverrideInterestingThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5890, length 80 bytes (ret 4).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::SetPItem(CBaseObject*)
// @ 0x00A01B90 (twice), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer (the temporary's own vtable, stored after the base constructor).
// Ego_r.pdb CTCTargetingBase: ParentThing +0x04 (CTCBase), PScriptOverrideInterestingThing +0x15 (byte-packed).
// Shape: the landed SetTargetedThing 0x004DDAB4 pattern (`CIntelligentPointer<const CThing> pointer(thing);
// member.SetPItem(pointer.GetPItem());`) but in a speed TU, so the pointer's converting constructor is INLINE here
// (base constructor, vtable store, SetPItem) in the `sub esp,8` frame; it has no destructor of its own, so the base
// destructor is called directly. The early-out compares the parent with the argument (`cmp eax,edi ; je`).
class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    CBaseIntelligentPointer();               // 0x00A01B10
    virtual ~CBaseIntelligentPointer();              // 0x00A01C10
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* thing) { SetPItem((CBaseObject*)thing); }
};

class CTCTargetingBase { public:
    void*         __vftable;                 // +0x00
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[4];              // CTCBase flags
    bool          ScriptOverrideIgnoreEverything;            // +0x0c
    CIntelligentPointer PNearestInterestingThing;            // +0x0d
    CIntelligentPointer PScriptOverrideInterestingThing;     // +0x15
    void SetPScriptOverrideInterestingThing(CThing* thing);
};
#pragma pack(pop)

void CTCTargetingBase::SetPScriptOverrideInterestingThing(CThing* thing)
{
    if (ParentThing != thing) {
        CIntelligentPointer pointer(thing);
        PScriptOverrideInterestingThing.SetPItem(pointer.GetPItem());
    }
}