// CAIStateGroup_MazeRangedCombat::OnActivate() -- remember the boss phase the ranged phase started in.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954630, length 93 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xAD = bit 13 of the bitset
// dword at +0x34, `test ch,20h`), then CTCBoss::GetCurrentPhase @ 0x007AED10 (manifest "CWorld::
// DrawGetEnvironment" is a propagated label: it is the boss component's phase getter, no arguments).
// Shape: the component is an UNINITIALISED local filled by the inlined GetInterface whose bool is NOT tested;
// on the miss path retail reads the never-written slot (`mov eax,[esp+4]`, shared with the key temporary)
// and the store to LastCompletedPhase (+0x20, engine header name) is duplicated on both paths. Frame = one
// `push ecx`. The engine header flattens the base, so the layout is local.
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
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
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
            CInterfaceMap::SEntry* it = map->LowerBound(type);
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

class CTCBoss { public:                                // interface type 0xAD component
    long GetCurrentPhase();                     // 0x007AED10
};

struct CAIStateGroup_MazeRangedCombat : CAIStateGroupBase {
    unsigned char _pad14[0xc];                  // +0x14 maze-combat state (untouched here)
    long LastCompletedPhase;                    // +0x20
    virtual void OnActivate();
};

void CAIStateGroup_MazeRangedCombat::OnActivate()
{
    CTCBoss* pBoss;
    PBrain->Creature->GetInterface(&pBoss, 0xAD);
    LastCompletedPhase = pBoss->GetCurrentPhase();
}