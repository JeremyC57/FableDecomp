// CAIStateGroup_SitForMeal::ReleaseControlOfPot() -- free the cooking-pot action point this creature holds
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945530, length 119 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D), CTCUserActionPointBase::ReleaseActionPoint(long) @ 0x007FEF40 (manifest label
// "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected).
//
// Shape: nothing to do while ActionPointIndex (+0x3c) is negative (`jl` straight to the epilogue). Otherwise,
// when PPot (+0x34) is alive and has the action-point component (bool GetInterface as the `if`, its bit test
// jumps to the reset), the index is released; the index is then forgotten (-1) on every path. No engine
// header exists: members carry their PDB names and offsets (sizeof 0x48).
struct CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CTCUserActionPointBase {                 // interface type 0x2D component
    void ReleaseActionPoint(long index);        // 0x007FEF40
};

struct CTCCarrying {                            // interface type 0x46 component
    CThing* RemoveThingFromPrimarySlot(bool drop);  // 0x006A1E00
};

struct CGameTime {
    unsigned char _pad[0x14];
    long Day;                                   // +0x14
};

struct CWorld {
    unsigned char _pad[0x1c];
    CGameTime* PGameTime;                       // +0x1c
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24)
};

// PDB layout (sizeof 0x48): the state-group bytes up to +0x34 are a second base's (unnamed in the PDB).
struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                 // +0x14
    CBaseIntelligentPointer PPot;               // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
    void ReleaseControlOfPot();
    void Cleanup();
};

void CAIStateGroup_SitForMeal::ReleaseControlOfPot()
{
    if (ActionPointIndex >= 0) {
        if (IS_THING_ALIVE(PPot)) {
            CTCUserActionPointBase* actionPoint;
            if (PPot.GetPItem()->GetInterface(&actionPoint, 0x2D))
                actionPoint->ReleaseActionPoint(ActionPointIndex);
        }
        ActionPointIndex = -1;
    }
}
