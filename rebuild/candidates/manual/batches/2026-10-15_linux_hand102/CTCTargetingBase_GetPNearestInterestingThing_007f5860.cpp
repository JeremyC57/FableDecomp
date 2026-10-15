// CTCTargetingBase::GetPNearestInterestingThing() -- nothing when ignoring everything, else the script-override thing when set, else the nearest interesting thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5860, length 48 bytes.
// Callee: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50 (three call sites; the last two are tail jumps).
// Ego_r.pdb CTCTargetingBase: ScriptOverrideIgnoreEverything +0x0c, PNearestInterestingThing +0x0d,
// PScriptOverrideInterestingThing +0x15 (byte-packed intelligent pointers).
// Shape: the override pointer's address is formed once (`lea esi,[edi+15h]`) and reused for the second GetPItem; both
// pointer reads return straight through sibling-call jumps.
struct CThing;
struct CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* m_pItem;                    // +0x04
    CBaseObject* GetPItem() const;           // 0x00A01B50
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* Get() const { return (T*)GetPItem(); }
};

struct CTCTargetingBase {
    void*         __vftable;                 // +0x00
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[4];              // CTCBase flags
    bool          ScriptOverrideIgnoreEverything;                  // +0x0c
    CIntelligentPointer<CThing> PNearestInterestingThing;          // +0x0d
    CIntelligentPointer<CThing> PScriptOverrideInterestingThing;   // +0x15
    CThing* GetPNearestInterestingThing();
};
#pragma pack(pop)

CThing* CTCTargetingBase::GetPNearestInterestingThing()
{
    if (ScriptOverrideIgnoreEverything)
        return 0;
    if (PScriptOverrideInterestingThing.Get())
        return PScriptOverrideInterestingThing.Get();
    return PNearestInterestingThing.Get();
}
