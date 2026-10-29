// CTCGuard::PlayerIsCriminalAndCanApologise() -- through the guard's village-member component (interface 0x23) find its village; true when the hero is a criminal there and can apologise for the crimes of the guard's apology window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B2BF0, length 113 bytes (+15 int3 padding).
// Ego_r.pdb: ?PlayerIsCriminalAndCanApologise@CTCGuard@@QAE_NXZ
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined interface lookup), the village-member
// component's village getter @ 0x007EE540 (manifest "IS_THING_ALIVE<...>" is a propagated label: it is a thiscall returning a
// CTCVillage*), CTCVillage::IsHeroCriminal @ 0x0065BFCE, CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor @ 0x007B29B0,
// CTCVillage::PlayerCanApologiseForCurrentCrimes(long) @ 0x0065A686.
// Shape: the interface lookup is an inline accessor returning an unset local when the component bit (bit 3 of CThing +0x24 =
// interface 0x23) is clear, so retail reads the `push ecx` frame word on that path; the result goes straight to the getter.
// The three tests share one `xor al,al` exit (nested ifs ending in one `return false`).
// Layout: CTCBase ParentThing +0x04; CThing component bitset +0x20, interface map +0x44.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            tc = (T*)it->second;
        }
        return tc;
    }
};
class CTCVillage {
public:
    bool IsHeroCriminal();                                  // 0x0065BFCE
    bool PlayerCanApologiseForCurrentCrimes(long timeAgo);  // 0x0065A686
};
class CTCVillageMember {
public:
public:
    CTCVillage* GetPVillage() const;   // 0x007EE540
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
};
class CTCGuard : public CTCBase {
public:
    long GetTimeAgoThatCrimesCanBeApologisedFor() const;    // 0x007B29B0
    bool PlayerIsCriminalAndCanApologise();
};

bool CTCGuard::PlayerIsCriminalAndCanApologise()
{
    CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetPVillage();
    if (village) {
        if (village->IsHeroCriminal()) {
            long timeAgo = GetTimeAgoThatCrimesCanBeApologisedFor();
            if (timeAgo > 0)
                return village->PlayerCanApologiseForCurrentCrimes(timeAgo);
        }
    }
    return false;
}