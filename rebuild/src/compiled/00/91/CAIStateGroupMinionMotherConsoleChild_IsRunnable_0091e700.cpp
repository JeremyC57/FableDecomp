// CAIStateGroup_MinionMotherConsoleChild::IsRunnable() -- runnable while the creature's mode manager holds
// mode 0x18 (the child-needs-consoling stance).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091E700, length 76 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup, type 0x42 = bit 2 of the bitset
// dword at +0x28), then the mode manager's HasMode(ulong) @ 0x00811340 (manifest "CPlayer::HasMode" is a
// propagated label; the component at interface 0x42 is a mode holder) whose bool is tested.
// Shape: `if (GetInterface(&p, 0x42)) { if (p->HasMode(0x18)) return true; } return false;` -- one shared
// `xor al,al` false tail, `mov al,1` on the hit. Base layout modelled locally (PDB names).
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CTCModeHolder {                          // interface type 0x42 component
    bool HasMode(unsigned long mode);           // 0x00811340
};

struct CAIStateGroup_MinionMotherConsoleChild : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionMotherConsoleChild::IsRunnable()
{
    CTCModeHolder* pModes;
    if (PBrain->Creature->GetInterface(&pModes, 0x42)) {
        if (pModes->HasMode(0x18))
            return true;
    }
    return false;
}