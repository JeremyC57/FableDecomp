// CAIStateGroup_SitForMeal::Cleanup() -- release the pot, stamp the meal day, put the bowl away
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945620, length 122 bytes (+ int3 padding).
// Callees: CAIStateGroup_SitForMeal::ReleaseControlOfPot @ 0x00945530, CAIStateGroupBase::GetPWorld
// @ 0x008FCFC0, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the brain's creature, type 0x46 =
// bit 6 of the presence word at +0x28), CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00.
//
// Shape: DayLastAte (+0x44) = world->game time (+0x1c)->day (+0x14), stored after the HoldingBowl (+0x40) load.
// When the creature holds the bowl, its carrying component (out-parameter GetInterface: a miss reads the
// unwritten frame slot) drops the primary-slot thing and HoldingBowl is cleared; retail tail-duplicates the
// call and the clear into the hit and miss arms (the hit arm pops esi first).
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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
    void ReleaseActionPoint(long index);        // 0x007FEF40
};

class CTCCarrying { public:                            // interface type 0x46 component
    CThing* RemoveThingFromPrimarySlot(bool drop);  // 0x006A1E00
};

struct CGameTime {
    unsigned char _pad[0x14];
    long Day;                                   // +0x14
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CGameTime* PGameTime;                       // +0x1c
};

class CAIStateGroupBase { public:
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

void CAIStateGroup_SitForMeal::Cleanup()
{
    ReleaseControlOfPot();
    DayLastAte = GetPWorld()->PGameTime->Day;
    if (HoldingBowl) {
        CTCCarrying* carrying;
        PBrain->PCreature->GetInterface(&carrying, 0x46);
        carrying->RemoveThingFromPrimarySlot(false);
        HoldingBowl = false;
    }
}