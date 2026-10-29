// CTCNymph::GetShouldSummon() const -- ready to summon and the parent's summon component (type 0xfa) reports a positive creature count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080FD30, length 85 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCAISummon::GetMaxCreatureCount() @ 0x007FDF60.
// Retail CTCNymph: ReadyToSummon +0x5a (PDB +0x5e: the spline's vector is 4 bytes narrower in retail).
// Shape: the inline out-parameter GetInterface(&summon, 0xfa) (bit 26 of the bitset word at +0x3c) inside the flag test, both
// false exits sharing `xor al,al`; the count test is a `return count > 0;` bool (`xor ecx,ecx ; setg cl ; mov al,cl`).
class CTCAISummon { public:
    long GetMaxCreatureCount();                     // 0x007FDF60
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};

struct CTCBase_0080fd30 {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    unsigned char _pad08[4];
};

class CTCNymph : public CTCBase_0080fd30 { public:
    unsigned char _pad0c[0x5a - 0x0c];
    bool          ReadyToSummon;                    // +0x5a
    bool GetShouldSummon();
};

bool CTCNymph::GetShouldSummon()
{
    if (ReadyToSummon) {
        CTCAISummon* summon;
        if (ParentThing->GetInterface(&summon, 0xfa))
            return summon->GetMaxCreatureCount() > 0;
    }
    return false;
}