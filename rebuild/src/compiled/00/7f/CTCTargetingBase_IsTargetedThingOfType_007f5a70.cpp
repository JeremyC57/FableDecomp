// CTCTargetingBase::IsTargetedThingOfType(unsigned long typeMask) const -- true when there is a targeted thing whose CTCTargeted component has any of the given targeting flags
// Leaf: IsTargetedThingOfType
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F5A70, length 99 bytes (`ret 4`).
// Callees: CTCTargetingBase virtual slot 33 GetPTargetedThing() (+0x84; CTCTargetingPlayer's override is 0x00802610) twice,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for CTCTargeted, interface 9 = bit 9 of
// CThing+0x20).
// Layout: CTCTargeted::TargetingFlags +0x0C (engine/CTCTargeted.h).
// Shape: the targeted thing is fetched again for the lookup (two virtual calls); the out-parameter GetInterface gives the dword load +
// `test ch,2` and `mov eax,[eax+4] ; mov eax,[eax+0Ch]`; `return (flags & mask) != 0;` gives `neg ; sbb ; neg`.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCTargeted { public:
    unsigned char _pad00[0x0c];
    unsigned long TargetingFlags;               // +0x0c
};
class CTCTargetingBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual const CThing* GetPTargetedThing() const;   // slot 33 (+0x84)
    bool IsTargetedThingOfType(unsigned long typeMask) const;
};

bool CTCTargetingBase::IsTargetedThingOfType(unsigned long typeMask) const
{
    if (GetPTargetedThing()) {
        const CTCTargeted* targeted;
        if (GetPTargetedThing()->GetInterface(&targeted, 9))
            return (targeted->TargetingFlags & typeMask) != 0;
    }
    return false;
}