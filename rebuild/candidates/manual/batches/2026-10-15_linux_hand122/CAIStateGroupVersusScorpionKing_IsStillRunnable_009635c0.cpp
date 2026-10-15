// CAIStateGroup_VersusScorpionKing::IsStillRunnable() -- base check, then "target not in creature mode 0x38"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009635C0, length 113 bytes (+ int3 padding).
// Callees: CAIStateGroup_VersusEnemyBase::IsStillRunnable @ 0x00962AF0 (PDB-mangled manifest name),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface,
// type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::HasMode(long) @ 0x0073A8A0.
//
// Shape: the base-class check returns its own al when false (`if (!Base::IsStillRunnable()) return false;`
// needs no `xor al,al`: VC reuses the tested byte). Then the target's mode manager comes from the out-parameter
// GetInterface whose result is not tested, so retail tail-duplicates `HasMode(0x38)` + the `neg ; sbb ; inc`
// negation (`return HasMode(0x38) ? false : true;` -- `!HasMode()` widens to `sbb eax,eax ; inc eax`) into
// the hit arm and the miss arm (which reads the unwritten frame slot). Modelled over the
// VersusEnemyBase PDB header (PTargetThing +0x14); the derived members follow the ScorpionKing header offsets.
// Leaf: ?IsStillRunnable@CAIStateGroup_VersusScorpionKing
#include "engine/CAIStateGroup_VersusEnemyBase.h"

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

struct CTCCreatureModeManager {                 // interface type 0x31 component
    bool HasMode(long mode);                    // 0x0073A8A0
};

struct CAIStateGroup_VersusEnemyBase_Methods : CAIStateGroup_VersusEnemyBase {
    const CBaseIntelligentPointer& Target() const { return *(const CBaseIntelligentPointer*)PTargetThing; }
    bool IsStillRunnable();                     // 0x00962AF0
};

struct CAIStateGroup_VersusScorpionKing : CAIStateGroup_VersusEnemyBase_Methods {
    bool GoClockwise;                           // +0x1c
    unsigned char PArenaCentreThing[0xb];       // +0x1d CIntelligentPointer<CThing>
    bool IsStillRunnable();
};

bool CAIStateGroup_VersusScorpionKing::IsStillRunnable()
{
    if (!CAIStateGroup_VersusEnemyBase_Methods::IsStillRunnable())
        return false;
    CTCCreatureModeManager* modes;
    Target().GetPItem()->GetInterface(&modes, 0x31);
    return modes->HasMode(0x38) ? false : true;
}
