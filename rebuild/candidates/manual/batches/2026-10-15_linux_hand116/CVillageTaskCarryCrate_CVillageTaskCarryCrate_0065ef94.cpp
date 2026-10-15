// CVillageTaskCarryCrate::CVillageTaskCarryCrate(CThing* crateStack, bool pickup, long priority) -- village task base, empty creature pointer, crate-stack pointer set from the argument, pickup flag and priority
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EF94, length 64 bytes. Pragma: optimize("s") (pass-through `push dword ptr [esp+0Ch]`).
// Leaf: ??0CVillageTaskCarryCrate
// Callees: CVillageTask::CVillageTask @ 0x0099A380 (base; the manifest's CBaseIntelligentPointer label is a folded body),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0.
// Vtables: CVillageTaskCarryCrate 0x0125A950, CIntelligentPointer 0x01237F1C (CreatureAssignedToTask).
// Shape: base call, derived vtable, CreatureAssignedToTask's inline default constructor (base call through edi + vtable store
// after it), PCrateStack's out-of-line converting constructor, then Pickup / Priority. Member names from the engine header
// (CVillageTaskCarryCrate.h, same offsets); local polymorphic model because the header flattens the vptr.
#pragma optimize("s", on)
struct CBaseObject;
struct CThing;
struct CThingTrackNode;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {    // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerConstThing() {}
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointerThing(CThing* item);                         // 0x004A96F0
};
struct CVillageTask {
    virtual void v00();
    void* m_pVillage;                       // +0x04
    CVillageTask();                         // 0x0099A380
};
struct CVillageTaskCarryCrate : CVillageTask {
    CIntelligentPointerConstThing CreatureAssignedToTask;   // +0x08
    CIntelligentPointerThing      PCrateStack;              // +0x10
    bool                          Pickup;                   // +0x18
    long                          Priority;                 // +0x1c
    virtual void v00();
    CVillageTaskCarryCrate(CThing* crateStack, bool pickup, long priority);
};
CVillageTaskCarryCrate::CVillageTaskCarryCrate(CThing* crateStack, bool pickup, long priority)
    : PCrateStack(crateStack), Pickup(pickup), Priority(priority)
{
}
