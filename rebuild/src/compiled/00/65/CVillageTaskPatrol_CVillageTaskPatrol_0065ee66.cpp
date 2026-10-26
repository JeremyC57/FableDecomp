// CVillageTaskPatrol::CVillageTaskPatrol(CThingTrackNode* patrolStart) -- village task base, patrol start pointer from the argument, empty creature pointer, task unlocked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EE66, length 54 bytes. Pragma: optimize("s") (pass-through `push dword ptr [esp+0Ch]`).
// Leaf: ??0CVillageTaskPatrol
// Callees: CVillageTask::CVillageTask @ 0x0099A380, CIntelligentPointer<CThingTrackNode const>::CIntelligentPointer(CThingTrackNode*)
// @ 0x0065E0F8, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10. Vtables: CVillageTaskPatrol 0x0125A900, CIntelligentPointer 0x01237F1C.
// Shape: base call, derived vtable, PatrolStart's out-of-line converting constructor, CreatureAssignedToTask's inline default
// constructor (edi = &member, vtable stored after the base call), then `TaskLocked = false`. Member names from CVillageTaskPatrol.h.
#pragma optimize("s", on)
class CBaseObject;
class CThing;
class CThingTrackNode;

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
struct CIntelligentPointerTrackNode : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointerTrackNode(CThingTrackNode* item);            // 0x0065E0F8
};
struct CVillageTask {
    virtual void v00();
    void* m_pVillage;                       // +0x04
    CVillageTask();                         // 0x0099A380
};
class CVillageTaskPatrol : public CVillageTask { public:
    CIntelligentPointerTrackNode  PatrolStart;              // +0x08
    CIntelligentPointerConstThing CreatureAssignedToTask;   // +0x10
    bool                          TaskLocked;               // +0x18
    virtual void v00();
    CVillageTaskPatrol(CThingTrackNode* patrolStart);
};
CVillageTaskPatrol::CVillageTaskPatrol(CThingTrackNode* patrolStart)
    : PatrolStart(patrolStart), TaskLocked(false)
{
}