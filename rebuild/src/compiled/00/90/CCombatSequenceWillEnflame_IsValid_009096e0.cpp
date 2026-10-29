// CCombatSequence_WillEnflame::IsValid() -- valid unless the creature's will component (type 0xce) says its flag byte at +0xc is clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009096E0, length 71 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup for type 0xce = bit 14 of
// bitset dword +0x38, tested as `test ch,40h`).
// The creature is the base's +0x1c (retail layout, see 0x008B0AB0). The found component's byte at +0xc is returned as `!= 0 ? true : false`
// (bare `setne al`); no will component means valid.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0 (CVectorMap::LowerBound)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44 sorted {type, component} vector map
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasInterface(long interfaceType) const   // the bitset test alone
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)   // forced: two lookups in one body exceed the inliner budget
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
struct CCombatSequenceBase_Retail {        // vtable slots 0..18, see 0x008B0AB0; only the data is needed here
    virtual ~CCombatSequenceBase_Retail();
    bool          Running;                     // +0x04
    long          NoAvailableActions;          // +0x08
    long          CurrentActionIndex;          // +0x0c
    unsigned char CombatActions[12];           // +0x10 std::vector<CCombatAction>
    CThing*       Creature;                    // +0x1c
    CThing*       Target;                      // +0x20
    bool          CreateCalled;                // +0x24
};

struct CTCWill {
    unsigned char _pad[0xc];
    unsigned char m_flag0c;                    // +0x0c
};

struct CCombatSequence_WillEnflame_Retail : CCombatSequenceBase_Retail {
    virtual bool IsValid();
};

bool CCombatSequence_WillEnflame_Retail::IsValid()
{
    CThing* creature = Creature;
    CTCWill* pWill;
    if (creature->GetInterface(&pWill, 0xce))
        return pWill->m_flag0c != 0 ? true : false;
    return true;
}