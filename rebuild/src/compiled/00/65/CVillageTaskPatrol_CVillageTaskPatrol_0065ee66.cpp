// CVillageTaskPatrol::CVillageTaskPatrol(CThingTrackNode* patrolStart) -- village task base, patrol start pointer from the argument, empty creature pointer, task unlocked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065EE66, length 54 bytes. Pragma: optimize("s") (pass-through `push dword ptr [esp+0Ch]`).
// Leaf: ??0CVillageTaskPatrol
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Callees: CVillageTask::CVillageTask @ 0x0099A380, CIntelligentPointer<CThingTrackNode const>::CIntelligentPointer(CThingTrackNode*)
// @ 0x0065E0F8, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10. Vtables: CVillageTaskPatrol 0x0125A900, CIntelligentPointer 0x01237F1C.
// Shape: base call, derived vtable, PatrolStart's out-of-line converting constructor, CreatureAssignedToTask's inline default
// constructor (edi = &member, vtable stored after the base call), then `TaskLocked = false`. Member names from CVillageTaskPatrol.h.
#pragma optimize("s", on)
class CBaseObject;
class CThing;
class CThingTrackNode;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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