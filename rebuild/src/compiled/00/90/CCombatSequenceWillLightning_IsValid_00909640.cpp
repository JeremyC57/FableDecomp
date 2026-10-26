// CCombatSequence_WillLightning::IsValid() -- valid unless the creature's will component (type 0xce) says its flag byte at +0xc is clear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909640, length 71 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup for type 0xce = bit 14 of
// bitset dword +0x38, tested as `test ch,40h`).
// The creature is the base's +0x1c (retail layout, see 0x008B0AB0). The found component's byte at +0xc is returned as `!= 0 ? true : false`
// (bare `setne al`); no will component means valid.
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;      // 0x004365B0 (CVectorMap::LowerBound)
};

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

struct CCombatSequence_WillLightning_Retail : CCombatSequenceBase_Retail {
    virtual bool IsValid();
};

bool CCombatSequence_WillLightning_Retail::IsValid()
{
    CThing* creature = Creature;
    CTCWill* pWill;
    if (creature->GetInterface(&pWill, 0xce))
        return pWill->m_flag0c != 0 ? true : false;
    return true;
}