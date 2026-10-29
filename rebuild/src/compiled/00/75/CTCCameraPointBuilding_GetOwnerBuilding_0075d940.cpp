// CTCCameraPointBuilding::GetOwnerBuilding(CThing** out) const -- fetch the parent thing's interface-51 component, follow its owner pointer and hand it out when that thing is a building
//
// Retail 0x0075D940, 87 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x004365B0 (a second out-of-line copy of the interface-map search), CBaseIntelligentPointer::
// GetPItem() const @ 0x00A01B50. Called from CTCCameraPointBuilding::IsLeafCriteriaMet (0x0075DAA0).
// Shape: the thing's inline GetInterface(type, &out) of the landed 0x007504E0 family with type 51
// (word 1, bit 19: `test [eax+24h],80000h`; key 33h in the `push ecx` frame slot); the component's
// owner pointer is an intelligent pointer at the UNALIGNED +0x0d (byte-packed component layout);
// the owner must carry thing-type flag 0x40 (ThingTypeFlags +0x6c) before it is stored through the
// out pointer. Two exits: `mov al,1` after the store, the shared `xor al,al` for every failure.

class CBaseObject;

struct CBaseIntelligentPointer
{
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer
{ public:
    T* Get() const { return (T*)GetPItem(); }
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
    const SEntry* LowerBound(const ETCInterfaceType& type) const;           // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing
{ public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x20];
    unsigned long ThingTypeFlags;                   // +0x6c

    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            if (it == map->last || type < it->first)
                it = map->last;
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};

enum { TC_INTERFACE_51 = 51, THING_TYPE_BUILDING = 0x40 };

#pragma pack(push, 1)
struct CTCBuildingOwned                             // the interface-51 component (owner link at +0x0d)
{
    unsigned char _pad_0x00[0xd];
    CIntelligentPointer<CThing> POwner;             // +0x0d
};

struct CTCCameraPointBuilding
{
    void*         __vftable;                        // +0x00 (CTCBase)
    CThing*       ParentThing;                      // +0x04 (CTCBase)

    bool GetOwnerBuilding(CThing** out) const;
};
#pragma pack(pop)

bool CTCCameraPointBuilding::GetOwnerBuilding(CThing** out) const
{
    CTCBuildingOwned* owned;
    if (ParentThing->GetInterface(TC_INTERFACE_51, &owned))
    {
        CThing* building = owned->POwner.Get();
        if (building->ThingTypeFlags & THING_TYPE_BUILDING)
        {
            *out = building;
            return true;
        }
    }
    return false;
}