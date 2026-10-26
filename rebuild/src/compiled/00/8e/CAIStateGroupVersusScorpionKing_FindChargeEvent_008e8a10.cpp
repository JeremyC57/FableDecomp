// CAIStateGroup_VersusScorpionKing::FindChargeEvent(const CIntelligentPointer<CThing>&) -- false while the target is alive and in mode 0x3F
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8A10, length 113 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::HasMode(long)
// @ 0x0073A8A0.
//
// Shape: a member that never reads `this` (`ret 4`, ecx untouched); the pointer argument is held in esi for
// the three GetPItem calls of `IS_THING_ALIVE(p) && p->GetInterface(&modes, 0x31) && modes->HasMode(0x3F)`;
// that chain returns false (`xor al,al` arm placed first), every failed link jumps to the shared `mov al,1`
// tail. The manifest name is kept; the body is a "target may be charged" predicate. `this` is the PDB header
// layout (unused here).
#include "engine/CAIStateGroup_VersusScorpionKing.h"

class CTCBase;

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

class CThing { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CTCCreatureModeManager { public:                 // interface type 0x31 component
    bool HasMode(long mode);                    // 0x0073A8A0
};

struct CAIStateGroup_VersusScorpionKing_Methods : CAIStateGroup_VersusScorpionKing {
    bool FindChargeEvent(const CBaseIntelligentPointer& target);
};

bool CAIStateGroup_VersusScorpionKing_Methods::FindChargeEvent(const CBaseIntelligentPointer& target)
{
    CTCCreatureModeManager* modes;
    if (IS_THING_ALIVE(target) && target.GetPItem()->GetInterface(&modes, 0x31) && modes->HasMode(0x3F))
        return false;
    return true;
}