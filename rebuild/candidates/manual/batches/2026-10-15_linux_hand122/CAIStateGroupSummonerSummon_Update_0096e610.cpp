// CAIStateGroup_SummonerSummon::Update() -- play the summon-into combat action on a living target, then terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096E610, length 101 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CCreatureAction_SummonerSummonInto::
// CCreatureAction_SummonerSummonInto(CThingCreatureBase*, CThing*) @ 0x008CBCF0 (landed),
// CAIStateGroupBase::AIAction_CombatPerformActionAndWait(CCombatActionBase*, bool) @ 0x0097DA40,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900, CAIStateGroupBase::Terminate @ 0x008FCF40.
//
// Shape: IS_THING_ALIVE(PTarget) (PTarget +0x14, PDB name; edi holds its address), then the landed
// perform-and-wait shape: the creature read into a named local first (ebx, loaded before the `false` push),
// the 0x114-byte action built as a temporary inside the call from the creature and the target's third
// GetPItem, its base destructor after the call. Terminate runs on both paths. `this` is the PDB header
// layout; PBrain lives in the flattened base bytes at +0x04.
#include "engine/CAIStateGroup_SummonerSummon.h"

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

class CThingCreatureBase;

struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase {
    unsigned char _combat[0x6c];                // +0xa8..+0x113
    ~CCombatActionBase();                       // 0x0062E900
};

struct CCreatureAction_SummonerSummonInto : CCombatActionBase {
    CCreatureAction_SummonerSummonInto(CThingCreatureBase* creature, CThing* target);   // 0x008CBCF0
};

struct CAIStateGroup_SummonerSummon_Methods : CAIStateGroup_SummonerSummon {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_pad_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    const CBaseIntelligentPointer& Target() const { return *(const CBaseIntelligentPointer*)PTarget; }
    long AIAction_CombatPerformActionAndWait(CCombatActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                           // 0x008FCF40
    void Update();
};

void CAIStateGroup_SummonerSummon_Methods::Update()
{
    if (IS_THING_ALIVE(Target())) {
        CThingCreatureBase* creature = (CThingCreatureBase*)GetPBrain()->PCreature;
        AIAction_CombatPerformActionAndWait(&CCreatureAction_SummonerSummonInto(creature, Target().GetPItem()), false);
    }
    Terminate();
}
