// CVillageTaskPatrol::AssignCreatureToTask(const CThing& creature) -- unlock the task and point CreatureAssignedToTask at the creature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00860A00, length 75 bytes (+ int3 padding).
// Leaf: AssignCreatureToTask
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10.
// Shape: `TaskLocked = false` first (stored before the temporary is built), then `member = CIntelligentPointer(&creature)`:
// the temporary is built in the 8-byte frame (base ctor, then the argument push, the CIntelligentPointer vtable 0x01237F1C,
// SetPItem), the inline operator= forwards GetPItem() to the member's SetPItem, and the temporary's base destructor runs
// (the compiler-generated derived destructor stores no vtable). Member names from CVillageTaskPatrol.h.
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CVillageTaskPatrol { public:
    virtual void v00();
    void*               m_pVillage;                 // +0x04
    CIntelligentPointer PatrolStart;                // +0x08
    CIntelligentPointer CreatureAssignedToTask;     // +0x10
    bool                TaskLocked;                 // +0x18
    virtual void AssignCreatureToTask(const CThing& creature);
};
void CVillageTaskPatrol::AssignCreatureToTask(const CThing& creature)
{
    TaskLocked = false;
    CreatureAssignedToTask = CIntelligentPointer(const_cast<CThing*>(&creature));
}