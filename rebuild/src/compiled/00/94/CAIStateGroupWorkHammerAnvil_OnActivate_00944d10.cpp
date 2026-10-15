// CAIStateGroup_WorkHammerAnvil::OnActivate() -- claim a free action point on the anvil, else terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944D10, length 154 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D = bit 13 of the presence word at +0x24), CTCUserActionPointBase::IsActionPointFree
// @ 0x007FEBF0, CTCUserActionPointBase::GetFreeActionPoint(creature, pos*, rhset*, index*) @ 0x007FEDE0,
// CAIStateGroupBase::Terminate @ 0x008FCF40 (manifest label "CXMVPlaybackInfo" is propagated and rejected).
//
// Shape: one `&&` chain -- IS_THING_ALIVE(PAnvil) (two GetPItem calls), then the third GetPItem feeds the
// bool-returning GetInterface (its bit test is the third `je fail`), then the action-point component (kept in
// esi) is asked whether a point is free and to reserve it into ActionPointPos (+0x1d), ActionPointRHSet
// (+0x29) and ActionPointIndex (+0x44) for the brain's creature. Success sets TookActionPoint (+0x14) and
// returns; any failure clears it and terminates the state group. The creature is a named local taken before
// the reservation call (retail loads PBrain->PCreature before the address pushes; inline it moves between them). `this` is the PDB header layout (pack 1:
// PAnvil sits unaligned at +0x15); PBrain lives in the flattened base bytes at +0x04.
#include "engine/CAIStateGroup_WorkHammerAnvil.h"

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

struct C3DVector;
struct CRightHandedSet;

struct CTCUserActionPointBase {                 // interface type 0x2D component
    bool IsActionPointFree();                   // 0x007FEBF0
    bool GetFreeActionPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* index); // 0x007FEDE0
};

struct CAIStateGroup_WorkHammerAnvil_Methods : CAIStateGroup_WorkHammerAnvil {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_base_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    const CBaseIntelligentPointer& Anvil() const { return *(const CBaseIntelligentPointer*)PAnvil; }
    void Terminate();                           // 0x008FCF40
    void OnActivate();
};

void CAIStateGroup_WorkHammerAnvil_Methods::OnActivate()
{
    CTCUserActionPointBase* actionPoint;
    if (IS_THING_ALIVE(Anvil()) && Anvil().GetPItem()->GetInterface(&actionPoint, 0x2D)
        && actionPoint->IsActionPointFree()) {
        CThing* creature = GetPBrain()->PCreature;
        if (actionPoint->GetFreeActionPoint(creature, (C3DVector*)&ActionPointPos_X,
                                            (CRightHandedSet*)ActionPointRHSet, &ActionPointIndex)) {
            TookActionPoint = true;
            return;
        }
    }
    TookActionPoint = false;
    Terminate();
}