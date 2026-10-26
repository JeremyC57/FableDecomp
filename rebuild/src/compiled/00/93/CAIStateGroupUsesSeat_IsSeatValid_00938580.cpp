// CAIStateGroup_UsesSeat::IsSeatValid() -- the seat thing exists and its action-point component reports usable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938580, length 98 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D = bit 13 of the presence word at +0x24), CTCUserActionPointBase::IsActionPointFree
// @ 0x007FEBF0 (manifest label; the shared 0x007FEBF0 is also called by the WorkSawingWood/HammerAnvil
// OnActivate bodies of this chunk).
//
// Shape: PSeat (+0x14, PDB name) is null-tested with one GetPItem, the second GetPItem feeds the inlined
// GetInterface whose miss leaves the component pointer unwritten (`mov eax,[esp+4]`, the key's frame slot);
// `if (f()) return true;` inside the null test with one shared `return false` tail gives the separate
// `mov al,1` exit and the `xor al,al` tail. No engine header exists for this class: the base is modelled
// with the CAIStateGroupBase PDB members, the derived members with their PDB names and offsets.
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

struct CTCUserActionPointBase {                 // interface type 0x2D component
    bool IsActionPointFree();                   // 0x007FEBF0
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CBaseIntelligentPointer PSeat;              // +0x14 CIntelligentPointer<CThingObject>
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    float SeatPos[3];                           // +0x24 C3DVector
    bool SittingOnBench;                        // +0x30
    bool IsSeatValid();
};

bool CAIStateGroup_UsesSeat::IsSeatValid()
{
    if (PSeat.GetPItem() != 0) {
        CTCUserActionPointBase* actionPoint;
        PSeat.GetPItem()->GetInterface(&actionPoint, 0x2D);
        if (actionPoint->IsActionPointFree())
            return true;
    }
    return false;
}