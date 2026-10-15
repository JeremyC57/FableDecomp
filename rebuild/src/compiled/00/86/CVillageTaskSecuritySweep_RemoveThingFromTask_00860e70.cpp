// CVillageTaskSecuritySweep::RemoveThingFromTask(const CThing& thing, bool& stillValid) -- when the thing is the assigned creature, clear the pointer and report the task as no longer valid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860E70, length 88 bytes (+ int3 padding).
// Leaf: RemoveThingFromTask
// Callees: CBaseIntelligentPointer GetPItem @ 0x00A01B50, ctor @ 0x00A01B10, SetPItem @ 0x00A01B90, dtor @ 0x00A01C10.
// Shape: the member's address formed once in esi (`lea esi,[ecx+8]`) and reused for the compare and the SetPItem; the
// assignment is `member = CIntelligentPointer(0)` through a frame temporary (push 0, vtable 0x01237F1C), then `*flag = 0`.
// Member names from CVillageTaskSecuritySweep.h (CreatureAssignedToTask +0x08).
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CVillageTaskSecuritySweep {
    virtual void v00();
    void*               m_pVillage;                 // +0x04
    CIntelligentPointer CreatureAssignedToTask;     // +0x08
    virtual void RemoveThingFromTask(const CThing& thing, bool& stillValid);
};
void CVillageTaskSecuritySweep::RemoveThingFromTask(const CThing& thing, bool& stillValid)
{
    if (CreatureAssignedToTask.GetPItem() == &thing) {
        CreatureAssignedToTask = CIntelligentPointer(0);
        stillValid = false;
    }
}