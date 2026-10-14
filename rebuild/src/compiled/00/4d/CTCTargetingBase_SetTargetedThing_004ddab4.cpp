#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCTargetingBase::SetTargetedThing(CThing*) -- wraps the thing in a temporary CIntelligentPointer<const CThing>,
// copies its item into PTargetedThing through SetPItem, then destroys the temporary.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDAB4, length 49 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp ;
// push ecx ; push ecx` for the 8-byte temporary, `leave`): `optimize("y",off)` + `("s",on)`.
// Callees: CIntelligentPointer<const CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50, CBaseIntelligentPointer::SetPItem(CBaseObject*) @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10.
// PDB CTCTargetingBase (sizeof 0x38): +0x25 CIntelligentPointer<CThing> PTargetedThing (the odd offset follows the
// bool at +0x24 of the packed PDB layout and the retail body addresses `this+25h`). The body sits among the
// CTCTargetingAI methods (vtable 0x012411FC slot 34); the manifest label "CCategory::SetCategoryThingAssociation" is
// propagated and rejected.
struct CThing;
struct CBaseObject;

struct CBaseIntelligentPointer {
    void* m_pItem;                               // +0x00
    void* m_pInfo;                               // +0x04
    CBaseObject* GetPItem();                     // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();                  // 0x00A01C10
};

struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointer_CThing(CThing* thing);   // 0x004A96F0
};

#pragma pack(push, 1)
struct CTCTargetingBase_Retail {
    unsigned char           _pad_0x00[0x25];     // vtable, CThingComponent members, bools, the other pointers
    CBaseIntelligentPointer PTargetedThing;      // +0x25
    void SetTargetedThing(CThing* thing);
};
#pragma pack(pop)

void CTCTargetingBase_Retail::SetTargetedThing(CThing* thing)
{
    CIntelligentPointer_CThing pointer(thing);
    PTargetedThing.SetPItem(pointer.GetPItem());
}