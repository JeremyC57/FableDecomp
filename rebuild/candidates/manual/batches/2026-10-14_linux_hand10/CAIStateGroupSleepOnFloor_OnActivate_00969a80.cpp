// CAIStateGroup_SleepOnFloor::OnActivate() -- assign the creature to the floor thing's village task and clear the sleeping flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969A80, length 92 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the floor pointer at +0x14),
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the thing's interface map at
// +0x44, key by address), CVillageTaskBase::AssignCreatureToTask(CThing*) @ 0x0087C390 (the
// manifest's "CarryCrate" label is the propagated one; the component is interface 0xac).
// Shape: the inlined `thing->GetInterface(0xac, &task)` lookup (presence bitset at +0x20, dword word
// 5 bit 12 -> `test ch,10h`, the lower bound with the key in the `push ecx` slot, end-or-larger ->
// the end entry) with its bool result IGNORED, so when the bit is clear the uninitialised `task`
// slot ([esp+4]) is what gets used (retail bug reproduced). Then PBrain->Creature is assigned to
// the task and the flag at +0x1c is cleared. The result slot shares the frame with the key.
struct CThing;

struct CVillageTaskBase {
    void AssignCreatureToTask(CThing* creature);    // 0x0087C390
};

struct CInterfaceMap {
    struct SEntry { long type; CVillageTaskBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline bool GetInterface(long type, CVillageTaskBase** out)
    {
        if (m_presentBits[type / 32] & (1 << (type % 32))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem();                         // 0x00A01B50
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_SleepOnFloor : CAIStateGroupBase {
    CBaseIntelligentPointer PFloor;             // +0x14 CIntelligentPointer<CThing>
    bool Sleeping;                              // +0x1c
    virtual void OnActivate();
};

void CAIStateGroup_SleepOnFloor::OnActivate()
{
    CVillageTaskBase* task;
    PFloor.GetPItem()->GetInterface(0xac, &task);
    task->AssignCreatureToTask(PBrain->Creature);
    Sleeping = false;
}
