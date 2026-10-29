// CTCTrespasser::IsCurrentlyTrespassing() -- true when the owner is inside a live building (through its interface-117 component) whose trespassable component says it is being trespassed in
// Leaf: IsCurrentlyTrespassing
// Retail: Fable.exe @ 0x007BAE70, 138 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (two inlined GetTC lookups: interface 117 = bit 21
//          of the bitset word at CThing+0x2c, interface 189 = bit 29 of the word at +0x34), CBaseIntelligentPointer::GetPItem
//          0x00A01B50.
// Layout: retail CThing: bitset +0x20, interface map +0x44, Dead flag +0x91 bit 0; the interface-117 component holds the
//         building as a CIntelligentPointer<CThing> at +0x0c; Ego_r.pdb CTCTrespassable: Trespassable +0x0c,
//         IsBeingTrespassedIn +0x0d.
// Shape: nested ifs ending in one shared `xor al,al` exit; the first lookup is HasTC (`if (bits & mask) return true;
//        return false;`) + a __forceinline GetTC, the second the __forceinline out-pointer GetInterface (a returning GetTC
//        there loads the component into edx instead of eax); the key lives in the `push ecx` slot.
class CTCBase;

struct CBaseIntelligentPointer;
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
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};

struct CTCBuildingLocation {                    // the owner's interface-117 component (class name unknown)
    unsigned char _pad_0x00[0xc];
    CBaseIntelligentPointer PBuilding;          // +0x0c
};
class CTCTrespassable { public:
    unsigned char _pad_0x00[0xc];
    bool Trespassable;                          // +0x0c
    bool IsBeingTrespassedIn;                   // +0x0d
};

class CTCTrespasser { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool IsCurrentlyTrespassing();
};

bool CTCTrespasser::IsCurrentlyTrespassing()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(117)) {
        CTCBuildingLocation* location = (CTCBuildingLocation*)thing->GetTC(117);
        CThing* building = ((CThing*)location->PBuilding.GetPItem());
        CTCTrespassable* trespassable;
        if (building && !building->IsDead() && building->GetInterface(&trespassable, 189))
            return trespassable->IsBeingTrespassedIn;
    }
    return false;
}