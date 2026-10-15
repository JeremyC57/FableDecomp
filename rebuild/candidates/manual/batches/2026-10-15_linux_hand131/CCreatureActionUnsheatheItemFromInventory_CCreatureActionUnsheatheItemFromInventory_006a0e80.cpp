// CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory&) -- copy constructor: base copy, own vtable, PUnsheatheItem intelligent pointer re-pointed through GetPItem/SetPItem, PFollowUpAction counted pointer copied with an add-ref
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A0E80, length 97 bytes.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0, CBaseIntelligentPointer::
// CBaseIntelligentPointer() @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem
// @ 0x00A01B90. Vtables: CCreatureAction_UnsheatheItemFromInventory 0x0125CCBC, CIntelligentPointer<CThing> 0x01238C6C.
// Shape: the landed UseLamp copy constructor (0x0084C070) plus the inline CCountedPointer<CCreatureActionBase> copy
// constructor (object word, then the count word loaded, tested and stored, `inc dword ptr [edi]`).
// Layout (Ego_r.pdb, retail offsets 0xc lower): PUnsheatheItem +0xa8, PFollowUpAction +0xb0/+0xb4.
struct CBaseObject;
struct CCreatureActionBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCountedPointer {                    // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* Object;            // +0
    long* RefCount;                         // +4
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), RefCount(other.RefCount)
    {
        if (RefCount)
            ++*RefCount;
    }
};

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    CIntelligentPointer PUnsheatheItem;     // +0xa8
    CCountedPointer PFollowUpAction;        // +0xb0
    virtual void v00();
    CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& other);
};

CCreatureAction_UnsheatheItemFromInventory::CCreatureAction_UnsheatheItemFromInventory(const CCreatureAction_UnsheatheItemFromInventory& other)
    : CCreatureActionBase(other)
    , PUnsheatheItem(other.PUnsheatheItem)
    , PFollowUpAction(other.PFollowUpAction)
{
}
