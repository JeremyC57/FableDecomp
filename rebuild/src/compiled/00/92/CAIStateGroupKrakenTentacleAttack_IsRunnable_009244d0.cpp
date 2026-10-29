// CAIStateGroup_KrakenTentacleAttack::IsRunnable() -- runnable while the tentacle component says it should strike.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009244D0, length 69 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xE3 = bit 3 of the bitset
// dword at +0x3c), then CTCKrakenTentacle::GetShouldStrike @ 0x007F1410 whose bool comes back in al unchanged.
// Shape: `if (GetInterface(&p, 0xE3)) return p->GetShouldStrike(); return false;` -- the miss path is the
// `xor al,al` tail after the hit path's `ret`. Base layout modelled locally (PDB names).
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

class CTCKrakenTentacle { public:                      // interface type 0xE3 component
    bool GetShouldStrike();                     // 0x007F1410
};

struct CAIStateGroup_KrakenTentacleAttack : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_KrakenTentacleAttack::IsRunnable()
{
    CTCKrakenTentacle* pTentacle;
    if (PBrain->Creature->GetInterface(&pTentacle, 0xE3))
        return pTentacle->GetShouldStrike();
    return false;
}