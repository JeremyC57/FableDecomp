// CAIStateGroup_CleanHouse::IsRunnable() -- defer to the idle scheduler, then the generic clean test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917B50, length 89 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (creature interface
// map at +0x44), CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0,
// CAIStateGroup_Clean::IsRunnable @ 0x00917A10 (the base test).
// The creature comes from PBrain (+0x4) -> Creature (+0x18). The interface lookup for type 0xd3
// (CTCIdleScheduler) is the __forceinline `GetInterface(type, &out)` helper: presence bitset at
// +0x20 tested as a branch (`test dword ptr [eax+38h],80000h`), then the sorted map's lower bound
// with the key copied to a frame slot (`push ecx`) and compared as an immediate. If a scheduler
// exists and does not prefer this idle, the group is not runnable; otherwise the base decides.
struct CAIStateGroupBase;

struct CTCIdleScheduler {
    bool IsPreferredIdle(CAIStateGroupBase* group);     // 0x00801BE0
};

struct CInterfaceMap {
    struct SEntry { long type; CTCIdleScheduler* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};

struct CThingAICreature {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    __forceinline bool GetInterface(long type, CTCIdleScheduler** out)
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;             // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_Clean : CAIStateGroupBase {
    virtual bool IsRunnable();              // 0x00917A10
};

struct CAIStateGroup_CleanHouse : CAIStateGroup_Clean {
    virtual bool IsRunnable();
};

bool CAIStateGroup_CleanHouse::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->Creature->GetInterface(0xd3, &scheduler)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    return CAIStateGroup_Clean::IsRunnable();
}
