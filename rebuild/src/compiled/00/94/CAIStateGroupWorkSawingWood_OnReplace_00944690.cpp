// CAIStateGroup_WorkSawingWood::OnReplace() -- give back the sawhorse action point, drop the right-hand item
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944690, length 149 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D), CTCUserActionPointBase::ReleaseActionPoint(long) @ 0x007FEF40 (manifest label
// "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CAIStateGroupBase::DestroyItem(const CCharString&)
// @ 0x008FD1B0, CCharString::~CCharString @ 0x0099EAE0.
//
// Shape: when the sawhorse is alive and an action point was taken (TookActionPoint +0x14 tested between the
// alive test and the lookup), the action-point component releases ActionPointIndex (+0x44); a lookup miss
// skips the release (the bool GetInterface is the `if`). Then the item in CARRY_SLOT_RIGHT_HAND is destroyed
// through a NAMED CCharString local in its OWN block (its address is re-formed with `lea` after the
// constructor); only the block scope lets the string share the single `push ecx` frame slot with the lookup
// key (at function scope the frame grows to `sub esp,8`).
#include "engine/CAIStateGroup_WorkSawingWood.h"

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

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CTCUserActionPointBase {                 // interface type 0x2D component
    void ReleaseActionPoint(long index);        // 0x007FEF40
};

struct CAIStateGroup_WorkSawingWood_Methods : CAIStateGroup_WorkSawingWood {
    const CBaseIntelligentPointer& Sawhorse() const { return *(const CBaseIntelligentPointer*)PSawhorse; }
    void DestroyItem(const CCharString& slot);  // 0x008FD1B0
    void OnReplace();
};

void CAIStateGroup_WorkSawingWood_Methods::OnReplace()
{
    if (IS_THING_ALIVE(Sawhorse()) && TookActionPoint) {
        CTCUserActionPointBase* actionPoint;
        if (Sawhorse().GetPItem()->GetInterface(&actionPoint, 0x2D))
            actionPoint->ReleaseActionPoint(ActionPointIndex);
    }
    {
        CCharString slot("CARRY_SLOT_RIGHT_HAND", -1);
        DestroyItem(slot);
    }
}