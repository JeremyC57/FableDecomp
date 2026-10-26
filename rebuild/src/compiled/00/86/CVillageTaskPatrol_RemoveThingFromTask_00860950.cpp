// CVillageTaskPatrol::RemoveThingFromTask(const CThing& thing, bool& removeTask) -- unlocks the task; if the thing is the patrol's start node the start is cleared and the whole task must go (removeTask = true), otherwise the assigned creature is cleared (removeTask = false)
// Leaf: RemoveThingFromTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860950, 160 bytes (padded to 176), `ret 8`.
//   PDB name ?RemoveThingFromTask@CVillageTaskPatrol@@UAEXABVCThing@@AA_N@Z.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90, dtor 0x00A01C10.
//   Vtables: CIntelligentPointer<const CThingTrackNode> 0x0125A894, CIntelligentPointer<const CThing> 0x01237F1C.
// Shape: `TaskLocked = false` first; `member = CIntelligentPointer<T>(0)` builds the null temporary in the frame
//   (base ctor, derived vtable, SetPItem(0)), the inline operator= forwards GetPItem() to the member's SetPItem and the
//   temporary's compiler-generated destructor runs the base destructor (landed AssignCreatureToTask 0x00860A00 model).
//   The start pointer's address (edi) is reused as `this` of its SetPItem.
// Layout (engine/CVillageTaskPatrol.h): PatrolStart +0x08, CreatureAssignedToTask +0x10, TaskLocked +0x18.
class CBaseObject { public: };
class CThing : public CBaseObject { public: };
class CThingTrackNode : public CThing { public: };
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    CBaseObject* GetPItem() const;                 // 0x00A01B50
    void SetPItem(CBaseObject* item);              // 0x00A01B90
    ~CBaseIntelligentPointer();                    // 0x00A01C10
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CVillageTaskPatrol { public:
    virtual void v00();
    void*                                          m_pVillage;              // +0x04
    CIntelligentPointer<const CThingTrackNode>     PatrolStart;             // +0x08
    CIntelligentPointer<const CThing>              CreatureAssignedToTask;  // +0x10
    bool                                           TaskLocked;              // +0x18
    virtual void RemoveThingFromTask(const CThing& thing, bool& removeTask);
};
void CVillageTaskPatrol::RemoveThingFromTask(const CThing& thing, bool& removeTask)
{
    TaskLocked = false;
    if (PatrolStart.GetPItem() == &thing) {
        PatrolStart = CIntelligentPointer<const CThingTrackNode>(0);
        removeTask = true;
    }
    else {
        CreatureAssignedToTask = CIntelligentPointer<const CThing>(0);
        removeTask = false;
    }
}